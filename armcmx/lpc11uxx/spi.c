 /*
  *
  *
  */
  
#include "spi.h"

#include "gpio.h"
//#include "usart.h"

/*
#define SD_CARD BIT8
#define SD_SELECTION_DIR_SET (LPC_GPIO->DIR[0] |= (1<<2))
#define SD_SELECTION_SET (LPC_GPIO->PIN[0] |= (1<<2))
#define SD_SELECTION_CLR (LPC_GPIO->PIN[0] &= ~(1<<2))
*/

SPIDef SPI0Def = { LPC_SSP0 }, SPI1Def = { LPC_SSP1 };

uint8_t SPI_transfer(SPIDef * port, uint8_t data) {
  port->SSPx->DR = data;
  /* Wait until the Busy bit is cleared */
  while ( (port->SSPx->SR & (SSPSR_BSY|SSPSR_RNE)) != SSPSR_RNE );
  data = port->SSPx->DR;
  return data;
}

void SPI_init(SPIDef * port, GPIOPin sck, GPIOPin miso, GPIOPin mosi, GPIOPin ncs) {
  SPI_reset(port);
  SPI_config(port, sck, miso, mosi, ncs);
  SPI_start(port);
}


void SPI_config(SPIDef * port, GPIOPin sck, GPIOPin miso, GPIOPin mosi, GPIOPin ncs) {

  if ( port->SSPx == LPC_SSP0 ) {
    LPC_SYSCON->PRESETCTRL |= (0x1<<0);  //SSP0 reset off
    LPC_SYSCON->SYSAHBCLKCTRL |= (0x1<<11); // power on
    LPC_SYSCON->SSP0CLKDIV = 0x2;			/* Divided by 2 */

    LPC_IOCON->PIO0_8 &= ~0x07;		/*  SSP I/O config */
    LPC_IOCON->PIO0_8 |= 0x01;		/* SSP MISO */
    LPC_IOCON->PIO0_9 &= ~0x07;	
    LPC_IOCON->PIO0_9 |= 0x01;		/* SSP MOSI */
    
    if ( sck == PIO0_10 ) {
      LPC_IOCON->SWCLK_PIO0_10 &= ~0x07;
      LPC_IOCON->SWCLK_PIO0_10 |= 0x02;		/* SSP CLK */
    } else if ( sck == PIO1_29 ) {
    /* On C1U, SSP CLK can be routed to different pins. */
      LPC_IOCON->PIO1_29 &= ~0x07;	
      LPC_IOCON->PIO1_29 = 0x01;
    } else {
      LPC_IOCON->PIO0_6 &= ~0x07;	
      LPC_IOCON->PIO0_6 = 0x02;
    }
    
    if ( ncs == NOT_A_PIN || ncs == PIN_NOT_DEFINED ) {
      LPC_IOCON->PIO0_2 &= ~0x07;	
      LPC_IOCON->PIO0_2 |= 0x01;		/* System SSP SSEL */
    } else {
      // as user controllable GPIO pin
      /* Enable AHB clock to the GPIO domain. */
      LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);
      pinFuncClear(ncs);		/* SSP SSEL is a GPIO pin */
      pinMode(ncs, OUTPUT);
      digitalWrite(ncs, HIGH );
    }

  }
  else if (port->SSPx == LPC_SSP1) 		/* port number 1 */
  {
    LPC_SYSCON->PRESETCTRL |= (0x1<<2);  // SSP1 reset off
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<18);  // power on
    LPC_SYSCON->SSP1CLKDIV = 0x02;			/* Divided by 2 */
    if ( sck == PIO1_20 ) {
      pinFuncClear(PIO1_20); // &= ~0x07;    /*  SSP I/O config */
      pinFuncSet(PIO1_20, 0x02); //0x02;		/* SSP CLK */
    } else {
      LPC_IOCON->PIO1_15 &= ~0x07;    /*  SSP I/O config */
      LPC_IOCON->PIO1_15 |= 0x03;		/* SSP CLK */
    }
    if ( miso == PIO1_21 ) {
      //LPC_IOCON->PIO1_21 &= ~0x07;	
      //LPC_IOCON->PIO1_21 |= 0x02;		/* SSP MISO */
      pinFuncClear(PIO1_21);
      pinFuncSet(PIO1_21, 0x02);
    } else {
      LPC_IOCON->PIO0_22 &= ~0x07;	
      LPC_IOCON->PIO0_22 |= 0x03;		/* SSP MISO */
    }
    if ( mosi == PIO1_22 ) {
      LPC_IOCON->PIO1_22 &= ~0x07;	
      LPC_IOCON->PIO1_22 |= 0x02;		/* SSP MOSI */
    } else {
      LPC_IOCON->PIO0_21 &= ~0x07;	
      LPC_IOCON->PIO0_21 |= 0x02;		/* SSP MOSI */
    }
    
    if ( ncs == PIN_NOT_DEFINED ) {
      LPC_IOCON->PIO1_23 &= ~0x07;	
      LPC_IOCON->PIO1_23 |= 0x02;		/* SSP SSEL */
      // alternative:
      //LPC_IOCON->PIO1_19 &= ~0x07;	
      //LPC_IOCON->PIO1_19 |= 0x02;		/* SSP SSEL */
    } else {
      /* Enable AHB clock to the GPIO domain. */
      LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);
      if ( ncs == PIO1_23 ) {
        LPC_IOCON->PIO1_23 &= ~0x07;		/* SSP SSEL is a GPIO pin */
        /* port2, bit 0 is set to GPIO output and high */
        pinMode( PIO1_23, OUTPUT );
        digitalWrite( PIO1_23, HIGH );
      } else {
        LPC_IOCON->PIO1_19 &= ~0x07;		/* SSP SSEL is a GPIO pin */
        /* port2, bit 0 is set to GPIO output and high */
        pinMode( PIO1_19, OUTPUT );
        digitalWrite( PIO1_19, HIGH );
      }
    }
  }
  return;		
}


void SPI_start(SPIDef * port) {
  uint8_t i, Dummy=Dummy;

  /* Set DSS data to 8-bit, Frame format SPI, CPOL = 0, CPHA = 0, and SCR is 15 */
  port->SSPx->CR0 = 0x0707;
  /* SSPCPSR clock prescale register, master mode, minimum divisor is 0x02 */
  port->SSPx->CPSR = 0x2;

  for ( i = 0; i < FIFOSIZE; i++ ) {
    Dummy = port->SSPx->DR;		/* clear the RxFIFO */
  }

  /* Enable the SSP Interrupt */
  if ( port->SSPx == LPC_SSP0 ) {
    NVIC_EnableIRQ(SSP0_IRQn);
  } else {
    NVIC_EnableIRQ(SSP1_IRQn);
  }
  /* Device select as master, SSP Enabled */
  /* Master mode */
  port->SSPx->CR1 = SSPCR1_SSE;
  /* Set SSPINMS registers to enable interrupts */
  /* enable all error related interrupts */
  port->SSPx->IMSC = SSPIMSC_RORIM | SSPIMSC_RTIM;

  return;
}


void SPI_reset(SPIDef * port) {
 if ( port->SSPx == LPC_SSP0 ) {
    LPC_SYSCON->PRESETCTRL &= ~SSP0_RST_N;
   __nop();
    LPC_SYSCON->PRESETCTRL |= SSP0_RST_N;
 } else if ( port->SSPx == LPC_SSP1 ) {
    LPC_SYSCON->PRESETCTRL &= ~SSP1_RST_N;
   __nop();
    LPC_SYSCON->PRESETCTRL |= SSP1_RST_N;
 }
}

void SPI_disable(SPIDef * port) {
}

void SPI_ClockDivier(SPIDef * port, uint8_t div) {
  if ( port->SSPx == LPC_SSP0 ) {
    LPC_SYSCON->SSP0CLKDIV = div & 0xff;
  } else {
    LPC_SYSCON->SSP1CLKDIV = div & 0xff;
  }
}

void SPI_DataSize(SPIDef * port, uint8_t bitsize) {
    port->SSPx->CR0 &= ~0x0f;
    port->SSPx->CR0 |= (0x0F & bitsize);				/* Select 8- or 16-bit mode */
}

void SPI_DataMode(SPIDef * port, uint8_t mode) {
  port->SSPx->CR0 &= ~SPIMODE_CPOL;
  if ( mode & 1 )
    port->SSPx->CR0 |= SPIMODE_CPOL;
  if ( mode & 2 )
    port->SSPx->CR0 |= SPIMODE_CPHA;
}


void SPI_select(void) {
  // cs high
  // set mode
}
