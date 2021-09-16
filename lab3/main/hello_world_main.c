#include <stdio.h>
#include "time.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#define duration  1000

void task( void * pv )
{
    for ( ;; )
    {
	  clock_t begin = clock();
      printf(( char * ) pv);
	  printf("Begin: %f\n",(double)((double)begin/ CLOCKS_PER_SEC));
      printf("Preemption: %d\n",configUSE_PREEMPTION);
      printf("Time slice: %d\n",configUSE_TIME_SLICING);
	  while((double)(clock()-begin)<duration)
	  {}
    printf("End %f of %f\n",(double)((double)clock()/ CLOCKS_PER_SEC),(double)((double)begin/ CLOCKS_PER_SEC)); 
    vTaskDelay(2000 / portTICK_RATE_MS);
	}
}

static const char * pcTextForTask1 = " Task 1 is running \r\n";
static const char * pcTextForTask2 = " Task 2 is running \r\n";
static const char * pcTextForTask3 = " Task 3 is running \r\n";
void app_main ( void )
{
  xTaskCreatePinnedToCore(&task,  "Task 1", 10000,( void *) pcTextForTask1 , 1, NULL ,1);
  xTaskCreatePinnedToCore(&task, "Task 2", 10000,( void *) pcTextForTask2 , 2, NULL ,1);//task2 
  xTaskCreatePinnedToCore(&task, "Task 3", 4096,( void *) pcTextForTask3 , 2, NULL ,1);//and task3 same priority
}     