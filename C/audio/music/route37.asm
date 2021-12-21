Music_Route37:
	channel_count 4
	channel 1, Music_Route37_Ch1
	channel 2, Music_Route37_Ch2
	channel 3, Music_Route37_Ch3
	channel 4, Music_Route37_Ch4

Music_Route37_Ch1:
	tempo 144
	volume 7, 7
	duty_cycle 3
	pitch_offset 2
	vibrato 16, 2, 2
	note_type 12, 11, 3
	rest 16
	rest 16
.mainloop:
	volume_envelope 11, 3
	rest 2
	octave 2
	note Fs, 2
	note B_, 2
	octave 3
	note E_, 2
	volume_envelope 11, 7
	note Ds, 10
	volume_envelope 11, 3
	note E_, 4
	note Ds, 4
	note Cs, 4
	octave 2
	note B_, 2
	octave 3
	note Ds, 3
	octave 2
	note B_, 1
	volume_envelope 11, 7
	note Fs, 8
	note B_, 8
	octave 3
	note B_, 12
	volume_envelope 11, 3
	rest 2
	octave 2
	note Fs, 2
	note B_, 2
	octave 3
	note E_, 2
	volume_envelope 11, 7
	note Ds, 10
	octave 2
	note A_, 4
	octave 3
	note Cs, 4
	note E_, 4
	note A_, 2
	note Fs, 3
	note A_, 1
	note B_, 6
	octave 2
	note B_, 2
	octave 3
	note Ds, 2
	note Fs, 2
	note B_, 2
	note A_, 4
	note Fs, 4
	note E_, 2
	note Ds, 4
	stereo_panning FALSE, TRUE
.loop1:
	sound_call .sub1
	octave 3
	note Cs, 4
	sound_loop 2, .loop1
	sound_call .sub2
	octave 3
	note Fs, 4
	sound_call .sub2
	octave 3
	note Fs, 4
	sound_call .sub1
	octave 3
	note E_, 4
	sound_call .sub1
	octave 3
	note Gs, 4
	sound_call .sub2
	octave 4
	note Ds, 4
	sound_call .sub2
	octave 3
	note F_, 4
	stereo_panning TRUE, TRUE
	sound_call .sub2
	rest 4
	sound_call .sub2
	rest 4
	sound_loop 0, .mainloop

.sub1:
	volume_envelope 12, 2
	octave 2
	note A_, 2
	note A_, 1
	note A_, 1
	note A_, 2
	note A_, 1
	note A_, 1
	note A_, 1
	note A_, 1
	note A_, 2
	volume_envelope 11, 7
	sound_ret

.sub2:
	volume_envelope 12, 2
	octave 2
	note B_, 2
	note B_, 1
	note B_, 1
	note B_, 2
	note B_, 1
	note B_, 1
	note B_, 1
	note B_, 1
	note B_, 2
	volume_envelope 11, 7
	sound_ret

Music_Route37_Ch2:
	duty_cycle 3
	vibrato 28, 1, 2
	note_type 12, 12, 2
	octave 2
	note B_, 2
	note_type 12, 12, 1
	note B_, 1
	note B_, 1
	volume_envelope 12, 2
	note B_, 2
	volume_envelope 12, 1
	note B_, 1
	note B_, 1
	note B_, 1
	note B_, 1
	volume_envelope 12, 2
	note B_, 6
	note B_, 2
	volume_envelope 12, 1
	note B_, 1
	note B_, 1
	volume_envelope 12, 2
	note B_, 2
	volume_envelope 12, 1
	note B_, 1
	note B_, 1
	note B_, 1
	note B_, 1
	volume_envelope 12, 2
	note B_, 2
.mainloop:
	sound_call .sub1
	volume_envelope 12, 6
	octave 4
	note Cs, 4
	octave 3
	note B_, 4
	note A_, 4
	note E_, 4
	volume_envelope 11, 0
	note Fs, 8
	volume_envelope 11, 7
	note Fs, 8
	volume_envelope 12, 7
	note Ds, 12
	sound_call .sub1
	volume_envelope 12, 7
	octave 4
	note Cs, 4
	octave 3
	note B_, 4
	octave 4
	note Cs, 4
	note E_, 4
	volume_envelope 11, 0
	note Ds, 16
	volume_envelope 11, 0
	note Ds, 8
	volume_envelope 11, 7
	note Ds, 8
	volume_envelope 12, 4
	rest 4
	octave 3
	note A_, 3
	octave 4
	note E_, 1
	note A_, 4
	note Gs, 4
	volume_envelope 10, 0
	note Cs, 6
	note Ds, 6
	note E_, 4
	volume_envelope 12, 3
	note Fs, 3
	note E_, 1
	volume_envelope 11, 0
	note Ds, 6
	volume_envelope 11, 7
	note Ds, 6
	volume_envelope 12, 3
	octave 3
	note B_, 3
	note Fs, 1
	volume_envelope 11, 0
	octave 4
	note Ds, 6
	volume_envelope 11, 7
	octave 4
	note Ds, 6
	volume_envelope 12, 4
	rest 4
	octave 3
	note A_, 3
	octave 4
	note E_, 1
	note A_, 4
	note Gs, 4
	volume_envelope 11, 0
	note Cs, 6
	note Ds, 6
	note E_, 4
	volume_envelope 12, 3
	note Fs, 3
	note Ds, 1
	volume_envelope 11, 0
	note B_, 6
	volume_envelope 11, 7
	note B_, 6
	volume_envelope 12, 3
	note B_, 3
	note Fs, 1
	volume_envelope 11, 0
	octave 5
	note Ds, 6
	volume_envelope 11, 7
	octave 5
	note Ds, 6
	volume_envelope 12, 2
	octave 3
	note Fs, 2
	note Fs, 1
	note Fs, 1
	note Fs, 2
	volume_envelope 12, 1
	note Fs, 1
	note Fs, 1
	note Fs, 1
	note Fs, 1
	volume_envelope 12, 2
	note Fs, 6
	note Fs, 2
	note Fs, 1
	note Fs, 1
	note Fs, 2
	volume_envelope 12, 1
	note Fs, 1
	note Fs, 1
	note Fs, 1
	note Fs, 1
	volume_envelope 12, 2
	note Fs, 2
	sound_loop 0, .mainloop

.sub1:
	volume_envelope 12, 7
	octave 3
	note Fs, 4
	volume_envelope 11, 0
	note B_, 5
	volume_envelope 11, 7
	note B_, 5
	volume_envelope 12, 3
	note Fs, 2
	note Fs, 2
	note B_, 2
	sound_ret

Music_Route37_Ch3:
	stereo_panning TRUE, FALSE
	note_type 12, 1, 4
.loop1:
	octave 2
	note Fs, 2
	rest 6
	sound_loop 4, .loop1
.mainloop:
	sound_call .sub1
	note A_, 2
	octave 3
	note Cs, 2
	octave 2
	note Gs, 2
	note B_, 2
	note Fs, 2
	note A_, 2
	note Fs, 2
	note E_, 2
.loop2:
	note Ds, 2
	note Fs, 2
	sound_loop 4, .loop2
	sound_call .sub1
	sound_call .sub1
	note A_, 2
	octave 3
	note Cs, 2
	octave 2
	note Gs, 2
	note B_, 2
	note Fs, 2
	note A_, 2
	octave 3
	note Cs, 2
	octave 2
	note A_, 2
	sound_call .sub1
.loop3:
	octave 2
	note B_, 2
	octave 3
	note Ds, 2
	sound_loop 4, .loop3
.loop4:
	octave 2
	note A_, 2
	octave 3
	note Cs, 2
	sound_loop 7, .loop4
	note E_, 2
	note Cs, 2
	sound_call .sub2
.loop5:
	octave 2
	note B_, 2
	octave 3
	note Fs, 2
	sound_loop 4, .loop5
.loop6:
	octave 2
	note A_, 2
	octave 3
	note Cs, 2
	sound_loop 7, .loop6
	note E_, 2
	note Cs, 2
	sound_call .sub2
	sound_call .sub1
.loop7:
	note Fs, 2
	rest 6
	sound_loop 4, .loop7
	sound_loop 0, .mainloop

.sub1:
	octave 2
	note Fs, 2
	note B_, 2
	note Fs, 2
	note B_, 2
	note Fs, 2
	note B_, 2
	note Fs, 2
	note B_, 2
	sound_ret

.sub2:
	octave 2
	note B_, 2
	octave 3
	note Ds, 2
	octave 2
	note B_, 2
	octave 3
	note Ds, 2
	octave 2
	note B_, 2
	octave 3
	note Ds, 2
	octave 2
	note B_, 2
	octave 3
	note Ds, 2
	sound_ret

Music_Route37_Ch4:
	stereo_panning FALSE, TRUE
	toggle_noise 0
.mainloop:
	drum_speed 12
	sound_call .sub1
	drum_note 3, 1
	drum_note 4, 1
	drum_note 4, 1
	drum_note 3, 1
	sound_call .sub1
	drum_speed 6
	drum_note 4, 1
	drum_note 4, 1
	drum_note 4, 1
	drum_note 4, 1
	drum_note 4, 1
	drum_note 3, 1
	drum_note 3, 1
	drum_note 3, 1
	sound_loop 0, .mainloop

.sub1:
	drum_note 3, 2
	drum_note 4, 1
	drum_note 4, 1
	drum_note 3, 2
	drum_note 4, 1
	drum_note 4, 1
	drum_note 3, 2
	drum_note 4, 1
	drum_note 4, 1
	sound_ret
