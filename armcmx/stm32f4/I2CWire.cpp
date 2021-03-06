
extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
  #include "i2c.h"
	#include "delay.h"
}

#include "I2CWire.h"

// Initialize Class Variables //////////////////////////////////////////////////

//uint8_t I2CWire::transmitting = 0;
//void (*I2CWire::user_onRequest)(void);
//void (*I2CWire::user_onReceive)(int);

// Constructors ////////////////////////////////////////////////////////////////


// Public Methods //////////////////////////////////////////////////////////////

void I2CWire::begin(uint32_t clkHz)
{
	i2c_init(&i2cbuf, i2cx, sclpin, sdapin);
	i2c_begin(&i2cbuf, clkHz);
}

void I2CWire::beginTransmission(uint8_t address) {
	dstaddress = address;
	txposition = 0;
	txlength = 0;
}

uint8_t I2CWire::receiveFrom(uint8_t address, uint16 quantity)
{
  // clamp to buffer length
  if(quantity > BUFFER_LENGTH)
    quantity = BUFFER_LENGTH;
	rxposition = 0;
 	if ( i2c_receive(&i2cbuf, rxbuffer, quantity) ) {
		return (rxlength = quantity);
	}
	return 0;
}

//
//	Originally, 'endTransmission' was an f(void) function.
//	It has been modified to take one parameter indicating
//	whether or not a STOP should be performed on the bus.
//	Calling endTransmission(false) allows a sketch to 
//	perform a repeated start. 
//
//	WARNING: Nothing in the library keeps track of whether
//	the bus tenure has been properly ended with a STOP. It
//	is very possible to leave the bus in a hung state if
//	no call to endTransmission(true) is made. Some I2C
//	devices will behave oddly if they do not see a STOP.
//
uint8_t I2CWire::endTransmission(uint8_t sendStop)
{
	//  int8_t ret = 0;
  // transmit buffer (blocking)
	if ( sendStop ) {
		if ( i2c_transmit(&i2cbuf, dstaddress, txbuffer, txlength) ) {
			return txlength;
		}
	} else {
		if ( i2c_request(&i2cbuf, dstaddress, txbuffer, txlength) ) {
			return txlength;
		}
	}
	return 0;
}

//	This provides backwards compatibility with the original
//	definition, and expected behaviour, of endTransmission
//
uint8_t I2CWire::endRequest() {
  return endTransmission(false);
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t I2CWire::write(uint8_t data)
{
  // don't bother if buffer is full
	if(txlength >= BUFFER_LENGTH){
//      setWriteError();
//      return 0;
  }
  // put byte in tx buffer
  txbuffer[txposition++] = data;
  txlength = txposition;
	return 1;
}

/*
// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t I2CWire::write(const uint8_t *data, size_t quantity)
{
  if(transmitting){
  // in master transmitter mode
    for(size_t i = 0; i < quantity; ++i){
      write(data[i]);
    }
  }else{
  // in slave send mode
    // reply to master
    twi_transmit(data, quantity);
  }
  return quantity;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
size_t I2CWire::available(void)
{
  return rxBufferLength - rxBufferIndex;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int16 I2CWire::read(void)
{
  int16 value = -1;
  
  // get each successive byte on each call
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
    ++rxBufferIndex;
  }

  return value;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int16 I2CWire::peek(void)
{
  int16 value = -1;
  
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
  }

  return value;
}
*/

void I2CWire::flush(void)
{
  // XXX: to be implemented.
}

/*
// behind the scenes function that is called when data is received
void I2CWire::onReceiveService(uint8_t* inBytes, int numBytes)
{
  // don't bother if user hasn't registered a callback
  if(!user_onReceive){
    return;
  }
  // don't bother if rx buffer is in use by a master requestFrom() op
  // i know this drops data, but it allows for slight stupidity
  // meaning, they may not have read all the master requestFrom() data yet
  if(rxBufferIndex < rxBufferLength){
    return;
  }
  // copy twi rx buffer into local read buffer
  // this enables new reads to happen in parallel
  for(uint8_t i = 0; i < numBytes; ++i){
    rxBuffer[i] = inBytes[i];    
  }
  // set rx iterator vars
  rxBufferIndex = 0;
  rxBufferLength = numBytes;
  // alert user program
  user_onReceive(numBytes);
}

// behind the scenes function that is called when data is requested
void I2CWire::onRequestService(void)
{
  // don't bother if user hasn't registered a callback
  if(!user_onRequest){
    return;
  }
  // reset tx buffer iterator vars
  // !!! this will kill any pending pre-master sendTo() activity
  txBufferIndex = 0;
  txBufferLength = 0;
  // alert user program
  user_onRequest();
}

// sets function called on slave write
void I2CWire::onReceive( void (*function)(int) )
{
  user_onReceive = function;
}

// sets function called on slave read
void I2CWire::onRequest( void (*function)(void) )
{
  user_onRequest = function;
}
*/

// Preinstantiate Objects //////////////////////////////////////////////////////

//I2CWire Wire1 = I2CWire(I2C1);

