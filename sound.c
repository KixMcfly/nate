#include <allegro.h>
#include "sound.h"

static DATAFILE *midi_loaded = NULL, **sound_list = NULL;

void
sound_midi_load_play (char *dfn, char *mn)
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

void
sound_play (int sn)
{
	play_sample ((SAMPLE *)sound_list[sn]->dat, 155, 128, 1000, 0);
}

void
sound_global_load (char *dfn)
{
	int i;
	sound_list = (DATAFILE **) malloc (NUM_SOUNDS * sizeof (DATAFILE *));
	
	sound_list[0] = load_datafile_object (dfn, "DOOR_WAV");
}

void
sound_free (void)
{
	int i;
	for (i = 0; i < NUM_SOUNDS; i++)
	{
		unload_datafile_object (sound_list[i]);
		sound_list[i] = NULL;
	}
	
	free (sound_list);
	sound_list = NULL;
}
