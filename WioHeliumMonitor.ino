/**
 * This file is part of Wio Helium Monitor.
 *
 *   Wio Helium Monitor is free software created by Paul Pinault aka disk91. 
 *   You can redistribute it and/or modify it under the terms of the 
 *   GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   any later version.
 *
 *  Wio Helium Monitor is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Wio LoRaWan Field Tester.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  Author : Paul Pinault (disk91.com)
 *  
 *  Dependencies
 *  - seeed Arduino rpcUnified 
 *  - seeed Arduino rpcWifi 
 *  - seeed Arduino mbdetps
 */  
#include "Arduino.h"
#include "config.h"
#include "ui.h"
#include "wifi.h"
#include "state.h"
#include "keys.h"
#include "sound.h"
#include "watchdog.h"

state_t state;
void setup() {
  #if defined SERIALCONFIG || defined DEBUG 
     Serial.begin(9600);
     while (!Serial && millis() < 5000);
     LOGLN(("####################################################"));
  #endif
  
  initScreen();
  displayTitle();
  delay(1000);
  if ( digitalRead(WIO_5S_PRESS) == LOW ) {
    while ( digitalRead(WIO_5S_PRESS) == LOW );
    wifiConfig();
    NVIC_SystemReset();
  }
  setupSound();
  initState();
  displaySplash();
  setupWifi();
  clearScreen();

  initWatchdog();
  enableWatchdog(20000);
}


#define PING_PERIOD_MS    (30*1000)
#define REPORT_PERIOD_MS  (15*60*1000)
#define SOUND_PERIOD_MS   (1*60*1000)

void loop() {
  static uint32_t cTime = 0;
  static uint32_t pTime = PING_PERIOD_MS - 1000;        // last ping
  static uint32_t rTime = REPORT_PERIOD_MS - 1000;      // report time
  static uint32_t sTime = 0;                            // last sound played
  uint32_t stTime;

  iAmAlive();
  
  stTime = millis();

  if ( pTime >= PING_PERIOD_MS ) {
    runMonitor();
    pTime = 0;
    if ( rTime >= REPORT_PERIOD_MS && state.extState == 5 ) {
      // only report data when the external ping works 
      // over network, otherwise make non sens and will crash the Wio
      iAmAlive();
      reportData();
      rTime = 0;
    }    
  }
  refreshUI();

  // Play sound notification when a problem is detected
  if ( state.withSound && sTime > SOUND_PERIOD_MS) {
    if ( state.intState == 0 || state.extState == 0 || state.hsState > 0 ) {
      playSound();
      sTime = 0;
    }
  }

  delay(10);
  long duration = millis() - stTime;
  if ( duration < 0 ) duration = 10;
  cTime += duration;
  pTime += duration;
  rTime += duration;
  sTime += duration;
}
