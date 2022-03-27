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
#include "wifi.h"
#include "ui.h"

IPAddress srv((uint32_t)0); // server IP

void setupWifi() {
 LOGLN(("Setup WiFi"));
 connectingWifi();
 WiFi.mode(WIFI_STA);
 WiFi.disconnect();
 do {
    connectingWifi();
    delay(250);
    WiFi.begin((char*)state.ssid,(char*)state.password);
 } while (WiFi.status() != WL_CONNECTED);
 
 LOG(("Connected to WiFi, IP:"));
 LOGLN((WiFi.localIP()));  

 // Get the UID - Mac Address
 byte mac[6];
 WiFi.macAddress(mac);
 uint8_t check = 0;
 for ( int i = 0 ; i < 6 ; i++ ) {
  check += (uint8_t)mac[i];
 }
 sprintf((char*)state.uid,"%02X%02X%02X%02X%02X%02X%02X",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],check);
 LOGF(("UID : %s\r\n",(char*)state.uid));

 if( !WiFi.hostByName("wiomonitor.disk91.com", srv) ){
  LOGLN(("Host resolution failed"));
 }
 LOG(("Server :"));
 LOGLN((srv));
 
}

// Hack the malloc wrapper to clear the memory leak
// in the WiFi stack ...
void erpcs_initMallocHisto();
void erpc_cleanMalloc();

int32_t pingIP(char * ip) {


  LOGF(("Ping : %s - ",ip));

  erpcs_initMallocHisto();
  bool r = Ping.ping(ip,3);
  erpc_cleanMalloc();

  if ( r ) {
    float t = Ping.averageTime();
    LOGF(("rtt %d ms\r\n",(int)t));
    return (int32_t)t;
  } else {
    LOGLN(("failed"));
    return 0;
  }

}


bool reportWatchium() {
   
   WiFiClient client;
//   if (client.connect("10.0.0.247", 80, 900) == 0) { // failure test
   if (!client.connect(srv, 80, 1000)) {
       LOGLN(("Failed to reach server"));
       return false;
   }
   String url = "GET /wioheliummonitor/v1.0/";
   url += (char *)state.uid;
   url += "/";
   url += state.intState;
   url += "/";
   url += state.extState;   
   url += "/";
   url += " HTTP/1.1";
  client.println(url);
  client.println("Host: wiomonitor.disk91.com");
  client.println("Scheme: http");
  client.println("Accept: application/json");
  client.println("User-Agent: wio-terminal");
  client.println();
  
   int maxloops = 0;
   
    //wait for the server's reply to become available
    while (!client.available() && maxloops < 1000) {
        maxloops++;
        delay(1); //delay 1 msec
    }
    if (client.available() > 0) {
        //read back one line from the server
        String line = client.readString(); // Read from the server response
        // Proceed various line-endings
        line.replace("\r\n", "\n");
        line.replace('\r', '\n');
        line.replace("\n", "\r\n");

        int iHotspot = line.indexOf("{\"hs\":");
        if ( iHotspot > 0 ) {
          String hotspot = line.substring(iHotspot);
          int eHotspot = hotspot.indexOf("\",");
          if ( eHotspot > 0 ) {
            hotspot = line.substring(iHotspot+7,iHotspot+eHotspot);
            LOGLN((hotspot));
            if ( hotspot.length() > HSNAME_SZ-1 ) {
              hotspot = hotspot.substring(0,HSNAME_SZ-1);
            }
            strcpy((char*)state.hsName,hotspot.c_str());
          }
        }

        int sStatus = line.indexOf("\"status\":");
        if ( sStatus > 0 ) {
          String status = line.substring(sStatus);
          int eStatus = status.indexOf("}");
          if ( eStatus > 0 ) {
            status = line.substring(sStatus+9,sStatus+eStatus);
            LOGLN((status));
            state.hsState = atoi(status.c_str());
          }
        }              
    } else {
        LOGLN(("Client Timeout"));
    }
    client.stop();

    return true;
}
