Music_BugCatchingContest:
	channel_count 4
	channel 1, Music_BugCatchingContest_Ch1
	channel 2, Music_BugCatchingContest_Ch2
	channel 3, Music_BugCatchingContest_Ch3
	channel 4, Music_BugCatchingContest_Ch4

Music_BugCatchingContest_Ch1:
	tempo 144
	volume 7, 7
	stereo_panning FALSE, TRUE
	vibrato 8, 2, 4
	duty_cycle 2
	note_type 12, 11, 7
	rest 8
	octave 4
	note Cs, 1
	rest 1
	note Cs, 3
	rest 1
	note Cs, 1
	note Cs, 1
	note D_, 1
	rest 1
	note D_, 3
	rest 1
	note D_, 1
	note D_, 1
	note Ds, 1
	rest 1
	note Ds, 3
	rest 1
	note Ds, 1
	note Ds, 1
	note E_, 1
	rest 1
	note E_, 3
	rest 1
	note E_, 1
	note E_, 1
.mainloop:
	note_type 12, 11, 1
.loop1:
	rest 2
	octave 2
	note A_, 2
	sound_loop 4, .loop1
.loop2:
	rest 2
	note B_, 2
	sound_loop 8, .loop2
	volume_envelope 11, 4
	duty_cycle 1
	octave 3
	note B_, 6
	note A_, 1
	note B_, 1
	note A_, 6
	octave 4
	note D_, 1
	note E_, 1
	volume_envelope 11, 1
	duty_cycle 2
.loop3:
	rest 2
	octave 2
	note A_, 2
	sound_loop 4, .loop3
	rest 2
	note B_, 2
	rest 2
	note B_, 2
	note_type 6, 11, 7
	octave 4
	note E_, 4
	rest 1
	octave 3
	note B_, 1
	octave 4
	note E_, 1
	note G_, 1
	note B_, 8
	octave 5
	note E_, 8
	volume_envelope 11, 1
	rest 4
	octave 2
	note B_, 4
	rest 4
	octave 3
	note E_, 4
	rest 4
	note E_, 4
.loop4:
	rest 4
	octave 2
	note A_, 4
	sound_loop 3, .loop4
	volume_envelope 10, 7
	octave 4
	note Fs, 4
	note A_, 4
	rest 4
	octave 3
	note G_, 2
	note B_, 2
	octave 4
	note D_, 2
	rest 2
	octave 3
	note B_, 2
	note G_, 2
	octave 4
	note G_, 1
	note A_, 1
	note G_, 1
	note A_, 1
	note G_, 1
	note A_, 1
	note G_, 1
	note A_, 1
	note G_, 8
	rest 4
	octave 3
	note Fs, 2
	note A_, 2
	octave 4
	note D_, 2
	rest 2
	octave 3
	note A_, 2
	note Fs, 2
	octave 4
	note Fs, 1
	note G_, 1
	note Fs, 1
	note G_, 1
	note Fs, 1
	note G_, 1
	note Fs, 1
	note G_, 1
	note Fs, 8
	note D_, 2
	rest 2
	octave 3
	note B_, 2
	note G_, 2
	octave 4
	note G_, 2
	rest 2
	note D_, 2
	octave 3
	note B_, 2
	octave 4
	note B_, 2
	rest 2
	note Gs, 2
	note D_, 2
	octave 3
	note B_, 2
	octave 4
	note D_, 2
	note Gs, 2
	note B_, 2
	note Cs, 2
	rest 2
	note Cs, 6
	rest 2
	note Cs, 2
	note Cs, 2
	note D_, 2
	rest 2
	note D_, 6
	rest 2
	note D_, 2
	note D_, 2
	note Ds, 2
	rest 2
	note Ds, 6
	rest 2
	note Ds, 2
	note Ds, 2
	note E_, 2
	rest 2
	note E_, 6
	rest 2
	note E_, 2
	note E_, 2
	sound_loop 0, .mainloop

Music_BugCatchingContest_Ch2:
	stereo_panning TRUE, FALSE
	vibrato 8, 2, 3
	duty_cycle 1
	note_type 12, 12, 7
	rest 8
.loop5:
	octave 4
	note A_, 1
	rest 1
	note A_, 3
	rest 1
	note A_, 1
	note A_, 1
	sound_loop 4, .loop5
.mainloop:
	duty_cycle 0
	sound_call .sub1
	octave 2
	note E_, 2
	octave 3
	note G_, 1
	rest 1
	octave 1
	note B_, 2
	octave 3
	note G_, 1
	rest 1
	octave 2
	note E_, 2
	octave 3
	note G_, 1
	rest 1
	octave 2
	note G_, 2
	octave 3
	note G_, 1
	rest 1
	octave 2
	note A_, 2
	octave 3
	note A_, 1
	rest 1
	octave 2
	note Gs, 2
	octave 3
	note Gs, 1
	rest 1
	octave 2
	note G_, 2
	octave 3
	note G_, 1
	rest 1
	octave 2
	note E_, 2
	octave 3
	note E_, 1
	rest 1
	sound_call .sub1
	octave 2
	note E_, 2
	octave 3
	note G_, 1
	rest 1
	octave 1
	note B_, 2
	octave 3
	note G_, 1
	rest 1
	octave 2
	note A_, 2
	octave 4
	note Cs, 1
	rest 1
	octave 2
	note E_, 2
	octave 4
	note Cs, 1
	rest 1
	octave 2
	note D_, 2
	octave 3
	note Fs, 1
	rest 1
	octave 1
	note A_, 2
	octave 3
	note Fs, 1
	rest 1
	octave 1
	note B_, 2
	octave 3
	note Fs, 1
	rest 1
	octave 2
	note Ds, 2
	octave 3
	note A_, 1
	rest 1
	duty_cycle 0
	rest 2
	octave 5
	note D_, 1
	rest 1
	note Cs, 2
	note D_, 1
	rest 1
	octave 4
	note B_, 8
	rest 2
	octave 5
	note D_, 1
	rest 1
	note Cs, 2
	note D_, 1
	rest 1
	octave 4
	note A_, 8
	rest 2
	octave 5
	note D_, 4
	note Fs, 2
	note E_, 2
	note D_, 1
	rest 1
	note Cs, 2
	note D_, 1
	rest 1
	note E_, 1
	rest 1
	note E_, 1
	rest 3
	note E_, 10
	note A_, 1
	rest 1
	note A_, 1
	rest 3
	note A_, 10
	sound_loop 0, .mainloop

.sub1:
	octave 2
	note D_, 2
	octave 3
	note Fs, 1
	rest 1
	octave 1
	note A_, 2
	octave 3
	note Fs, 1
	rest 1
	octave 1
	note B_, 2
	octave 3
	note Fs, 1
	rest 1
	octave 2
	note Cs, 2
	octave 3
	note Fs, 1
	rest 1
	octave 2
	note E_, 2
	octave 3
	note G_, 1
	rest 1
	octave 1
	note B_, 2
	octave 3
	note G_, 1
	rest 1
	octave 2
	note Cs, 2
	octave 3
	note G_, 1
	rest 1
	octave 2
	note Ds, 2
	octave 3
	note G_, 1
	rest 1
	sound_ret

Music_BugCatchingContest_Ch3:
	stereo_panning TRUE, TRUE
	vibrato 16, 2, 3
	note_type 12, 1, 4
	rest 8
	octave 3
	note A_, 1
	rest 1
	note A_, 3
	rest 1
	note A_, 1
	note A_, 1
	note B_, 1
	rest 1
	note B_, 3
	rest 1
	note B_, 1
	note B_, 1
	octave 4
	note C_, 1
	rest 1
	note C_, 3
	rest 1
	note C_, 1
	note C_, 1
	note Cs, 1
	rest 1
	note Cs, 3
	rest 1
	note Cs, 1
	note Cs, 1
.mainloop:
	note_type 12, 1, 4
	note Fs, 5
	rest 1
	note Fs, 1
	note G_, 1
	note A_, 4
	octave 5
	note D_, 4
	note Cs, 2
	volume_envelope 1, 0
	note E_, 1
	note G_, 1
	note B_, 1
	rest 1
	note G_, 1
	note E_, 1
	note_type 6, 2, 0
	octave 6
	note E_, 1
	note Fs, 1
	note E_, 1
	note Fs, 1
	note E_, 1
	note Fs, 1
	note E_, 1
	note Fs, 1
	note E_, 8
	volume_envelope 1, 4
	octave 4
	note E_, 10
	rest 2
	note E_, 2
	note Fs, 2
	note G_, 8
	octave 5
	note Cs, 8
	rest 4
	volume_envelope 1, 0
	note E_, 2
	note G_, 2
	note A_, 2
	rest 2
	note E_, 2
	note C_, 2
	octave 6
	note Cs, 1
	note D_, 1
	note Cs, 1
	note D_, 1
	note Cs, 1
	note D_, 1
	note Cs, 1
	note D_, 1
	note Cs, 8
	volume_envelope 1, 5
	octave 5
	note Fs, 4
	rest 4
	note G_, 4
	rest 4
	note A_, 4
	rest 4
	octave 6
	note D_, 4
	rest 4
	note Cs, 12
	octave 5
	note B_, 2
	octave 6
	note Cs, 2
	octave 5
	note B_, 4
	rest 4
	octave 6
	note G_, 4
	note Fs, 4
	rest 4
	octave 5
	note G_, 4
	note A_, 4
	note B_, 4
	octave 6
	note E_, 4
	note D_, 4
	note Cs, 4
	octave 5
	note B_, 2
	octave 6
	note Cs, 2
	note D_, 4
	rest 8
	octave 5
	note B_, 4
	note A_, 4
	rest 1
	note Ds, 1
	note Fs, 1
	note A_, 1
	octave 6
	note C_, 8
	octave 3
	note G_, 2
	rest 2
	note B_, 2
	rest 2
	note D_, 2
	rest 2
	note B_, 2
	rest 2
	note E_, 2
	rest 2
	note B_, 2
	rest 2
	note G_, 2
	rest 2
	note B_, 2
	rest 2
	note D_, 2
	rest 2
	note Fs, 2
	rest 2
	octave 2
	note A_, 2
	rest 2
	octave 3
	note Fs, 2
	rest 2
	note Ds, 2
	rest 2
	octave 4
	note C_, 2
	rest 2
	octave 3
	note A_, 2
	rest 2
	octave 4
	note C_, 2
	rest 2
	octave 3
	note G_, 2
	rest 2
	note B_, 2
	rest 2
	note D_, 2
	rest 2
	note B_, 2
	rest 2
	note Gs, 2
	rest 2
	note B_, 2
	rest 2
	note E_, 2
	rest 2
	note B_, 2
	rest 2
	note A_, 2
	rest 2
	note A_, 6
	rest 2
	note A_, 2
	note A_, 2
	note B_, 2
	rest 2
	note B_, 6
	rest 2
	note B_, 2
	note B_, 2
	octave 4
	note C_, 2
	rest 2
	note C_, 6
	rest 2
	note C_, 2
	note C_, 2
	note Cs, 2
	rest 2
	note Cs, 6
	rest 2
	note Cs, 2
	note Cs, 2
	sound_loop 0, .mainloop

Music_BugCatchingContest_Ch4:
	toggle_noise 4
	drum_speed 6
	drum_note 2, 1
	drum_note 2, 1
	drum_note 2, 1
	drum_note 2, 1
	drum_note 2, 1
	drum_note 2, 1
	drum_note 2, 1
	drum_note 2, 1
	drum_note 2, 1
	drum_note 2, 1
	drum_note 2, 1
	drum_note 2, 1
	drum_note 5, 2
	drum_note 5, 2
	sound_call Music_BugCatchingContest_Ch4.sub1
.mainloop:
.loop1:
	drum_note 3, 4
	drum_note 3, 4
	drum_note 3, 2
	drum_note 2, 2
	drum_note 3, 2
	drum_note 2, 2
	drum_note 3, 4
	drum_note 3, 4
	drum_note 3, 2
	drum_note 2, 2
	drum_note 3, 2
	drum_note 2, 2
	drum_note 3, 4
	drum_note 3, 4
	drum_note 3, 2
	drum_note 2, 2
	drum_note 3, 2
	drum_note 2, 2
	drum_note 3, 4
	drum_note 3, 4
	drum_note 2, 1
	drum_note 2, 1
	drum_note 2, 1
	drum_note 2, 1
	drum_note 2, 1
	drum_note 2, 1
	drum_note 2, 1
	drum_note 2, 1
	sound_loop 5, .loop1
	drum_note 3, 4
	drum_note 3, 4
	drum_note 3, 2
	drum_note 3, 2
	drum_note 3, 2
	drum_note 3, 2
	drum_note 3, 4
	drum_note 3, 4
	drum_note 3, 2
	drum_note 3, 2
	drum_note 3, 2
	drum_note 3, 2
	sound_call .sub1
	sound_loop 0, .mainloop

.sub1:
	drum_note 3, 4
	drum_note 12, 8
	drum_note 3, 2
	drum_note 3, 2
	drum_note 3, 4
	drum_note 12, 8
	drum_note 3, 2
	drum_note 3, 2
	drum_note 3, 4
	drum_note 12, 8
	drum_note 3, 2
	drum_note 3, 2
	drum_note 3, 4
	drum_note 12, 8
	drum_note 3, 2
	drum_note 3, 2
	sound_ret
