#include <stdio.h>
#include <contiki.h>
#include </dev/button-sensor.h>
PROCESS(hello_world_process, "Hello World! process");
AUTOSTART_PROCESSES(&hello_world_process);
PROCESS_THREAD(hello_world_process,ev,data)
{
  PROCESS_BEGIN();

  SENSOR_ACTIVATE(button_sensor);

  while(1){
    PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);
    printf("Hello World!\n");
  }
  PROCESS_END();
}
