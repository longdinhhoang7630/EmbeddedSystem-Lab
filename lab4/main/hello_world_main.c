#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "freertos/queue.h"

#define NumberOfTask 3
#define delay 1000
#define wait_time 100

typedef struct Data
{
    int id;
    int state;
    int comingTask;
} Data;

static QueueHandle_t queue;

void comingTask(void *pv)
{
    for (int i = 0;; i++)
    {
        Data task;
        task.id = i;
        if (i % 2 == 0)
		{
            task.state = 1; 
		}
        else if (i % 3 == 0)
		{
			task.state = 2; 
		}
        else if (i % 7 == 0)
		{
			task.state = 3; 
		}
        else
		{
			task.state = -1;
		}
        task.comingTask = 0;
        xQueueSend(queue, &task, wait_time);
        vTaskDelay(delay / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

void workingTask(void *pv)
{
    Data task;
    char *name_task;
    name_task = (char *)pv;
    for (;;)
    {
        if (xQueueReceive(queue, (void *)&task, wait_time) == pdTRUE)
        {
            if (((task.state == 1) && (!strcmp(name_task, "task1"))) || ((task.state == 2) && (!strcmp(name_task, "task2"))) || ((task.state == 3) && (!strcmp(name_task, "task3"))))
                printf("%s comes and does the request id %d \n", name_task, task.id);
            else
            {
                task.comingTask += 1;
                if (NumberOfTask > task.comingTask)
                    xQueueSendToFront(queue, &task, wait_time);
                else
                    printf("Warning: no task does the request id %d \n", task.id);
            }
        }
        vTaskDelay(delay / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

static const char *task0 = "task0";
static const char *task1 = "task1";
static const char *task2 = "task2";
static const char *task3 = "task3";

void app_main(void)
{
    queue = xQueueCreate(10, sizeof(Data));
    xTaskCreatePinnedToCore(&comingTask, "Task 0", 4096, (void *)task0, 2, NULL, 1);
    xTaskCreatePinnedToCore(&workingTask, "Task 1", 4096, (void *)task1, 1, NULL, 1);
    xTaskCreatePinnedToCore(&workingTask, "Task 2", 4096, (void *)task2, 1, NULL, 1);
    xTaskCreatePinnedToCore(&workingTask, "Task 3", 4096, (void *)task3, 1, NULL, 1);
}