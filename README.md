# Arduino Clock Divider
Clock divider module based on Arduino Nano, compatible with Eurorack and Kosmo modular synthesizers. This is a DIY project, suitable for beginners, although a general knowledge of electronics, basic tools and skills are required to build this module. The schematics presented here feature:
 
 - 8 buffered outputs
 - adjustable division from 1 to 199
 - 7-segment displays showing division
 - internal clock (20-600 bpm) with separate output
 - external clock input
 - rotate up/down and reset inputs
 - adjustable number of rotating outputs

A [short demonstration](https://youtu.be/Doicn2hUYTk) is available on YouTube. View the [schematic](schematic+BOM/arduino_clock_divider.pdf) and [bill of materials](https://htmlpreview.github.io/?https://github.com/TimMJN/Arduino-Clock-Divider/blob/main/schematic%2BBOM/arduino_clock_divider_BOM.html) on this repository.

# Frequently Asked Questions
## The build
#### Do you have PCBs / panels available?
A PCB design for Kosmo format is available, along with a panel drill guide. I might have some PCBs available, please get in touch. However, it is also possible to build this design on perfboard/stripboard. I want to encourage any builder to come up with their own panel layout, make whatever feels natural to them. If you document your work, I'll be happy to add it here for future reference.

#### I cannot find 2354BS displays, can I use xx?
Yes you can. Though chances are you might need to edit the `setDisplay` function, which maps the MAX7219 display drivers to the displays.

## The code
#### Submodules
This repository uses submodules to include libraries in the firmware. Please make sure, when cloning this repository, to also include the submodules in the [`src`](https://github.com/TimMJN/Arduino-Clock-Divider/tree/main/arduino_clock_divider_firmware/src) directory.

#### What settings are available to me in the firmware?
- `N_ROTATE` sets how many of the outputs are subjected to rotation. Its default setting is 4, which means that the lower 4 division are rotate when the rotate inputs are triggered. See below what division rotation actually means.
- `MIN_DIV` and `MAX_DIV` set the minimum and maximum available divisions.
- `BRIGHTNESS` sets the brightness of the screens, 0 being the lowest and 15 being the brightest option. Default value is 4.
- `SELECT_BLINK` and `EDIT_BLINK` set the display blinking rate during selection and editing of a division (in ms).
- `TIMEOUT` set the time delay after which the module exists selection/editing mode.
- The initial values of `division` set the divisions the module shows on startup.

## The patch
#### How do I change divisions?
By turning the rotary encoder, the module enters 'selection mode'. One of the displays will blink slowly, twist the encoder to select the division you wish to edit. Pushing down on the encoder will enable 'editing mode'. The selected division will blink fast, and the encoder can be twisted to select the desired division. Pushing the encoder again will confirm your choice.

Note: updating a division will reset the corresponding counter value. This means that by timing the final button press, outputs can be off-set with respect to eachother. This behaviour can be disabled by commenting out the following line: 

    counter[selected_out] = 0; //(line 144)

#### What does rotation of the divisions do?
By triggering the rotation inputs (either with a CV signal or by pushing the corresponding buttons), the division rates of the last few (default 4, see above how to adjust) will shift up or down. By assigning new division rates to the outputs, complex rhythms can by created.

#### What does reset do?
Triggering the reset input (either with a CV signal or by pushing the corresponding button), all rotated division will return to their original position. Furthermore, all counters will be reset. This means that all division will re-align, and all outputs will be triggered on the next clock pulse. Disable the rotating behaviour by commenting out the following line: 

    rotate(N_ROTATE - rotation); // (line 171)

Disable the re-aligning behaviour by commenting out the following lines:

    noInterrupts(); // do not interrupts resetting of counters
    for (int i = 0; i < N_OUTS; i++)
        counter[i] = 0; // reset counters
    interrupts();  // re-enable interrupts (lines 173-176)

# License
All designs in this repository are released under GPL-3.0 licence. Feel free to use and adapt them to your heart's desire. I only ask to refrain from mass-production and commercialisation of PCBs/modules, as I rely on PCB sales for funding new module designs. If you use (parts of) my designs in your own modules, please credit me on your schematics and PCBs to help users find the original creator. I call upon your good conscience to make fair use of my work shared here.

# Support
If you like the resources I have made available here, and wish to support the development of new modules, feel free to buy me a few components through a small donation. I mainly design modules for fun, but you probably already know it can be a costly endeavour. All small contributions help me, thank you very much!

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/donate?hosted_button_id=FZJELWSAH4UKU)
