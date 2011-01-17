#include <Wire.h>
#include <PCFKeypad.h>

/**
 * The PCFKeypad class has been tested on the PCF874AP IC chip
 * produced by NXP. This chip is an I2C I/O expander chip accessed
 * via the Arduino Wire library. It assumes a 12 button keypad (3 cols,
 * 4 rows) is in use with a keypad pin for each col and each row. Either
 * the cols or the rows will utilize pull up resistors. The row pins
 * should be wired to the PCF's PO-3 pins, the col pins should be wired
 * to the PCF's P4-6 pins.
 *
 * To determine which button is currently being pressed, the class does
 * a mapping from the decimal representation of the data read from
 * the IC during a keypress event to the character represented by that
 * button press. These values are determined in 'trial' fashion by
 * correctly wiring up your keypad, and using this class in deubg mode.
 * 
 * This example uses an Arduino intterupt to detect when a key has been
 * pressed. Check your Arduino model documentation to determine the pin
 * to use for interrupts.
 *
 * Author: Jon Moniaci <jonmoniaci [at] gmail.com>
 * Github: http://www.github/corporealfunk
 * Website: http://bitsandpieces.jonmoniaci.com
 */

// which intterupt number do we want to use?
const int INTERRUPT = 0;

// which pin does that interrupt utilize? 
const int INTERRUPT_PIN = 2;

// flag to determine if we are in an interrupted state
volatile byte interrupt_flag = 0;

/**
 * This is a list of buttons on the keypad, from top left to bottom
 * right.
 */
char keymap_char[12] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '0', '#' };

/**
 * This is the decimal representation of the raw byte that is read
 * from the I/O expander during a keypress. This has been determiend
 * by trial and error with the PCFKeypad debugging enabled, while
 * monitoring the Serial Monitor during keypresses. Your keypad may
 * be different and especially so depending on how you wire up your
 * keypad pins to the I/O expander
 */
int keymap_val[12] = {190, 222, 238, 189, 221, 237, 187, 219, 235, 183, 215, 231};

/**
 * The keypad: make sure you address your I/O expander correctly.
 * (default address with address pins wired to GND is 0x38). The last
 * bool parameter determines if you want a stream of data sent to the
 * Serial Monitor.
 */
PCFKeypad keypad(12, keymap_char, keymap_val, 0x38, true);

void setup() {
  // init the wire library
  Wire.begin();

  // setup our interrupt pin as input
  pinMode(INTERRUPT_PIN, INPUT);

  // init the serial port
  Serial.begin(9600);

  // init the keypad
  keypad.begin();

  // attach the intterupt method that will run when a key is pressed
  attachInterrupt(INTERRUPT, keypadInterrupt, LOW);
}

// main loop
void loop() {
  char key;

  // if we have been interrupted due to a keypress, check the key
  if (interrupt_flag == 1) {
    // get the char of the key pressed, only if a key is actually
    // pressed (a very fast keypress might trigger an interrupt, but
    // by the time we query the I/O chip, no key may continue to be
    // pressed)
    if (keypad.getKey(key)) {
      Serial.print("BUTTON  : ");
      Serial.println(key);
      Serial.println("");
    }
    // reattach the interrupt
    interrupt_flag = 0;
    attachInterrupt(INTERRUPT, keypadInterrupt, LOW);
  }
}

// the interrupt method (triggered when a key is pressed)
void keypadInterrupt() {
  // disable the intterupt: only one key press is processed at a time
  detachInterrupt(INTERRUPT);
  interrupt_flag = 1;
}
