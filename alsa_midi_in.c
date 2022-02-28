/* 
 * Daniel Viksporre 2022-02-28
 * Demo example of ALSA MIDI in c-code.
 * Licence type: MIT No Attribution
 *
 * Purpose of the program:
 * It's an example to get a ALSA MIDI in port, to you started with writing our own programs.
 * And can be used for simple debuggnig of your programs using ALSA MIDI out. 
 *
 * dependencies for compilation (please report missing dependencies to list).
 *	sudo apt-get install libasound2-dev
 * 
 * compile with: 
 *	gcc -o midi_in midi_in.c -lasound
 *
 * start program in a terminal with:
 *	./alsa_midi_in
 *
 * how to use program:
 * 	Start program in terminal it will give a MIDI in unit to connect to a MIDI-out source.
 *	Connect it together with for example a program like aconnectgui
 *	The program will print out the interpetation of the incomming messages to the terminal.
 *
 * API interface for ALSA sequencer:
 * 	cat /usr/include/alsa/seqmid.h		// The ALSA sequencer API
 *	cat /usr/include/alsa/seq_event.h  	// The event structure
 */

#include <stdio.h>
#include <alsa/asoundlib.h>

int main(void)
{
	snd_seq_t *seq_handle = NULL;
	snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_INPUT, 0); // Create a alsa client.

	snd_seq_set_client_name(seq_handle, "MIDI message terminal"); // Give you client a name (not expicity needed, will work without it).
	snd_seq_create_simple_port(seq_handle, "in", SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE, SND_SEQ_PORT_TYPE_APPLICATION);// Create a MIDI in port.

	for(;;) {
		snd_seq_event_t *ev = NULL;
		snd_seq_event_input(seq_handle, &ev); // Read incomming MIDI event (blocking, the program will wait for an event)

		switch(ev->type){	// switch on the type of the MIDI event 
			case SND_SEQ_EVENT_NOTEON: // play a note
				if(ev->data.note.velocity != 0){
					printf("note on:  Channel[%d] Note[%3d] velocity(%3d)\n",
					ev->data.note.channel+1, ev->data.note.note, ev->data.note.velocity);	
					break;
				}
			// NOTE: no break; makes a fallthrough to case below. Used because velocity 0 is same as a note off
			case SND_SEQ_EVENT_NOTEOFF: // turn off a note
				printf("note off: Channel[%d] Note[%3d] velocity(%3d)\n",
				ev->data.note.channel+1, ev->data.note.note, ev->data.note.velocity);	
			break;

			case SND_SEQ_EVENT_PGMCHANGE: // change sound
				printf("program change: on Channel[%d] to no.[%2x] \n",	
				ev->data.note.channel+1,ev->data.control.value+1);	
			break;
			case SND_SEQ_EVENT_PITCHBEND: // pich bend
				printf("pitch bend: Channel[%d] value(%5d) \n",	
				ev->data.note.channel+1, ev->data.control.value);	
			break;

			// MIDI System Real-Time Messages
			case SND_SEQ_EVENT_START:	printf("START\n");	break;
			case SND_SEQ_EVENT_CONTINUE:	printf("CONTINUE\n");	break;
			case SND_SEQ_EVENT_STOP:	printf("STOP\n");	break;
			case SND_SEQ_EVENT_CLOCK:	printf("CLOCK\n");	break; // 24 clocks = 1 quarter note
			case SND_SEQ_EVENT_TICK:	printf("TICK\n");	break;

			// MIDI controll change messages
			// https://www.alsa-project.org/alsa-doc/alsa-lib/group___m_i_d_i___controllers.html
			case SND_SEQ_EVENT_CONTROLLER: // Control change MIDI message
				switch(ev->data.control.param){ // switch on cc number according to MIDI specification. See MIDI spec. to implement more cases
					case MIDI_CTL_MSB_BANK: // 0: // CC0: Bank select
						printf("Bank select: on Channel[%d] to no.[%d]\n",
						ev->data.note.channel+1,ev->data.control.value);
					break;
					case MIDI_CTL_MSB_MODWHEEL: 1: // Modulation Wheel
						printf("modulation wheel: on Channel[%d] Value(%d)\n",
						ev->data.note.channel+1,ev->data.control.value);
					break;
					// Channel Mode Messages
					case 120: // CC120: Mute channel, turn off notes immediatly
					case 123: // CC123: All MIDI Notes OFF
						printf("Panic! All notes turned off, realeases hanged notes\n");
					break;	
					default: // knob change or not recognized (there is more to decode here according to the MIDI spec.)
						printf("control change:  Channel[%d] CC[%d] Value(%3d)\n",	
						ev->data.note.channel+1, ev->data.control.param, ev->data.control.value);
					break;
				}	
			break;
			default:
				printf("unrecognized message: event type no. %d\n",ev->type); 	
			break;
		}
	}

	return 0;
}

/*
MIT No Attribution

Copyright <2022> <Daniel Viksporre>

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

