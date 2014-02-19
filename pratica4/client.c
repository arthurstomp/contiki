#include "contiki-net.h"
#include "dev/button-sensor.h"
#include <stdio.h>

static struct psock ps;
static unsigned int *buffer;
static unsigned int count = -1;

PROCESS(example_psock_client_process, "Example protosocket client");

static int handle_connection(struct psock *p)
{
  PSOCK_BEGIN(p);

  count++;
  PSOCK_READBUF(p);
  printf("[Client] - Got : %u\n",buffer);

  PSOCK_SEND(p,count,sizeof(count));
  printf("[Client] - SEND %u\n",count);

  PSOCK_END(p);
}

AUTOSTART_PROCESSES(&example_psock_client_process);
PROCESS_THREAD(example_psock_client_process, ev, data)
{
  uip_ipaddr_t addr;

  PROCESS_BEGIN();
  SENSORS_ACTIVATE(button_sensor);

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
      PROCESS_WAIT_EVENT();
      if(ev == sensors_event && data == &button_sensor){
        printf("[Client] - button pressed\n");
        handle_connection(&ps);
      }
    } while(!(uip_closed() || uip_aborted() || uip_timedout()));

    printf("\nConnection closed.\n");
  }
  PROCESS_END();
}

