Music_WildPokemonVictory:
	channel_count 3
	channel 1, Music_WildPokemonVictory_Ch1
	channel 2, Music_WildPokemonVictory_Ch2
	channel 3, Music_WildPokemonVictory_Ch3

Music_WildPokemonVictory_Ch1:
	tempo 126
	volume 7, 7
	duty_cycle 3
	pitch_offset 1
	note_type 12, 11, 1
	octave 3
	note B_, 1
	note A_, 1
	note Gs, 1
	note Fs, 1
	volume_envelope 11, 6
	note E_, 12
.body:
	stereo_panning FALSE, TRUE
.mainloop:
	sound_call .sub1
	note Fs, 2
	note Fs, 2
	note Fs, 4
	sound_call .sub1
	note Gs, 4
	volume_envelope 7, 8
	octave 3
	note G_, 4
	transpose 0, 1
	sound_call .sub1
	note Fs, 2
	note Fs, 2
	note Fs, 4
	sound_call .sub1
	note Gs, 4
	transpose 0, 0
	volume_envelope 7, 8
	octave 3
	note Fs, 4
	sound_loop 0, .mainloop

.sub1:
	volume_envelope 7, 1
	octave 3
	note Gs, 1
	note B_, 1
	octave 4
	note E_, 2
	octave 3
	note B_, 2
	octave 4
	note E_, 2
	octave 3
	note Gs, 1
	note Fs, 1
	note E_, 1
	note Fs, 1
	note Gs, 2
	note E_, 2
	note A_, 1
	octave 4
	note Cs, 1
	note E_, 2
	note E_, 2
	note A_, 2
	sound_ret

Music_WildPokemonVictory_Ch2:
	vibrato 18, 2, 4
	duty_cycle 2
	note_type 12, 13, 1
	octave 4
	note E_, 1
	note Fs, 1
	note Gs, 1
	note A_, 1
	volume_envelope 13, 6
	note B_, 12
.body:
	stereo_panning TRUE, FALSE
.mainloop:
	sound_call .sub1
	octave 4
	note B_, 2
	note B_, 2
	note B_, 4
	sound_call .sub1
	octave 4
	note B_, 4
	volume_envelope 8, 1
	note C_, 1
	note E_, 1
	note G_, 1
	note As, 1
	transpose 0, 1
	sound_call .sub1
	octave 4
	note B_, 2
	note B_, 2
	note B_, 4
	sound_call .sub1
	octave 4
	note B_, 4
	transpose 0, 0
	volume_envelope 8, 1
	octave 3
	note B_, 1
	octave 4
	note Ds, 1
	note Fs, 1
	note A_, 1
	sound_loop 0, .mainloop

.sub1:
	volume_envelope 8, 1
	octave 4
	note B_, 2
	note A_, 2
	note Gs, 2
	note A_, 2
	note B_, 2
	note B_, 2
	note B_, 4
	octave 5
	note Cs, 2
	note Cs, 2
	note Cs, 4
	sound_ret

Music_WildPokemonVictory_Ch3:
	note_type 12, 2, 0
	rest 6
	octave 6
	note Ds, 1
	rest 1
	note E_, 1
	rest 3
	note E_, 1
	rest 3
.body:
.mainloop:
	sound_call .sub1
	note Ds, 2
	note Cs, 2
	octave 2
	note B_, 2
	rest 2
	sound_call .sub1
	note E_, 2
	rest 2
	note C_, 4
	transpose 0, 1
	sound_call .sub1
	note Ds, 2
	note Cs, 2
	octave 2
	note B_, 2
	rest 2
	sound_call .sub1
	note E_, 2
	rest 2
	transpose 0, 0
	octave 2
	note B_, 4
	sound_loop 0, .mainloop

.sub1:
	volume_envelope 2, 5
	octave 3
	note E_, 2
	rest 4
	note E_, 2
	rest 2
	note D_, 2
	rest 2
	note D_, 2
	note Cs, 2
	rest 4
	note Cs, 2
	sound_ret
