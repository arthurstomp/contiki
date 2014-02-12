#include"contiki.h"
#include"contiki-net.h"
#include<stdio.h>
#define UIP_APPCALL example1_app
static struct uip_conn *c;
static struct etimer et;
void example1_connect(void){    
  uip_ipaddr_t ipaddrServer;
  uip_ipaddr(&ipaddrServer, 172,16,2,0);
  c = tcp_connect(&ipaddrServer, uip_htons(1234), NULL);
  printf("Tentando conectar\n");      
}

void example1_app(void){
  printf("Chegou dados\n");
  uip_send("ok!\n", 4);
  printf("enviei ok\n");
}

void enviar_welcome(void){
  printf("Cliente enviando welcome\n");
  uip_send("Welcome!\n", 9);
  printf("Mensagem welcome enviada pelo cliente\n");
}


PROCESS(cliente_process, "Processo cliente");
AUTOSTART_PROCESSES(&cliente_process);

PROCESS_THREAD(cliente_process, ev, data){
      
  PROCESS_BEGIN();
        
  etimer_set(&et,CLOCK_SECOND);
  PROCESS_WAIT_EVENT_UNTIL(timer_expired(&et));
  etimer_reset(&et);        
  example1_connect();

  PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
  enviar_welcome();

  PROCESS_END();
}
                
