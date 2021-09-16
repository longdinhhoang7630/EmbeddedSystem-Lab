#include <stdio.h>
#include <time.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include<freertos/timers.h>

int TIME_1 = 200;
int TIME_2 = 300;
int repeat1 = 10;
int repeat2= 5;
xTimerHandle  two_second_timer = NULL;
xTimerHandle  three_second_timer = NULL;
void task (xTimerHandle  pxTimer)
{
    int timer = (int) pvTimerGetTimerID( pxTimer );
    if (timer == 1)
    {
        static int count1 = 0;
        if (count1 < repeat1)
        {
            printf("Ahihi at %d ms\n",(int) clock());
            xTimerReset(two_second_timer,0);
            count1++;
        }
    }
    else 
    {
        static int count2 = 0;
        if (count2 < repeat2)
        {
            printf("Ihaha at %d ms\n",(int) clock());
            xTimerReset(three_second_timer,0);
            count2++;
        }
    }
}
void app_main(void)
{
    two_second_timer = xTimerCreate("ahihi",TIME_1,pdFALSE,(void*)1,task);
    three_second_timer = xTimerCreate("ihaha",TIME_2,pdFALSE,(void*)2,task);
    xTimerStart(two_second_timer,0);
    xTimerStart(three_second_timer,0);
}