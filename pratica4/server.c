#include <string.h>
#include <stdlib.h>
#include "contiki-net.h"
static struct psock ps;
static unsigned int *buffer;
  static
PT_THREAD(handle_connection(struct psock *p))
{
  PSOCK_BEGIN(p);
  PSOCK_SEND(p,0,sizeof(int));
  printf("[Server] - send 0\n");
  PSOCK_READBUF_LEN(p,sizeof(uint8_t));
  PSOCK_CLOSE(p);
  PSOCK_END(p);
}
PROCESS(example_psock_server_process, "Example protosocket server");
AUTOSTART_PROCESSES(&example_psock_server_process);
PROCESS_THREAD(example_psock_server_process, ev, data)
{
  PROCESS_BEGIN();
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
