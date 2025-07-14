RC Car Project

UART Testing Code: Tests to see if the device successfully received the 
transmitted byte through UART:

    if (USART2->ISR & USART_ISR_ORE) {
            printf("Overrun Error\n");
        }
        if (USART2->ISR & USART_ISR_FE) {
            printf("Framing Error\n");
        }
        if (USART2->ISR & USART_ISR_NE) {
            printf("Noise Error\n");
        }