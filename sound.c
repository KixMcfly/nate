#include <allegro.h>
#include "sound.h"

static DATAFILE *midi_loaded = NULL;

int
sound_midi_load_play (char *mn, char *dfn)
{
	midi_loaded = load_datafile_object (dfn, mn);
	play_midi ((MIDI *)midi_loaded->dat, TRUE);
}

void
sound_midi_stop_free ()
{
	stop_midi ();
	
	if (midi_loaded){
		unload_datafile_object (midi_loaded);
		midi_loaded = NULL;
	}
}
