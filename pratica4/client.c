#include "contiki-net.h"
#include <stdio.h>

static struct psock ps;
static int *buffer;

PROCESS(example_psock_client_process, "Example protosocket client");

static int handle_connection(struct psock *p)
{
  PSOCK_BEGIN(p);

  //This line is for trigger the server, generate a tcpip_event
  PSOCK_SEND_STR(p, "hello server\n");

  while(1){
    //PSOCK_SEND(p, "test\n", 5);
    PSOCK_WAIT_UNTIL(p,PSOCK_NEWDATA(p));
    if(PSOCK_NEWDATA(p)){
      PSOCK_READBUF_LEN(p,PSOCK_DATALEN(p) );
      printf("[Cliente] - Got: %i", *buffer);
    }
  }

  PSOCK_END(p);
}
AUTOSTART_PROCESSES(&example_psock_client_process);
PROCESS_THREAD(example_psock_client_process, ev, data)
{
  uip_ipaddr_t addr;

  PROCESS_BEGIN();

  uip_ipaddr(&addr, 172,16,2,0);
  tcp_connect(&addr, UIP_HTONS(5555), NULL);

  printf("Connecting...\n");
  PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);

  if(uip_aborted() || uip_timedout() || uip_closed()) {
    printf("Could not establish connection...\n");
  } else if(uip_connected()) {
    printf("Connected\n");

    PSOCK_INIT(&ps, buffer, sizeof(buffer));

    do {
      handle_connection(&ps);
      PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    } while(!(uip_closed() || uip_aborted() || uip_timedout()));

    printf("\nConnection closed.\n");
  }
  PROCESS_END();
}

