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
#ifndef __LORA_COM__
#define __LORA_COM__

// Common
void loraSetup();
void loraLoop();
bool loraInit();
void do_send(uint8_t port, uint8_t * data, uint8_t sz, uint8_t _dr, uint8_t pwr, bool acked, uint8_t retries );
uint32_t nextPossibleSendMs();

// LoRa E5 specific
bool loraE5Setup();
bool loraE5QuickSetup();
void do_sendE5(uint8_t port, uint8_t * data, uint8_t sz, uint8_t _dr, uint8_t pwr, bool acked, uint8_t retries );
void loraLoopE5();
boolean isLoRaE5Connected();

#define DEVEUI_STR_SZ 16
#define APPEUI_STR_SZ 16
#define APPKEY_STR_SZ 32

#define DEVEUI_SZ 8
#define APPEUI_SZ 8
#define APPKEY_SZ 16

typedef struct {
  uint8_t deveui[DEVEUI_SZ];
  uint8_t appeui[APPEUI_SZ];
  uint8_t appkey[APPKEY_SZ];
  uint8_t zone;
} loraConf_t;

extern loraConf_t loraConf;

enum e_state {
  NOT_JOINED    = 1,    // Not connected yet
  JOIN_FAILED   = 2,    // Connection failed
  JOINING       = 3,    // Currently running the joining procedure
  JOINED        = 4,    // Joined, waiting for action
  IN_TX         = 5,    // Transmitting 
  IN_RPT        = 6,    // 1st transmission failed, running a repeat
  EMPTY_DWNLINK = 7,    // Pending downlink to retrieve before goint to next point

  UKN = 255
};

#define ZONE_UNDEFINED  0
#define ZONE_EU868      1
#define ZONE_US915      2
#define ZONE_AS923_1    3
#define ZONE_AS923_2    4
#define ZONE_AS923_3    5
#define ZONE_AS923_4    6
#define ZONE_KR920      7
#define ZONE_IN865      8
#define ZONE_AU915      9
#define ZONE_LATER      99
#define ZONE_MAX        ZONE_AU915
#define ZONE_MIN        ZONE_EU868

#endif
