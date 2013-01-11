/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef _SPIBus_H_INCLUDED
#define _SPIBus_H_INCLUDED

#include <stdio.h>
//#include <Arduino.h>
//#include <avr/pgmspace.h>
#include "armcore.h"
#include "gpio.h"
#include "spi.h"



class SPIBus {
	SPI_TypeDef * SPIx;
	SPIBuffer *SPIxBuf;
	GPIOPin pin_sck, pin_miso, pin_mosi, pin_nss;
	
public:
	
	SPIBus() : SPIx(SPI1), pin_sck(PB3), pin_miso(PB4), pin_mosi(PB5), pin_nss(PA15) { }

  // SPI Configuration methods

//  inline static void attachInterrupt();
//  inline static void detachInterrupt(); // Default

  void begin(); // Default
  void end();

	inline uint16 transfer(uint16 _data) {
		return spi_transfer(SPIxBuf, _data);
	}

  void setBitOrder(uint16_t);
  void setDataMode(uint16_t);
  void setClockDivider(uint16_t);
};

extern SPIBus SPIBus1();

/*
void SPIClass::attachInterrupt() {
  SPCR |= _BV(SPIE);
}

void SPIClass::detachInterrupt() {
  SPCR &= ~_BV(SPIE);
}
*/

#endif