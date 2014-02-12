#include <contiki.h>
#include <contiki-net.h>
#include <stdio.h>
PROCESS(simple_udp_client_process, "Simple UDP client process");
static struct simple_udp_connection client_conn;
static void tcpip_handler(void)
{
  char *str;

  if(uip_newdata()) {
    str = uip_appdata;
    str[uip_datalen()] = '\0';
    printf("Response from the server: '%s'\n", str);
  }
}

static void receiver(struct simple_udp_connection *c,
                     const uip_ipaddr_t *sender_addr,
                     uint16_t sender_port,
                     const uip_ipaddr_t *receiver_addr,
                     uint16_t receiver_port,
                     const uint8_t *data,
                     uint16_t datalen)
{
  printf("callback server datalen = %u\n",datalen);
}
static void set_connection_address(uip_ipaddr_t *ipaddr){
  uip_ipaddr(ipaddr,172,16,3,0);
}
static struct etimer timer;
AUTOSTART_PROCESSES(&simple_udp_client_process);
PROCESS_THREAD(simple_udp_client_process, ev, data)
{
  uip_ipaddr_t ipaddr;
  PROCESS_BEGIN();
  set_connection_address(&ipaddr);
  simple_udp_register(&client_conn, 1234, NULL, 1234, receiver);
  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 1);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    uip_create_linklocal_allnodes_mcast(&ipaddr);
    simple_udp_sendto(&client_conn,"test",4,&ipaddr);
  }

  PROCESS_END();
}
