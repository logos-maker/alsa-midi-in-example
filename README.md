# alsa-midi-in-example
Linux example code in C for a ALSA MIDI in port. Small and simple code example, to get you started.

## Purpose of the program:
It's an example to get a ALSA MIDI in port, to get you started with writing our own programs.
And it can also be used for simple debuggnig of your programs using ALSA MIDI out. 

### dependencies for compilation (please report missing dependencies to list).
	sudo apt-get install libasound2-dev

### compile with: 
	gcc -o midi_in midi_in.c -lasound
 
### start program in a terminal with:
	./alsa_midi_in

### how to use program:
	Start program in terminal, and it will give a MIDI in port to connect to a MIDI-out source.
	Connect it them together with a program like aconnectgui.
	The program will print out the interpetation of the incomming messages to the terminal.
