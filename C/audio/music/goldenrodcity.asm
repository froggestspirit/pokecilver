Music_GoldenrodCity:
	channel_count 4
	channel 1, Music_GoldenrodCity_Ch1
	channel 2, Music_GoldenrodCity_Ch2
	channel 3, Music_GoldenrodCity_Ch3
	channel 4, Music_GoldenrodCity_Ch4

Music_GoldenrodCity_Ch1:
	stereo_panning FALSE, TRUE
	tempo 176
	volume 7, 7
	note_type 12, 9, 7
	rest 16
	rest 16
	rest 16
	rest 16
.mainloop:
	duty_cycle 0
	sound_call .sub1
	octave 3
	note Gs, 1
	octave 4
	note C_, 1
	note Ds, 1
	note Fs, 1
	note Ds, 1
	note Fs, 1
	note Gs, 1
	octave 5
	note C_, 1
	volume_envelope 7, 1
	duty_cycle 0
	sound_call .sub2
	volume_envelope 7, 7
	note Ds, 2
	note F_, 2
	note Fs, 2
	note Gs, 4
	volume_envelope 7, 1
	sound_call .sub2
	note Ds, 1
	note Fs, 1
	volume_envelope 7, 7
	octave 5
	note C_, 2
	octave 4
	note Gs, 2
	note Fs, 2
	note Ds, 2
	duty_cycle 2
	volume_envelope 9, 7
	sound_call .sub1
	note C_, 4
	octave 3
	note Gs, 2
	octave 4
	note C_, 2
	sound_loop 0, .mainloop

.sub1:
	octave 4
	note Cs, 2
	octave 3
	note Gs, 1
	rest 3
	note Gs, 1
	rest 1
	octave 4
	note C_, 2
	octave 3
	note Gs, 1
	rest 3
	note Gs, 1
	rest 1
	note As, 2
	note F_, 1
	rest 1
	note Cs, 2
	note F_, 1
	rest 1
	note Ds, 2
	note C_, 2
	note Cs, 2
	note Ds, 2
	note Fs, 2
	note Cs, 1
	rest 3
	note Cs, 1
	rest 1
	note Fs, 2
	note Cs, 1
	rest 1
	note Fs, 2
	note As, 2
	note Cs, 2
	note Ds, 2
	note Gs, 2
	octave 4
	note Cs, 2
	sound_ret

.sub2:
	octave 4
	note F_, 1
	note Cs, 1
	octave 3
	note Gs, 1
	octave 4
	note Cs, 1
	note F_, 1
	note Gs, 1
	note F_, 1
	note Gs, 1
	note Cs, 1
	octave 3
	note Gs, 1
	note F_, 1
	note Gs, 1
	octave 4
	note Cs, 1
	note F_, 1
	note Cs, 1
	note F_, 1
	note Ds, 1
	note C_, 1
	octave 3
	note Gs, 1
	octave 4
	note C_, 1
	note Ds, 1
	note Fs, 1
	note Ds, 1
	note Fs, 1
	note C_, 1
	octave 3
	note Gs, 1
	note Fs, 1
	note Gs, 1
	octave 4
	note C_, 1
	note Ds, 1
	note C_, 1
	note Ds, 1
	note Cs, 1
	octave 3
	note As, 1
	note Fs, 1
	note As, 1
	octave 4
	note Cs, 1
	note F_, 1
	note Cs, 1
	note F_, 1
	octave 3
	note As, 1
	note Fs, 1
	note Cs, 1
	note Fs, 1
	note As, 1
	octave 4
	note Cs, 1
	octave 3
	note As, 1
	octave 4
	note Cs, 1
	note Ds, 1
	note C_, 1
	octave 3
	note Gs, 1
	octave 4
	note C_, 1
	note Ds, 1
	note Fs, 1
	sound_ret

Music_GoldenrodCity_Ch2:
	stereo_panning TRUE, FALSE
	vibrato 18, 2, 3
	duty_cycle 2
	note_type 12, 9, 7
.mainloop:
	volume_envelope 9, 7
	sound_call .sub1
	octave 4
	note F_, 2
	note Fs, 2
	note Gs, 2
	note F_, 2
	note Ds, 8
	sound_call .sub1
	note Gs, 2
	note As, 2
	octave 5
	note C_, 2
	note Cs, 2
	note Ds, 8
	duty_cycle 2
	volume_envelope 7, 7
	sound_call .sub2
	octave 4
	note Gs, 2
	note As, 2
	note B_, 2
	octave 5
	note C_, 4
	duty_cycle 3
	volume_envelope 7, 7
	sound_call .sub2
	note F_, 2
	note Ds, 2
	note Cs, 2
	note C_, 2
	octave 4
	note Gs, 2
	sound_loop 0, .mainloop

.sub1:
	octave 4
	note F_, 4
	note Cs, 4
	note Ds, 4
	note C_, 4
	note Cs, 2
	note C_, 2
	octave 3
	note As, 2
	octave 4
	note C_, 2
	octave 3
	note Gs, 6
	octave 3
	note Gs, 1
	octave 3
	note A_, 1
	note As, 4
	octave 4
	note Cs, 4
	octave 3
	note As, 2
	octave 4
	note As, 2
	note Gs, 2
	note Fs, 2
	duty_cycle 1
	sound_ret

.sub2:
	note Cs, 6
	note F_, 6
	note Cs, 4
	note C_, 6
	note Ds, 6
	note C_, 2
	octave 4
	note Gs, 1
	note A_, 1
	note As, 6
	octave 5
	note Cs, 6
	octave 4
	note As, 4
	octave 5
	note C_, 4
	note Ds, 2
	sound_ret

Music_GoldenrodCity_Ch3:
	stereo_panning TRUE, TRUE
	vibrato 8, 2, 3
	note_type 12, 2, 5
.mainloop:
	sound_call .sub1
	sound_call .sub1
	sound_call .sub2
	rest 3
	octave 3
	note C_, 1
	note Ds, 1
	note G_, 1
	note Gs, 1
	note Ds, 2
	sound_call .sub2
	rest 1
	octave 3
	note Gs, 2
	note Fs, 2
	note Ds, 2
	octave 2
	note Gs, 2
	sound_loop 0, .mainloop

.sub1:
	octave 3
	note Cs, 4
	rest 2
	note Cs, 1
	rest 1
	note C_, 4
	rest 2
	note C_, 1
	rest 1
	octave 2
	note As, 4
	rest 2
	note As, 1
	rest 1
	note Gs, 4
	rest 2
	note Gs, 1
	rest 1
	note Fs, 4
	rest 2
	note Fs, 1
	rest 1
	octave 3
	note Ds, 4
	rest 2
	note Ds, 1
	rest 1
	note Gs, 4
	rest 2
	note Gs, 1
	rest 1
	octave 2
	note Gs, 1
	rest 1
	note Gs, 3
	rest 1
	octave 3
	note Gs, 1
	rest 1
	sound_ret

.sub2:
	note Cs, 1
	rest 1
	octave 4
	note Cs, 1
	octave 3
	note Cs, 1
	rest 2
	note Cs, 1
	rest 5
	octave 4
	note C_, 1
	note Cs, 1
	octave 3
	note Gs, 2
	note C_, 1
	rest 1
	octave 4
	note C_, 1
	octave 3
	note C_, 1
	rest 2
	note C_, 1
	rest 5
	note B_, 1
	octave 4
	note C_, 1
	octave 3
	note Gs, 2
	octave 2
	note As, 1
	rest 1
	octave 3
	note As, 1
	octave 2
	note As, 1
	rest 2
	note As, 1
	rest 5
	octave 3
	note A_, 1
	note As, 1
	note Fs, 2
	octave 2
	note Gs, 1
	rest 1
	octave 3
	note Gs, 1
	octave 2
	note Gs, 1
	rest 2
	note Gs, 1
	sound_ret

Music_GoldenrodCity_Ch4:
	toggle_noise 3
	drum_speed 12
	rest 16
	rest 16
	rest 16
	rest 8
	drum_note 3, 2
	drum_note 7, 2
	drum_note 3, 1
	drum_note 3, 1
	drum_note 7, 2
.mainloop:
.loop1:
	drum_note 4, 2
	drum_note 7, 2
	drum_note 3, 2
	drum_note 7, 2
	drum_note 4, 2
	drum_note 7, 2
	drum_note 3, 2
	drum_note 7, 2
	sound_loop 3, .loop1
	sound_call .sub1
	sound_call .sub2
	drum_note 8, 1
	drum_note 8, 1
	drum_note 7, 2
	sound_call .sub2
	drum_note 3, 2
	drum_note 7, 2
	sound_call .sub2
	drum_note 3, 1
	drum_note 3, 1
	drum_note 7, 2
	sound_call .sub2
	drum_note 3, 2
	drum_note 3, 1
	drum_note 3, 1
	sound_call .sub2
	drum_note 8, 1
	drum_note 8, 1
	drum_note 7, 2
	sound_call .sub2
	drum_note 3, 2
	drum_note 7, 2
	sound_call .sub2
	drum_note 3, 1
	drum_note 3, 1
	drum_note 7, 2
	sound_call .sub2
	drum_note 3, 2
	drum_note 3, 1
	drum_note 3, 1
.loop2:
	drum_note 4, 2
	drum_note 7, 2
	drum_note 3, 2
	drum_note 7, 2
	drum_note 4, 2
	drum_note 7, 2
	drum_note 3, 2
	drum_note 7, 2
	sound_loop 3, .loop2
	sound_call .sub1
	sound_loop 0, .mainloop

.sub1:
	drum_note 4, 2
	drum_note 7, 2
	drum_note 3, 2
	drum_note 7, 2
	drum_note 4, 2
	drum_note 7, 2
	drum_note 3, 1
	drum_note 3, 1
	drum_note 7, 2
	sound_ret

.sub2:
	drum_note 4, 2
	drum_note 3, 1
	drum_note 4, 3
	drum_note 4, 2
	drum_note 8, 1
	drum_note 8, 1
	drum_note 8, 1
	drum_note 8, 1
	sound_ret
