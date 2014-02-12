#include <contiki.h>
#include <contiki-net.h>
#include <stdio.h>
PROCESS(simple_udp_process, "Example process");
AUTOSTART_PROCESSES(&simple_udp_process);
static struct etimer timer;
PROCESS_THREAD(simple_udp_process, ev, data)
{
  static struct uip_udp_conn *c;
  PROCESS_BEGIN();
  c = udp_broadcast_new(UIP_HTONS(4321), NULL);
  while(1) {
    tcpip_poll_udp(c);
    PROCESS_WAIT_EVENT_UNTIL(ev==tcpip_event);
    printf("tcpip_event\n");

    uip_send("Hello", 5);
  }

  PROCESS_END();
}
