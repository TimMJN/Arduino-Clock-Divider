////////////////////////////////////////////////////////////////////////////////////////////////////

/*
    This function maps the MAX7219 outputs to the 2354BS displays
    to form numbers. This is neccesairy as this combination does
    not follow the standard 7-segment display configuration.

    If you use different displays, or have wired them differently, 
    this is the function you need to change.
*/

////////////////////////////////////////////////////////////////////////////////////////////////////

#define CLEAR 255

byte display_data[N_MAX7219][8];  // current display data

void set_display(byte index, byte value) {
  // find the right display to write to
  // this is specific to how to displays are wired to the MAX7219's
  byte address = index / 4;
  byte startRow = 4 * ((index % 4) / 2);
  byte startCol = 4 * ((index + 1) % 2);

  // clear the screen if CLEAR value is found
  if (value == CLEAR) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        bitWrite(display_data[address][startRow + i], startCol + j, 0);
      }
    }
  }
  else {

    // find the 3 digits of the value
    byte digit3 = value % 10;
    byte digit2 = ((value - digit3) / 10) % 10;
    byte digit1 = ((value - digit3 - 10 * digit2) / 100) % 10;

    // digit 1
    switch (digit1) {
      case 0:
        bitWrite(display_data[address][startRow + 3], startCol + 0, 0);
        bitWrite(display_data[address][startRow + 3], startCol + 2, 0);
        break;
      case 1:
        bitWrite(display_data[address][startRow + 3], startCol + 0, 1);
        bitWrite(display_data[address][startRow + 3], startCol + 2, 1);
        break;
    }

    // digit 2
    // don't write leading zeros
    if (digit1 == 0 && digit2 == 0) {
      bitWrite(display_data[address][startRow + 0], startCol + 3, 0);
      bitWrite(display_data[address][startRow + 1], startCol + 3, 0);
      bitWrite(display_data[address][startRow + 2], startCol + 3, 0);
      bitWrite(display_data[address][startRow + 3], startCol + 3, 0);
      bitWrite(display_data[address][startRow + 2], startCol + 2, 0);
      bitWrite(display_data[address][startRow + 0], startCol + 2, 0);
      bitWrite(display_data[address][startRow + 1], startCol + 2, 0);
    }
    else {
      switch (digit2) {
        case 0:
          bitWrite(display_data[address][startRow + 0], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 1], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 2], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 3], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 2], startCol + 2, 1);
          bitWrite(display_data[address][startRow + 0], startCol + 2, 1);
          bitWrite(display_data[address][startRow + 1], startCol + 2, 0);
          break;
        case 1:
          bitWrite(display_data[address][startRow + 0], startCol + 3, 0);
          bitWrite(display_data[address][startRow + 1], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 2], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 3], startCol + 3, 0);
          bitWrite(display_data[address][startRow + 2], startCol + 2, 0);
          bitWrite(display_data[address][startRow + 0], startCol + 2, 0);
          bitWrite(display_data[address][startRow + 1], startCol + 2, 0);
          break;
        case 2:
          bitWrite(display_data[address][startRow + 0], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 1], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 2], startCol + 3, 0);
          bitWrite(display_data[address][startRow + 3], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 2], startCol + 2, 1);
          bitWrite(display_data[address][startRow + 0], startCol + 2, 0);
          bitWrite(display_data[address][startRow + 1], startCol + 2, 1);
          break;
        case 3:
          bitWrite(display_data[address][startRow + 0], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 1], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 2], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 3], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 2], startCol + 2, 0);
          bitWrite(display_data[address][startRow + 0], startCol + 2, 0);
          bitWrite(display_data[address][startRow + 1], startCol + 2, 1);
          break;
        case 4:
          bitWrite(display_data[address][startRow + 0], startCol + 3, 0);
          bitWrite(display_data[address][startRow + 1], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 2], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 3], startCol + 3, 0);
          bitWrite(display_data[address][startRow + 2], startCol + 2, 0);
          bitWrite(display_data[address][startRow + 0], startCol + 2, 1);
          bitWrite(display_data[address][startRow + 1], startCol + 2, 1);
          break;
        case 5:
          bitWrite(display_data[address][startRow + 0], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 1], startCol + 3, 0);
          bitWrite(display_data[address][startRow + 2], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 3], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 2], startCol + 2, 0);
          bitWrite(display_data[address][startRow + 0], startCol + 2, 1);
          bitWrite(display_data[address][startRow + 1], startCol + 2, 1);
          break;
        case 6:
          bitWrite(display_data[address][startRow + 0], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 1], startCol + 3, 0);
          bitWrite(display_data[address][startRow + 2], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 3], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 2], startCol + 2, 1);
          bitWrite(display_data[address][startRow + 0], startCol + 2, 1);
          bitWrite(display_data[address][startRow + 1], startCol + 2, 1);
          break;
        case 7:
          bitWrite(display_data[address][startRow + 0], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 1], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 2], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 3], startCol + 3, 0);
          bitWrite(display_data[address][startRow + 2], startCol + 2, 0);
          bitWrite(display_data[address][startRow + 0], startCol + 2, 0);
          bitWrite(display_data[address][startRow + 1], startCol + 2, 0);
          break;
        case 8:
          bitWrite(display_data[address][startRow + 0], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 1], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 2], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 3], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 2], startCol + 2, 1);
          bitWrite(display_data[address][startRow + 0], startCol + 2, 1);
          bitWrite(display_data[address][startRow + 1], startCol + 2, 1);
          break;
        case 9:
          bitWrite(display_data[address][startRow + 0], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 1], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 2], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 3], startCol + 3, 1);
          bitWrite(display_data[address][startRow + 2], startCol + 2, 0);
          bitWrite(display_data[address][startRow + 0], startCol + 2, 1);
          bitWrite(display_data[address][startRow + 1], startCol + 2, 1);
          break;
      }
    }

    switch (digit3) {
      case 0:
        bitWrite(display_data[address][startRow + 0], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 1], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 2], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 3], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 2], startCol + 0, 1);
        bitWrite(display_data[address][startRow + 0], startCol + 0, 1);
        bitWrite(display_data[address][startRow + 1], startCol + 0, 0);
        break;
      case 1:
        bitWrite(display_data[address][startRow + 0], startCol + 1, 0);
        bitWrite(display_data[address][startRow + 1], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 2], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 3], startCol + 1, 0);
        bitWrite(display_data[address][startRow + 2], startCol + 0, 0);
        bitWrite(display_data[address][startRow + 0], startCol + 0, 0);
        bitWrite(display_data[address][startRow + 1], startCol + 0, 0);
        break;
      case 2:
        bitWrite(display_data[address][startRow + 0], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 1], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 2], startCol + 1, 0);
        bitWrite(display_data[address][startRow + 3], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 2], startCol + 0, 1);
        bitWrite(display_data[address][startRow + 0], startCol + 0, 0);
        bitWrite(display_data[address][startRow + 1], startCol + 0, 1);
        break;
      case 3:
        bitWrite(display_data[address][startRow + 0], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 1], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 2], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 3], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 2], startCol + 0, 0);
        bitWrite(display_data[address][startRow + 0], startCol + 0, 0);
        bitWrite(display_data[address][startRow + 1], startCol + 0, 1);
        break;
      case 4:
        bitWrite(display_data[address][startRow + 0], startCol + 1, 0);
        bitWrite(display_data[address][startRow + 1], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 2], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 3], startCol + 1, 0);
        bitWrite(display_data[address][startRow + 2], startCol + 0, 0);
        bitWrite(display_data[address][startRow + 0], startCol + 0, 1);
        bitWrite(display_data[address][startRow + 1], startCol + 0, 1);
        break;
      case 5:
        bitWrite(display_data[address][startRow + 0], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 1], startCol + 1, 0);
        bitWrite(display_data[address][startRow + 2], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 3], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 2], startCol + 0, 0);
        bitWrite(display_data[address][startRow + 0], startCol + 0, 1);
        bitWrite(display_data[address][startRow + 1], startCol + 0, 1);
        break;
      case 6:
        bitWrite(display_data[address][startRow + 0], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 1], startCol + 1, 0);
        bitWrite(display_data[address][startRow + 2], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 3], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 2], startCol + 0, 1);
        bitWrite(display_data[address][startRow + 0], startCol + 0, 1);
        bitWrite(display_data[address][startRow + 1], startCol + 0, 1);
        break;
      case 7:
        bitWrite(display_data[address][startRow + 0], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 1], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 2], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 3], startCol + 1, 0);
        bitWrite(display_data[address][startRow + 2], startCol + 0, 0);
        bitWrite(display_data[address][startRow + 0], startCol + 0, 0);
        bitWrite(display_data[address][startRow + 1], startCol + 0, 0);
        break;
      case 8:
        bitWrite(display_data[address][startRow + 0], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 1], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 2], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 3], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 2], startCol + 0, 1);
        bitWrite(display_data[address][startRow + 0], startCol + 0, 1);
        bitWrite(display_data[address][startRow + 1], startCol + 0, 1);
        break;
      case 9:
        bitWrite(display_data[address][startRow + 0], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 1], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 2], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 3], startCol + 1, 1);
        bitWrite(display_data[address][startRow + 2], startCol + 0, 0);
        bitWrite(display_data[address][startRow + 0], startCol + 0, 1);
        bitWrite(display_data[address][startRow + 1], startCol + 0, 1);
        break;
    }
  }

  // write new data to displays
  for (int i = 0; i < 4; i++) {
    lc.setRow(address, startRow + i, display_data[address][startRow + i]);
  }
}
