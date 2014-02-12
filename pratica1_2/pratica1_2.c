#include <contiki.h>
#include <stdio.h>
#include <dev/button-sensor.h>
#include <dev/leds.h>

PROCESS(pratica1_2, "pratica 1 2");
AUTOSTART_PROCESSES(&pratica1_2);

PROCESS_THREAD(pratica1_2, ev, data)
{
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(button_sensor);
  leds_off(LEDS_BLUE);
  leds_off(LEDS_GREEN);
  leds_off(LEDS_RED);
  while(1){
    static struct etimer et;
    static uint32_t seconds = 5;
    static uint32_t count = 0;
    PROCESS_WAIT_EVENT();
    if(ev == sensors_event && data == &button_sensor){
      count = 1 + count;
      etimer_set(&et,CLOCK_SECOND*seconds);
      printf("button count = %lu\n",count);
    }
    if(etimer_expired(&et) && count % 2 == 0){
      leds_off(LEDS_BLUE);
      leds_off(LEDS_GREEN);
      leds_off(LEDS_RED);
    }
    if(etimer_expired(&et) && count % 2 == 1){
      leds_on(LEDS_BLUE);
      leds_on(LEDS_GREEN);
      leds_on(LEDS_RED);
    }
  }
  PROCESS_END();
}
