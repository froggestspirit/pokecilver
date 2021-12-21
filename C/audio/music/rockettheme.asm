Music_RocketTheme:
	channel_count 4
	channel 1, Music_RocketTheme_Ch1
	channel 2, Music_RocketTheme_Ch2
	channel 3, Music_RocketTheme_Ch3
	channel 4, Music_RocketTheme_Ch4

Music_RocketTheme_Ch1:
	tempo 128
	volume 7, 7
	duty_cycle 3
	stereo_panning FALSE, TRUE
	vibrato 5, 6, 4
	note_type 12, 10, 8
	octave 4
	note C_, 1
	note D_, 1
.mainloop:
	duty_cycle 3
	note_type 12, 10, 8
	sound_call Music_LookRocket_Ch1.sub1
	duty_cycle 1
	volume_envelope 10, 8
	sound_call Music_LookRocket_Ch1.sub2
	sound_call Music_LookRocket_Ch2.sub5
	octave 4
	note Fs, 1
	rest 1
	volume_envelope 5, 3
	sound_call Music_LookRocket_Ch1.sub3
	volume_envelope 12, 3
	sound_call Music_LookRocket_Ch1.sub3
	volume_envelope 14, 3
	sound_call Music_LookRocket_Ch1.sub3
	rest 16
	duty_cycle 0
	note_type 6, 11, 3
	vibrato 6, 6, 4
	sound_call .sub1
	octave 4
	note F_, 1
	note Fs, 3
	note F_, 2
	rest 2
	note Ds, 2
	rest 2
	note C_, 2
	note D_, 2
	note Ds, 2
	note C_, 2
	octave 3
	note As, 2
	note G_, 2
	note As, 2
	octave 4
	note C_, 2
	rest 4
	note D_, 4
	rest 4
	note Ds, 4
	rest 4
	note Ds, 2
	note Fs, 2
	sound_call .sub1
	octave 3
	note Fs, 1
	note G_, 3
	note F_, 2
	rest 2
	note Ds, 2
	rest 2
	note C_, 2
	note D_, 2
	note Ds, 2
	note C_, 2
	octave 2
	note As, 2
	note G_, 2
	note As, 2
	octave 3
	note C_, 2
	rest 4
	note D_, 4
	rest 4
	note Ds, 4
	note F_, 2
	note E_, 2
	note Ds, 2
	note D_, 2
.loop1:
	rest 4
	note F_, 2
	rest 14
	note Fs, 2
	rest 10
	sound_loop 3, .loop1
	rest 4
	note F_, 2
	rest 14
	note Fs, 2
	rest 2
	note Fs, 2
	rest 2
	note Ds, 2
	note C_, 2
.loop2:
	note Fs, 2
	note A_, 2
	sound_loop 4, .loop2
.loop3:
	note G_, 2
	note As, 2
	sound_loop 4, .loop3
.loop4:
	note Gs, 2
	note B_, 2
	sound_loop 4, .loop4
.loop5:
	octave 3
	note A_, 2
	octave 4
	note C_, 2
	sound_loop 4, .loop5
	note_type 12, 11, 3
	rest 16
	rest 12
	note F_, 2
	note C_, 1
	note D_, 1
	sound_loop 0, .mainloop

.sub1:
	rest 7
	note B_, 1
	octave 4
	note C_, 2
	rest 6
	note C_, 2
	rest 2
	octave 3
	note G_, 2
	rest 6
	octave 4
	note Ds, 12
	note D_, 8
	octave 3
	note As, 2
	rest 6
	note G_, 2
	rest 2
	note A_, 2
	rest 2
	note As, 2
	rest 10
	sound_ret

Music_RocketTheme_Ch2:
	stereo_panning TRUE, TRUE
	vibrato 4, 6, 4
	duty_cycle 3
	note_type 12, 11, 7
	octave 5
	note C_, 1
	note D_, 1
.mainloop:
	note_type 12, 11, 7
	sound_call Music_LookRocket_Ch2.sub1
	volume_envelope 11, 7
	sound_call Music_LookRocket_Ch2.sub5
	octave 5
	note C_, 1
	rest 1
	sound_call Music_LookRocket_Ch2.sub2
	volume_envelope 5, 3
	sound_call Music_LookRocket_Ch2.sub3
	volume_envelope 12, 3
	sound_call Music_LookRocket_Ch2.sub3
	volume_envelope 14, 3
	sound_call Music_LookRocket_Ch2.sub3
	sound_call Music_LookRocket_Ch2.sub4
	vibrato 6, 6, 4
	sound_call .sub1
	octave 4
	note A_, 1
	vibrato 0, 0, 0
	volume_envelope 11, 8
	note As, 14
	vibrato 5, 2, 2
	volume_envelope 11, 7
	note As, 5
	vibrato 6, 6, 4
	note_type 12, 11, 7
	note As, 12
	rest 2
	octave 5
	note C_, 1
	note D_, 1
	sound_call .sub1
	note B_, 1
	vibrato 0, 0, 0
	volume_envelope 11, 8
	octave 4
	note C_, 14
	vibrato 5, 2, 2
	volume_envelope 11, 7
	note C_, 5
	vibrato 6, 6, 4
	note_type 12, 11, 7
	note C_, 12
	octave 3
	note As, 1
	note A_, 1
	note Gs, 1
	note G_, 1
	note_type 6, 11, 7
	sound_call .sub2
	octave 4
	note D_, 1
	note Ds, 9
	note D_, 1
	note Cs, 1
	note C_, 12
	note F_, 2
	rest 2
	note Fs, 2
	rest 2
	sound_call .sub2
	octave 3
	note B_, 2
	rest 2
	octave 4
	note C_, 2
	rest 2
	note D_, 2
	rest 2
	note Ds, 8
	rest 4
	note D_, 2
	rest 2
	note C_, 2
	octave 3
	note G_, 2
	note B_, 1
	octave 4
	note C_, 15
	note Cs, 16
	note D_, 16
	note Ds, 16
	note E_, 2
	note Cs, 2
	octave 3
	note As, 2
	note G_, 2
	octave 4
	note F_, 2
	note D_, 2
	octave 3
	note B_, 2
	note Gs, 2
	octave 4
	note Fs, 2
	note Ds, 2
	note C_, 2
	octave 3
	note A_, 2
	octave 4
	note G_, 2
	note E_, 2
	note Cs, 2
	octave 3
	note As, 2
	octave 4
	note Gs, 2
	note F_, 2
	note D_, 2
	octave 3
	note B_, 2
	octave 4
	note A_, 2
	note Fs, 2
	note Ds, 2
	note C_, 2
	note As, 2
	note G_, 2
	note E_, 2
	note Cs, 2
	note B_, 4
	octave 5
	note C_, 2
	note D_, 2
	sound_loop 0, .mainloop

.sub1:
	note_type 6, 11, 7
	rest 7
	octave 4
	note D_, 1
	note Ds, 2
	rest 6
	note Ds, 2
	rest 2
	note C_, 2
	rest 6
	note Fs, 12
	note F_, 8
	note Ds, 2
	rest 6
	note C_, 2
	rest 2
	note D_, 2
	rest 2
	note Ds, 2
	rest 2
	octave 3
	note Ds, 2
	rest 2
	note Ds, 2
	rest 2
	sound_ret

.sub2:
	octave 3
	note B_, 1
	octave 4
	note C_, 3
	note Ds, 4
	note C_, 4
	note Fs, 8
	note F_, 8
	note C_, 4
	sound_ret

Music_RocketTheme_Ch3:
	stereo_panning TRUE, FALSE
	vibrato 4, 2, 2
	note_type 12, 1, 4
	rest 2
.mainloop:
	note_type 12, 1, 4
	sound_call Music_LookRocket_Ch3.sub1
	sound_call Music_LookRocket_Ch3.sub4
	sound_call Music_LookRocket_Ch3.sub4
	volume_envelope 2, 4
	sound_call Music_LookRocket_Ch3.sub5
	volume_envelope 1, 4
	sound_call Music_LookRocket_Ch3.sub2
	sound_call Music_LookRocket_Ch3.sub5
	sound_call Music_LookRocket_Ch3.sub3
	sound_call Music_LookRocket_Ch3.sub4
	sound_call Music_LookRocket_Ch3.sub4
	sound_call Music_LookRocket_Ch3.sub4
	sound_call Music_LookRocket_Ch3.sub4
	sound_call Music_LookRocket_Ch3.sub5
	sound_call Music_LookRocket_Ch3.sub2
	sound_call Music_LookRocket_Ch3.sub5
	sound_call Music_LookRocket_Ch3.sub2
	octave 3
	note C_, 1
	rest 1
	octave 4
	note C_, 1
	rest 1
	octave 3
	note C_, 1
	rest 1
	note C_, 1
	rest 1
	note Cs, 1
	rest 1
	octave 4
	note Cs, 1
	rest 1
	octave 3
	note As, 1
	note G_, 1
	note E_, 1
	note Cs, 1
	note D_, 1
	rest 1
	octave 4
	note D_, 1
	rest 1
	octave 3
	note D_, 1
	rest 1
	note D_, 1
	rest 1
	note Ds, 1
	rest 1
	octave 4
	note Ds, 1
	rest 1
	note C_, 1
	octave 3
	note A_, 1
	note Fs, 1
	note Ds, 1
	note E_, 1
	rest 1
	octave 4
	note Cs, 1
	rest 1
	octave 3
	note F_, 1
	rest 1
	octave 4
	note D_, 1
	rest 1
	octave 3
	note Fs, 1
	rest 1
	octave 4
	note Ds, 1
	rest 1
	octave 3
	note G_, 1
	rest 1
	octave 4
	note E_, 1
	rest 1
	octave 3
	note Gs, 1
	rest 1
	octave 4
	note F_, 1
	rest 1
	octave 3
	note A_, 1
	rest 1
	octave 4
	note Fs, 1
	rest 1
	octave 3
	note As, 1
	rest 1
	octave 4
	note G_, 1
	rest 1
	octave 3
	note B_, 2
	octave 4
	note Gs, 1
	rest 1
	sound_loop 0, .mainloop

Music_RocketTheme_Ch4:
	toggle_noise 3
	drum_speed 12
	rest 2
.mainloop:
	sound_call Music_LookRocket_Ch4.sub1
	sound_call Music_LookRocket_Ch4.sub2
	sound_call Music_LookRocket_Ch4.sub2
	sound_call Music_LookRocket_Ch4.sub2
	sound_call Music_LookRocket_Ch4.sub2
	sound_call Music_LookRocket_Ch4.sub2
	sound_call Music_LookRocket_Ch4.sub2
	sound_call Music_LookRocket_Ch4.sub2
	sound_call Music_LookRocket_Ch4.sub1
.loop1:
	drum_note 11, 2
	drum_note 7, 2
	drum_note 3, 2
	drum_note 7, 2
	drum_note 11, 2
	drum_note 7, 2
	drum_note 3, 2
	drum_note 7, 2
	sound_loop 11, .loop1
	sound_call Music_LookRocket_Ch4.sub2
	sound_call Music_LookRocket_Ch4.sub2
	sound_call Music_LookRocket_Ch4.sub2
	sound_call Music_LookRocket_Ch4.sub1
	sound_call Music_LookRocket_Ch4.sub1
	sound_loop 0, .mainloop
