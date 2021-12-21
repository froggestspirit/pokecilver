Music_LookPokemaniac:
	channel_count 3
	channel 1, Music_LookPokemaniac_Ch1
	channel 2, Music_LookPokemaniac_Ch2
	channel 3, Music_LookPokemaniac_Ch3

Music_LookPokemaniac_Ch1:
	stereo_panning FALSE, TRUE
	tempo 144
	volume 7, 7
	vibrato 2, 3, 3
	pitch_offset 2
	note_type 12, 11, 3
	rest 8
.mainloop:
.loop1:
	rest 4
	octave 3
	note As, 1
	rest 3
	note As, 1
	rest 3
	sound_loop 4, .loop1
	rest 4
	note G_, 1
	rest 3
	note G_, 1
	rest 3
	sound_loop 4, .loop1
	sound_loop 0, .mainloop

Music_LookPokemaniac_Ch2:
	stereo_panning TRUE, TRUE
	vibrato 2, 3, 3
	pitch_offset 1
	note_type 12, 11, 3
	octave 2
	note A_, 1
	note Fs, 1
	note Ds, 1
	note C_, 1
	octave 1
	note A_, 4
.mainloop:
.loop1:
	octave 2
	note C_, 2
	rest 2
	octave 3
	note Fs, 1
	rest 3
	note A_, 1
	rest 3
	octave 1
	note G_, 2
	rest 2
	octave 3
	note C_, 1
	rest 3
	note Ds, 1
	rest 3
	sound_loop 2, .loop1
.loop2:
	octave 1
	note A_, 2
	rest 2
	octave 3
	note Ds, 1
	rest 3
	note Fs, 1
	rest 3
	octave 1
	note E_, 2
	rest 2
	octave 2
	note A_, 1
	rest 3
	octave 3
	note C_, 1
	rest 3
	sound_loop 2, .loop2
	sound_loop 0, .mainloop

Music_LookPokemaniac_Ch3:
	stereo_panning TRUE, FALSE
	vibrato 6, 3, 3
	note_type 12, 1, 5
	octave 4
	note C_, 1
	note Ds, 1
	note Fs, 1
	note A_, 1
	octave 5
	note C_, 4
	volume_envelope 1, 0
.mainloop:
	sound_call .sub1
	volume_envelope 1, 4
	sound_call .sub1
	volume_envelope 1, 0
	sound_loop 0, .mainloop

.sub1:
	note As, 6
	note A_, 2
	note Gs, 2
	note G_, 2
	note Fs, 6
	note F_, 2
	note Fs, 2
	note A_, 2
	octave 4
	note Ds, 4
	note C_, 1
	rest 1
	note Ds, 1
	rest 1
	note C_, 1
	rest 1
	note Ds, 1
	rest 1
	octave 5
	note Fs, 4
	note C_, 1
	rest 1
	note Fs, 1
	rest 1
	note C_, 1
	rest 1
	note Fs, 1
	rest 1
	note G_, 6
	note Fs, 2
	note F_, 2
	note E_, 2
	note Ds, 6
	note D_, 2
	note Ds, 2
	note Fs, 2
	note C_, 4
	octave 4
	note A_, 1
	rest 1
	octave 5
	note C_, 1
	rest 1
	note Ds, 1
	rest 1
	note C_, 1
	rest 1
	note Ds, 4
	octave 4
	note A_, 1
	rest 1
	octave 5
	note Ds, 1
	rest 1
	note Fs, 1
	rest 1
	note C_, 1
	rest 1
	sound_ret
