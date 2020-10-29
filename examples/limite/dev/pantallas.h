// MTE MK1 v4.7
// Copyleft 2010, 2011 by The Mojon Twins

// pantallas.h
// Includes the binary of the aplib-compressed static screens.
// title.bin, marco.bin (if suited) and ending.bin should be
// in the same directory.

extern unsigned char s_title [];
extern unsigned char s_marco [];
extern unsigned char s_ending [];

#asm
	._s_title
		BINARY "title.bin"
	._s_marco
#endasm
#ifndef DIRECT_TO_PLAY
#asm
		BINARY "marco.bin"
#endasm
#endif
#asm
	._s_ending
		BINARY "ending.bin"
#endasm

void unpack (unsigned int address) {
	asm_int = address;
	
	#asm
		ld hl, 22528
		ld (hl), 0
		push hl
		pop de
		inc de
		ld bc, 767
		ldir
		ld hl, (_asm_int)
		ld de, 16384
		call depack
	#endasm
}

void espera_activa (int espera) {
	// Waits until "espera" halts have passed 
	// or a key has been pressed.

	#asm 
		ei
	#endasm
	while (espera--)  {
		#asm
			halt
		#endasm
		if (sp_GetKey()) {
			break;
		}
	}
	#asm
		di
	#endasm
}

void title_screen (void) {
	sp_UpdateNow();
	unpack ((unsigned int) (s_title));

	#asm
		; Music generated by beepola
		call musicstart
		di
	#endasm
	
	while (1) {
		if (sp_KeyPressed (key_1)) {
			joyfunc = sp_JoyKeyboard; break;
		} else if (sp_KeyPressed (key_2)) {
			joyfunc = sp_JoyKempston; break;
		} else if (sp_KeyPressed (key_3)) {
			joyfunc = sp_JoySinclair1; break;
		}			
	}
}

void game_ending (void) {
	sp_UpdateNow();
	unpack ((unsigned int) (s_ending));
	
	for (gpit = 0; gpit < 4; gpit ++) {
		peta_el_beeper (6);
		peta_el_beeper (3);
	}
	peta_el_beeper (11);
	
	espera_activa (500);
}

void game_over (void) {
	draw_rectangle (10, 11, 21, 13, GAME_OVER_ATTR);		
	draw_text (11, 12, GAME_OVER_ATTR, "GAME OVER!");

	sp_UpdateNow ();
		
	for (gpit = 0; gpit < 4; gpit ++) {
		peta_el_beeper (6);
		peta_el_beeper (3);
	}
	peta_el_beeper (10);
	
	espera_activa (500);
}