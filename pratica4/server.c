#include <string.h>
#include <stdlib.h>
#include "contiki-net.h"
#include <dev/leds.h> 
static struct psock ps;
static int *buffer;

void lightUpLed(int c){
  if(c % 8 < 4){
    leds_off(LEDS_BLUE);
  }else{
    leds_on(LEDS_BLUE);
  }
  if(c % 4 < 2){
    leds_off(LEDS_GREEN);
  }else{
    leds_on(LEDS_GREEN);
  }
  if(c % 2 == 0){
    leds_off(LEDS_RED);
  }else{
    leds_on(LEDS_RED);
  }
}

static PT_THREAD(handle_connection(struct psock *p))
{
  PSOCK_BEGIN(p);
  printf("[Server] - handle connection\n");
    PSOCK_WAIT_UNTIL(p,PSOCK_NEWDATA(p));
    if(PSOCK_NEWDATA(p)){
      PSOCK_READBUF_LEN(p,PSOCK_DATALEN(p) );
      printf("[Server] - Got: %i\n", *buffer);
      lightUpLed(*buffer);
    }
  PSOCK_CLOSE(p);
  PSOCK_END(p);
}
PROCESS(example_psock_server_process, "Example protosocket server");
AUTOSTART_PROCESSES(&example_psock_server_process);
PROCESS_THREAD(example_psock_server_process, ev, data)
{
  PROCESS_BEGIN();
  leds_off(LEDS_BLUE);
  leds_off(LEDS_GREEN);
  leds_off(LEDS_RED);
  tcp_listen(UIP_HTONS(5555));
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    if(uip_connected()) {
      PSOCK_INIT(&ps, buffer, sizeof(buffer));
      while(!(uip_aborted() || uip_closed() || uip_timedout())) {
        PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
        handle_connection(&ps);
      }
    }
  }
  PROCESS_END();
}
