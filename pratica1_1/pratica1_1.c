#include <contiki.h>
#include <stdio.h>
#include <dev/button-sensor.h>
#include <dev/leds.h>

PROCESS(pratica1_1, "pratica 1 1");
AUTOSTART_PROCESSES(&pratica1_1);

PROCESS_THREAD(pratica1_1, ev, data)
{
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(button_sensor);
  leds_off(LEDS_BLUE);
  leds_off(LEDS_GREEN);
  leds_off(LEDS_RED);
  while(1){
    static uint32_t count = 0;
    PROCESS_WAIT_EVENT();
    if(ev == sensors_event && data == &button_sensor){
      count = 1 + count;
      if(count % 2 == 0){
        leds_off(LEDS_BLUE);
        leds_off(LEDS_GREEN);
        leds_off(LEDS_RED);
      }
      if(count % 2 == 1){
        leds_on(LEDS_BLUE);
        leds_on(LEDS_GREEN);
        leds_on(LEDS_RED);
      }
      printf("button count = %lu\n",count);
    }
  }
  PROCESS_END();
}
