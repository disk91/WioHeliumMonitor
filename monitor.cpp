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
  state.hasRefreshed = false;
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


// Hack the malloc wrapper to clear the memory leak
// in the WiFi stack ...
void erpcs_initMallocHisto();
void erpc_cleanMalloc();

void runMonitor() {

    erpcs_initMallocHisto();
    uint32_t rttE = pingIP((char*)state.extPingIp);
    erpc_cleanMalloc();
    erpcs_initMallocHisto();
    uint32_t rttI = pingIP((char*)state.intPingIp);
    erpc_cleanMalloc();
    addInBuffer(rttE,rttI);

}
