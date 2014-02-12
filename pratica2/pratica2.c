#include <contiki.h>
#include <stdio.h>
#include <dev/button-sensor.h>
#include <dev/leds.h>

PROCESS(pratica2, "pratica 2");
AUTOSTART_PROCESSES(&pratica2);

PROCESS_THREAD(pratica2, ev, data)
{
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(button_sensor);
  leds_off(LEDS_BLUE);
  leds_off(LEDS_GREEN);
  leds_off(LEDS_RED);
  while(1){
    static struct etimer et;
    static uint32_t seconds = 2;
    static uint32_t count = 0;
    etimer_set(&et,CLOCK_SECOND*seconds);
    PROCESS_WAIT_EVENT();
    if(etimer_expired(&et)){
      count ++;
      etimer_reset(&et);
      printf("button count = %lu\n",count);
      if(count % 8 < 4){
        leds_off(LEDS_BLUE);
      }else{
        leds_on(LEDS_BLUE);
      }
      if(count % 4 < 2){
        leds_off(LEDS_GREEN);
      }else{
        leds_on(LEDS_GREEN);
      }
      if(count % 2 == 0){
        leds_off(LEDS_RED);
      }else{
        leds_on(LEDS_RED);
      }
    }
    if(ev == sensors_event && data == &button_sensor){
      count = -1;
    }
  }
  PROCESS_END();
}
