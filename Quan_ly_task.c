#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

//============================//
//  C?U TRÚC D? LI?U G?I QUA QUEUE
//============================//
typedef struct {
    float frequency;   // T?n s? LED (Hz)
    float duty;        // Ð? r?ng xung (%)
} BlinkData_t;

//============================//
//  BI?N TOÀN C?C
//============================//
QueueHandle_t xBlinkQueue;

#define LED_PORT   GPIOC
#define LED_PIN    GPIO_Pin_13

//============================//
//  HÀM C?U HÌNH GPIO CHO LED
//============================//
void GPIO_Config(void)
{
    GPIO_InitTypeDef gpio;   // ? khai báo bi?n tru?c
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    gpio.GPIO_Pin = LED_PIN;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(LED_PORT, &gpio);
}

void LED_ON(void)
{
    GPIO_ResetBits(LED_PORT, LED_PIN);   // PC13 d?o logic
}

void LED_OFF(void)
{
    GPIO_SetBits(LED_PORT, LED_PIN);
}

//============================//
//  TASK 1: ÐI?U KHI?N LED
//============================//
void LED_Task(void *pvParameters)
{
    BlinkData_t recv;
    float period_ms;
    float on_time;
    float off_time;

    for (;;)
    {
        // Nh?n d? li?u t?n s? + duty t? Queue
        if (xQueueReceive(xBlinkQueue, &recv, portMAX_DELAY) == pdPASS)
        {
            period_ms = 1000.0f / recv.frequency;
            on_time = period_ms * (recv.duty / 100.0f);
            off_time = period_ms - on_time;

            // L?p l?i cho d?n khi có d? li?u m?i trong queue
            while (uxQueueMessagesWaiting(xBlinkQueue) == 0)
            {
                LED_ON();
                vTaskDelay(pdMS_TO_TICKS((uint32_t)on_time));

                LED_OFF();
                vTaskDelay(pdMS_TO_TICKS((uint32_t)off_time));
            }
        }
    }
}

//============================//
//  TASK 2: SINH D? LI?U G?I SANG TASK 1
//============================//
void Gen_Task(void *pvParameters)
{
    BlinkData_t send;
    send.frequency = 1.0f;
    send.duty = 30.0f;

    for (;;)
    {
        // T?o giá tr? thay d?i d? d? quan sát
        send.frequency += 0.5f;
        if (send.frequency > 5.0f)
            send.frequency = 1.0f;

        send.duty += 10.0f;
        if (send.duty > 90.0f)
            send.duty = 30.0f;

        xQueueSend(xBlinkQueue, &send, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));   // G?i m?i 1 giây
    }
}

//============================//
//  HÀM MAIN
//============================//
int main(void)
{
    SystemInit();
    GPIO_Config();

    xBlinkQueue = xQueueCreate(5, sizeof(BlinkData_t));

    // T?o Task
    xTaskCreate(LED_Task, "LED", 128, NULL, 2, NULL);
    xTaskCreate(Gen_Task, "GEN", 128, NULL, 1, NULL);

    vTaskStartScheduler();   // B?t d?u FreeRTOS

    while (1);
}

