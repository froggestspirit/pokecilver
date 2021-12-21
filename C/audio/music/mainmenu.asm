Music_MainMenu:
	channel_count 4
	channel 1, Music_MainMenu_Ch1
	channel 2, Music_MainMenu_Ch2
	channel 3, Music_MainMenu_Ch3
	channel 4, Music_MainMenu_Ch4

Music_MainMenu_Ch1:
	tempo 160
	volume 7, 7
	duty_cycle 3
	pitch_offset 1
	vibrato 16, 1, 5
	stereo_panning TRUE, FALSE
	note_type 12, 0, -7
	octave 3
	note D_, 8
.mainloop:
.loop1:
	note_type 12, 9, 2
	octave 3
	note Cs, 6
	note Cs, 1
	note Cs, 1
	note Cs, 2
	note Cs, 2
	note Cs, 4
	sound_loop 2, .loop1
	note Ds, 6
	note Ds, 1
	note Ds, 1
	note Ds, 2
	note Ds, 2
	note Ds, 4
	octave 2
	note B_, 6
	note B_, 1
	note B_, 1
	note B_, 2
	note B_, 2
	note_type 12, 9, 4
	octave 3
	note C_, 4
	sound_loop 0, .mainloop

Music_MainMenu_Ch2:
	duty_cycle 3
	stereo_panning FALSE, TRUE
	note_type 12, 0, -7
	octave 3
	note G_, 8
.mainloop:
.loop1:
	note_type 12, 10, 2
	note Fs, 6
	note Fs, 1
	note Fs, 1
	note Fs, 2
	note Fs, 2
	note Fs, 4
	sound_loop 2, .loop1
	note Gs, 6
	note Gs, 1
	note Gs, 1
	note Gs, 2
	note Gs, 2
	note Gs, 4
	note E_, 6
	note E_, 1
	note E_, 1
	note E_, 2
	note E_, 2
	note_type 12, 10, 4
	note F_, 4
	sound_loop 0, .mainloop

Music_MainMenu_Ch3:
	stereo_panning TRUE, FALSE
	note_type 12, 2, 5
	rest 8
.mainloop:
.loop1:
	octave 2
	note Fs, 1
	rest 5
	note Fs, 1
	note Fs, 1
	note Fs, 1
	rest 1
	note Fs, 1
	rest 1
	note Fs, 1
	rest 3
	sound_loop 2, .loop1
	note Gs, 1
	rest 5
	note Gs, 1
	note Gs, 1
	note Gs, 1
	rest 1
	note Gs, 1
	rest 1
	note Gs, 1
	rest 3
	note E_, 1
	rest 5
	note E_, 1
	note E_, 1
	note E_, 1
	rest 1
	note E_, 1
	rest 1
	note F_, 1
	rest 3
	sound_loop 0, .mainloop

Music_MainMenu_Ch4:
	stereo_panning FALSE, TRUE
	toggle_noise 3
	drum_speed 12
	rest 8
	rest 16
	rest 16
	rest 16
	rest 12
	drum_note 3, 1
	drum_note 3, 1
	drum_note 3, 1
	drum_note 3, 1
.mainloop:
	drum_note 3, 6
	drum_note 3, 1
	drum_note 3, 1
	drum_note 3, 2
	drum_note 3, 2
	drum_note 3, 4
	drum_note 3, 6
	drum_note 3, 1
	drum_note 3, 1
	drum_note 3, 2
	drum_note 3, 2
	drum_note 4, 1
	drum_note 4, 1
	drum_note 3, 1
	drum_note 3, 1
	sound_loop 0, .mainloop
