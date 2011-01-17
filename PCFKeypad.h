#ifndef PCFKeypad_h
#define PCFKeypad_h

#include "WProgram.h"
#include "../Wire/Wire.h"
#include <inttypes.h>

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
 * button press. These values are determined in "trial" fashion by
 * correctly wiring up your keypad, and using this class in deubg mode.
 *
 * The provided "PCFSerial" example will do this for you.
 */
class PCFKeypad {
  public:
    /**
     * Constructor
     *
     * num_keys       the number of keys in your keypad
     * keymap_chars   an array of chars listing the buttons
     *  on the keypad (left to right, starting top left)
     * keymap_vals    an array of integers listing the decimal
     *  value registered by the I2C expander when it reads a byte
     *  of data off the expander IC. The indexes should correspond
     *  the kemap_chars button character. These values are unknown
     *  until you have wired your keypad to the expander. You can then
     *  query for the values by using hte PCFKeypad class with the debug
     *  flag set to true, then monitoring your Serial Monitor to see
     *  the raw data queried from the I2C expander
     * address        the address of the I2C expander
     * debug          whether or not to send debug data over the Serial
     *  port
     */
    PCFKeypad(int num_keys, char keymap_chars[], int keymap_vals[], byte address, bool debug);

    /**
     * Fetches the button that was pressed
     * returns true/false indicating if a button is actually being pressed
     * if a button is being pressed, it's character is written into the
     * key parameter passed in
     */
    bool getKey(char &key);

    /**
     * Intializes the I2C expander for use with the keypad
     */
    void begin();
  private:
    byte _expanderRead();
    void _expanderWrite(byte data);
    char _mapKey(int val);

    bool _debug;
    int _num_keys;
    int _address;
    char* _keymap_chars;
    int* _keymap_vals;
};
#endif
