#define RCC_BASE        0x40023800UL
#define GPIOG_BASE      0x40021800UL

#define RCC_AHB1ENR_OFFSET    0x30UL
#define GPIOG_MODER_OFFSET     0x00UL
#define GPIOG_ODR_OFFSET       0x14UL

#define RCC_AHB1ENR   (*(volatile unsigned int *)(RCC_BASE + RCC_AHB1ENR_OFFSET))
#define GPIOG_MODER    (*(volatile unsigned int *)(GPIOG_BASE + GPIOG_MODER_OFFSET))
#define GPIOG_ODR      (*(volatile unsigned int *)(GPIOG_BASE + GPIOG_ODR_OFFSET))

#define GPIOG_EN       (1U << 6)
#define LED_PIN        (1U << 13)

int main(void)
{
    // Enable clock to GPIOG
    RCC_AHB1ENR |= GPIOG_EN;

    // Configure PG13 as output
    GPIOG_MODER &= ~(0x3U << 26);
    GPIOG_MODER |=  (0x1U << 26);

    // Toggle LED forever
    while (1)
    {
        GPIOG_ODR ^= LED_PIN;

        for (volatile unsigned int i = 0; i < 1000000; i++);
    }
}
