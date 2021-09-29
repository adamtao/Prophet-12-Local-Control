# P12 Local Control Button

### Motivation

If you're like me, I use my DSI/Sequential Prophet 12 synthesizer a lot.
Lately I use it both as my primary MIDI controller and as a standalone
synth.

The synth has a setting in the "Global" menu to turn "Local Control"
on or off. Generally, I like having Local Control turned ON when using
it as a synth. It's just a nicer experience to have all of the controls
function as expected.

When using it as a MIDI controller with my DAW, I turn Local Control OFF.

Well, this tends to get tedious using the Global menu to toggle the two
modes. I've memorized the setting as item 15 in the menu to scroll to.
So, rather than going to Global, scrolling 15 items down, then toggling
the setting, I've made an external button to toggle it.

I decided to use up the Prophet's MIDI IN jack for this project since
I had previously only been using the USB MIDI. Sure, it's kind of a
waste of a MIDI input for a single button, but hey I like it.

## Code Notes

The code here is an Arduino sketch to send the P12 NRPN command which
corresponds to Local Control (1035). Some of the other Prophets of
the same era use the same NRPN, so this should also work for the OB6
and Prophet 6, and maybe others.

You'll notice the code depends on a few libraries. The Arduino IDE should
help you locate and install them if you don't have them already:

* EEPROM
* [MIDI](https://github.com/FortySevenEffects/arduino_midi_library)
* [EasyButton](https://github.com/evert-arias/EasyButton)

### The Circuit

The circuit consists of a momentary button, a convenience LED, a few
resistors, MIDI connector and an Arduino. In my case I used an Arduino
Nano, but others should work, too.

I've included some JPGs showing off my rudimentary skills in making a
schematic and a breadboard layout. Prepare to be impressed, but do
double-check everything. I put this together _after_ I made my device.

### MIDI Channel Configuration

By default, the device sends the message on MIDI channel 12. To change
that, alter the code, or follow this sequence after building the
device:

1. Press and hold the button, observe 3 LED flashes
2. Press the button again several times. The number of times you press will be the new MIDI channel.
3. Press and hold the button, observe 3 LED flashes

Now the new channel is stored in EEPROM.

