#include <contiki.h>
#include <contiki-net.h>
#include <stdio.h>
PROCESS(simple_udp2_server_process, "Simple UDP server process");

static struct simple_udp_connection broadcast_connection;
static struct etimer timer;
static void receiver(struct simple_udp_connection *c,
                     const uip_ipaddr_t *sender_addr,
                     uint16_t sender_port,
                     const uip_ipaddr_t *receiver_addr,
                     uint16_t receiver_port,
                     const uint8_t *data,
                     uint16_t datalen)
{
  printf("callback\n");
}
AUTOSTART_PROCESSES(&simple_udp2_server_process);
PROCESS_THREAD(simple_udp2_server_process, ev, data)
{
  uip_ipaddr_t addr;
  PROCESS_BEGIN();
  simple_udp_register(&broadcast_connection, 1234,NULL,1234, receiver);
  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 1);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    uip_create_linklocal_allnodes_mcast(&addr);
    simple_udp_sendto(&broadcast_connection,"test",4,&addr);
  }

  PROCESS_END();
}
