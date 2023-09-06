#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/timers.h"
#include "freertos/task.h"

#define led 33
#define button 32
#define STACK_SIZE 1024*2
uint8_t count=0;

esp_err_t init_led(void);
esp_err_t create_tasks(void);
esp_err_t init_button(void);
void isr_handler(void *args);

void vTaskLed(void *pvParameters);
void vTaskButtonInterruptor(void *pvParameters);
void app_main(void)
{
    init_led();
    init_button();
    create_tasks();
    if (count==1)
    {
        gpio_set_level(led,0);
        vTaskDelay(500);
    }
    
}
esp_err_t init_led(void)
{
    gpio_reset_pin(led);
    gpio_set_direction(led, GPIO_MODE_OUTPUT);
    return ESP_OK;
}
esp_err_t init_button(void)
{
    gpio_config_t pGPIOConfig;
    pGPIOConfig.pin_bit_mask = (1ULL << button); 
    pGPIOConfig.mode =GPIO_MODE_DEF_INPUT;
    pGPIOConfig.pull_up_en =GPIO_PULLUP_DISABLE;
    pGPIOConfig.pull_down_en =GPIO_PULLDOWN_DISABLE;
    pGPIOConfig.intr_type =GPIO_INTR_NEGEDGE;

    gpio_config(&pGPIOConfig);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(button, isr_handler, NULL);
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
void vTaskButtonInteruptor(void *pvParameters)
{
    
}
void isr_handler(void *args)
{
    count ++;
}