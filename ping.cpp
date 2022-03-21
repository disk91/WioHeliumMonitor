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

// TODO : to get ping working, the file SeeeD_Arduino_rpcWifi/ping.c 
// needs to be modified with the following ping_recv function
// 
// The main problem is related to the recv function not respecting the 1000 ms timeout and exiting after 8ms

/*
static void ping_recv(int s) {
    char buf[64];
    int fromlen, len;
    struct sockaddr_in from;
    struct ip_hdr *iphdr;
    struct icmp_echo_hdr *iecho = NULL;
    char ipa[16];
    struct timeval begin;
    struct timeval end;
    uint64_t micros_begin;
    uint64_t micros_end;
    float elapsed;

    // Register begin time
    gettimeofday(&begin, NULL);
    uint32_t start=millis();
    uint32_t stop=start;

    // Send
    while ((len = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr*)&from, (socklen_t*)&fromlen)) > 0 || ((uint32_t)(stop-start)) < 1000 ) {
        stop=millis();
        if (len >= (int)(sizeof(struct ip_hdr) + sizeof(struct icmp_echo_hdr))) {
            // Register end time
            gettimeofday(&end, NULL);

            /// Get from IP address
            ip4_addr_t fromaddr;
            inet_addr_to_ip4addr(&fromaddr, &from.sin_addr);

            strcpy(ipa, inet_ntoa(fromaddr));

            // Get echo
            iphdr = (struct ip_hdr *)buf;
            iecho = (struct icmp_echo_hdr *)(buf + (IPH_HL(iphdr) * 4));

            // Print ....
            if ((iecho->id == PING_ID) && ( ntohs(iecho->seqno) == ping_seq_num) ) {
                received++;

                // Get elapsed time in milliseconds
                micros_begin = begin.tv_sec * 1000000;
                micros_begin += begin.tv_usec;

                micros_end = end.tv_sec * 1000000;
                micros_end += end.tv_usec;

                elapsed = (float)(micros_end - micros_begin) / (float)1000.0;

                // Update statistics
                // Mean and variance are computed in an incremental way
                if (elapsed < min_time) {
                    min_time = elapsed;
                }

                if (elapsed > max_time) {
                    max_time = elapsed;
                }

                last_mean_time = mean_time;
                mean_time = (((received - 1) * mean_time) + elapsed) / received;

                if (received > 1) {
                    var_time = var_time + ((elapsed - last_mean_time) * (elapsed - mean_time));
                }

                // Print ...
                //rpc_printf("%d bytes from %s: icmp_seq=%d time=%.3f ms\r\n", len, ipa,
                //     ntohs(iecho->seqno), elapsed
                //);

                //rpc_printf("%d code %d type %d seq_exp %d dur\r\n",ICMPH_CODE(iecho), ICMPH_TYPE(iecho), ping_seq_num, (stop-start));
                return;
            } else {
                //rpc_printf("receive id %X, seq %d wseq %d\r\n",iecho->id,ntohs(iecho->seqno),ping_seq_num - 1);
                // TODO
            }
        } else {
             //   rpc_printf("receive len %X",len);            
        }
    }
    //stop=millis();
    //rpc_printf("### quitting after %d ms\r\n", (stop-start));  

    if (len < 0) {
//        log_d("Request timeout for icmp_seq %d\r\n", ping_seq_num);
    }
}

 */

/* 
 *  Garbage collection in erpc_port_freertos
 */

 /*

// -------------------------
// Hack it
//#include <Arduino.h>

void * allocHisto[128];
int ptr = 0;

void erpcs_initMallocHisto() {
  ptr = 0;
}

void erpc_addMalloc(void * m) {
  if ( ptr == 128 ) return;
  allocHisto[ptr] = m;
  ptr++;
}

void erpc_delMalloc(void * m) {
  for ( int i = 0 ; i < ptr ; i++ ) {
    if ( allocHisto[i] == m ) {
      allocHisto[i] = NULL;
      break;
    }
  }
}

void erpc_cleanMalloc() {
  int cleaned = 0;
  for ( int i = 0 ; i < ptr ; i++ ) {
    if ( allocHisto[i] != NULL ) {
      cleaned++;
      vPortFree(allocHisto[i]);
    }
  }
  //Serial.printf("Cleaned %d\r\n",cleaned);
  ptr = 0;
}
// ----

void *erpc_malloc(size_t size)
{
    if(size == 0)
    {
        return NULL;
    }
    
    void *p = pvPortMalloc(size);
    // Hack it
    if ( p != NULL ) {
      erpc_addMalloc(p);
    }
    // ...
    return p;
}

void erpc_free(void *ptr)
{
  // Hack ...
  erpc_delMalloc(ptr);
  // ...
    vPortFree(ptr);
}

 
  */

 
