Music_KantoGymBattle:
	channel_count 3
	channel 1, Music_KantoGymBattle_Ch1
	channel 2, Music_KantoGymBattle_Ch2
	channel 3, Music_KantoGymBattle_Ch3

Music_KantoGymBattle_Ch1:
	tempo 101
	volume 7, 7
	duty_cycle 3
	pitch_offset 2
	vibrato 18, 1, 5
	note_type 12, 11, 2
	octave 3
	note As, 1
	note A_, 1
	note Gs, 1
	note G_, 1
	note Gs, 1
	note G_, 1
	note Fs, 1
	note G_, 1
	note Fs, 1
	note F_, 1
	note Fs, 1
	note F_, 1
	note E_, 1
	note F_, 1
	note E_, 1
	note Ds, 1
	note E_, 1
	note Ds, 1
	note D_, 1
	note Ds, 1
	note D_, 1
	note Cs, 1
	note D_, 1
	note Cs, 1
	note C_, 1
	note Cs, 1
	note C_, 1
	octave 2
	note B_, 1
	octave 3
	note C_, 1
	octave 2
	note B_, 1
	note As, 1
	note B_, 1
	note As, 2
	note As, 6
	note As, 2
	note As, 6
	note As, 2
	note As, 8
	volume_envelope 11, 7
	note B_, 2
	volume_envelope 11, 2
	note As, 2
	note As, 6
	note As, 2
	note As, 6
	note As, 2
	note As, 8
	volume_envelope 11, 7
	octave 3
	note E_, 2
	volume_envelope 11, 2
	octave 2
	note As, 2
	note As, 6
	note As, 2
	note As, 6
	note As, 2
	note As, 8
	volume_envelope 11, 7
	note B_, 2
	volume_envelope 11, 2
	note As, 2
	note As, 6
	note As, 2
	note As, 6
	note As, 2
	volume_envelope 11, 4
	octave 3
	note E_, 4
	note E_, 4
	note E_, 2
.mainloop:
	sound_call .sub1
	octave 2
	note As, 2
	volume_envelope 11, 7
	note Gs, 8
	note B_, 8
	octave 3
	note Ds, 10
	note E_, 6
	sound_call .sub1
	note Ds, 2
	volume_envelope 11, 7
	note Cs, 8
	octave 2
	note B_, 8
	note Gs, 10
	octave 3
	note E_, 6
	volume_envelope 11, 2
	note Ds, 2
	note Ds, 2
	note G_, 1
	note E_, 1
	note Ds, 1
	note G_, 1
	note Ds, 2
	note Ds, 2
	note As, 1
	note Gs, 1
	note E_, 1
	note As, 1
	note Ds, 2
	note Ds, 2
	note B_, 1
	note As, 1
	note Gs, 1
	note B_, 1
	note Ds, 2
	volume_envelope 11, 7
	note E_, 2
	volume_envelope 11, 2
	note Ds, 2
	note Ds, 2
	note G_, 1
	note E_, 1
	note Ds, 1
	note G_, 1
	note Ds, 2
	note Ds, 2
	note As, 1
	note Gs, 1
	note E_, 1
	note As, 1
	note Ds, 2
	note Ds, 2
	note B_, 1
	note As, 1
	note Gs, 1
	note B_, 1
	note Gs, 2
	volume_envelope 11, 5
	note B_, 2
	note Ds, 2
	note Cs, 2
	octave 2
	note As, 4
	note Ds, 2
	note Gs, 2
	note B_, 2
	octave 3
	note E_, 2
	note Ds, 4
	note Cs, 2
	octave 2
	note As, 2
	note As, 2
	note Ds, 2
	note As, 2
	octave 3
	note Cs, 2
	volume_envelope 11, 7
	octave 2
	note B_, 8
	note As, 8
	note Gs, 10
	note B_, 6
	volume_envelope 11, 5
	octave 3
	note Ds, 2
	note Cs, 2
	octave 2
	note As, 4
	note Ds, 2
	note Gs, 2
	note B_, 2
	octave 3
	note E_, 2
	note Ds, 4
	note Cs, 2
	octave 2
	note As, 2
	note As, 2
	note Gs, 2
	note As, 2
	octave 3
	note Cs, 2
	volume_envelope 11, 7
	note E_, 8
	note Fs, 8
	note G_, 16
	volume_envelope 11, 2
	octave 2
	note Gs, 2
	note Gs, 2
	volume_envelope 11, 7
	octave 3
	note Cs, 12
	volume_envelope 11, 2
	note Gs, 2
	note F_, 4
	volume_envelope 11, 5
	octave 2
	note F_, 4
	note G_, 2
	note Gs, 2
	note As, 2
	volume_envelope 11, 2
	note As, 2
	note As, 2
	volume_envelope 11, 7
	octave 3
	note Ds, 12
	volume_envelope 11, 2
	note As, 2
	note G_, 4
	volume_envelope 11, 7
	octave 4
	note Ds, 6
	volume_envelope 4, -4
	octave 3
	note D_, 4
	sound_loop 0, .mainloop

.sub1:
	volume_envelope 11, 5
	note Ds, 2
	octave 2
	note As, 2
	octave 3
	note Ds, 2
	note E_, 4
	note Ds, 2
	note Cs, 2
	octave 2
	note As, 2
	note Ds, 2
	note Gs, 2
	note As, 2
	octave 3
	note Ds, 2
	note E_, 2
	note Ds, 2
	note Cs, 2
	sound_ret

Music_KantoGymBattle_Ch2:
	duty_cycle 3
	vibrato 8, 3, 6
	pitch_offset 1
	note_type 12, 12, 2
.loop1:
	octave 4
	note As, 1
	note Gs, 1
	note As, 1
	octave 5
	note Ds, 1
	sound_loop 8, .loop1
	octave 3
.loop2:
	volume_envelope 12, 2
	note Ds, 2
	note Ds, 2
	note Fs, 1
	note E_, 1
	note Ds, 1
	note Fs, 1
	note Ds, 2
	note Ds, 2
	note As, 1
	note Gs, 1
	note Fs, 1
	note As, 1
	note Ds, 2
	note Ds, 2
	note B_, 1
	note As, 1
	note Gs, 1
	note B_, 1
	note D_, 2
	volume_envelope 12, 7
	note E_, 2
	volume_envelope 12, 2
	note Ds, 2
	note Ds, 2
	note Fs, 1
	note E_, 1
	note Ds, 1
	note Fs, 1
	note Ds, 2
	note Ds, 2
	note As, 1
	note Gs, 1
	note Fs, 1
	note As, 1
	note Ds, 2
	note Ds, 2
	note B_, 1
	note As, 1
	note Gs, 1
	note B_, 1
	note Gs, 2
	volume_envelope 12, 7
	note B_, 2
	volume_envelope 12, 2
	octave 4
	sound_loop 2, .loop2
.mainloop:
	sound_call .sub1
	note E_, 8
	note Gs, 8
	note B_, 10
	note Gs, 6
	sound_call .sub1
	note E_, 8
	note Gs, 8
	note B_, 10
	octave 4
	note E_, 6
	note Ds, 8
	volume_envelope 11, 7
	note Ds, 8
	volume_envelope 10, 2
	rest 6
	octave 5
	note E_, 4
	note E_, 4
	note E_, 2
	volume_envelope 10, 0
	note Ds, 8
	volume_envelope 10, 7
	note Ds, 8
	rest 12
	volume_envelope 12, 5
	octave 3
	note As, 4
	note B_, 4
	note Gs, 4
	note As, 4
	note B_, 4
	note Gs, 4
	note E_, 4
	volume_envelope 11, 0
	note Ds, 8
	note Ds, 16
	volume_envelope 11, 7
	note Ds, 12
	volume_envelope 12, 5
	note As, 4
	note B_, 4
	note Gs, 4
	note As, 4
	note B_, 4
	note Gs, 4
	note B_, 4
	volume_envelope 11, 0
	note As, 8
	volume_envelope 11, 7
	note As, 8
	volume_envelope 11, 0
	octave 4
	note Ds, 8
	volume_envelope 11, 7
	note Ds, 8
	volume_envelope 12, 2
	octave 3
	note Cs, 2
	note Cs, 2
	volume_envelope 12, 7
	note F_, 8
	volume_envelope 12, 2
	octave 4
	note Cs, 4
	note F_, 2
	note Cs, 4
	volume_envelope 12, 7
	note Gs, 4
	note G_, 2
	note F_, 2
	note Ds, 2
	volume_envelope 12, 2
	octave 3
	note Ds, 2
	note Ds, 2
	volume_envelope 12, 7
	note G_, 8
	volume_envelope 12, 2
	octave 4
	note Ds, 4
	note G_, 2
	note Ds, 4
	volume_envelope 12, 7
	note As, 6
	octave 3
	note A_, 4
	sound_loop 0, .mainloop

.sub1:
	duty_cycle 2
	volume_envelope 13, 1
	octave 3
	note As, 2
	volume_envelope 10, 2
	note As, 2
	volume_envelope 13, 1
	note Gs, 2
	volume_envelope 10, 2
	note Gs, 2
	volume_envelope 13, 1
	note As, 2
	volume_envelope 10, 2
	note As, 2
	volume_envelope 12, 5
	note B_, 2
	octave 4
	note Cs, 2
	note Cs, 2
	octave 3
	note B_, 4
	note As, 2
	note Gs, 2
	note Fs, 2
	note Gs, 2
	note As, 2
	duty_cycle 3
	volume_envelope 11, 0
	sound_ret

Music_KantoGymBattle_Ch3:
	note_type 12, 1, -1
	octave 2
	note E_, 1
	rest 1
.loop1:
	note B_, 3
	rest 1
	sound_loop 6, .loop1
	note B_, 2
	note Gs, 2
	note B_, 2
.loop2:
	note Ds, 1
	rest 1
	note Ds, 1
	rest 1
	note Fs, 4
	note Ds, 1
	rest 1
	note Ds, 1
	rest 1
	note As, 4
	note Ds, 1
	rest 1
	note Ds, 1
	rest 1
	note B_, 4
	note D_, 1
	rest 1
	note F_, 2
	note Ds, 1
	rest 1
	note Ds, 1
	rest 1
	note Fs, 4
	note Ds, 1
	rest 1
	note Ds, 1
	rest 1
	note As, 4
	note Ds, 1
	rest 1
	note Ds, 1
	rest 1
	note B_, 4
	note Ds, 1
	rest 1
	note E_, 2
	sound_loop 2, .loop2
.mainloop:
.loop3:
	octave 2
	note Ds, 2
	note As, 2
	sound_loop 8, .loop3
.loop4:
	note E_, 2
	note B_, 2
	sound_loop 4, .loop4
	octave 1
	note B_, 2
	octave 2
	note E_, 2
	octave 1
	note B_, 2
	octave 2
	note E_, 4
	note B_, 2
	octave 3
	note E_, 2
	octave 2
	note B_, 2
.loop5:
	octave 2
	note Fs, 2
	octave 3
	note Cs, 2
	sound_loop 8, .loop5
.loop6:
	octave 2
	note E_, 2
	note B_, 2
	sound_loop 7, .loop6
	octave 3
	note E_, 2
	octave 2
	note B_, 2
.loop7:
	note Ds, 1
	rest 1
	note Ds, 1
	rest 1
	note As, 4
	note Ds, 1
	rest 1
	note Ds, 1
	rest 1
	note B_, 4
	note Ds, 1
	rest 1
	note Ds, 1
	rest 1
	note As, 4
	note Ds, 1
	rest 1
	note E_, 2
	sound_loop 2, .loop7
.loop8:
	note Ds, 2
	note As, 2
	sound_loop 7, .loop8
	octave 3
	note Cs, 2
	octave 2
	note As, 2
	note E_, 2
	note B_, 2
	octave 3
	note E_, 2
	octave 2
	note E_, 2
	note B_, 2
	octave 3
	note E_, 2
.loop9:
	octave 2
	note E_, 2
	note B_, 2
	sound_loop 5, .loop9
.loop10:
	octave 2
	note Fs, 2
	octave 3
	note Cs, 2
	sound_loop 7, .loop10
	octave 2
	note As, 2
	octave 3
	note Cs, 2
	octave 2
	note E_, 2
	note As, 2
	octave 3
	note E_, 2
	note Fs, 2
	note Gs, 2
	note Fs, 2
	note E_, 2
	note Cs, 2
.loop11:
	octave 2
	note Ds, 2
	note As, 2
	sound_loop 4, .loop11
.loop12:
	note Cs, 2
	note Gs, 2
	sound_loop 8, .loop12
.loop13:
	note Ds, 2
	note As, 2
	sound_loop 4, .loop13
	octave 3
	note Ds, 2
	octave 2
	note As, 2
	note Ds, 2
	note D_, 2
	note D_, 2
	note As, 2
	octave 3
	note Ds, 2
	note F_, 2
	sound_loop 0, .mainloop
