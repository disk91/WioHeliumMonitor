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
#include "wifiHide.h"
#include "ui.h"
#include "loraCom.h"
#include "memory.h"

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
 uint32_t boardId = getSamd51Uid();
 state.uidi[0] = (boardId >> 24) & 0xFF;
 state.uidi[1] = (boardId      ) & 0xFF;
 check = state.uidi[0] + state.uidi[1];
 for ( int i = 2 ; i < 6 ; i++ ) {
  check += (uint8_t)mac[i];
  state.uidi[i] = mac[i];
 }
 state.uidi[6] = check;
 sprintf((char*)state.uid,"%02X%02X%02X%02X%02X%02X%02X",state.uidi[0],state.uidi[1],state.uidi[2],state.uidi[3],state.uidi[4],state.uidi[5],state.uidi[6]);
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

uint32_t hexStrToInt(char * st, int sz){
  char x[17] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', '\0' };
  int i;
  uint32_t v = 0;
  if ( sz > 8 ) return 0;
  for ( int k = 0 ; k < sz ; k++ ) {  
    v *= 16;
    for ( i = 0; i < 17 ; i++) {
      if ( st[k] == x[i] ) {
        if ( i == 16 ) return 0;
        v += i;
        break;
      }
    }
    if ( i == 17 ) return 0;
  }
  return v;  
}


bool reportWatchium() {
   bool ret = false;
   WiFiClient client;
//   if (client.connect("10.0.0.247", 80, 900) == 0) { // failure test
   if (!client.connect(srv, 80, 1000)) {
       client.stop();
       LOGLN(("Failed to reach server"));
       return false;
   }

  client.print(F("GET /wioheliummonitor/v1.0/"));
  client.print((char*)state.uid);
  client.print(F("/"));
  client.print(state.intState);
  client.print(F("/"));
  client.print(state.extState);
  client.println(F("/ HTTP/1.1"));
  
  client.println(F("Host: wiomonitor.disk91.com"));
  client.println(F("Scheme: http"));
  client.println(F("Accept: application/json"));
  client.println(F("User-Agent: wio-terminal"));
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
            state.isRegistered = true;
          }
          hotspot = (const char*)NULL;
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
          status = (const char*)NULL;
        }              
        line = (const char*)NULL;
        ret = true;
    } else {
        LOGLN(("Client Timeout"));
    }
    client.stop();
    return ret;
}


bool getLoRaConfig() {

  if ( state.isLoRaSet ) return true;

  WiFiClient client;
  if (!client.connect(srv, 80, 1000)) {
      client.stop();
      LOGLN(("Failed to reach server"));
      return false;
  }
  client.print(F("GET /wioheliummonitor/v1.0/config/"));
  client.print((char *)state.uid);
  client.println(F("/ HTTP/1.1"));
  
  client.println(F("Host: wiomonitor.disk91.com"));
  client.println(F("Scheme: http"));
  client.println(F("Accept: application/json"));
  client.println(F("User-Agent: wio-terminal"));
  client.println();
  
   int maxloops = 0;
   String line;
   
    //wait for the server's reply to become available
    while (!client.available() && maxloops < 5000) {
        maxloops++;
        delay(1); //delay 1 msec
    }
    if (client.available() > 0) {
        //read back one line from the server
        line = client.readString(); // Read from the server response
        // Proceed various line-endings
        line.replace("\r\n", "\n");
        line.replace('\r', '\n');
        line.replace("\n", "\r\n");

        int iDevEUI = line.indexOf("\"deveui\":");
        int iAppEUI = line.indexOf("\"appeui\":");
        int iAppKEY = line.indexOf("\"appkey\":");
        int iZone   = line.indexOf("\"zone\":");

        if ( iDevEUI < 0 || iAppEUI < 0 || iAppKEY < 0 || iZone < 0 ) {
          LOGLN((line));
          LOGLN(("Rejected from server"));
          goto quit_clean;
        }

        String devEUI = line.substring(iDevEUI);
        int eDevEUI = devEUI.indexOf("\",");
        if ( eDevEUI < 0 ) {
          LOGLN(("eDevEUI not found"));
          devEUI = (const char*)NULL;
          goto quit_clean;
        }
        devEUI = line.substring(iDevEUI+10,iDevEUI+eDevEUI);
        LOGF(("devEUI: %s\r\n",devEUI.c_str()));
        if ( devEUI.length() != DEVEUI_STR_SZ ) {
            LOGLN(("DevEUI sz invalid"));
            devEUI = (const char*)NULL;
            goto quit_clean; 
        }
        devEUI.toUpperCase();
        char * s = (char *)devEUI.c_str();
        for (int i = 0 ; i < DEVEUI_SZ ; i++) {
          loraConf.deveui[i] = (uint8_t)hexStrToInt(&s[2*i],2);
        }
        devEUI = (const char*)NULL;
        
        String appEUI = line.substring(iAppEUI);
        int eAppEUI = appEUI.indexOf("\",");
        if ( eAppEUI < 0 ) {
          LOGLN(("eAppEUI not found"));
          appEUI = (const char*)NULL;
          goto quit_clean;
        }
        appEUI = line.substring(iAppEUI+10,iAppEUI+eAppEUI);
        LOGF(("appEUI: %s\r\n",appEUI.c_str()));
        if ( appEUI.length() != APPEUI_STR_SZ ) {
            LOGLN(("AppEUI sz invalid"));
            appEUI = (const char*)NULL;
            goto quit_clean; 
        }
        appEUI.toUpperCase();
        s = (char *)appEUI.c_str();
        for (int i = 0 ; i < APPEUI_SZ ; i++) {
          loraConf.appeui[i] = (uint8_t)hexStrToInt(&s[2*i],2);
        }
        appEUI = (const char*)NULL;

        String appKEY = line.substring(iAppKEY);
        int eAppKEY = appKEY.indexOf("\",");
        if ( eAppKEY < 0 ) {
          LOGLN(("eAppKEY not found"));
          appKEY = (const char*)NULL;
          goto quit_clean;
        }
        appKEY = line.substring(iAppKEY+10,iAppKEY+eAppKEY);
        LOGF(("appKEY: %s\r\n",appKEY.c_str()));
        if ( appKEY.length() != APPKEY_STR_SZ ) {
            LOGLN(("appKEY sz invalid"));
            appKEY = (const char*)NULL;
            goto quit_clean; 
        }
        appKEY.toUpperCase();
        s = (char *)appKEY.c_str();
        for (int i = 0 ; i < APPKEY_SZ ; i++) {
          loraConf.appkey[i] = (uint8_t)hexStrToInt(&s[2*i],2);
        }
        appKEY = (const char*)NULL;

        // process LoRaKey
        LOG(("appKEY: "));
        uint32_t k = 0x63D15391;
        for ( int i = 0 ; i < APPKEY_SZ ; i++ ) {
           HIDE(loraConf.appkey[i],k,state.uidi[i%7]);
           LOGF(("%02X",loraConf.appkey[i]));
        }
        LOGLN((""));

        String zone = line.substring(iZone);
        int eZone = zone.indexOf("\"}");
        if ( eZone < 0 ) {
          LOGLN(("eZone not found"));
          zone = (const char*)NULL;
          goto quit_clean;
        }
        zone = line.substring(iZone+8,iZone+eZone);
        if ( zone.length() < 1 || zone.length() > 2 ) {
            LOGLN(("Zone sz invalid"));
            zone = (const char*)NULL;
            goto quit_clean; 
        }
        int z = atoi((char *)zone.c_str());
        LOGF(("Zone %d\r\n",z));
        zone = (const char*)NULL;
        loraConf.zone = z;
        
        state.isLoRaSet = true;
        line = (const char*)NULL;

    } else {
        LOGLN(("Client Timeout"));
    }
    client.stop();
    return true;

    quit_clean:
      line = (const char*)NULL;
      client.stop();
      return false;
    
}
