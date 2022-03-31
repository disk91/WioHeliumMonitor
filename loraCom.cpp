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
 #include "config.h"
 #include "loraCom.h"
 #include "state.h"

loraConf_t loraConf;

 void loraSetup() {
    if ( loraE5QuickSetup() ) {
      state.e5Detected = true;
      LOGLN(("LoRa E5 detected"));
    } else {
      state.e5Detected = false;     
    } 
 }

 void loraLoop() {
   if ( state.isLoRaInit && state.isLoRaSet ) {
     if ( state.e5Detected ) {
       loraLoopE5();
     }
   }
 }


 bool loraInit() {
  if ( state.isLoRaInit) return true;
  if ( state.isLoRaSet ) {
    if ( state.e5Detected ) {
      if ( loraE5Setup() ) {
        state.isLoRaInit = true;
        LOGLN(("LoRa E5 initialized"));
        return true;
      } else {
        LOGLN(("LoRa E5 init failed"));
      }
    }
  }
  return false;
 }


void do_send(uint8_t port, uint8_t * data, uint8_t sz, uint8_t _dr, uint8_t pwr, bool acked, uint8_t retries ) {
  if ( state.isLoRaInit && state.isLoRaSet ) {
    if ( state.e5Detected ) {
       do_sendE5(port, data, sz, _dr, pwr, acked, retries );
    }
  }
}
 
