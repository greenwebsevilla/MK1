// MTE MK1 v4.8
// Copyleft 2010-2013, 2020-2021 by The Mojon Twins

// Add here your custom routines & vars

// Two sets

unsigned char map_behaviours [] = {
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 1, 1, 1, 1,
	0, 1, 1, 1, 1, 1, 1
};

// Texts

unsigned char top_string []    = "<======================>";
unsigned char temp_string []   = ";                      [";
unsigned char bottom_string [] = "\\]]]]]]]]]]]]]]]]]]]]]]^";
unsigned char redraw_after_text;
unsigned char intro_text;
unsigned char talk_sounds [] = { 7, 11 };

//                        XXXXXXXXXXXXXXXXXXXXXX
unsigned char text0 [] = "(DOTTY)%"
						 "HAY SETAS A RAUDALES!%"
						 "PERO PAPI, YA LO PETAS%"
						 "SI CON LAS NORMALES%"
						 "TRAES SETAS CON TETAS!";

unsigned char text1 [] = "(PEPI CARAORTIGA)%"
						 "SOY AMIGA DE TU HIJA,%"
						 "NO SOY NADA MOJIGATA.%"
						 "LUEGO, SI NO SE FIJA,%"
						 "TE VOY A DAR, PIRATA!";

unsigned char text2 [] = "(TAMARA LA TIA RARA)%"
						 "BUSCO NOVIO VERSADO%"
						 "EN TEMAS DEL QUERER%"
						 "ERES ALGO ESMIRRIADO%"
						 "PERO ME PUEDES VALER";

unsigned char text3 [] = "(RAMIRO EL VAMPIRO)%"
						 "YO ESTARIA INTERESADO%"
						 "EN BUSCAR UN CAMASTRO%"
						 "PERO ANDO MUY CANSADO%"
						 "Y TENGO UN PADRASTRO";

unsigned char text4 [] = "(ALBERTO EL TUERTO)%"
						 "TE MIRO SIN COMPLEJOS%"
						 "DESDE HACE UN RATITO%"
						 "NO SE SI ESTAS LEJOS%"
						 "O ERES MUY BAJITO.";

unsigned char text5 [] = "(RAMIRO EL VAMPIRO)%"
						 "PERDONA SI TE GRITO%"
						 "ESTO ES LO QUE HAY.%"
						 "AUNQUE YO SEA BAJITO%"
						 "TU TAMPOCO ERES ROMAY!";

unsigned char text6 [] = "(DONOSO EL SEBOSO)%"
						 "COMO HABRAS NOTADO%"
						 "SOY UN TIPO MUY FINO%"
						 "ME GUSTA COMER HELADO%"
						 "CUBIERTO DE TOCINO!";

unsigned char text7 [] = "(RAMIRO EL VAMPIRO)%"
						 "VAYA MEZCLA SIN PAR%"
						 "BAJA POR TUS CONDUCTOS%"
						 "NI ME QUIERO IMAGINAR%"
						 "EL OLOR DE TUS ERUCTOS";

unsigned char text8 [] = "(CAMILA LA WILA)%"
						 "ME GUSTA CAZAR VARONES%"
						 "COMO A BRUJAS Y MAGAS%"
						 "PERO NO HAGO POCIONES%"
						 "YO ME BAJO LAS BRAGAS";

unsigned char text9 [] = "(RAMIRO EL VAMPIRO)%"
						 "APARTA DE MI, SIRENA%"
						 "QUE ANDO DE MISION%"
						 "AY, VAYA UNA CONDENA%"
						 "SER VAMPIRO GUAPETON!";

unsigned char text10 [] = "(RAMIRO EL VAMPIRO)%"
						  "VAYA, EL BOSQUE MOLA%"
						  "PERO !AY, QUE OLVIDO!%"
						  "NO TRAEMOS MERENDOLA%"
						  "Y NO HEMOS COMIDO...";						  

unsigned char text11 [] = "(RAMIRO EL VAMPIRO)%"
						  "DOTTY ESTA COMO IDA%"
						  "EL HAMBRE LE PIERDE!%"
						  "SI NO TRAIGO COMIDA%"
						  "SEGURO QUE ME MUERDE!";

unsigned char text12 [] = "(RAMIRO EL VAMPIRO)%"
						  "YA TENGO SUFICIENTES%"
						  "CON TETAS O SIN TETAS%"
						  "SON ESTUPEFACIENTES%"
						  "Y LAS DEJAN MAJARETAS";

unsigned char text13 [] = "(DOTTY)%"
						  "AY, ERES EL MAS MEJOR%"
						  "UN PAPI AZUL CIELO%"
						  "CON CARITA DE ALFAJOR%"
						  "Y GRASITA POR EL PELO";

unsigned char text14 [] = "(RAMIRO EL VAMPIRO)%"
						  "COMEROS ESTAS SETAS,%"
						  "QUE NO SON TONTERIAS%"
						  "LO DICEN LAS RECETAS:%"
						  "DE LO QUE COMES,CRIAS";

unsigned char *texts [] = {
	text0, text1, 			// Dotty y Pepi Caraortiga
	text2, text3, 			// Tamara la tía rara
	text4, text5, 			// Alberto el tuerto
	text6, text7, 			// Donoso el seboso
	text8, text9, 			// Camila la Wila
	text10, text11,			// Intro
	text12, 				// Ya tienes todas
	text13, text14,			// Final
};

// Jumo
extern unsigned char sprites_jumo [0];
#asm
	._sprite_jumo
		BINARY "sprites_jumo.bin"
#endasm

unsigned char jumo_x, jumo_y, jumo_ct;

// Functions & protos

void enems_en_an_calc (unsigned char n);

void redraw_from_buffer (void) {
	#asm
			ld  a, VIEWPORT_X
			ld  (__x), a
			ld  a, VIEWPORT_Y
			ld  (__y), a
			
			xor a
		.redraw_from_buffer_loop
			ld  (_gpit), a

			ld  bc, (_gpit)
			ld  b, 0
			ld  hl, _map_buff
			add hl, bc
			ld  a, (hl)
			cp  16
			jr  nc, redraw_set_tile
			ld  c, a
			ld  a, (_tileoffset)
			add c

		.redraw_set_tile
			ld  (__t), a

			call _draw_coloured_tile_do

			ld  a, (__x)
			add a, 2
			cp  VIEWPORT_X + 30
			jr  nz, redraw_from_buffer_set_x
			ld  a, (__y)
			add a, 2
			ld  (__y), a
			ld  a, VIEWPORT_X
		.redraw_from_buffer_set_x
			ld  (__x), a

			ld  a, (_gpit)
			inc a
			cp  150
			jr  nz, redraw_from_buffer_loop
	#endasm
}

void clear_temp_string (void) {
	#asm
			ld  hl, _temp_string+1
			ld  de, _temp_string+2
			ld  bc, 21
			ld  a, 32
			ld  (hl), a
			ldir
	#endasm
}

void show_text_box (unsigned char n) {
	//sprite_remove_aid ();

	gp_gen = texts [n];

	// Text renderer will read the string and
	// build substrings for draw_text.

	//clear_temp_string ();
	draw_text (4, 6, 40, top_string);
	rdy = 7;

	while (1) {
		clear_temp_string ();
		if (rdy > 7) draw_text (4, rdy - 1, 40, temp_string);
		
		#asm
				// Fill buffer
				ld  de, _temp_string + 1
				ld  hl, (_gp_gen)

			.fill_buffer_loop
				ld  a, (hl)
				or  a
				jr  z, fill_buffer_end
				cp  '%'
				jr  z, fill_buffer_end

				ld  (de), a

				inc hl
				inc de
				jr  fill_buffer_loop

			.fill_buffer_end
				ld  (_gp_gen), hl
		#endasm

		draw_text (4, rdy ++, 40, temp_string);
		draw_text (4, rdy ++, 40, bottom_string);

		cpc_UpdateNow (0); 	// Update, no sprites
		play_sfx (talk_sounds [rand () & 1]);
		
		#asm
				ld  b, #(20*6)
			.textbox_delay
				halt
				djnz textbox_delay
		#endasm
	
		if (*gp_gen == 0) break;
		gp_gen ++;
	}

	while (any_key ()); while (!any_key ()); 
	if (redraw_after_text) {
		redraw_from_buffer ();
		hotspot_paint ();
		render_all_sprites ();
		cpc_UpdateNow (1);
	}
	redraw_after_text = 1;
}


#ifdef ENABLE_CODE_HOOKS

	// Hooks

	void hook_system_inits (void) {
		// Create a custom 4x8 sprite for jumos
		// 0   2   4      6   7   8  9  10 11 12      14
		// sp0 sp1 coord0 cox coy cx cy ox oy invfunc updfunc
		#asm
				ld  ix, #(BASE_SPRITES+(SP_CUSTOM_BASE*16))

				ld  hl, _sprite_18_a 					// sm_sprptr [0]
				ld  (ix + 1), h
				ld  (ix + 0), l

				ld  (ix + 3), h
				ld  (ix + 2), l	
				
				xor a
				ld  (ix + 6), a 		// .cox
				ld  (ix + 7), a 		// .coy
				ld  (ix + 8), a 		// .cx
				ld  (ix + 9), a 		// .cy
				ld  (ix + 10), a 		// .ox
				ld  (ix + 11), a 		// .oy

				ld  hl, cpc_PutSpTileMap4x8Px			// .invfunc
				ld  (ix + 13), h
				ld  (ix + 12), l

				ld  hl, cpc_PutTrSp4x8TileMap2bPx 		// .updfunc
				ld  (ix + 15), h
				ld  (ix + 14), l
		#endasm

	}

	void hook_init_game (void) {
		intro_text = 1;
		//n_pant = 1;
		//n_pant = 15;
		//n_pant = 17;
	}

	void hook_init_mainloop (void) {
		// Intro
		if (intro_text && n_pant == 6 && player.possee) {
			intro_text = 0;
			redraw_after_text = 1;
			show_text_box (10);
			show_text_box (11);
		}
	}

	void hook_mainloop (void) {
		if (player.killingzone_beepcount && jumo_ct == 0) {
			#asm
					ld  ix, #(BASE_SPRITES+(SP_CUSTOM_BASE*16))

					ld  hl, _sprite_jumo
					ld  (ix + 1), h
					ld  (ix + 0), l

					ld  a, 16
					ld  (_jumo_ct), a
					ld  a, (_gpx)
					add #(4 + (VIEWPORT_X * 4))
					srl a
					ld  (_jumo_x), a
					ld  a, (_gpy)
					add #(VIEWPORT_Y * 8)
					ld  (_jumo_y), a
			#endasm
		}

		if (jumo_ct) {
			#asm
					ld  hl, _jumo_y
					dec (hl)

					ld  hl, _jumo_ct
					dec (hl)
			#endasm

			if (jumo_y <= 16) jumo_ct = 0;

			if (jumo_ct == 0) {
				#asm				
						ld  ix, #(BASE_SPRITES+(SP_CUSTOM_BASE*16))

						ld  hl, _sprite_18_a 					// sm_sprptr [0]
						ld  (ix + 1), h
						ld  (ix + 0), l
				#endasm
			} else {
				#asm
						ld  ix, #(BASE_SPRITES+(SP_CUSTOM_BASE*16))
						ld  a, (_jumo_x)
						ld  (ix + 8), a 		// .cx
						ld  a, (_jumo_y)
						ld  (ix + 9), a 		// .cy
				#endasm
			}
		}
	}

	void hook_entering (void) {		
		// Jumo off
		#asm
				xor a 
				ld  (_jumo_ct), a

				ld  ix, #(BASE_SPRITES+(SP_CUSTOM_BASE*16))

				ld  hl, _sprite_18_a 					// sm_sprptr [0]
				ld  (ix + 1), h
				ld  (ix + 0), l
		#endasm

	}

#endif

#ifdef ENABLE_CUSTOM_ENEMS

	void extra_enems_init (void) {
		
		if (n_pant == 21) {
			if (_en_t == 1) 
				enems_en_an_calc (6); 
			else
				enems_en_an_calc (_en_t - 10);
		}
		
	}

	void extra_enems_move (void) {		
		switch (_en_t) {
			case 14:

				break;
			case 15:
				break;
		}
	}

	void extra_enems_checks (void) {
	}

	void extra_enems_killed (void) {
	}
	
#endif
