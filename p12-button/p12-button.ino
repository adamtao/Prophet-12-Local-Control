#include <EEPROM.h>
#include <MIDI.h>
#include <EasyButton.h>

/*
 * Nifty 1-button to toggle "Local Control" on a Prophet 12 synthesizer. This
 * is handy so you can quickly go back and forth between using the Prophet as
 * a MIDI controller (with local control off) vs. a synth (with local control
 * on).
 * 
 * There are two modes: Normal mode and Setup mode.
 * 
 * Normal mode lets you use a momentary button on pin 5 to toggle the local
 * control of the Prophet 12. This device starts up without knowing the state
 * of the Prophet, so the first press turns ON local control. Each press after
 * that toggles local control on/off. The LED on pin 4 lights up when local
 * control is ON.
 * 
 * Setup mode is accessed by pressing and holding the button for 2 sec. You'll
 * see 3 quick blinks of the LED. Now you can set the MIDI channel by pressing
 * the button several times. Count 1, 2, 3... as you press the button until 
 * you get to the desired MIDI channel number. Then press and hold the button
 * for 2 sec. to exit setup mode. The new MIDI channel is stored in memory so
 * the next time you turn on the device, it will remember that channel.
 * 
 * (The default MIDI channel on a fresh device is 12.)
 * 
 * Hooking it up: (how I did it at least)
 * 
 * Arduino pin 5 -> momentary button
 * other leg of momentary button -> 1k resistor -> ground
 * 
 * Arduino pin 4 -> LED (+)
 * LED (-) -> 1k resistor -> ground
 * 
 * Arduino pin 2 (TX) -> 220 ohm resistor -> MIDI jack pin 5
 * MIDI jack pin 2 -> ground
 * MIDI jack pin 4 -> 220 ohm resistor -> +5v
 * 
 * MIDI cable from this device to MIDI In on Prophet 12
 * 
 * NOTE: You'll need to disconnect pin 2 (TX) on the Arduino while uploading
 * the code to the Arduino.
 */

#define DEFAULT_MIDI_CHANNEL 12
#define LED 4
#define BUTTON 5


MIDI_CREATE_DEFAULT_INSTANCE();
EasyButton toggle_button(BUTTON);

boolean local_control = false;
boolean setup_mode = false;
int midi_channel = DEFAULT_MIDI_CHANNEL;
int new_midi_channel = 0;
int address = 0;

void setup() {
  MIDI.begin(MIDI_CHANNEL_OFF);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  // Read stored MIDI channel from memory
  int stored_midi_channel = EEPROM.read(address);
  if (stored_midi_channel > 0 && stored_midi_channel <= 16) {
    midi_channel = stored_midi_channel;
  }

  toggle_button.begin();
  toggle_button.onPressed(perform_button_function);
  toggle_button.onPressedFor(2000, toggle_setup_mode);
}

void loop() {
  toggle_button.read();
}

void perform_button_function() {
  if (setup_mode == true) {
    increment_midi_channel();
  } else {
    toggle_local_control();
  }
}

void toggle_local_control() {
  MIDI.beginNrpn(1035, midi_channel);
  MIDI.sendNrpnValue(0, local_control ? 0 : 1, midi_channel);
  MIDI.endNrpn(midi_channel);
  digitalWrite(LED, local_control ? LOW : HIGH);
  local_control = !local_control;
}

void toggle_setup_mode() {
  setup_mode = !setup_mode;
  new_midi_channel = 0;
  blink(3);
  if (setup_mode == false && local_control == true) {
    digitalWrite(LED, HIGH); // return to LED state before setup mode
  }
}

void increment_midi_channel() {
  if (new_midi_channel < 16) {
    new_midi_channel += 1;
    EEPROM.write(address, new_midi_channel);
    midi_channel = new_midi_channel;
    digitalWrite(LED, HIGH);
    delay(50);
    digitalWrite(LED, LOW);
  }
}

void blink(int count) {
  while(count > 0) {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
    count -= 1;
  }
}
