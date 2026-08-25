#include "led.h"


/* ============================================================
 * RCC
 * ============================================================ */

#define RCC_BASE        0x40023800UL
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30UL))


/* ============================================================
 * GPIOA
 * ============================================================ */

#define GPIOA_BASE      0x40020000UL

#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00UL))
#define GPIOA_OTYPER    (*(volatile uint32_t *)(GPIOA_BASE + 0x04UL))
#define GPIOA_OSPEEDR   (*(volatile uint32_t *)(GPIOA_BASE + 0x08UL))
#define GPIOA_PUPDR     (*(volatile uint32_t *)(GPIOA_BASE + 0x0CUL))
#define GPIOA_ODR       (*(volatile uint32_t *)(GPIOA_BASE + 0x14UL))
#define GPIOA_BSRR      (*(volatile uint32_t *)(GPIOA_BASE + 0x18UL))


/* ============================================================
 * GPIOB
 * ============================================================ */

#define GPIOB_BASE      0x40020400UL

#define GPIOB_MODER     (*(volatile uint32_t *)(GPIOB_BASE + 0x00UL))
#define GPIOB_OTYPER    (*(volatile uint32_t *)(GPIOB_BASE + 0x04UL))
#define GPIOB_OSPEEDR   (*(volatile uint32_t *)(GPIOB_BASE + 0x08UL))
#define GPIOB_PUPDR     (*(volatile uint32_t *)(GPIOB_BASE + 0x0CUL))
#define GPIOB_ODR       (*(volatile uint32_t *)(GPIOB_BASE + 0x14UL))
#define GPIOB_BSRR      (*(volatile uint32_t *)(GPIOB_BASE + 0x18UL))


/* ============================================================
 * GPIO PIN ASSIGNMENT
 * ============================================================ */

#define GREEN_PIN       5U      /* PA5 - onboard LD2 */
#define ORANGE_PIN      6U      /* PA6 - external LED */
#define BLUE_PIN        7U      /* PA7 - external LED */
#define RED_PIN         0U      /* PB0 - external LED */


/* ============================================================
 * Helper function
 * ============================================================ */

static void gpio_output_init(volatile uint32_t *MODER,
                             volatile uint32_t *OTYPER,
                             volatile uint32_t *OSPEEDR,
                             volatile uint32_t *PUPDR,
                             uint32_t pin)
{
    /* GPIO mode = Output (01) */

    *MODER &= ~(3U << (pin * 2U));
    *MODER |=  (1U << (pin * 2U));

    /* Push-pull */

    *OTYPER &= ~(1U << pin);

    /* Low speed */

    *OSPEEDR &= ~(3U << (pin * 2U));

    /* No pull-up / pull-down */

    *PUPDR &= ~(3U << (pin * 2U));
}


/* ============================================================
 * LED INITIALIZATION
 * ============================================================ */

void delay(uint32_t count)
{
    for(volatile uint32_t i = 0; i < count; i++)
    {
        __asm volatile("NOP");
    }
}

void led_init_all(void)
{
    /*
     * Enable GPIOA clock
     *
     * RCC_AHB1ENR
     * bit 0 = GPIOAEN
     */

    RCC_AHB1ENR |= (1U << 0);


    /*
     * Enable GPIOB clock
     *
     * RCC_AHB1ENR
     * bit 1 = GPIOBEN
     */

    RCC_AHB1ENR |= (1U << 1);


    /* PA5 = Green */
    gpio_output_init(
        &GPIOA_MODER,
        &GPIOA_OTYPER,
        &GPIOA_OSPEEDR,
        &GPIOA_PUPDR,
        GREEN_PIN
    );


    /* PA6 = Orange */
    gpio_output_init(
        &GPIOA_MODER,
        &GPIOA_OTYPER,
        &GPIOA_OSPEEDR,
        &GPIOA_PUPDR,
        ORANGE_PIN
    );


    /* PA7 = Blue */
    gpio_output_init(
        &GPIOA_MODER,
        &GPIOA_OTYPER,
        &GPIOA_OSPEEDR,
        &GPIOA_PUPDR,
        BLUE_PIN
    );


    /* PB0 = Red */
    gpio_output_init(
        &GPIOB_MODER,
        &GPIOB_OTYPER,
        &GPIOB_OSPEEDR,
        &GPIOB_PUPDR,
        RED_PIN
    );


    /* Initially turn all LEDs OFF */

    GPIOA_BSRR = (1U << (GREEN_PIN + 16U));
    GPIOA_BSRR = (1U << (ORANGE_PIN + 16U));
    GPIOA_BSRR = (1U << (BLUE_PIN + 16U));

    GPIOB_BSRR = (1U << (RED_PIN + 16U));
}


/* ============================================================
 * LED ON
 * ============================================================ */

void led_on(uint8_t led)
{
    switch(led)
    {
        case LED_GREEN:

            GPIOA_BSRR = (1U << GREEN_PIN);
            break;


        case LED_ORANGE:

            GPIOA_BSRR = (1U << ORANGE_PIN);
            break;


        case LED_BLUE:

            GPIOA_BSRR = (1U << BLUE_PIN);
            break;


        case LED_RED:

            GPIOB_BSRR = (1U << RED_PIN);
            break;


        default:

            break;
    }
}


/* ============================================================
 * LED OFF
 * ============================================================ */

void led_off(uint8_t led)
{
    switch(led)
    {
        case LED_GREEN:

            GPIOA_BSRR = (1U << (GREEN_PIN + 16U));
            break;


        case LED_ORANGE:

            GPIOA_BSRR = (1U << (ORANGE_PIN + 16U));
            break;


        case LED_BLUE:

            GPIOA_BSRR = (1U << (BLUE_PIN + 16U));
            break;


        case LED_RED:

            GPIOB_BSRR = (1U << (RED_PIN + 16U));
            break;


        default:

            break;
    }
}
