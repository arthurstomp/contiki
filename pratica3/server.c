#include <string.h>
#include <stdlib.h>
#include "contiki-net.h"
static struct psock ps;
static char buffer[10];
  static
PT_THREAD(handle_connection(struct psock *p))
{
  PSOCK_BEGIN(p);
  PSOCK_SEND_STR(p, "Welcome, please type something and press return.\n");
  PSOCK_READTO(p, '\n');
  char *newstr = malloc(PSOCK_DATALEN(p));
  int i = 0;
  for(i = 0; i < PSOCK_DATALEN(p) ; i++){
    if(buffer[i] != '\n'){
      newstr[i] = toupper(buffer[i]);
    }else{
      newstr[i] = toupper(buffer[i]);
    }
  }
  printf("upper string %s",newstr);
  //PSOCK_SEND_STR(p, "[Server] - Got the following data: ");
  PSOCK_SEND(p, newstr, PSOCK_DATALEN(p));
  PSOCK_SEND_STR(p, "[Server] - Good bye!\r\n");
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
