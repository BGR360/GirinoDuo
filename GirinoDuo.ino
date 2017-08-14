//-----------------------------------------------------------------------------
// Girino.ino
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

#include "Girino.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

volatile  boolean wait;
         uint16_t waitDuration;
volatile uint16_t stopIndex;
volatile uint16_t ADCCounter;
volatile  uint8_t ADCBuffer[ADC_BUFFER_SIZE];
volatile  boolean freeze;

          uint8_t prescaler;
          uint8_t triggerEvent;
          uint8_t threshold;

             char commandBuffer[COM_BUFFER_SIZE+1];

//-----------------------------------------------------------------------------
// Main routines
//-----------------------------------------------------------------------------
//
// The setup function initializes registers.
//
void setup (void) {		// Setup of the microcontroller
	// Open serial port with a baud rate of BAUDRATE b/s
	Serial.begin(BAUDRATE);

	dshow("# setup()");
	// Clear buffers
	memset( (void *)ADCBuffer, 0, sizeof(ADCBuffer) );
	memset( (void *)commandBuffer, 0, sizeof(commandBuffer) );
	ADCCounter = 0;
	wait = false;
	waitDuration = ADC_BUFFER_SIZE - 32;
	stopIndex = -1;
	freeze = false;

	prescaler = 128;
	triggerEvent = 3;

	threshold = 127;

	// Activate interrupts
	sei();

	initPins();
	initADC();
	initAnalogComparator();

	Serial.println("Girino ready");
	//printStatus();
}

void loop (void) {
	dprint(ADCCounter);
	dprint(stopIndex);
	dprint(wait);
	dprint(freeze);
	#if DEBUG == 1
	Serial.println( ADCSRA, BIN );
	Serial.println( ADCSRB, BIN );
	#endif

	// If freeze flag is set, then it is time to send the buffer to the serial port
	if ( freeze )
	{
		dshow("# Frozen");

		// Send buffer through serial port in the right order
		//Serial.print("Buffer: ");
		//Serial.write( ADCBuffer, ADCBUFFERSIZE );
		//Serial.print("End of Buffer");
		Serial.write( (uint8_t *)ADCBuffer + ADCCounter, ADC_BUFFER_SIZE - ADCCounter );
		Serial.write( (uint8_t *)ADCBuffer, ADCCounter );

		// Turn off infoPin
		info(LOW);

		wait = false;
		freeze = false;

		// Clear buffer
		//memset( (void *)ADCBuffer, 0, sizeof(ADCBuffer) );

		//startADC();
		// Let the ADC fill the buffer a little bit
		//delay(1);
		//startAnalogComparator();

		#if DEBUG == 1
		delay(3000);
		#endif
	}

  // Poll for any commands sent from the UI
	pollCommands();
}
