#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/timers.h"
#include "freertos/task.h"

#define led 33
#define button 32
#define STACK_SIZE 1024

esp_err_t init_led(void);
esp_err_t create_tasks(void);
esp_err_t init_button(void);

void vTaskLed(void *pvParameters);
void vTaskButtonInterruptor(void *pvParameters);
void app_main(void)
{
    
}
esp_err_t init_led(void)
{
    gpio_reset_pin(led);
    gpio_set_direction(led, GPIO_MODE_OUTPUT);
    return ESP_OK;
}
esp_err_t init_button(void)
{
    gpio_reset_pin(button);
    gpio_set_direction(button, GPIO_MODE_INPUT);
    return ESP_OK;
}
esp_err_t create_tasks(void)
{
    static uint8_t ucParameterToPass;
    TaskHandle_t xHandle = NULL;

    xTaskCreate(vTaskLed,   // nombre
                "vTaskLed", // nombre
                STACK_SIZE, // cantidad de memoria
                &ucParameterToPass,
                1, // prioridad
                &xHandle);

    xTaskCreate(vTaskButtonInterruptor,   // nombre
                "vTaskButtonInterruptor", // nombre
                STACK_SIZE,               // cantidad de memoria
                &ucParameterToPass,
                1, // prioridad
                &xHandle);

    return ESP_OK;
}
void vTaskLed(void *pvParameters)
{
    while (1)
    {
        gpio_set_level(led,1);
        vTaskDelay(250);
        gpio_set_level(led,0);
        vTaskDelay(250);
    }
}