////////////////////////////////////////////////////////////////////////////////////////////////////

/*
    This function maps the MAX7219 outputs to the 2354BS displays
    to form numbers. This is neccesairy as this combination does
    not follow the standard 7-segment display configuration.

    It's quite messy. If you use different displays, or have
    wired them differently, this is the function you need to change.
*/

////////////////////////////////////////////////////////////////////////////////////////////////////

#define CLEAR 255

void set_display(byte index, byte value) {
  // find the right display to write to
  // this is specific to how to displays are wired to the MAX7219's
  byte address = index / 4;
  byte row = 4 * ((index % 4) / 2);
  byte col = (4 * (index % 2)) + 1;

  // clear the screen if CLEAR value is found
  if (value == CLEAR) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        lc.setLed(address, row + i, (col + j)%8, false);
      }
    }
    return 0;
  }

  // find the 3 digits of the value
  byte digit3 = value % 10;
  byte digit2 = ((value - digit3) / 10) % 10;
  byte digit1 = ((value - digit3 - 10 * digit2) / 100) % 10;

  // digit 1
  switch (digit1) {
    case 0:
      lc.setLed(address, row + 3, (col + 3)%8, false);
      lc.setLed(address, row + 3, col + 1, false);
      break;
    case 1:
      lc.setLed(address, row + 3, (col + 3)%8, true);
      lc.setLed(address, row + 3, col + 1, true);
      break;
  }

  // digit 2
  // don't write leading zeros
  if (digit1 == 0 && digit2 == 0) {
    lc.setLed(address, row + 0, col + 0, false);
    lc.setLed(address, row + 1, col + 0, false);
    lc.setLed(address, row + 2, col + 0, false);
    lc.setLed(address, row + 3, col + 0, false);
    lc.setLed(address, row + 2, col + 1, false);
    lc.setLed(address, row + 0, col + 1, false);
    lc.setLed(address, row + 1, col + 1, false);
  }
  else {
    switch (digit2) {
      case 0:
        lc.setLed(address, row + 0, col + 0, true);
        lc.setLed(address, row + 1, col + 0, true);
        lc.setLed(address, row + 2, col + 0, true);
        lc.setLed(address, row + 3, col + 0, true);
        lc.setLed(address, row + 2, col + 1, true);
        lc.setLed(address, row + 0, col + 1, true);
        lc.setLed(address, row + 1, col + 1, false);
        break;
      case 1:
        lc.setLed(address, row + 0, col + 0, false);
        lc.setLed(address, row + 1, col + 0, true);
        lc.setLed(address, row + 2, col + 0, true);
        lc.setLed(address, row + 3, col + 0, false);
        lc.setLed(address, row + 2, col + 1, false);
        lc.setLed(address, row + 0, col + 1, false);
        lc.setLed(address, row + 1, col + 1, false);
        break;
      case 2:
        lc.setLed(address, row + 0, col + 0, true);
        lc.setLed(address, row + 1, col + 0, true);
        lc.setLed(address, row + 2, col + 0, false);
        lc.setLed(address, row + 3, col + 0, true);
        lc.setLed(address, row + 2, col + 1, true);
        lc.setLed(address, row + 0, col + 1, false);
        lc.setLed(address, row + 1, col + 1, true);
        break;
      case 3:
        lc.setLed(address, row + 0, col + 0, true);
        lc.setLed(address, row + 1, col + 0, true);
        lc.setLed(address, row + 2, col + 0, true);
        lc.setLed(address, row + 3, col + 0, true);
        lc.setLed(address, row + 2, col + 1, false);
        lc.setLed(address, row + 0, col + 1, false);
        lc.setLed(address, row + 1, col + 1, true);
        break;
      case 4:
        lc.setLed(address, row + 0, col + 0, false);
        lc.setLed(address, row + 1, col + 0, true);
        lc.setLed(address, row + 2, col + 0, true);
        lc.setLed(address, row + 3, col + 0, false);
        lc.setLed(address, row + 2, col + 1, false);
        lc.setLed(address, row + 0, col + 1, true);
        lc.setLed(address, row + 1, col + 1, true);
        break;
      case 5:
        lc.setLed(address, row + 0, col + 0, true);
        lc.setLed(address, row + 1, col + 0, false);
        lc.setLed(address, row + 2, col + 0, true);
        lc.setLed(address, row + 3, col + 0, true);
        lc.setLed(address, row + 2, col + 1, false);
        lc.setLed(address, row + 0, col + 1, true);
        lc.setLed(address, row + 1, col + 1, true);
        break;
      case 6:
        lc.setLed(address, row + 0, col + 0, true);
        lc.setLed(address, row + 1, col + 0, false);
        lc.setLed(address, row + 2, col + 0, true);
        lc.setLed(address, row + 3, col + 0, true);
        lc.setLed(address, row + 2, col + 1, true);
        lc.setLed(address, row + 0, col + 1, true);
        lc.setLed(address, row + 1, col + 1, true);
        break;
      case 7:
        lc.setLed(address, row + 0, col + 0, true);
        lc.setLed(address, row + 1, col + 0, true);
        lc.setLed(address, row + 2, col + 0, true);
        lc.setLed(address, row + 3, col + 0, false);
        lc.setLed(address, row + 2, col + 1, false);
        lc.setLed(address, row + 0, col + 1, false);
        lc.setLed(address, row + 1, col + 1, false);
        break;
      case 8:
        lc.setLed(address, row + 0, col + 0, true);
        lc.setLed(address, row + 1, col + 0, true);
        lc.setLed(address, row + 2, col + 0, true);
        lc.setLed(address, row + 3, col + 0, true);
        lc.setLed(address, row + 2, col + 1, true);
        lc.setLed(address, row + 0, col + 1, true);
        lc.setLed(address, row + 1, col + 1, true);
        break;
      case 9:
        lc.setLed(address, row + 0, col + 0, true);
        lc.setLed(address, row + 1, col + 0, true);
        lc.setLed(address, row + 2, col + 0, true);
        lc.setLed(address, row + 3, col + 0, true);
        lc.setLed(address, row + 2, col + 1, false);
        lc.setLed(address, row + 0, col + 1, true);
        lc.setLed(address, row + 1, col + 1, true);
        break;
    }
  }

  switch (digit3) {
    case 0:
      lc.setLed(address, row + 0, col + 2, true);
      lc.setLed(address, row + 1, col + 2, true);
      lc.setLed(address, row + 2, col + 2, true);
      lc.setLed(address, row + 3, col + 2, true);
      lc.setLed(address, row + 2, (col + 3)%8, true);
      lc.setLed(address, row + 0, (col + 3)%8, true);
      lc.setLed(address, row + 1, (col + 3)%8, false);
      break;
    case 1:
      lc.setLed(address, row + 0, col + 2, false);
      lc.setLed(address, row + 1, col + 2, true);
      lc.setLed(address, row + 2, col + 2, true);
      lc.setLed(address, row + 3, col + 2, false);
      lc.setLed(address, row + 2, (col + 3)%8, false);
      lc.setLed(address, row + 0, (col + 3)%8, false);
      lc.setLed(address, row + 1, (col + 3)%8, false);
      break;
    case 2:
      lc.setLed(address, row + 0, col + 2, true);
      lc.setLed(address, row + 1, col + 2, true);
      lc.setLed(address, row + 2, col + 2, false);
      lc.setLed(address, row + 3, col + 2, true);
      lc.setLed(address, row + 2, (col + 3)%8, true);
      lc.setLed(address, row + 0, (col + 3)%8, false);
      lc.setLed(address, row + 1, (col + 3)%8, true);
      break;
    case 3:
      lc.setLed(address, row + 0, col + 2, true);
      lc.setLed(address, row + 1, col + 2, true);
      lc.setLed(address, row + 2, col + 2, true);
      lc.setLed(address, row + 3, col + 2, true);
      lc.setLed(address, row + 2, (col + 3)%8, false);
      lc.setLed(address, row + 0, (col + 3)%8, false);
      lc.setLed(address, row + 1, (col + 3)%8, true);
      break;
    case 4:
      lc.setLed(address, row + 0, col + 2, false);
      lc.setLed(address, row + 1, col + 2, true);
      lc.setLed(address, row + 2, col + 2, true);
      lc.setLed(address, row + 3, col + 2, false);
      lc.setLed(address, row + 2, (col + 3)%8, false);
      lc.setLed(address, row + 0, (col + 3)%8, true);
      lc.setLed(address, row + 1, (col + 3)%8, true);
      break;
    case 5:
      lc.setLed(address, row + 0, col + 2, true);
      lc.setLed(address, row + 1, col + 2, false);
      lc.setLed(address, row + 2, col + 2, true);
      lc.setLed(address, row + 3, col + 2, true);
      lc.setLed(address, row + 2, (col + 3)%8, false);
      lc.setLed(address, row + 0, (col + 3)%8, true);
      lc.setLed(address, row + 1, (col + 3)%8, true);
      break;
    case 6:
      lc.setLed(address, row + 0, col + 2, true);
      lc.setLed(address, row + 1, col + 2, false);
      lc.setLed(address, row + 2, col + 2, true);
      lc.setLed(address, row + 3, col + 2, true);
      lc.setLed(address, row + 2, (col + 3)%8, true);
      lc.setLed(address, row + 0, (col + 3)%8, true);
      lc.setLed(address, row + 1, (col + 3)%8, true);
      break;
    case 7:
      lc.setLed(address, row + 0, col + 2, true);
      lc.setLed(address, row + 1, col + 2, true);
      lc.setLed(address, row + 2, col + 2, true);
      lc.setLed(address, row + 3, col + 2, false);
      lc.setLed(address, row + 2, (col + 3)%8, false);
      lc.setLed(address, row + 0, (col + 3)%8, false);
      lc.setLed(address, row + 1, (col + 3)%8, false);
      break;
    case 8:
      lc.setLed(address, row + 0, col + 2, true);
      lc.setLed(address, row + 1, col + 2, true);
      lc.setLed(address, row + 2, col + 2, true);
      lc.setLed(address, row + 3, col + 2, true);
      lc.setLed(address, row + 2, (col + 3)%8, true);
      lc.setLed(address, row + 0, (col + 3)%8, true);
      lc.setLed(address, row + 1, (col + 3)%8, true);
      break;
    case 9:
      lc.setLed(address, row + 0, col + 2, true);
      lc.setLed(address, row + 1, col + 2, true);
      lc.setLed(address, row + 2, col + 2, true);
      lc.setLed(address, row + 3, col + 2, true);
      lc.setLed(address, row + 2, (col + 3)%8, false);
      lc.setLed(address, row + 0, (col + 3)%8, true);
      lc.setLed(address, row + 1, (col + 3)%8, true);
      break;
  }
}
