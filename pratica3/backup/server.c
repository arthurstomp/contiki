#include "contiki.h"
#include "contiki-net.h"
#include <stdio.h>
#include <string.h>
static struct psock ps;
static char buffer[100];

PROCESS(example_psock_server_process, "Example protosocket server");
static int handle_connection(struct psock *p)
{
  printf("connection_handler server\n");
  PSOCK_BEGIN(p);
  PSOCK_SEND_STR(p, "Welcome, please type something and press return.\n");
  PSOCK_READTO(p, '\n');
  printf("Server Got: %s",buffer);
  PSOCK_SEND_STR(p, "Got the following data: \n");
  PSOCK_SEND(p, buffer, PSOCK_DATALEN(p));
  PSOCK_SEND_STR(p, "Good bye!\n");

  PSOCK_CLOSE(p);
  PSOCK_END(p);
}
AUTOSTART_PROCESSES(&example_psock_server_process);
PROCESS_THREAD(example_psock_server_process, ev, data)
{
  PROCESS_BEGIN();
  tcp_listen(UIP_HTONS(1010));
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    if(uip_connected()) {
      PSOCK_INIT(&ps,buffer, sizeof(buffer));
      while(!(uip_aborted() || uip_closed() || uip_timedout())) {
        PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
        handle_connection(&ps);
      }
    }
  }
  PROCESS_END();
}
