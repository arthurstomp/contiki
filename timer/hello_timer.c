#include <stdio.h>
#include <contiki.h>
#include <dev/button-sensor.h>
#include <dev/leds.h>


PROCESS(timer_process, "timer process");
AUTOSTART_PROCESSES(&timer_process);
PROCESS_THREAD(timer_process,ev,data)
{
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(button_sensor);
  leds_off(LEDS_GREEN);
  leds_on(LEDS_RED);
  printf("hello world\n");

  while(1){
    static struct etimer et;
    static uint32_t seconds = 5;
    PROCESS_WAIT_EVENT();
    if(ev == sensors_event && data == &button_sensor){
      etimer_set(&et,CLOCK_SECOND*seconds);
      printf("button pressed\n");
    }
    if(etimer_expired(&et)){
      leds_toggle(LEDS_GREEN);
      leds_toggle(LEDS_RED);
      printf("timer expired\n");
    }
  }

  PROCESS_END();
}
