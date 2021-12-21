Music_KantoWildBattle:
	channel_count 3
	channel 1, Music_KantoWildBattle_Ch1
	channel 2, Music_KantoWildBattle_Ch2
	channel 3, Music_KantoWildBattle_Ch3

	db $3 ; unused, leftover from a channel 4

Music_KantoWildBattle_Ch1:
	tempo 112
	volume 7, 7
	vibrato 18, 2, 3
	duty_cycle 2
	note_type 12, 11, 7
	rest 16
	octave 4
	note As, 6
	note A_, 1
	note Gs, 1
	note A_, 1
	note Gs, 1
	note G_, 1
	note Fs, 1
	note Gs, 1
	note G_, 1
	note Fs, 1
	note F_, 1
.mainloop:
	note_type 12, 11, 7
	sound_call .sub1
	octave 4
	note F_, 4
	note Cs, 4
	octave 3
	note B_, 8
	sound_call .sub1
	rest 8
	note G_, 1
	note B_, 1
	octave 4
	note Cs, 1
	octave 3
	note B_, 1
	note G_, 1
	note B_, 1
	octave 4
	note D_, 1
	note Ds, 1
	volume_envelope 11, 1
	note E_, 6
	note C_, 6
	octave 3
	note G_, 4
	octave 4
	note E_, 6
	note C_, 6
	note E_, 4
	volume_envelope 11, 7
	note F_, 1
	note Cs, 1
	octave 3
	note Gs, 1
	octave 4
	note C_, 1
	note Cs, 1
	octave 3
	note Gs, 1
	note F_, 1
	note G_, 1
	note Gs, 4
	note Fs, 1
	note G_, 1
	note Gs, 1
	octave 4
	note C_, 1
	note Cs, 4
	octave 3
	note F_, 1
	note Gs, 1
	octave 4
	note Cs, 1
	note E_, 1
	note F_, 3
	note G_, 1
	note Gs, 4
	volume_envelope 11, 1
	octave 3
	note G_, 6
	note F_, 6
	note E_, 4
	note Gs, 6
	note G_, 6
	note F_, 4
	octave 4
	note C_, 6
	octave 3
	note As, 6
	note G_, 4
	note_type 8, 11, 7
	octave 2
	note G_, 2
	octave 3
	note C_, 2
	note E_, 2
	note C_, 2
	note E_, 2
	note G_, 2
	note E_, 2
	note G_, 2
	octave 4
	note C_, 2
	octave 3
	note G_, 2
	octave 4
	note C_, 2
	note E_, 2
	note F_, 2
	note Cs, 2
	octave 3
	note Gs, 2
	octave 4
	note Cs, 2
	octave 3
	note Gs, 2
	note F_, 2
	note Gs, 2
	note F_, 2
	note Cs, 2
	note F_, 2
	note Cs, 2
	octave 2
	note Gs, 2
	note_type 12, 11, 7
	octave 3
	note Gs, 4
	octave 4
	note Cs, 4
	volume_envelope 7, 1
	note F_, 1
	note F_, 1
	volume_envelope 9, 1
	note F_, 1
	note F_, 1
	volume_envelope 11, 1
	note Gs, 1
	note Gs, 1
	volume_envelope 13, 1
	note Gs, 1
	note Gs, 1
	volume_envelope 12, 7
	note E_, 1
	note D_, 1
	note Cs, 1
	note D_, 1
	note E_, 12
	note E_, 8
	volume_envelope 11, 1
	note E_, 1
	note D_, 1
	note C_, 1
	octave 3
	note As, 1
	note A_, 1
	note G_, 1
	note F_, 1
	note E_, 1
	volume_envelope 11, 7
	note D_, 8
	note F_, 8
	note As, 8
	octave 4
	note D_, 8
	octave 3
	note G_, 8
	octave 4
	note C_, 8
	note E_, 8
	note G_, 8
	rest 8
	octave 3
	note Ds, 8
	note Gs, 8
	octave 4
	note C_, 8
	octave 3
	note As, 8
	rest 8
	octave 4
	note D_, 8
	octave 3
	note Gs, 2
	note F_, 2
	note D_, 2
	note F_, 2
	octave 4
	note C_, 4
	rest 6
	note C_, 1
	rest 5
	note Cs, 2
	note C_, 1
	rest 13
	note Cs, 4
	rest 6
	note Cs, 1
	rest 5
	note F_, 2
	note Ds, 1
	rest 5
	note Cs, 8
.loop1:
	volume_envelope 11, 1
	octave 3
	note C_, 1
	note Ds, 1
	note D_, 1
	note Cs, 1
	sound_loop 8, .loop1
.loop2:
	note Cs, 1
	note F_, 1
	note E_, 1
	note Ds, 1
	sound_loop 8, .loop2
.loop3:
	note C_, 1
	note Ds, 1
	note D_, 1
	note Cs, 1
	sound_loop 16, .loop3
.loop4:
	note Cs, 1
	note E_, 1
	note Ds, 1
	note D_, 1
	sound_loop 8, .loop4
.loop5:
	note C_, 1
	note Ds, 1
	note D_, 1
	note Cs, 1
	sound_loop 8, .loop5
	sound_loop 0, .mainloop

.sub1:
	octave 4
	note C_, 1
	rest 5
	octave 3
	note B_, 1
	rest 5
	note As, 1
	rest 3
	rest 10
	octave 4
	note C_, 2
	rest 2
	note Ds, 2
	note Cs, 1
	rest 5
	note C_, 1
	rest 5
	octave 3
	note B_, 1
	rest 3
	sound_ret

Music_KantoWildBattle_Ch2:
	duty_cycle 1
	vibrato 18, 2, 3
	note_type 12, 11, 1
	octave 4
	note G_, 1
	note Fs, 1
	note F_, 1
	note E_, 1
	note G_, 1
	note Fs, 1
	note F_, 1
	note E_, 1
	note G_, 1
	note Fs, 1
	note F_, 1
	note E_, 1
	note G_, 1
	note Fs, 1
	note F_, 1
	note E_, 1
	note G_, 1
	note Fs, 1
	note F_, 1
	note E_, 1
	note Fs, 1
	note F_, 1
	note E_, 1
	note Ds, 1
	note F_, 1
	note E_, 1
	note Ds, 1
	note D_, 1
	note E_, 1
	note Ds, 1
	note D_, 1
	note Cs, 1
.mainloop:
	note_type 12, 11, 7
	sound_call .sub1
	octave 2
	note F_, 2
	octave 1
	note B_, 1
	rest 1
	octave 2
	note G_, 2
	sound_call .sub1
	octave 2
	note G_, 1
	rest 1
	octave 1
	note B_, 1
	rest 1
	octave 2
	note F_, 1
	note Fs, 1
	note_type 6, 11, 7
	sound_call .sub2
	sound_call .sub2
	transpose 0, 1
	sound_call .sub2
	sound_call .sub2
	transpose 0, 0
	sound_call .sub2
	sound_call .sub2
	sound_call .sub2
	sound_call .sub2
	transpose 0, 1
	sound_call .sub2
	sound_call .sub2
	transpose 0, 0
	sound_call .sub2
	sound_call .sub2
	rest 16
	octave 3
	note As, 16
	octave 4
	note D_, 16
	note F_, 16
	note_type 12, 11, 7
	note E_, 16
	note G_, 8
	octave 5
	note C_, 8
	rest 8
	octave 3
	note Gs, 8
	octave 4
	note C_, 8
	note Ds, 8
	note F_, 8
	note F_, 1
	octave 3
	note As, 1
	note Gs, 1
	note As, 1
	octave 4
	note F_, 1
	octave 3
	note As, 1
	note Gs, 1
	note As, 1
	octave 4
	note Gs, 8
	note Gs, 1
	note F_, 1
	note D_, 1
	octave 3
	note B_, 1
	octave 4
	note F_, 1
	note D_, 1
	octave 3
	note B_, 1
	note Gs, 1
	note_type 6, 11, 7
	sound_call .sub3
	transpose 0, 1
	sound_call .sub3
	transpose 0, 0
	sound_call .sub4
	octave 2
	note C_, 2
	rest 2
	octave 3
	note G_, 4
	octave 2
	note C_, 2
	rest 2
	note C_, 2
	rest 2
	octave 4
	note C_, 4
	octave 2
	note C_, 2
	rest 2
	octave 4
	note Ds, 4
	octave 2
	note C_, 2
	rest 2
	octave 4
	note Cs, 4
	octave 2
	note Cs, 2
	rest 2
	note Cs, 2
	rest 2
	octave 4
	note Cs, 4
	octave 2
	note Cs, 2
	rest 2
	note Cs, 2
	rest 2
	octave 4
	note Cs, 4
	octave 2
	note Cs, 2
	rest 2
	note Cs, 2
	rest 2
	octave 4
	note Cs, 4
	octave 2
	note Cs, 2
	rest 2
	note Cs, 2
	rest 2
	octave 4
	note Cs, 4
	octave 2
	note Cs, 2
	rest 2
	octave 4
	note F_, 4
	octave 2
	note Cs, 2
	rest 2
	sound_call .sub4
	sound_call .sub5
	sound_call .sub4
	sound_call .sub5
	transpose 0, 1
	sound_call .sub4
	transpose 0, 0
	note Cs, 2
	rest 2
	octave 3
	note Gs, 4
	octave 2
	note Cs, 2
	rest 2
	note Cs, 2
	rest 2
	octave 4
	note Cs, 4
	octave 2
	note Cs, 2
	rest 2
	octave 3
	note As, 4
	octave 2
	note Cs, 2
	rest 2
	sound_call .sub4
	note C_, 2
	rest 2
	octave 3
	note G_, 4
	octave 2
	note C_, 2
	rest 2
	note C_, 2
	rest 2
	octave 4
	note C_, 4
	octave 2
	note C_, 2
	rest 2
	octave 3
	note As, 4
	octave 2
	note C_, 2
	rest 2
	sound_loop 0, .mainloop

.sub1:
	octave 2
	note C_, 1
	rest 2
	note C_, 1
	octave 1
	note G_, 1
	rest 1
	octave 2
	note C_, 1
	rest 1
	octave 3
	note C_, 2
	octave 2
	note C_, 1
	rest 1
	octave 1
	note G_, 1
	rest 2
	note G_, 1
	octave 2
	note C_, 3
	note C_, 1
	octave 1
	note G_, 2
	octave 2
	note C_, 2
	octave 1
	note G_, 2
	octave 2
	note Fs, 2
	octave 1
	note G_, 2
	octave 2
	note G_, 2
	note Cs, 1
	rest 2
	note Cs, 1
	octave 1
	note Gs, 1
	rest 1
	octave 2
	note Cs, 1
	rest 1
	octave 3
	note Cs, 1
	rest 1
	octave 2
	note Cs, 2
	octave 1
	note Gs, 1
	rest 2
	note Gs, 1
	octave 2
	note Cs, 1
	rest 2
	note Cs, 1
	octave 1
	note Gs, 1
	rest 1
	octave 2
	note Cs, 1
	rest 1
	octave 1
	note G_, 2
	sound_ret

.sub2:
	octave 2
	note C_, 2
	rest 4
	note C_, 1
	rest 1
	note C_, 2
	octave 3
	note C_, 2
	octave 2
	note C_, 4
	rest 2
	note C_, 2
	rest 2
	note C_, 2
	octave 3
	note C_, 2
	rest 2
	octave 2
	note C_, 4
	sound_ret

.sub3:
	octave 2
	note C_, 2
	rest 4
	note C_, 2
	rest 4
	octave 1
	note G_, 1
	rest 1
	note G_, 1
	rest 1
	octave 2
	note E_, 2
	rest 2
	note E_, 2
	rest 2
	note E_, 2
	note C_, 6
	note C_, 2
	rest 4
	note C_, 2
	octave 1
	note G_, 2
	octave 2
	note E_, 2
	note C_, 2
	octave 1
	note G_, 2
	octave 2
	note C_, 2
	rest 4
	note C_, 2
	note E_, 2
	note G_, 2
	note B_, 2
	octave 3
	note C_, 2
	sound_ret

.sub4:
	octave 3
	note G_, 4
	octave 2
	note C_, 2
	rest 2
	note C_, 2
	rest 2
	octave 3
	note G_, 4
	octave 2
	note C_, 2
	rest 2
	note C_, 2
	rest 2
	octave 3
	note G_, 4
	octave 2
	note C_, 2
	rest 2
	sound_ret

.sub5:
	note C_, 2
	rest 2
	octave 3
	note G_, 4
	octave 2
	note C_, 2
	rest 2
	note C_, 2
	rest 2
	octave 3
	note G_, 4
	octave 2
	note C_, 2
	rest 2
	octave 3
	note G_, 4
	octave 2
	note C_, 2
	rest 2
	sound_ret

Music_KantoWildBattle_Ch3:
	vibrato 20, 2, 3
	note_type 12, 1, 4
	octave 3
	note C_, 1
	rest 1
	note C_, 1
	octave 4
	note C_, 1
	octave 3
	note Cs, 1
	rest 1
	note Cs, 1
	octave 4
	note Cs, 1
	octave 3
	note D_, 1
	rest 1
	note D_, 1
	octave 4
	note D_, 1
	octave 3
	note Ds, 1
	rest 1
	note Ds, 1
	octave 4
	note Ds, 1
	octave 3
	note E_, 1
	rest 1
	note E_, 1
	octave 4
	note E_, 1
	octave 3
	note F_, 1
	rest 1
	note F_, 1
	octave 4
	note F_, 1
	octave 3
	note Fs, 1
	rest 1
	note Fs, 1
	octave 4
	note Fs, 1
	octave 3
	note G_, 1
	rest 1
	note G_, 1
	octave 4
	note G_, 1
.mainloop:
	note_type 12, 1, 4
	octave 5
	note G_, 1
	rest 2
	octave 4
	note G_, 1
	rest 2
	octave 5
	note G_, 1
	rest 3
	octave 4
	note G_, 1
	rest 1
	octave 5
	note G_, 1
	rest 2
	octave 4
	note G_, 1
	octave 3
	note G_, 1
	note B_, 1
	octave 4
	note C_, 1
	note D_, 1
	note Ds, 6
	octave 5
	note Ds, 2
	rest 2
	note G_, 2
	note Gs, 1
	rest 2
	octave 4
	note Gs, 1
	rest 2
	octave 5
	note Gs, 1
	rest 3
	octave 4
	note Gs, 1
	rest 1
	octave 5
	note Gs, 1
	rest 2
	octave 4
	note Gs, 1
	octave 6
	note Cs, 6
	octave 5
	note Gs, 1
	note F_, 1
	note B_, 1
	note G_, 1
	note F_, 1
	note D_, 1
	note G_, 1
	note F_, 1
	note D_, 1
	octave 4
	note B_, 1
	octave 5
	note G_, 1
	rest 2
	octave 4
	note G_, 1
	rest 2
	octave 5
	note G_, 1
	rest 3
	octave 4
	note G_, 1
	rest 1
	octave 5
	note G_, 1
	rest 2
	octave 4
	note G_, 1
	note C_, 1
	note D_, 1
	note Ds, 1
	note F_, 1
	note G_, 6
	octave 5
	note Ds, 2
	rest 2
	note G_, 2
	note Gs, 1
	rest 2
	octave 4
	note G_, 1
	rest 2
	octave 5
	note Gs, 1
	rest 3
	octave 4
	note G_, 1
	rest 1
	octave 5
	note Gs, 1
	rest 2
	octave 4
	note G_, 1
	note Cs, 4
	note Gs, 4
	octave 5
	note F_, 7
	note Fs, 1
	note G_, 2
	volume_envelope 2, 4
	note C_, 1
	octave 4
	note G_, 1
	octave 5
	note C_, 1
	octave 4
	note G_, 1
	volume_envelope 1, 4
	octave 5
	note Fs, 2
	volume_envelope 2, 4
	note C_, 1
	octave 4
	note G_, 1
	octave 5
	note C_, 1
	octave 4
	note G_, 1
	volume_envelope 1, 4
	octave 5
	note E_, 2
	volume_envelope 2, 4
	note C_, 1
	octave 4
	note G_, 1
	volume_envelope 1, 4
	octave 5
	note G_, 2
	volume_envelope 2, 4
	note C_, 1
	octave 4
	note G_, 1
	octave 5
	note C_, 1
	octave 4
	note G_, 1
	volume_envelope 1, 4
	octave 5
	note A_, 2
	volume_envelope 2, 4
	note C_, 1
	octave 4
	note G_, 1
	octave 5
	note C_, 1
	octave 4
	note G_, 1
	volume_envelope 1, 4
	octave 5
	note G_, 2
	volume_envelope 2, 4
	note C_, 1
	octave 4
	note Gs, 1
	volume_envelope 1, 4
	octave 5
	note Gs, 12
	note G_, 2
	note F_, 2
	note Gs, 2
	note G_, 2
	note Cs, 1
	note F_, 1
	note Gs, 1
	octave 6
	note C_, 1
	note Cs, 8
	octave 5
	note C_, 2
	volume_envelope 2, 4
	octave 4
	note E_, 1
	note G_, 1
	note E_, 1
	note G_, 1
	volume_envelope 1, 4
	note As, 2
	volume_envelope 2, 4
	note E_, 1
	note G_, 1
	note E_, 1
	note G_, 1
	volume_envelope 1, 4
	note Gs, 2
	volume_envelope 2, 4
	note E_, 1
	note G_, 1
	volume_envelope 1, 4
	octave 5
	note Cs, 2
	volume_envelope 2, 4
	octave 4
	note E_, 1
	note G_, 1
	note E_, 1
	note G_, 1
	volume_envelope 1, 4
	octave 5
	note C_, 2
	volume_envelope 2, 4
	octave 4
	note E_, 1
	note G_, 1
	note E_, 1
	note G_, 1
	volume_envelope 1, 4
	note As, 2
	volume_envelope 2, 4
	note E_, 1
	note G_, 1
	volume_envelope 1, 4
	octave 5
	note F_, 2
	volume_envelope 2, 4
	octave 4
	note G_, 1
	octave 5
	note C_, 1
	octave 4
	note G_, 1
	octave 5
	note C_, 1
	volume_envelope 1, 4
	note E_, 2
	volume_envelope 2, 4
	octave 4
	note G_, 1
	octave 5
	note C_, 1
	octave 4
	note G_, 1
	octave 5
	note C_, 1
	volume_envelope 1, 4
	note D_, 2
	volume_envelope 2, 4
	octave 4
	note G_, 1
	octave 5
	note C_, 1
	volume_envelope 1, 4
	octave 4
	note As, 4
	octave 5
	note C_, 4
	note D_, 4
	note F_, 4
	note Gs, 16
	note_type 6, 1, 4
	note Gs, 1
	rest 1
	volume_envelope 2, 4
	note Gs, 1
	rest 1
	volume_envelope 3, 4
	note Gs, 1
	rest 1
	note Gs, 1
	rest 1
	note Gs, 1
	rest 1
	note Gs, 1
	rest 1
	volume_envelope 2, 4
	note Gs, 1
	rest 1
	note Gs, 1
	rest 1
	note Gs, 1
	rest 1
	note Gs, 1
	rest 1
	volume_envelope 1, 4
	note Gs, 1
	rest 1
	note Gs, 1
	rest 1
	octave 6
	note Cs, 1
	rest 1
	note Cs, 1
	rest 1
	note Cs, 1
	rest 1
	note Cs, 1
	rest 1
	octave 5
	note G_, 2
	note F_, 2
	note E_, 2
	note F_, 2
	note_type 12, 1, 5
	note G_, 12
	note G_, 8
	note_type 6, 1, 5
	note G_, 1
	rest 1
	note G_, 1
	rest 1
	note G_, 1
	rest 1
	note G_, 1
	rest 1
	volume_envelope 2, 4
	note G_, 1
	rest 1
	note G_, 1
	rest 1
	volume_envelope 1, 4
	note G_, 1
	rest 1
	note G_, 1
	rest 1
	note_type 12, 1, 4
	octave 2
	note As, 16
	note As, 1
	rest 2
	note As, 1
	rest 2
	note As, 1
	rest 1
	octave 3
	note As, 1
	rest 2
	note As, 1
	rest 2
	note As, 1
	rest 1
	note C_, 1
	rest 2
	note_type 6, 1, 5
	note C_, 1
	rest 1
	note C_, 2
	octave 4
	note C_, 2
	octave 3
	note C_, 4
	rest 2
	note C_, 2
	rest 2
	note C_, 2
	octave 4
	note C_, 2
	rest 2
	octave 3
	note C_, 4
	note C_, 2
	rest 4
	note C_, 1
	rest 1
	note C_, 2
	octave 4
	note C_, 2
	octave 3
	note C_, 4
	rest 2
	note C_, 2
	rest 2
	note C_, 2
	octave 2
	note As, 2
	rest 2
	note As, 2
	rest 2
	note_type 12, 1, 4
	note Gs, 16
	note Gs, 16
	note As, 2
	rest 2
	note As, 1
	rest 3
	note As, 1
	rest 1
	octave 3
	note As, 1
	rest 1
	octave 2
	note As, 1
	rest 1
	octave 3
	note As, 1
	rest 1
	note D_, 2
	rest 2
	note D_, 1
	rest 3
	note D_, 1
	rest 1
	octave 4
	note D_, 1
	rest 1
	octave 2
	note B_, 1
	rest 1
	octave 3
	note B_, 1
	rest 1
	octave 5
	note E_, 4
	octave 4
	note C_, 1
	note E_, 1
	note G_, 1
	note B_, 1
	octave 5
	note C_, 1
	rest 1
	note E_, 1
	rest 5
	note F_, 2
	note E_, 1
	rest 1
	octave 4
	note C_, 1
	octave 3
	note G_, 1
	octave 4
	note C_, 1
	note E_, 1
	note G_, 1
	note E_, 1
	note C_, 1
	note E_, 1
	note G_, 1
	octave 5
	note C_, 1
	note E_, 1
	note G_, 1
	note F_, 4
	octave 4
	note Cs, 1
	note F_, 1
	note Gs, 1
	octave 5
	note C_, 1
	note Cs, 1
	rest 1
	note F_, 1
	rest 5
	note Gs, 2
	note G_, 1
	rest 5
	note F_, 8
	volume_envelope 1, 3
	note G_, 6
	note Fs, 6
	note E_, 2
	rest 2
	note G_, 6
	note A_, 6
	note G_, 2
	rest 2
	note Gs, 12
	note G_, 2
	note F_, 2
	note Gs, 2
	note G_, 2
	rest 4
	octave 6
	note Cs, 8
	octave 5
	note C_, 6
	octave 4
	note As, 6
	note Gs, 2
	rest 2
	octave 5
	note Cs, 6
	note C_, 6
	octave 4
	note As, 2
	rest 2
	octave 5
	note F_, 6
	note E_, 6
	note D_, 2
	rest 2
	octave 4
	note As, 4
	octave 5
	note C_, 4
	note D_, 4
	note F_, 4
	note Gs, 16
	note Gs, 16
	note G_, 16
	note G_, 16
	sound_loop 0, .mainloop
