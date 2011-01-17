#include "PCFKeypad.h"

#include "WProgram.h"
#include "../Wire/Wire.h"
#include <inttypes.h>


PCFKeypad::PCFKeypad(int num_keys, char keymap_chars[], int keymap_vals[], byte address, bool debug=false) {
  _num_keys = num_keys;
  _keymap_chars = keymap_chars;
  _keymap_vals = keymap_vals;
  _address = address;
  _debug = debug;
}

void PCFKeypad::begin() {
  _expanderWrite(B00001111); //B00001111
}

bool PCFKeypad::getKey(char &key) {
  delay(50); //hacky debounce method;
  byte data_last_bits;
  byte data_first_bits;
  byte whole_byte;
  bool ret = false;
  
  data_last_bits = _expanderRead();
  _expanderWrite(B11110000); //B11110000
  data_first_bits = _expanderRead();
  whole_byte = data_first_bits ^ data_last_bits;
  
  if (whole_byte - 0 != 255) {
    if (_debug) {
      Serial.print("RAW BYTE: ");
      Serial.println(whole_byte, BIN);
      Serial.print("DECIMAL : ");
      Serial.print(whole_byte, DEC);
      Serial.println(" (this is the value that should go in keymap_vals)");
    }
  
    key = _mapKey(whole_byte - 0);
    ret = true;
  }
  _expanderWrite(B00001111); //B00001111
  return ret;
}

char PCFKeypad::_mapKey(int val) {
  int i;
  for (i = 0; i < _num_keys; i++) {
    if (_keymap_vals[i] == val) {
      return _keymap_chars[i];
    }
  }
  return '?';
}

void PCFKeypad::_expanderWrite(byte data) {
  Wire.beginTransmission(_address);
  Wire.send(data);
  Wire.endTransmission();
}

byte PCFKeypad::_expanderRead() {
  byte data;
  Wire.requestFrom(_address, 1);
  if (Wire.available()) {
    data = Wire.receive();
  } else {
    if (_debug) {
      Serial.println("ERROR: Wire.available() returned false");
    }
  }
  return data;
}
