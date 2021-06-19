////////////////////////////////////////////////////////////////////////////////////////////////////

/*
   Arduino Rotating Clock Divider
   synthesizer module firmware

   by TimMJN

   v1.1
   15-05-2021

   For schematics and other information, see
   https://github.com/TimMJN/Arduino-Clock-Divider
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
#include "src/LedControl/src/LedControl.h"

// pin definitions
#define CLOCK_PIN        2
#define ENC_A_PIN        3
#define ENC_B_PIN        4
#define ENC_S_PIN        5
#define RESET_PIN        6
#define ROT_UP_PIN       7
#define ROT_DOWN_PIN     8
#define DISP_LOAD_PIN    9
#define DISP_CLOCK_PIN  10
#define DISP_DATA_PIN   11
#define SER_CLOCK_PIN   A0
#define SER_DATA_PIN    A1
#define SER_LATCH_PIN   A2

// outputs
#define N_OUTS    8                   // number of outputs
#define N_ROTATE  4                   // rotate last N outputs
#define ROTATE_FROM (N_OUTS-N_ROTATE) // first output to rotate
#define MIN_DIV   1                   // minimum division
#define MAX_DIV   199                 // maximum division

// displays
#define N_MAX7219     (N_OUTS/4)  // number of display drivers
#define BRIGHTNESS    4           // screen brightness (0-15)
#define SELECT_BLINK  200         // blink time during selecting (ms)
#define EDIT_BLINK    50          // blink time during editing (ms)
#define TIMEOUT       5000        // timeout after which editing is cancelled (ms)

// divisions
byte division[N_OUTS]  = {1, 2, 3, 4, 8, 12, 16, 32}; // initial (startup) divisions
byte counter[N_OUTS]   = {0, 0, 0, 0, 0,  0,  0,  0}; // clock pulse counters
byte rotation          = 0;   // current rotation
byte selected_div      = 0;   // division of selected output
byte selected_out      = 0;   // currently selected output
byte prev_selected_out = 0;   // previously selected output

// displays
LedControl lc = LedControl(DISP_DATA_PIN, DISP_CLOCK_PIN, DISP_LOAD_PIN, N_MAX7219);
#include "set_displays.h"         // this file holds the function for writing numbers to the displays

// modes
bool selectMode = false;  // selecting an output
bool editMode   = false;  // editing a division
unsigned long last_click; // time stamp of last encoder action

// pin/timer states
bool cur_enc_but_state    = true;
bool prev_enc_but_state   = true;
bool cur_blink            = true;
bool prev_blink           = true;
bool cur_rot_up_state     = true;
bool prev_rot_up_state    = true;
bool cur_rot_down_state   = true;
bool prev_rot_down_state  = true;
bool cur_reset_state      = true;
bool prev_reset_state     = true;

////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  // pinModes
  pinMode(CLOCK_PIN,      INPUT);
  pinMode(ENC_A_PIN,      INPUT);
  pinMode(ENC_B_PIN,      INPUT);
  pinMode(ENC_S_PIN,      INPUT);
  pinMode(RESET_PIN,      INPUT);
  pinMode(ROT_UP_PIN,     INPUT);
  pinMode(ROT_DOWN_PIN,   INPUT);
  pinMode(DISP_LOAD_PIN,  OUTPUT);
  pinMode(DISP_CLOCK_PIN, OUTPUT);
  pinMode(DISP_DATA_PIN,  OUTPUT);
  pinMode(SER_DATA_PIN,   OUTPUT);
  pinMode(SER_CLOCK_PIN,  OUTPUT);
  pinMode(SER_LATCH_PIN,  OUTPUT);

  // output shift register
  digitalWrite(SER_LATCH_PIN, LOW);
  set_outputs_low(); // set all outputs low
  digitalWrite(SER_LATCH_PIN, HIGH);
  set_outputs_next(); // prepare for next pulse

  // initialise displays
  for (int i = 0; i < N_MAX7219; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, BRIGHTNESS);
  }
  for (int i = 0; i < N_OUTS; i++) {
    set_display(i, division[i]);
  }

  // set-up interrups
  attachInterrupt(digitalPinToInterrupt(ENC_A_PIN), encoder_isr, FALLING);
  attachInterrupt(digitalPinToInterrupt(CLOCK_PIN), clock_isr  , CHANGE );
  interrupts();

  // initialise timeout timer
  last_click = millis();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  bool div_changed = false; // has any division changed this loop?

  // read buttons/inputs
  cur_enc_but_state    = digitalRead(ENC_S_PIN);
  cur_rot_up_state     = digitalRead(ROT_UP_PIN);
  cur_rot_down_state   = digitalRead(ROT_DOWN_PIN);
  cur_reset_state      = digitalRead(RESET_PIN);

  // process encoder push button
  if (!cur_enc_but_state && prev_enc_but_state) {
    last_click = millis();

    if (!(selectMode || editMode)) {
      selectMode   = true; // enter select mode on first push
      selected_div = division[selected_out];
    }
    else if (selectMode) {
      selectMode = false; // start editing selected division on next push
      editMode   = true;
    }
    else if (editMode) {
      division[selected_out] = selected_div; // confirm edited division on last push
      counter[selected_out] = 0;             // reset this outputs counter
      editMode   = false;                    // jump back to selection mode
      selectMode = true;
      div_changed  = true;
    }
  }

  // timeout if the encoder hasn't been used for a while
  if (selectMode || editMode) {
    if ((millis() - last_click) > TIMEOUT) {
      selectMode   = false; // stop selecting/editing
      editMode     = false;
      selected_div = division[selected_out]; // restore division
      set_display(selected_out, selected_div);
    }
  }

  // process rotation/reset inputs
  if (!cur_rot_up_state && prev_rot_up_state) {
    rotate(1);
    div_changed = true;
  }
  if (!cur_rot_down_state && prev_rot_down_state) {
    rotate(N_ROTATE - 1);
    div_changed = true;
  }
  if (!cur_reset_state && prev_reset_state) {
    rotate(N_ROTATE - rotation);

    noInterrupts(); // do not interrupts resetting of counters
    for (int i = 0; i < N_OUTS; i++)
      counter[i] = 0; // reset counters
    interrupts();  // re-enable interrupts
    div_changed = true;
  }

  // if any division has changed and we're in between clock pulses, update the shift register
  if (div_changed && digitalRead(CLOCK_PIN))
    set_outputs_next();

  // blink the selected display
  if (selectMode)
    cur_blink = (millis() / SELECT_BLINK) % 2;
  else if (editMode)
    cur_blink = (millis() / EDIT_BLINK) % 2;
  else
    cur_blink = true;

  if (cur_blink != prev_blink) {
    if (!cur_blink)
      set_display(selected_out, CLEAR);
    else
      set_display(selected_out, selected_div);
  }

  // if a new output is selected, re-enable the previously selected display
  if (selected_out != prev_selected_out)
    set_display(prev_selected_out, division[prev_selected_out]);

  // update previous cycle values
  prev_blink          = cur_blink;
  prev_selected_out   = selected_out;
  prev_enc_but_state  = cur_enc_but_state;
  prev_rot_up_state   = cur_rot_up_state;
  prev_rot_down_state = cur_rot_down_state;
  prev_reset_state    = cur_reset_state;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// interrupt routine
// read encoder direction and change selected output / division accordingly
void encoder_isr() {
  last_click = millis();

  // read direction
  bool dir = !digitalRead(ENC_B_PIN);

  // enter select mode on the first tic
  if (!(selectMode || editMode)) {
    selectMode = true;
  }

  // select next display
  if (selectMode) {
    if (dir)
      selected_out++;
    else
      selected_out--;

    selected_out %= N_OUTS; // constrain selected output index
    selected_div = division[selected_out];
    return;
  }

  // update division
  if (editMode) {
    if (dir)
      selected_div++;
    else
      selected_div--;

    selected_div = min(max(MIN_DIV, selected_div), MAX_DIV); // constrain division
    return;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// rotate the division of the last N_ROTATE outputs with a given increment
void rotate(byte increment) {
  byte new_division[N_OUTS];
  byte new_counter[N_OUTS];
  byte new_selected_out = selected_out;

  for (int i = ROTATE_FROM; i < N_OUTS; i++) {  // loop over all rotating outputs
    byte moveTo = (i + increment - ROTATE_FROM) % N_ROTATE + ROTATE_FROM;  // find the output to move the current division to
    new_division[moveTo] = division[i];         // save the new division
    new_counter[moveTo]  = counter[i];          // save the counter value
    if (selected_out == i) {
      new_selected_out = moveTo;                // move the selected output along
    }
  }

  noInterrupts();                               // disable interrupts while assigning the new values
  for (int i = ROTATE_FROM; i < N_OUTS; i++) {  // loop over all rotating outputs
    division[i] = new_division[i];              // assign the new division
    counter[i]  = new_counter[i];               // assign the new counter
  }
  interrupts();                                 // re-enable interrupts

  for (int i = ROTATE_FROM; i < N_OUTS; i++) {  // loop over all rotating outputs
    if (selected_out == i)                            // display the new division
      set_display(selected_out, selected_div);
    else
      set_display(i, division[i]);
  }
  selected_out = new_selected_out;               // update the currently selected output

  rotation = (rotation + increment) % N_ROTATE; // remember the current rotation
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// interrupt routine
// push the shift register to the outputs, update counters and update shift register
void clock_isr() {
  // push to output register (pulse latch pin)
  digitalWrite(SER_LATCH_PIN, HIGH);
  digitalWrite(SER_LATCH_PIN, LOW);

  if (digitalRead(CLOCK_PIN)) {
    set_outputs_next();
  }
  else {
    set_outputs_low();
    // increment counters
    for (int i = 0; i < N_OUTS; i++) {
      counter[i]++;
      counter[i] %= division[i];
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// set all outputs low on the shift register
void set_outputs_low() {
  noInterrupts(); // do not interrupt writing to the shift register

  digitalWrite(SER_DATA_PIN, LOW); // set output low
  for (int i = (N_OUTS-1); i >= 0; i--) {
    digitalWrite(SER_CLOCK_PIN, HIGH); // give clock pin a pulse
    digitalWrite(SER_CLOCK_PIN, LOW);
  }

  interrupts(); // re-enable interrupts
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// set the shift register in anticipation of the next clock pulse
void set_outputs_next() {
  noInterrupts(); // do not interrupt writing to the shift register

  // loop over the outputs
  for (int i = (N_OUTS-1); i >= 0; i--) {
    if (counter[i] == 0)                 // check the counter value
      digitalWrite(SER_DATA_PIN, HIGH);  // set output high
    else
      digitalWrite(SER_DATA_PIN, LOW);   // set output low

    digitalWrite(SER_CLOCK_PIN, HIGH);   // give clock pin a pulse
    digitalWrite(SER_CLOCK_PIN, LOW);
  }

  interrupts(); // re-enable interrupts
}
