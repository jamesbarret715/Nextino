#include <Nextino.h>
#include <SoftwareSerial.h>

// Plug the Nextion display into 5V, GND and pins 5 (TX) and 6 (RX).
SoftwareSerial displaySerial(5, 6);

Nextino::Display display(displaySerial);

// In our HMI file, we have a Text element with id 1, and a Number with id 2.
Nextino::Text heading(display, 0, 1);
Nextino::Number timeElapsed(display, 0, 2);

void setup()
{
    // Start the serial port at the display's baud.
    displaySerial.begin(9600);

    // Change the text in the header.
    heading.setText("Hello, Arduino!");
}

void loop()
{
    // Continuously update the time elapsed (seconds).
    timeElapsed.setValue(millis() / 1000);
}