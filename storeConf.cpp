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
#include <FlashStorage.h>
#include "config.h"
#include "state.h"

#define MAGIC 0xD254
#define VERSION 0x01

#define FLAG_BACKUPED 0x01
#define FLAG_HIDE_KEY 0x02
#define FLAG_GPS_OK   0x04

typedef struct {
  uint16_t  magic;
  uint8_t   version;
  uint8_t   csum;     // checksum after byte 4 ...
  
  uint8_t   deveui[8];   // device EUI
  uint8_t   appeui[8];   // App EUI
  uint8_t   appkey[16];  // App KEY
  uint8_t   zone;        // Zone EU868... cf loracom.h ZONE_XXX
  uint8_t   flag;        // to store some flags

  uint8_t   ssid[SSID_SZ];          // WiFi ssid
  uint8_t   password[SSID_PWD_SZ];  // WiFi password
  uint8_t   extPingIp[IP_SZ];       // an IP address on Internet 
  uint8_t   intPingIp[IP_SZ];       // the hotspot IP address on LAN
  
} Config;


FlashStorage(my_flash_store, Config);

uint8_t computeCSum(Config * c) {
    uint8_t * t = (uint8_t *)c;
    uint8_t csum = 0;
    for ( int k = 4 ; k < sizeof(Config); k++) {
      csum += t[k];
    }
    return csum;
}


// Load configuration from flash
// return true if loaded false if default must be set
bool readConfig() {
  Config c = my_flash_store.read();
  uint8_t csum = computeCSum(&c);
  if ( c.magic == MAGIC && c.version == VERSION && c.csum == csum ) {
    memcpy(state.ssid,c.ssid,SSID_SZ);
    memcpy(state.password,c.password,SSID_PWD_SZ);
    memcpy(state.extPingIp,c.extPingIp,IP_SZ);
    memcpy(state.intPingIp,c.intPingIp,IP_SZ);
  } else {
    return false;
  }
  return true;
}

void storeConfig() {
  Config c;
  c.magic = MAGIC;
  c.version = VERSION;
  memcpy(c.ssid, state.ssid, SSID_SZ);
  memcpy(c.password, state.password, SSID_PWD_SZ);
  memcpy(c.extPingIp, state.extPingIp, IP_SZ);
  memcpy(c.intPingIp, state.intPingIp, IP_SZ);
  c.csum = computeCSum(&c);
  my_flash_store.write(c);
  LOGLN(("Config saved"));
}
