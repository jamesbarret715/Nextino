# Nextino
An interface for using Nextion HMI displays with Arduino.

# Installation
You can install the library using the Arduino IDE. 

1. Download [ArduinoSTL](https://github.com/mike-matera/ArduinoSTL/archive/refs/heads/master.zip)
1. Download [this library](https://github.com/jamesbarret715/Nextino/archive/refs/heads/master.zip)
1. In the Arduino IDE, navigate to Sketch > Include Library > Add .zip Library...
1. Do this for both the downloaded ZIP files and you're ready to start using Nextino.

# Usage

Include the header in your Arduino code.
```ino
#include <Nextino.h>

```

Create a `Nextino::Display`, supplying a `Stream` (ie `Serial` or `SoftwareSerial`) connected to the device.

```ino
SoftwareSerial displaySerial(5, 6);

Nextino::Device nextion(displaySerial);

// Begin the SoftwareSerial with the same baudrate of the Nextion.
void setup() 
{
    displaySerial.begin(9600);
}
```

Now you're ready to start sending commands to your Nextion.

```ino
void loop()
{
    nextion.write("page 0");
}
```
You can also define individual elements to be accessed more easily.

```ino
Nextino::Display nextion(displaySerial);

// Specify the element by its objname
Nextino::Element elapsedMillis(nextion, "t0");

// or by its page number and id
Nextino::Element headingText(nextion, 0, 1);

void setup()
{
    // ...
    headingText.setProperty("txt", "Hello from Arduino!");
}

void loop() 
{
    // ...
    elapsedMillis.setProperty("val", millis());
}
```

Many element types have their own class with some useful functions.

```ino
Nextino::Text subheadingText(nextion, "t1");

void setup() 
{
    // ...
    subheadingText.setText("Connected to Nextino on Arduino!");
}
```
