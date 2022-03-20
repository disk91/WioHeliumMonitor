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
#include "rpcWiFi.h"
#include <rpcPing.h>
#include "config.h"
#include "state.h"

void setupWifi() {
 LOGLN(("Setup WiFi"));
 WiFi.mode(WIFI_STA);
 WiFi.disconnect();
 do {
    delay(500);
    WiFi.begin((char*)state.ssid,(char*)state.password);
 } while (WiFi.status() != WL_CONNECTED);
 
 LOG(("Connected to WiFi, IP:"));
 LOGLN((WiFi.localIP()));  
}

int32_t pingIP(char * ip) {

  LOGF(("Ping : %s - ",ip));
  bool r = Ping.ping(ip,3);
  if ( r ) {
    float t = Ping.averageTime();
    LOGF(("rtt %d ms\r\n",(int)t));
    return (int32_t)t;
  } else {
    LOGLN(("failed"));
    return 0;
  }

}
 
