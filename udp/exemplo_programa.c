#include "contiki.h"
#include "contiki-net.h"
PROCESS(example_program_process, "Example process");
static struct etimer timer;
AUTOSTART_PROCESSES(&example_program_process);
PROCESS_THREAD(example_program_process, ev, data)
{
  static struct uip_udp_conn *c;
  PROCESS_BEGIN();
  c = udp_broadcast_new(UIP_HTONS(4321), NULL);
  while(1) {
    etimer_set(&timer, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    tcpip_poll_udp(c);
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    uip_send("Hello", 5);
  }

  PROCESS_END();
}
