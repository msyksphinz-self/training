#include <Arduino.h>
#include "serial.h"

void setup()
{
  int bauds = 9600;
  GPIO_REG(GPIO_OUTPUT_XOR)&= ~(IOF0_UART0_MASK);
  GPIO_REG(GPIO_IOF_SEL)   &= ~(IOF0_UART0_MASK);
  GPIO_REG(GPIO_IOF_EN)    |= IOF0_UART0_MASK;

  UART_REG(UART_REG_DIV) = F_CPU / bauds - 1;
  UART_REG(UART_REG_TXCTRL) |= UART_TXEN;
  UART_REG(UART_REG_RXCTRL) |= UART_RXEN;
}

void loop()
{
  return;
}

void uart_print (uint8_t *str)
{
  while (*str != '\0') {
    uart_putchar(*str);
    str++;
  }

  return;
}


void uart_putchar (uint8_t a)
{
  const int blocking = 1;

  volatile uint32_t *val = UART_REGP(UART_REG_TXFIFO);
  uint32_t busy = (*val) & 0x80000000;
  if (blocking) {
    while (*val & 0x80000000);
  } else if (busy) {
    return;
  }
  UART_REG(UART_REG_TXFIFO) = a;

  return;
}
