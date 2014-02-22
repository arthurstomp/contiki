#include "contiki-net.h"
#include <stdio.h>
#include <dev/button-sensor.h>

static struct psock ps;
static int *buffer;
static int count = -1;

PROCESS(example_psock_client_process, "Example protosocket client");

static int handle_connection(struct psock *p)
{
  PSOCK_BEGIN(p);
  printf("[Client] - handle connection\n");

  int *n;
  count ++;
  n = &count;

  PSOCK_SEND(p,n,sizeof(n));

  PSOCK_END(p);
}
AUTOSTART_PROCESSES(&example_psock_client_process);
PROCESS_THREAD(example_psock_client_process, ev, data)
{
  uip_ipaddr_t addr;

  PROCESS_BEGIN();
  SENSORS_ACTIVATE(button_sensor);

  while(1){
    PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);
    uip_ipaddr(&addr, 172,16,2,0);
    tcp_connect(&addr, UIP_HTONS(5555), NULL);

    printf("Connecting...\n");
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);

    if(uip_aborted() || uip_timedout() || uip_closed()) {
      printf("Could not establish connection...\n");
    } else if(uip_connected()) {
      printf("Connected\n");

      PSOCK_INIT(&ps, buffer, sizeof(buffer));

      PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
      handle_connection(&ps);
      printf("\nConnection closed.\n");
    }
  }
  PROCESS_END();
}

