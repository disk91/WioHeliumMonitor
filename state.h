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

#ifndef __STATE_H
#define __STATE_H

#define SSID_SZ     64
#define SSID_PWD_SZ 64
#define IP_SZ       16
#define HSNAME_SZ   24

#define MAXBUFFER    32
#define HSSTATE_OK    0 
#define HSSTATE_WARN  1
#define HSSTATE_ERROR 2
#define HSSTATE_UNKN  99

typedef struct s_state {

  uint8_t   ssid[SSID_SZ];          // WiFi ssid
  uint8_t   password[SSID_PWD_SZ];  // WiFi password
  uint8_t   extPingIp[IP_SZ];       // an IP address on Internet 
  uint8_t   intPingIp[IP_SZ];       // the hotspot IP address on LAN
  uint8_t   uid[15];                // MAC address in hex + Checksum
  uint8_t   uidi[7];                // MAC addresse in byte + Checksum

  uint8_t   avgExtPing[MAXBUFFER];  // ping rtt history / 8 ms (apparently the step we have)
  uint8_t   avgIntPing[MAXBUFFER];

  int       readPtr;          // circular buffer
  int       writePtr;
  int       elements;         // number of data in buffer
  bool      hasRefreshed;     // Data has refreshed
  bool      withSound;        // Enable sound

  uint8_t   intState;         // internal network state (5 consecutive failed = down)
  uint8_t   extState;         // external network state (5 consecutive failed = down)

  uint8_t   hsName[HSNAME_SZ];  // Hotspot name associated
  uint8_t   hsState;            // watchium Hotspot state 0 = good / 1 Warn / 2 Error
  bool      isRegistered;     // true when the device is associated with a hotspot

  bool      e5Detected;
  bool      isLoRaSet;        // true when the lora config has been retreived
  bool      isLoRaInit;       // true when lora stack has been initialized
  bool      isLoRaConnected;  // true when joined

  uint32_t  uptime;           // total uptime in second (for debug)
  
} state_t;

extern state_t state;

bool readConfig();
void storeConfig();


void initState();
void runMonitor();
bool reportData();
void runLoRaPing();

uint8_t getIndexInBuffer(int i);

#endif
