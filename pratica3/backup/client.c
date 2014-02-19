#include "contiki.h"
#include "contiki-net.h"
#include<stdio.h>
#include<string.h>
static struct psock *p;
static char buffer[100];
PROCESS(sample_protosocket_process,"Example protosocket client");
static int handle_connection(struct psock *p)
{
  printf("handle_connection client\n");
  PSOCK_BEGIN(p);
  PSOCK_SEND_STR(p,"GET / HTTP/1.0\n");
  PSOCK_READTO(p,'\n');
  printf("Client Got: %s\n",buffer);
  //PSOCK_SEND_STR(p,"Server: Contike sample protothread");
  //PSOCK_SEND_STR(p,"\n");
  //while(1){
  //PSOCK_SEND(p,"test\n",5);
  //}
  PSOCK_END(p);
}
AUTOSTART_PROCESSES(&sample_protosocket_process);
PROCESS_THREAD(sample_protosocket_process,ev,data)
{
  uip_ipaddr_t ipaddr;
  PROCESS_BEGIN();
  uip_ipaddr(&ipaddr,172,16,2,0);
  tcp_connect(&ipaddr,UIP_HTONS(5555),NULL);
  PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
  printf("connecting...\n");
  if(uip_aborted() || uip_timedout() || uip_closed()){
    printf("Could not establish connection\n");
  }else if(uip_connected()){
    printf("Connected\n");
    PSOCK_INIT(p, buffer, sizeof(buffer));
    do{
      handle_connection(p);
      PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    }while(!(uip_close() || uip_aborted() || uip_timedout()));
    printf("\nConnection closed.\n");
  }
  PROCESS_END();
}

