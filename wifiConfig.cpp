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
#include "rpcWiFi.h"
#include "ui.h"
#include "state.h"

WiFiServer server(80);
const char * srv_ssid = "WioHeliumMonitor";
const char index_html1[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>WioHeliumMonitor Configuration page</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/config">
    <label for="ssid">Choose a network:</label>
    <select id="ssid" name="ssid_l">
)rawliteral";
const char index_html2[] PROGMEM = R"rawliteral(
    </select><br/>
    <label for="ssid_s">Or network name:</label>
    <input id="ssid_s" type="text" name="ssid_n"><br/>
    <label for="passwd">Password:</label>
    <input id="passwd" type="text" name="password"><br/>

    <label for="extIp">External IP:</label>
    <input id="extIp" type="text" name="extip" value="8.8.8.8" maxlength="15"><br/>    
    <label for="intIp">Hotspot IP:</label>
    <input id="intIp" type="text" name="intip" maxlength="15"><br/>    
    
    <input type="submit" value="Submit"><br/>    
  </form>
</body></html>)rawliteral";

#define PATTERN_ENTRIES 35
String urlDecode(String s) {
  char patterns[PATTERN_ENTRIES*2][4] = {
    "%20", " ",  // [0]
    "%21", "!",
    "%22", "\"",
    "%23", "#",
    "%24", "$",
    "%25", "%",
    "%26", "&",
    "%27", "'",
    "%28", "(",
    "%29", ")",
    "%2A", "*",  // [10]
    "%2B", "+",
    "%2C", ",",
    "%2D", "-",
    "%2E", ".",
    "%2F", "/",
    "%2F", "/",
    "%3A", ":",
    "%3B", ";",
    "%3C", "<",
    "%3D", "=", // [20]
    "%3E", ">",
    "%3F", "?",
    "%40", "@",
    "%5B", "[", 
    "%5C", "\\",
    "%5D", "]",
    "%5E", "^",
    "%5F", "_",
    "%60", "`",
    "%7B", "{", // [30]
    "%7C", "|",
    "%7D", "}", 
    "%7E", "~",
    "%80", "€"  // [34]
  };

  if ( s.indexOf("%") >= 0 ) {
   for ( int i = 0 ; i < PATTERN_ENTRIES ; i++ ) {
    int p;
    while ( (p = s.indexOf( patterns[2*i] ) ) >= 0 ) {
      String t = s.substring(0,p);
      t+= patterns[1+2*i];
      t+= s.substring(p+3);
      s = t;
    }
   }  
  }
  return s;
}


void wifiConfig() {

    displaySetup();

    LOGLN(("Scan Wifi"));
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    int n = WiFi.scanNetworks();
    String ssids = "";
    if (n > 0) {
     for (int i = 0; i < n; ++i) {
          if ( WiFi.SSID(i).length() > 0 ) {
            ssids += "<option value='";
            ssids += WiFi.SSID(i);
            ssids += "'>";
            ssids += WiFi.SSID(i);
            ssids += "</option>";
          }
          delay(10);
     }
    }

    LOGLN(("Create server"));
    WiFi.softAP(srv_ssid);
    IPAddress myIp = WiFi.softAPIP();
    LOG(("IP: "));
    LOGLN((myIp));
    server.begin();

    bool configDone = false;
    char ssid[64];
    char pass[64];
    char intip[16];
    char extip[16];
    while ( !configDone ) {
      WiFiClient client = server.available();   
      if (client) {                             
          String currentLine = "";                
          while (client.connected()) {            
              if (client.available()) {             
                  char c = client.read();             
                  LOG((c));                    
                  if (c == '\n') {
                      // if the current line is blank, you got two newline characters in a row.
                      // that's the end of the client HTTP request, so send a response:
                      if (currentLine.length() == 0) {
                          client.println("HTTP/1.1 200 OK");
                          client.println("Content-type:text/html");
                          client.println();
   
                          // the content of the HTTP response follows the header:
                          client.print(index_html1);
                          client.print(ssids);
                          client.print(index_html2);

                          // The HTTP response ends with another blank line:
                          client.println();
                          // break out of the while loop:
                          break;
                      } else {    // if you got a newline, then clear currentLine:
                          // test if the GET line
                          if ( currentLine.length() > 20 && currentLine.startsWith("GET /config?") ) {
                            currentLine = urlDecode(currentLine);
                            if (    currentLine.indexOf("ssid_l=") >= 0 
                                 && currentLine.indexOf("ssid_n=") >= 0 
                                 && currentLine.indexOf("password=") >= 0 
                                 && currentLine.indexOf("intip=") >= 0
                                 ) {
                              int i = currentLine.indexOf("ssid_n=")+7;
                              int k = 0;
                              while ( k < 62 && currentLine.charAt(i) != '&' && currentLine.charAt(i) != ' ') {
                                ssid[k] = currentLine.charAt(i);
                                k++;
                                i++;
                              }
                              ssid[k] = '\0';
                              if ( k == 0 ) {
                                 i = currentLine.indexOf("ssid_l=")+7;
                                 while ( k < 62 && currentLine.charAt(i) != '&' && currentLine.charAt(i) != ' ') {
                                   ssid[k] = currentLine.charAt(i);
                                   k++;
                                   i++;
                                 }
                                 ssid[k] = '\0';
                              }
                              i = currentLine.indexOf("password=")+9;
                              k=0;
                              while ( k < 62 && (currentLine.charAt(i) != '&' && currentLine.charAt(i) != ' ')) {
                                 pass[k] = currentLine.charAt(i);
                                 k++;
                                 i++;
                              }
                              pass[k] = '\0';

                              i = currentLine.indexOf("intip=")+6;
                              k=0;
                              while ( k < 16 && (currentLine.charAt(i) != '&' && currentLine.charAt(i) != ' ')) {
                                 intip[k] = currentLine.charAt(i);
                                 k++;
                                 i++;
                              }

                              i = currentLine.indexOf("extip=")+6;
                              k=0;
                              while ( k < 16 && (currentLine.charAt(i) != '&' && currentLine.charAt(i) != ' ')) {
                                 extip[k] = currentLine.charAt(i);
                                 k++;
                                 i++;
                              }
                              if ( k < 7 ) {
                                strcpy(extip,"8.8.8.8"); 
                              }

                              
                              LOGF(("SSID : %s\r\n",ssid));
                              LOGF(("PASS : %s\r\n",pass));
                              LOGF(("INTIP : %s\r\n",intip));
                              LOGF(("EXTIP : %s\r\n",extip));
                              strcpy((char*)state.ssid,ssid);
                              strcpy((char*)state.password,pass);
                              strcpy((char*)state.extPingIp,extip);
                              strcpy((char*)state.intPingIp,intip);
                              storeConfig();
                              configDone = true;
                            }
                          }
                          currentLine = "";
                      }
                  } else if (c != '\r') {  // if you got anything else but a carriage return character,
                      currentLine += c;      // add it to the end of the currentLine
                  }   
              }
          }
          // close the connection:
          client.stop();
      }
      
    }
    // out 
  
}
 
