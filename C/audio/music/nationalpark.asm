Music_NationalPark:
	channel_count 4
	channel 1, Music_NationalPark_Ch1
	channel 2, Music_NationalPark_Ch2
	channel 3, Music_NationalPark_Ch3
	channel 4, Music_NationalPark_Ch4

Music_NationalPark_Ch1:
	tempo 192
	volume 7, 7
	stereo_panning FALSE, TRUE
	vibrato 24, 2, 4
	duty_cycle 1
	note_type 12, 7, 7
	rest 2
.mainloop:
	sound_call .sub1
	sound_call .sub1
	note_type 6, 6, 7
	sound_call .sub2
	note_type 12, 7, 7
	rest 16
	octave 2
	note As, 1
	octave 3
	note D_, 1
	note F_, 1
	note Gs, 1
	note D_, 1
	note F_, 1
	note As, 1
	note_type 6, 6, 7
	octave 4
	note D_, 1
	note Cs, 1
	note D_, 1
	rest 1
	note D_, 1
	rest 1
	note Cs, 1
	rest 1
	note D_, 1
	rest 3
	note D_, 1
	rest 3
	note D_, 1
	rest 1
	sound_call .sub2
	volume_envelope 7, 7
	octave 3
	note E_, 1
	note F_, 5
	note Ds, 4
	rest 2
	note C_, 12
	note F_, 8
	note Cs, 1
	volume_envelope 9, 8
	note D_, 16
	volume_envelope 7, 7
	note D_, 7
	note_type 12, 7, 7
	rest 4
	sound_loop 0, .mainloop

.sub1:
	octave 2
	note Gs, 6
	octave 3
	note F_, 2
	volume_envelope 4, 7
	note F_, 1
	note Gs, 1
	volume_envelope 5, 7
	note F_, 1
	note Gs, 1
	volume_envelope 7, 7
	note F_, 1
	note Gs, 1
	volume_envelope 9, 7
	note F_, 1
	note Gs, 1
	volume_envelope 7, 7
	octave 2
	note A_, 6
	octave 3
	note Fs, 2
	volume_envelope 4, 7
	note Fs, 1
	note A_, 1
	volume_envelope 5, 7
	note Fs, 1
	note A_, 1
	volume_envelope 7, 7
	note Fs, 1
	note A_, 1
	volume_envelope 9, 7
	note Fs, 1
	note A_, 1
	volume_envelope 7, 7
	octave 2
	note Gs, 6
	octave 3
	note F_, 2
	volume_envelope 4, 7
	octave 4
	note C_, 1
	note Cs, 1
	volume_envelope 5, 7
	note C_, 1
	note Cs, 1
	volume_envelope 7, 7
	note C_, 1
	note Cs, 1
	volume_envelope 9, 7
	note C_, 1
	note Cs, 1
	volume_envelope 7, 7
	octave 2
	note A_, 6
	octave 3
	note Cs, 2
	volume_envelope 4, 7
	octave 4
	note Cs, 1
	note Ds, 1
	volume_envelope 5, 7
	note Cs, 1
	note Ds, 1
	volume_envelope 7, 7
	note Cs, 1
	note Ds, 1
	volume_envelope 9, 7
	note Cs, 1
	note Ds, 1
	volume_envelope 7, 7
	note Cs, 2
	octave 3
	note As, 2
	octave 4
	note Cs, 2
	octave 3
	note As, 2
	octave 4
	note Cs, 2
	octave 3
	note As, 2
	octave 4
	note Cs, 2
	octave 3
	note As, 2
	octave 4
	note Cs, 2
	octave 3
	note A_, 2
	octave 4
	note Cs, 2
	octave 3
	note A_, 2
	volume_envelope 4, 7
	octave 4
	note Cs, 1
	note Ds, 1
	volume_envelope 5, 7
	note Cs, 1
	note Ds, 1
	volume_envelope 7, 7
	note Cs, 1
	note Ds, 1
	volume_envelope 9, 7
	note Cs, 1
	note Ds, 1
	volume_envelope 7, 7
	note C_, 2
	octave 3
	note Gs, 2
	octave 4
	note C_, 2
	octave 3
	note Gs, 2
	octave 4
	note Ds, 2
	note C_, 2
	note Ds, 2
	note C_, 2
	note D_, 2
	octave 3
	note As, 2
	octave 4
	note D_, 2
	octave 3
	note As, 2
	volume_envelope 5, 7
	octave 4
	note F_, 1
	note Ds, 1
	volume_envelope 6, 7
	note D_, 1
	note Ds, 1
	volume_envelope 7, 7
	note F_, 1
	note Fs, 1
	volume_envelope 9, 7
	note Gs, 1
	note As, 1
	volume_envelope 7, 7
	note Cs, 2
	octave 3
	note As, 2
	octave 4
	note Cs, 2
	octave 3
	note As, 2
	octave 4
	note Cs, 2
	octave 3
	note As, 2
	octave 4
	note Cs, 2
	octave 3
	note As, 2
	octave 4
	note Cs, 2
	octave 3
	note A_, 2
	octave 4
	note Cs, 2
	octave 3
	note A_, 2
	volume_envelope 4, 7
	octave 4
	note Cs, 1
	note Ds, 1
	volume_envelope 5, 7
	note Cs, 1
	note Ds, 1
	volume_envelope 7, 7
	note Cs, 1
	note Ds, 1
	volume_envelope 9, 7
	note Cs, 1
	note Ds, 1
	volume_envelope 7, 7
	note C_, 2
	octave 3
	note Gs, 2
	octave 4
	note C_, 2
	octave 3
	note Gs, 2
	octave 4
	note Ds, 2
	note C_, 2
	note Ds, 2
	note C_, 2
	note D_, 2
	octave 3
	note As, 2
	octave 4
	note D_, 2
	octave 3
	note As, 2
	octave 4
	note D_, 2
	octave 3
	note As, 2
	octave 4
	note D_, 2
	octave 3
	note As, 2
	sound_ret

.sub2:
	rest 16
	rest 3
	octave 4
	note As, 1
	note As, 1
	rest 1
	note As, 1
	rest 1
	note As, 1
	rest 1
	note As, 2
	rest 4
	rest 16
	rest 3
	note A_, 1
	note A_, 1
	rest 1
	note A_, 1
	rest 1
	note A_, 1
	rest 1
	note A_, 2
	rest 4
	sound_ret

Music_NationalPark_Ch2:
	vibrato 20, 2, 4
	duty_cycle 1
	note_type 12, 10, 7
	rest 2
	stereo_panning TRUE, FALSE
.mainloop:
	note_type 12, 10, 7
	sound_call .sub1
	octave 3
	note D_, 8
	sound_call .sub1
	rest 2
	volume_envelope 10, 7
	octave 4
	note Gs, 2
	note Fs, 2
	note F_, 2
	note_type 6, 10, 7
	sound_call .sub2
	octave 3
	note B_, 1
	octave 4
	note C_, 5
	octave 3
	note Gs, 4
	rest 2
	octave 4
	note As, 8
	note Gs, 2
	rest 2
	note Fs, 2
	rest 2
	note Gs, 4
	note E_, 1
	note F_, 14
	volume_envelope 8, 7
	note G_, 1
	note Gs, 1
	rest 1
	note Gs, 1
	rest 1
	note G_, 1
	rest 1
	note Gs, 1
	rest 3
	note Gs, 1
	rest 3
	note Gs, 1
	rest 1
	volume_envelope 10, 7
	sound_call .sub2
	octave 3
	note B_, 1
	octave 4
	note C_, 5
	octave 3
	note Gs, 4
	rest 2
	note As, 12
	octave 4
	note C_, 8
	octave 3
	note A_, 1
	vibrato 0, 0, 0
	volume_envelope 11, 8
	note As, 16
	vibrato 20, 2, 4
	volume_envelope 11, 7
	note As, 11
	octave 2
	note Ds, 2
	note D_, 2
	sound_loop 0, .mainloop

.sub1:
	octave 2
	note Cs, 6
	note Gs, 2
	octave 3
	note Cs, 8
	octave 2
	note Cs, 6
	note A_, 2
	octave 3
	note Cs, 8
	octave 2
	note Cs, 6
	note Gs, 2
	octave 3
	note Cs, 8
	octave 2
	note Cs, 6
	note A_, 2
	octave 3
	note Fs, 8
	octave 1
	note Fs, 6
	octave 2
	note Cs, 2
	note As, 8
	octave 1
	note Fs, 6
	octave 2
	note Cs, 2
	note A_, 8
	octave 1
	note F_, 6
	octave 2
	note C_, 2
	note Gs, 8
	octave 1
	note As, 6
	octave 2
	note F_, 2
	octave 3
	note D_, 8
	octave 1
	note Fs, 6
	octave 2
	note Cs, 2
	note As, 8
	octave 1
	note Fs, 6
	octave 2
	note Cs, 2
	note A_, 8
	octave 1
	note F_, 6
	octave 2
	note C_, 2
	note Gs, 8
	octave 1
	note As, 6
	octave 2
	note F_, 2
	sound_ret

.sub2:
	octave 4
	note E_, 1
	note F_, 5
	note Fs, 4
	rest 2
	note Cs, 7
	volume_envelope 8, 7
	octave 5
	note E_, 1
	note F_, 1
	rest 1
	note F_, 1
	rest 1
	note E_, 1
	rest 1
	note F_, 2
	volume_envelope 10, 7
	octave 3
	note As, 2
	octave 4
	note Cs, 2
	octave 4
	note E_, 1
	note F_, 5
	note Fs, 4
	rest 2
	note Ds, 7
	volume_envelope 8, 7
	octave 5
	note D_, 1
	note Ds, 1
	rest 1
	note Ds, 1
	rest 1
	note D_, 1
	rest 1
	note Ds, 2
	volume_envelope 10, 7
	octave 4
	note D_, 2
	note Cs, 2
	sound_ret

Music_NationalPark_Ch3:
	vibrato 22, 2, 4
	note_type 12, 1, 1
	octave 4
	note Gs, 1
	note As, 1
	stereo_panning TRUE, TRUE
.mainloop:
	sound_call .sub1
	octave 5
	note As, 14
	octave 4
	note Gs, 1
	note As, 1
	sound_call .sub1
	octave 5
	note As, 8
	octave 4
	note D_, 8
	volume_envelope 1, 5
	sound_call .sub2
	octave 4
	note D_, 1
	octave 3
	note F_, 3
	sound_call .sub2
	octave 4
	note D_, 1
	octave 3
	note F_, 1
	volume_envelope 1, 1
	octave 4
	note Gs, 1
	note As, 1
	sound_loop 0, .mainloop

.sub1:
	octave 5
	note C_, 16
	note Cs, 14
	note Gs, 1
	note As, 1
	octave 6
	note C_, 16
	note Cs, 10
	note C_, 2
	note Cs, 2
	note Ds, 2
	note F_, 2
	note Ds, 2
	note Cs, 2
	octave 5
	note As, 10
	octave 6
	note F_, 2
	note Ds, 2
	note Cs, 2
	octave 5
	note A_, 9
	octave 6
	note D_, 1
	octave 6
	note Ds, 2
	note Cs, 2
	note C_, 2
	octave 5
	note Gs, 4
	octave 6
	note Gs, 4
	note Ds, 2
	note F_, 14
	note D_, 1
	note Ds, 1
	note F_, 2
	note Ds, 2
	note Cs, 2
	octave 5
	note As, 10
	octave 6
	note F_, 2
	note Ds, 2
	note Cs, 2
	octave 5
	note A_, 10
	octave 6
	note Ds, 2
	note Cs, 2
	note C_, 2
	octave 5
	note Gs, 4
	note As, 4
	octave 6
	note C_, 2
	sound_ret

.sub2:
	octave 2
	note Fs, 3
	octave 3
	note Cs, 2
	rest 1
	note As, 4
	note Cs, 1
	rest 1
	note As, 1
	note Cs, 3
	octave 2
	note Fs, 3
	octave 3
	note Cs, 2
	rest 1
	note A_, 4
	note Cs, 1
	rest 1
	note A_, 1
	note Cs, 3
	octave 2
	note F_, 3
	octave 3
	note C_, 2
	rest 1
	note Gs, 4
	note C_, 1
	rest 1
	note Gs, 1
	note C_, 3
	octave 2
	note As, 3
	octave 3
	note F_, 2
	rest 1
	octave 4
	note D_, 4
	octave 3
	note F_, 1
	rest 1
	sound_ret

Music_NationalPark_Ch4:
	toggle_noise 3
	drum_speed 12
	rest 2
.mainloop:
.loop1:
	rest 16
	sound_loop 23, .loop1
	rest 8
	drum_note 11, 2
	drum_note 3, 2
	drum_note 11, 1
	drum_note 3, 1
	drum_note 12, 2
.loop2:
	drum_note 11, 2
	drum_note 8, 1
	drum_note 8, 1
	drum_note 3, 2
	drum_note 8, 1
	drum_note 8, 1
	drum_note 11, 2
	drum_note 8, 1
	drum_note 8, 1
	drum_note 3, 2
	drum_note 12, 2
	sound_loop 7, .loop2
	drum_note 11, 2
	drum_note 8, 1
	drum_note 8, 1
	drum_note 3, 2
	drum_note 8, 1
	drum_note 8, 1
	drum_note 11, 2
	drum_note 4, 1
	drum_note 4, 1
	drum_note 3, 1
	drum_note 3, 1
	drum_note 11, 1
	drum_note 4, 1
	sound_loop 0, .mainloop
