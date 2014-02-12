#include<stdio.h>
#include"contiki.h"
#include"contiki-net.h"
#define UIP_APPCALL       example2_app
#define UIP_APPSTATE_SIZE sizeof(struct example2_state)

static struct example2_state{
      enum {WELCOME_SENT, WELCOME_ACKED} state;
};

void example2_listen(void){
  tcp_listen(UIP_HTONS(1234));
  printf("Escutando\n");
}


void example2_app(void) { 
  printf("entrei na appcall\n");
  if(uip_newdata() || uip_rexmit()){
    printf("Chegou Dados no servidor\n");
    struct example2_state *s;
    s = (struct example2_state *)&uip_conn->appstate; 
    if(uip_connected()) {
      uip_send("Welcome!\n", 9);
      printf("está conectado\n");
      s->state = WELCOME_SENT;
      return;
    }
    if(uip_acked() && s->state == WELCOME_SENT) {
      s->state = WELCOME_ACKED;
    } 
    if(uip_newdata()) { 
      uip_send("ok\n", 3);
      printf("server: send ok\n");
    }
    if(uip_rexmit()) {
      switch(s->state) { 
        case WELCOME_SENT: 
          uip_send("Welcome!\n", 9);
          printf("server: remix send Welcome!\n");
          break;
        case WELCOME_ACKED: 
          uip_send("ok\n", 3); 
          printf("server: remix send ok\n");
          break; } 
    }
  }
}
PROCESS(exemplo_2_process, "exemplo 2 servidor");
AUTOSTART_PROCESSES(&exemplo_2_process);
PROCESS_THREAD(exemplo_2_process, ev, data){
  PROCESS_BEGIN();
  uip_init();
  example2_listen();
  while(1){
    PROCESS_WAIT_EVENT_UNTIL(ev==tcpip_event);
    printf("Iniciando a escuta\n");
    example2_app();
  }
              
  PROCESS_END();
}
