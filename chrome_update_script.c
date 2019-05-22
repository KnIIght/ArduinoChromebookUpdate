/*
* Copyright (c) 2016, Amplified IT
* See the full description at http://labs.amplifiedit.com/centipede
*
* Support forums are available at  https://amplifiedlabs.zendesk.com/hc/en-us/categories/202956448-Centipede
*
* Published under an MIT License https://opensource.org/licenses/MIT
*
* ********** READ THIS SECTION ******************
* Use all lower case for the definitions to ensure the most compatibility for all devices
* Some keyboard emulators do not work properly with upper case or special characters that require modifier key presses
*
* Simply put, if you need to press the shift key to enter any of the information in the lines below, there's a chance it won't work properly

*/

/*
* Do not use any upper case letters here. Chromebook keyboards are emulated differently, and will not work.
*/

#include <Keyboard.h>

#define username "techadmin" // Define the user name for enrolling the device. Do not use capital letters
#define domain "" // Set your domain here
#define password "" // The password 

// Special characters definition
#define KEY_LEFT_CTRL  0x80
#define KEY_LEFT_SHIFT 0x81
#define KEY_LEFT_ALT   0x82
#define KEY_RIGHT_CTRL 0x84
#define KEY_RIGHT_SHIFT    0x85
#define KEY_RIGHT_ALT  0x86
#define KEY_UP_ARROW   0xDA
#define KEY_DOWN_ARROW 0xD9
#define KEY_LEFT_ARROW 0xD8
#define KEY_RIGHT_ARROW    0xD7
#define KEY_BACKSPACE  0xB2
#define KEY_TAB        0xB3
#define KEY_ENTER 0xB0
#define KEY_ESC        0xB1
#define KEY_CAPS_LOCK  0xC1

int buttonPin = 2;  // Set a button to any pin
int RXLED = 17;
static uint8_t __clock_prescaler = (CLKPR & (_BV(CLKPS0) | _BV(CLKPS1) | _BV(CLKPS2) | _BV(CLKPS3)));


void wait(int cycles = 1);

void setup()
{
  setPrescaler();
  Keyboard.begin();
  pinMode(buttonPin, INPUT);  // Set the button as an input
  pinMode(RXLED, OUTPUT);
  digitalWrite(buttonPin, HIGH);
  digitalWrite(RXLED,HIGH);
  wait(8);
}

void loop()
{
  if (digitalRead(buttonPin) == 1 ) {
    TXLED1;
    wait(5);
    while (digitalRead(buttonPin) != 1) {
      wait(1);
    }
    TXLED0;
   enterCredentials();
   showSuccess();
  }
  bootLoop();
}

void bootLoop() {
    TXLED1; //TX LED is not tied to a normally controlled pin
    delay(200);              // wait for a second
    TXLED0;
    delay(200);
    TXLED1; //TX LED is not tied to a normally controlled pin
    delay(200);              // wait for a second
    TXLED0;
    delay(800);
}

void showSuccess() {
  digitalWrite(RXLED, HIGH);    // set the LED off
    while (true) {
    bootLoop();
  }
}
void blink() {
  digitalWrite(RXLED, LOW);
  delay(250);
  digitalWrite(RXLED, HIGH);
  delay(250);
}

void wait(int cycles) {
  for (int i = 0; i < cycles; i++) {
    blink();
  }
}

void repeatKey(byte key, int num)
{
  for (int i = 0; i < num; i++)
  {
    Keyboard.write(key);
    wait(1);
  }
}

void enterCredentials() {
  wait(4);
  Keyboard.print(username);
  // Keyboard.press(KEY_LEFT_SHIFT);
  // Keyboard.write(64);
  // Keyboard.release(KEY_LEFT_SHIFT);
  // Keyboard.print(domain);
  wait(4);
  Keyboard.write(KEY_ENTER);
  wait(18);
  Keyboard.print(password);
  wait(3);
  Keyboard.write(KEY_ENTER);
  wait(35);
  updateIT();
}
void updateIT()
{
  Keyboard.write(KEY_LEFT_GUI);
  wait(5);
  Keyboard.print("chrome://help");
  wait(1);
  Keyboard.write(KEY_ENTER);
  wait(4);
  //Version 72 and up (so far)
  // Keyboard.write(KEY_TAB);
  //Version 63 - 71
  repeatKey(KEY_TAB, 3);

  Keyboard.write(KEY_ENTER);
  wait(9);
}

void setPrescaler() {
  // Disable interrupts.
    uint8_t oldSREG = SREG;
    cli();

    // Enable change.
    CLKPR = _BV(CLKPCE); // write the CLKPCE bit to one and all the other to zero

    // Change clock division.
    CLKPR = 0x0; // write the CLKPS0..3 bits while writing the CLKPE bit to zero

    // Copy for fast access.
    __clock_prescaler = 0x0;

    // Recopy interrupt register.
    SREG = oldSREG;
}
