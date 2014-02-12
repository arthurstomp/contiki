#include <contiki.h>
#include <stdio.h>
#include <dev/button-sensor.h>
#include <dev/leds.h>

PROCESS(hello_world_led, "hello world led");
AUTOSTART_PROCESSES(&hello_world_led);

PROCESS_THREAD(hello_world_led, ev, data)
{
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(button_sensor);
  leds_off(LEDS_GREEN);
  leds_on(LEDS_RED);
  printf("hello world\n");
  while(1){
    PROCESS_WAIT_EVENT();
    if(ev == sensors_event && data == &button_sensor){
      leds_toggle(LEDS_GREEN);
      leds_toggle(LEDS_RED);
      printf("button pressed\n");
    }
  }
  PROCESS_END();
}
