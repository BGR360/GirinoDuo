//-----------------------------------------------------------------------------
// Girino.h
//-----------------------------------------------------------------------------
// Copyright 2012 Cristiano Lino Fontana
//
// This file is part of Girino.
//
//	Girino is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//
//	Girino is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with Girino.  If not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <Arduino.h>

//-----------------------------------------------------------------------------
// Defines and Typedefs
//-----------------------------------------------------------------------------

// Defines for board type
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define MEGA_1280
#endif
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
#define MEGA_328
#endif
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
#define MEGA_32U4
#endif

#define DEBUG		0

#define ADC_BUFFER_SIZE	1280
#define ADC_PIN			    0
#define ERROR_PIN       13
#define THRESHOLD_PIN   3

// To do fast digital writes on the info pin, 
// we'll want to directly modify the registers.

// On the ATmega328, pin 13 is PORTB5
#if defined(MEGA_328)
#define INFO_PIN_REG  PORTB
#define INFO_PIN_MSK  PORTB5
// On the ATmega32U4, pin 13 is PORTC7
#elseif defined(MEGA_32U4)
#define INFO_PIN_REG  PORTC
#define INFO_PIN_MSK  PORTC7
#else
#define INFO_PIN      13
#endif

#define BAUDRATE	115200	// Baud rate of UART in bps
#define COMMAND_DELAY	10	// ms to wait for the filling of Serial buffer
#define COM_BUFFER_SIZE	3	// Size of buffer for incoming numbers

#if DEBUG == 1
	#define dprint(expression) Serial.print("# "); Serial.print( #expression ); Serial.print( ": " ); Serial.println( expression )
	#define dshow(expression) Serial.println( expression )
#else
	#define dprint(expression) 
	#define dshow(expression)
#endif

// Defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void initPins(void);
void initADC(void);
void initAnalogComparator(void);

void startADC( void );
void stopADC( void );
void startAnalogComparator( void );
void stopAnalogComparator( void );

void setADCPrescaler( uint8_t prescaler );
void setVoltageReference( uint8_t reference );
void setTriggerEvent( uint8_t event );

void error (void);
void info (int);
void pollCommands (void);
void processCommand (
  char command, 
  const char *args );
void fillBuffer( \
	char *buffer, \
	byte bufferSize, \
	Stream* serial = &Serial );
void printStatus(void);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
extern volatile  boolean wait;
extern          uint16_t waitDuration;
extern volatile uint16_t stopIndex;
extern volatile uint16_t ADCCounter;
extern volatile  uint8_t ADCBuffer[ADC_BUFFER_SIZE];
extern volatile  boolean freeze;

extern           uint8_t prescaler;
extern           uint8_t triggerEvent;
extern           uint8_t threshold;

extern              char commandBuffer[COM_BUFFER_SIZE+1];

