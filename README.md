# Fencing_Light_LED_Strip
Fencing_Light_LED_Strip for SG and Leon Paul Scoreboxes

The output of a Leon Paul or St George scorebox is done through 8 pins, either RJ45 or 8 pin din connector:
https://www.leonpaul.com/content/leon-paul-wireless-fencing-instructions.pdf
The power connections are used with pins that denote each lamp color: Green/Red/White Green/White Red/Yellow Green/Yellow Red
This project uses the connections to relays, which connects the Transmitting ESP8266 Wemos input pins to ground creating a signal. The Transmitting ESP8266 Wemos then sends the signal to a Receiving ESP8266 Wemos using the Arduino ESPN-NOW library. The receiving ESP8266 Wemos then uses the FastLED library to send data signals to an individually addressable LED strip.
The receiving ESP8266 
