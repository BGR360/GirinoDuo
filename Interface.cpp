//-----------------------------------------------------------------------------
// Interface.cpp
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
void error (void) {
	digitalWrite( ERROR_PIN, HIGH );
	delay( 500 );
	digitalWrite( ERROR_PIN, LOW );
	delay( 250 );
	digitalWrite( ERROR_PIN, HIGH );
	delay( 500 );
	digitalWrite( ERROR_PIN, LOW );
}

void info (int highOrLow) {
#if defined(INFO_PIN_REG) && defined(INFO_PIN_MSK)
  if (highOrLow) {
    sbi(INFO_PIN_REG, INFO_PIN_MSK);
  } else {
    cbi(INFO_PIN_REG, INFO_PIN_MSK);
  }
#else
  digitalWrite( INFO_PIN, highOrLow );
#endif
}

//-----------------------------------------------------------------------------
// fillBuffer
//-----------------------------------------------------------------------------
// Fills the given buffer with bufferSize chars from a Serial object

void fillBuffer( char *buffer, byte bufferSize, Stream* serial )
{
	// Clean buffer
	memset( (void *)buffer, '\0', sizeof(char) * bufferSize );

	dprint(serial->available());
	
	byte limit = ( bufferSize < serial->available() ) ? bufferSize : serial->available();

	dprint(serial->available());
	dprint(bufferSize);
	dprint(limit);

	// Fill buffer
	for ( byte i = 0; i < limit; i++ ) {
		dprint(serial->available());
		dprint(i);
		dprint(buffer);

		buffer[i] = serial->read();
	}
}

void printStatus( void )
{
	Serial.print("Buffer size: ");
	Serial.println(ADC_BUFFER_SIZE);
	Serial.print("Baud rate: ");
	Serial.println(BAUDRATE);
	Serial.print("Wait duration: ");
	Serial.println(waitDuration);
	Serial.print("Prescaler: ");
	Serial.println(prescaler);
	Serial.print("Trigger event: ");
	Serial.println(triggerEvent);
	Serial.print("Threshold: ");
	Serial.println(threshold);
}
