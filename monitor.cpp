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
 */ 
 #include <Arduino.h>
 #include "config.h"
 #include "keys.h"
 #include "state.h"
 #include "wifi.h"

 void initState() {
  if ( readConfig() ) {
    LOGF(("SSID : %s\r\n",state.ssid));
    LOGF(("PASS : %s\r\n",state.password));
    LOGF(("INTIP : %s\r\n",state.intPingIp));
    LOGF(("ETXIP : %s\r\n",state.extPingIp));
  } else {
    LOGLN(("No Config found"));
    #if defined __WIFISSID && defined __WIFIKEY
       strcpy((char*)state.ssid,__WIFISSID);
       strcpy((char*)state.password,__WIFIKEY);
       strcpy((char*)state.extPingIp,__EXTIP);
       strcpy((char*)state.intPingIp,__HSIP);
       storeConfig();
    #else
      wifiConfig();
    #endif
    NVIC_SystemReset();
  }

  state.readPtr = 0;
  state.writePtr = 0;
  state.elements = 0;
  state.intState = 0;
  state.extState = 0;  
  state.hsName[0] = '\0';
  state.hsState = HSSTATE_UNKN;
  state.hasRefreshed = false;
  state.withSound = true;
}

// ---
// manage the historical data
// ---


void addInBuffer(uint32_t rttExt, uint32_t rttInt ) {
  state.avgExtPing[state.writePtr] = (rttExt < (8*255))?rttExt/8:255; // storing step is 8ms, max 255*8ms
  state.avgIntPing[state.writePtr] = (rttInt < (8*255))?rttInt/8:255; // storing step is 8ms, max 255*8ms
  state.writePtr = (state.writePtr+1) & (MAXBUFFER-1);
  if ( state.elements == MAXBUFFER ) {
    state.readPtr = (state.readPtr+1) & (MAXBUFFER-1);  
  } else {
    state.elements++;
  }
  state.hasRefreshed = true;
}

uint8_t getIndexInBuffer(int i) {
  int t = state.readPtr;
  if ( i > state.elements ) return MAXBUFFER;
  for ( int k = 0 ; k < i ; k++ ) {
    t = (t+1) & (MAXBUFFER-1);
  }
  return t;
}

uint8_t getLastIndexWritten() {
  if ( state.elements == 0 ) return MAXBUFFER;
  if ( state.writePtr == 0 ) return MAXBUFFER-1;
  return state.writePtr-1;
}


void runMonitor() {

    uint32_t rttE = pingIP((char*)state.extPingIp);
    uint32_t rttI = pingIP((char*)state.intPingIp);
    addInBuffer(rttE,rttI);
    if ( rttE > 0 ) state.extState = 5;
    else if ( state.extState > 0 ) state.extState--;
    if ( rttI > 0 ) state.intState = 5;
    else if ( state.intState > 0 ) state.intState--;
    
}


void reportData() {
  //if ( state.extState > 0 ) {
    // network looks available
    reportWatchium();
  //}
}
