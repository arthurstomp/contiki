#include <contiki.h>
#include <contiki-net.h>
#include <stdio.h>
PROCESS(simple_udp_client_process, "Simple UDP client process");
static struct uip_udp_conn *server_conn;
AUTOSTART_PROCESSES(&simple_udp_client_process);
PROCESS_THREAD(simple_udp_client_process, ev, data)
{
  PROCESS_BEGIN();
  server_conn = udp_new(NULL, UIP_HTONS(1234), NULL);
  udp_bind(server_conn, UIP_HTONS(3000));
  while(1){
    if(ev == tcpip_event){
      printf("tcpip_event");
    }
  }
  PROCESS_END();
}
