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
// Fills the given buffer with bufferSize chars from Serial

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
	for ( char *i = buffer; i < buffer + limit; i++ ) {
		dprint(serial->available());
		dprint(i - buffer);
		dprint(buffer);

		*i = serial->read();
	}
}

//-----------------------------------------------------------------------------
// printStatus
//-----------------------------------------------------------------------------
// Prints the status of the Girino to Serial
void printStatus(void)
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

//-----------------------------------------------------------------------------
// pollCommands
//-----------------------------------------------------------------------------
// Reads incoming Serial data from the computer to see if the computer has
// sent any commands to the Girino. If it finds commands, it executes them
void pollCommands(void) {
  while (Serial.available()) {
    // Read the command character
    char command = Serial.read();
    // Read the command arguments
    fillBuffer(commandBuffer, COM_BUFFER_SIZE);
    // Process the command
    processCommand(command, commandBuffer);
  }
}

//-----------------------------------------------------------------------------
// processCommand
//-----------------------------------------------------------------------------
// Based on the specified command character and command arguments, executes a
// command
void processCommand (char command, const char *args) {
  switch (command) {
    case 's':     // 's' for starting ADC conversions
      //Serial.println("ADC conversions started");
      
      // Clear buffer
      memset( (void *)ADCBuffer, 0, sizeof(ADCBuffer) );

      startADC();
      // Let the ADC fill the buffer a little bit
      //delay(1);
      startAnalogComparator();
      break;
    case 'S':     // 'S' for stopping ADC conversions
      //Serial.println("ADC conversions stopped");
      stopAnalogComparator();
      stopADC();
      break;
    case 'p':     // 'p' for new prescaler setting
    case 'P': {
      // Wait for COMMANDDELAY ms to be sure that the Serial buffer is filled
      delay(COMMAND_DELAY);

      fillBuffer( commandBuffer, COM_BUFFER_SIZE );

      // Convert buffer to integer
      uint8_t newP = atoi( commandBuffer );

      // Display moving status indicator
      Serial.print("Setting prescaler to: ");
      Serial.println(newP);

      prescaler = newP;
      setADCPrescaler(newP);
      }
      break;

    case 'r':     // 'r' for new voltage reference setting
    case 'R': {
      // Wait for COMMANDDELAY ms to be sure that the Serial buffer is filled
      delay(COMMAND_DELAY);

      fillBuffer( commandBuffer, COM_BUFFER_SIZE );

      // Convert buffer to integer
      uint8_t newR = atoi( commandBuffer );

      // Display moving status indicator
      Serial.print("Setting voltage reference to: ");
      Serial.println(newR);

      setVoltageReference(newR);
      }
      break;

    case 'e':     // 'e' for new trigger event setting
    case 'E': {
      // Wait for COMMANDDELAY ms to be sure that the Serial buffer is filled
      delay(COMMAND_DELAY);

      fillBuffer( commandBuffer, COM_BUFFER_SIZE );

      // Convert buffer to integer
      uint8_t newE = atoi( commandBuffer );

      // Display moving status indicator
      Serial.print("Setting trigger event to: ");
      Serial.println(newE);

      triggerEvent = newE;
      setTriggerEvent(newE);
      }
      break;

    case 'w':     // 'w' for new wait setting
    case 'W': {
      // Wait for COMMANDDELAY ms to be sure that the Serial buffer is filled
      delay(COMMAND_DELAY);

      fillBuffer( commandBuffer, COM_BUFFER_SIZE );

      // Convert buffer to integer
      uint8_t newW = atoi( commandBuffer );

      // Display moving status indicator
      Serial.print("Setting waitDuration to: ");
      Serial.println(newW);

      waitDuration = newW;
      }
      break;

    case 't':     // 'w' for new threshold setting
    case 'T': {
      // Wait for COMMANDDELAY ms to be sure that the Serial buffer is filled
      delay(COMMAND_DELAY);

      fillBuffer( commandBuffer, COM_BUFFER_SIZE );

      // Convert buffer to integer
      uint8_t newT = atoi( commandBuffer );

      // Display moving status indicator
      Serial.print("Setting threshold to: ");
      Serial.println(newT);

      threshold = newT;
      analogWrite( THRESHOLD_PIN, threshold );
      }
      break;

    case 'd':     // 'd' for display status
    case 'D':
      printStatus();
      break;

    default:
      // Display error message
      Serial.print("ERROR: Command not found, it was: ");
      Serial.println(command);
      error();
  }
}

