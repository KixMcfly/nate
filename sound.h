#ifndef __SOUND_H

enum
{
	SND_DOOR,
	NUM_SOUNDS
};

void
sound_midi_load_play (char *dfn, char *mn);

void
sound_midi_stop_free ();

void
sound_play (int sn);

void
sound_load (char *dfn);

void
sound_free (void);

#endif
