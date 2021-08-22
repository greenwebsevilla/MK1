// Motor.h
// Contiene las cosas del motor para la churrera, a saber:

// 1.- Inicializaciones (de enemigos y de cosas varias).
// 2.- Movimiento del prota.
// 3.- Movimiento de los enemigos/plataformas moviles.
// 4.- Dibujado de la pantalla (depacking/render).

#define EST_NORMAL 		0
#define EST_PARP 		2
#define EST_MUR 		4
#define sgni(n)			(n < 0 ? -1 : 1)

typedef struct {
	signed int x, y, cx;
	signed int vx, vy;
	signed char g, ax, rx;
	unsigned char salto, cont_salto;
	unsigned char *current_frame, *next_frame;
	unsigned char saltando;
	unsigned char frame, subframe, facing;
	unsigned char estado;
	unsigned char ct_estado;
	unsigned char gotten;
	unsigned char life, objs, keys;
} INERCIA;

INERCIA player;

typedef struct {
	unsigned char frame;
	unsigned char count;
	unsigned char *current_frame, *next_frame;
} ANIMADO;

ANIMADO en_an [3];

// atributos de la pantalla: Contiene informaci�n
// sobre qu� tipo de tile hay en cada casilla
unsigned char map_attr [150];
unsigned char map_buff [150];

// posici�n del objeto (hotspot). Para no objeto,
// se colocan a 240,240, que est� siempre fuera de pantalla.
unsigned char hotspot_x;
unsigned char hotspot_y;
unsigned char orig_tile;	// Tile que hab�a originalmente bajo el objeto

unsigned char pant_final;

// Funciones:

void srand (unsigned int new_seed) {
	seed [0] = new_seed;	
}

unsigned char rand () {
	unsigned char res;
	
	#asm
		.rand16
			ld	hl, _seed
			ld	a, (hl)
			ld	e, a
			inc	hl
			ld	a, (hl)
			ld	d, a
			
			;; Ahora DE = [SEED]
						
			ld	a,	d
			ld	h,	e
			ld	l,	253
			or	a
			sbc	hl,	de
			sbc	a, 	0
			sbc	hl,	de
			ld	d, 	0
			sbc	a, 	d
			ld	e,	a
			sbc	hl,	de
			jr	nc,	nextrand
			inc	hl
		.nextrand
			ld	d,	h
			ld	e,	l
			ld	hl, _seed
			ld	a,	e
			ld	(hl), a
			inc	hl
			ld	a,	d
			ld	(hl), a
			
			;; Ahora [SEED] = HL
		
			ld 	hl, _asm_int
			ld	a,	e	
			ld	(hl), a
			inc	hl
			ld	a,	d
			ld	(hl), a
			
			;; Ahora [ASM_INT] = HL
	#endasm
	
	res = asm_int [0];

	return res;
}

void step () {
	#asm
		ld a, 16
		out (254), a
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		xor 16
		out (254), a
	#endasm	
}

void cortina () {
	/*
	#asm
		;; Antes que nada vamos a limpiar el PAPER de toda la pantalla
		;; para que no queden artefactos feos
		
		ld	de, 22528			; Apuntamos con DE a la zona de atributos
		ld	b,	3				; Procesamos 3 tercios
	.clearb1
		push bc
		
		ld	b, 255				; Procesamos los 256 atributos de cada tercio
	.clearb2
	
		ld	a, (de)				; Nos traemos un atributo
		and	199					; Le hacemos la m�scara 11000111 y dejamos PAPER a 0
		ld	(de), a				; Y lo volvemos a poner
		
		inc de					; Siguiente atributo
	
		djnz clearb2
		
		pop bc
		djnz clearb1
		
		;; Y ahora el c�digo original que escrib� para UWOL:	
	
		ld	a,	8
	
	.repitatodo
		ld	c,	a			; Salvamos el contador de "repitatodo" en 'c'
	
		ld	hl, 16384
		ld	a,	12
	
	.bucle
		ld	b,	a			; Salvamos el contador de "bucle" en 'b'
		ld	a,	255
	
	.bucle1
		sla (hl)
		inc hl
		dec a
		jr	nz, bucle1
			
		ld	a,	255
	.bucle2
		srl (hl)
		inc hl
		dec a
		jr	nz, bucle2
			
		ld	a,	b			; Restituimos el contador de "bucle" a 'a'
		dec a
		jr	nz, bucle
	
		ld	a,	c			; Restituimos el contador de "repitatodo" a 'a'
		dec a
		jr	nz, repitatodo
	#endasm
	// Estoy llorando 
	*/
}

// Game

void espera_activa (int espera) {
	
	// jL
	
	// Esta funci�n espera un rato o hasta que se pulse una tecla.
	// Si se pulsa una tecla, devuelve 0
	
	// Esta funci�n s�lo funciona en Spectrum.
	// en CPC no hay una interrupci�n cada 20ms, as�n que esto no
	// sirve "pa n�".
	
	unsigned int i;
	
	for (i = 0; i < espera; i ++) {
		#asm
			halt
		#endasm
		if (cpc_AnyKeyPressed ()) 
			break;	
	}
}

void attr (char x, char y) {
	// x + 15 * y = x + (16 - 1) * y = x + 16 * y - y = x + (y << 4) - y.
	if (x < 0 || y < 0) return 2;
	return map_attr [x + (y << 4) - y];	
}

void qtile (unsigned char x, unsigned char y) {
	// x + 15 * y = x + (16 - 1) * y = x + 16 * y - y = x + (y << 4) - y.
	return map_buff [x + (y << 4) - y];	
}

void draw_life () {
	cpc_PutTile2x8 ((unsigned int *) (tile_0 + ((16 + player.life / 100) << 4)), (unsigned char *) (8 + (LIFE_X << 1)), (unsigned char *) (4 + (LIFE_Y << 3)));
	cpc_PutTile2x8 ((unsigned int *) (tile_0 + ((16 + (player.life % 100) / 10) << 4)), (unsigned char *) (8 + (LIFE_X << 1) + 2), (unsigned char *) (4 + (LIFE_Y << 3)));
	cpc_PutTile2x8 ((unsigned int *) (tile_0 + ((16 + player.life % 10) << 4)), (unsigned char *) (8 + (LIFE_X << 1) + 4), (unsigned char *) (4 + (LIFE_Y << 3)));
}

void draw_objs () {
	cpc_PutTile2x8 ((unsigned int *) (tile_0 + ((16 + player.objs / 10) << 4)), (unsigned char *) (8 + (OBJECTS_X << 1)), (unsigned char *) (4 + (OBJECTS_Y << 3)));
	cpc_PutTile2x8 ((unsigned int *) (tile_0 + ((16 + player.objs % 10) << 4)), (unsigned char *) (8 + (OBJECTS_X << 1) + 2), (unsigned char *) (4 + (OBJECTS_Y << 3)));
}

void draw_keys () {
	cpc_PutTile2x8 ((unsigned int *) (tile_0 + ((16 + player.keys / 10) << 4)), (unsigned char *) (8 + (KEYS_X << 1)), (unsigned char *) (4 + (KEYS_Y << 3)));
	cpc_PutTile2x8 ((unsigned int *) (tile_0 + ((16 + player.keys % 10) << 4)), (unsigned char *) (8 + (KEYS_X << 1) + 2), (unsigned char *) (4 + (KEYS_Y << 3)));
}

void draw_coloured_tile (unsigned char x, unsigned char y, unsigned char t) {
	
		t = 64 + (t << 2);
	cpc_SetTile (x, y, t);
	cpc_SetTile (x + 1, y, t + 1);
	cpc_SetTile (x, y + 1, t + 2);
	cpc_SetTile (x + 1, y + 1, t + 3);
		}

void draw_coloured_tile_inv (unsigned char x, unsigned char y, unsigned char t) {
	
	t = 64 + (t << 2);
	cpc_SetTouchTileXY (x, y, t);
	cpc_SetTouchTileXY (x + 1, y, t + 1);
	cpc_SetTouchTileXY (x, y + 1, t + 2);
	cpc_SetTouchTileXY (x + 1, y + 1, t + 3);
}

void game_ending () {
	unsigned char x;
	
	unpack ((unsigned int) (s_ending));
	
	for (x = 0; x < 4; x ++) {
		peta_el_beeper (3);
		peta_el_beeper (8);
	}
	peta_el_beeper (5);
	
	espera_activa (500);
}

void game_over () {
	unsigned char x, y;
	for (y = 9; y < 12; y ++)
		for (x = 9; x < 21; x ++)
			cpc_SetTile (x, y, 0);
			
	cpc_SetTile (10, 10, 39);
	cpc_SetTile (11, 10, 33);
	cpc_SetTile (12, 10, 45);
	cpc_SetTile (13, 10, 37);
	
	cpc_SetTile (15, 10, 53);
	cpc_SetTile (16, 10, 46);
	cpc_SetTile (17, 10, 36);
	cpc_SetTile (18, 10, 37);
	cpc_SetTile (19, 10, 50);

	cpc_ResetTouchedTiles ();
	cpc_ShowTileMap (0);
		
	for (x = 0; x < 4; x ++) {
		peta_el_beeper (0);
		peta_el_beeper (1);
	}
	peta_el_beeper (2);
	
	espera_activa (500);
}

void clear_cerrojo (unsigned char np, unsigned char x, unsigned char y) {
	unsigned char i;
	
	// search & toggle
		
	for (i = 0; i < MAX_CERROJOS; i ++) 
		if (cerrojos [i].x == x && cerrojos [i].y == y && cerrojos [i].np == np)
			cerrojos [i].st = 0;
}

void init_cerrojos () {
	unsigned char i;
	
	// Activa todos los cerrojos
	
	for (i = 0; i < MAX_CERROJOS; i ++)
		cerrojos [i].st = 1;	
}

unsigned char move (unsigned char n_pant) {
	unsigned char xx, yy;
	unsigned char x, y;
	unsigned char i, allpurp;

	/* Por partes. Primero el movimiento vertical. La ecuaci�n de movimien-
	   to viene a ser, en cada ciclo:

	   1.- vy = vy + g
	   2.- y = y + vy

	   O sea la velocidad afectada por la gravedad. 
	   Para no colarnos con los nmeros, ponemos limitadores:
	*/

	if (player.vy < PLAYER_MAX_VY_CAYENDO)
		player.vy += player.g;
	else
		player.vy = PLAYER_MAX_VY_CAYENDO;

	if (player.gotten) player.vy = 0;
		
	player.y += player.vy;
	
	// Safe
		
	if (player.y < 0)
		player.y = 0;
		
	if (player.y > 9216)
		player.y = 9216;
	
	/* El problema es que no es tan f�cil... Hay que ver si no nos chocamos.
	   Si esto pasa, hay que "recular" hasta el borde del obst�culo.

	   Por eso miramos el signo de vy, para que los c�lculos sean m�s sencillos.
	   De paso vamos a precalcular un par de cosas para que esto vaya m�s r�pido.
	*/

	x = player.x >> 6;				// dividimos entre 64 para pixels, y luego entre 16 para tiles.
	y = player.y >> 6;
	xx = x >> 4;
	yy = y >> 4;
	
	// Par�ntesis, aprovechando las variables para ahorrar... 
	// Tiles que te matan. 
	// Joer, qu� ofuscao... Pero es corto y bello.
	
	if (attr (xx, yy) == 1 || 
		((x & 15) != 0 && attr (xx + 1, yy) == 1) ||
		((y & 15) != 0 && attr (xx, yy + 1) == 1) ||
		((x & 15) != 0 && (y & 15) != 0 && attr (xx + 1, yy + 1) == 1)) {
		if (player.life > 0) {
			peta_el_beeper (4);
			player.life --;	
			draw_life ();
			if (player.vy < 0) {
				player.vy = PLAYER_MAX_VX;
				player.y = (yy + 1) << 10;
				y = player.y >> 6;
				yy = y >> 4;
			} else {
				player.vy = -PLAYER_MAX_VX;
				player.y = yy << 10;
				y = player.y >> 6;
				yy = y >> 4;
			}
		}
	}
	
	// Ya	

	if (player.vy < 0) { 			// estamos ascendiendo
		//if (player.y >= 1024)
			if (attr (xx, yy) > 1 || ((x & 15) != 0 && attr (xx + 1, yy) > 1)) {
				// paramos y ajustamos:
				player.vy = 0;
				player.y = (yy + 1) << 10;
			}
	} else if (player.vy > 0) { 	// estamos descendiendo
		if (player.y < 9216)
			if (attr (xx, yy + 1) > 1 || ((x & 15) != 0 && attr (xx + 1, yy + 1) > 1))
			{
				// paramos y ajustamos:
				player.vy = 0;
				player.y = yy << 10;
			}
	}

	/* Salto: El salto se reduce a dar un valor negativo a vy. Esta es la forma m�s
	   sencilla. Sin embargo, para m�s control, usamos el tipo de salto "mario bros".
	   Para ello, en cada pulsaci�n dejaremos decrementar vy hasta un m�nimo, y de-
	   tectando que no se vuelva a pulsar cuando estemos en el aire. Juego de banderas ;)
	*/

	if ( (cpc_TestKey (KEY_ARR) || cpc_TestKey (KEY_FIR) || cpc_TestKey (KEY_JOY_ARR) || cpc_TestKey (KEY_JOY_FIR)) && ((player.vy == 0 && player.saltando == 0 && (attr (xx, yy + 1) > 1 || ((x & 15) != 0 && attr (xx + 1, yy + 1) > 1))) || player.gotten)) {
		player.saltando = 1;
		player.cont_salto = 0;
		peta_el_beeper (7);
		// CPC: Ajusto a m�ltiplo de 4 p�xels
		player.x = (player.x >> 8) << 8;
		// Ajuste bestia de cojones, 6+2.
	}

	if ( (cpc_TestKey (KEY_ARR) || cpc_TestKey (KEY_FIR) || cpc_TestKey (KEY_JOY_ARR) || cpc_TestKey (KEY_JOY_FIR)) && player.saltando ) {
		player.vy -= (player.salto + PLAYER_INCR_SALTO - (player.cont_salto>>1));
		if (player.vy < -PLAYER_MAX_VY_SALTANDO) player.vy = -PLAYER_MAX_VY_SALTANDO;

#ifndef PLAYER_JUMP_UNLIMITED
		player.cont_salto ++;
		if (player.cont_salto == 8)
			player.saltando = 0;
#endif
	}

	if ( !cpc_TestKey (KEY_ARR) && !cpc_TestKey (KEY_FIR) && !cpc_TestKey (KEY_JOY_ARR) && !cpc_TestKey (KEY_JOY_FIR) )
		player.saltando = 0;
	
	// ------ ok con el movimiento vertical.

	/* Movimiento horizontal:

	   Mientras se pulse una tecla de direcci�n, 
	   
	   x = x + vx
	   vx = vx + ax

	   Si no se pulsa nada:

	   x = x + vx
	   vx = vx - rx
	*/

	if ( ! (cpc_TestKey (KEY_IZQ) || cpc_TestKey (KEY_DER) || cpc_TestKey (KEY_JOY_IZQ) || cpc_TestKey (KEY_JOY_DER)))
		if (player.vx > 0) {
			player.vx -= player.rx;
			if (player.vx < 0)
				player.vx = 0;
		} else if (player.vx < 0) {
			player.vx += player.rx;
			if (player.vx > 0)
				player.vx = 0;
		}

	if (cpc_TestKey (KEY_IZQ) || cpc_TestKey (KEY_JOY_IZQ))
		if (player.vx > -PLAYER_MAX_VX) {
			player.facing = 0;
			player.vx -= player.ax;
		}

	if (cpc_TestKey (KEY_DER) || cpc_TestKey (KEY_JOY_DER))
		if (player.vx < PLAYER_MAX_VX) {
			player.vx += player.ax;
			player.facing = 1;
		}

	player.x = player.x + player.vx;
	
	// Safe
	
	if (player.x < 0)
		player.x = 0;
		
	if (player.x > 14336)
		player.x = 14336;
		
	/* Ahora, como antes, vemos si nos chocamos con algo, y en ese caso
	   paramos y reculamos */

	y = player.y >> 6;
	x = player.x >> 6;
	yy = y >> 4;
	xx = x >> 4;
	
	if (player.vx < 0) {
		if (attr (xx, yy) > 1 || ((y & 15) != 0 && attr (xx, yy + 1) > 1)) {
			// paramos y ajustamos:
			player.vx = 0;
			player.x = (xx + 1) << 10;
		}
	} else if (player.vx > 0) {
		if (attr (xx + 1, yy) > 1 || ((y & 15) != 0 && attr (xx + 1, yy + 1) > 1)) {
			// paramos y ajustamos:
			player.vx = 0;
			player.x = xx << 10;
		}
	} else if (!player.gotten) {
		// CPC: Ajusto a m�ltiplo de 4 p�xels
		player.x = (player.x >> 8) << 8;
		// Ajuste bestia de cojones, 6+2.	
	}

	// Abrir cerrojo
	if ((x & 15) == 0 && (y & 15) == 0) {
		if (qtile (xx + 1, yy) == 15 && player.keys > 0) {
			map_attr [15 * yy + xx + 1] = 0;
			map_buff [15 * yy + xx + 1] = 0;
			clear_cerrojo (n_pant, xx + 1, yy);
			draw_coloured_tile_inv (VIEWPORT_X + xx + xx + 2, VIEWPORT_Y + yy + yy, 0);
			player.keys --;
			draw_keys ();
			peta_el_beeper (3);
		} else if (qtile (xx - 1, yy) == 15 && player.keys > 0) {
			map_attr [15 * yy + xx - 1] = 0;
			map_buff [15 * yy + xx - 1] = 0;
			clear_cerrojo (n_pant, xx - 1, yy);
			draw_coloured_tile_inv (VIEWPORT_X + xx + xx - 2, VIEWPORT_Y + yy + yy, 0);
			player.keys --;
			draw_keys ();
			peta_el_beeper (3);
		}
	}
	
	// Calculamos el frame que hay que poner:
 
	if (player.vy != 0) {
		if (player.facing == 0)
			player.next_frame = spr_7;
		else
			player.next_frame = spr_3;
	} else {
		if (player.vx == 0) {
			if (player.facing == 0)
				player.next_frame = spr_5;
			else
				player.next_frame = spr_1;
		} else {
			player.subframe ++;
			
			if (player.subframe == 4) {
				player.subframe = 0;
				player.frame = (player.frame + 1) & 3;
				step ();
			}
			if (player.facing == 0) {
				if (player.frame == 1 || player.frame == 2)
					player.next_frame = spr_5;
				else if (player.frame == 0)
					player.next_frame = spr_4;
				else if (player.frame == 3)
					player.next_frame = spr_6;
			} else {
				if (player.frame == 1 || player.frame == 2)
					player.next_frame = spr_1;
				else if (player.frame == 0)
					player.next_frame = spr_0;
				else if (player.frame == 3)
					player.next_frame = spr_2;
			}
		}	
	}
}

void init_player () {
	// Inicializa player con los valores iniciales
	// (de ah� lo de inicializar).
	
	player.x = 			PLAYER_INI_X << 10;
	player.y = 			PLAYER_INI_Y << 10;
	player.vy = 		0;
	player.g = 			PLAYER_G; 
	player.vx = 		0;
	player.ax = 		PLAYER_AX;
	player.rx = 		PLAYER_RX;
	player.salto = 		PLAYER_VY_INICIAL_SALTO;
	player.cont_salto = 1;
	player.saltando = 	0;
	player.frame = 		0;
	player.subframe = 	0;
	player.facing = 	1;
	player.estado = 	EST_NORMAL;
	player.life = 		PLAYER_LIFE;
	player.objs =		0;
	player.keys = 		0;
	
	pant_final = SCR_FIN;
}

void init_hotspots () {
	unsigned char i;
	for (i = 0; i < MAP_W * MAP_H; i ++)
		hotspots [i].act = 1;
}

void draw_scr (unsigned char n_pant) {
	// Desempaqueta y dibuja una pantalla, actualiza el array map_attr
	// y hace algunas otras cosillas m�s (cambiar sprites de enemigos/plataformas, etc)
	
	unsigned char x = 0, y = 0, i, d, t1, t2;
	unsigned int idx = n_pant * 75;
	unsigned char location;
	
	for (i = 0; i < 75; i ++) {
		location = 15 * (y >> 1) + (x >> 1);
		d = mapa [idx++];
		t1 = d >> 4;
		t2 = d & 15;
		map_attr [location] = comportamiento_tiles [t1];
		if ((rand () & 15) < 2 && t1 == 0 && map_buff [location - 16] == 0)
			t1 = 19;
		draw_coloured_tile (VIEWPORT_X + x, VIEWPORT_Y + y, t1);
		map_buff [location] = t1;
		x += 2;
		if (x == 30) {
			x = 0;
			y += 2;
		}
		location ++;
		map_attr [location] = comportamiento_tiles [t2];
		if ((rand () & 15) < 2 && t2 == 0 && map_buff [location - 16] == 0)
			t2 = 19;
		draw_coloured_tile (VIEWPORT_X + x, VIEWPORT_Y + y, t2);
		map_buff [location] = t2;
		x += 2;
		if (x == 30) {
			x = 0;
			y += 2;
		}
	}
	
	// �Hay objeto en esta pantalla?
	
	hotspot_x = hotspot_y = 240;
	if (hotspots [n_pant].act == 1) {
		if (hotspots [n_pant].tipo != 0) {
			// Sacamos la posici�n a nivel de tiles del objeto
			x = (hotspots [n_pant].xy >> 4);
			y = (hotspots [n_pant].xy & 15);
			// Convertimos la posici�n almacenada en p�xels
			hotspot_x = x << 4;
			hotspot_y = y << 4;
			// Guardamos el tile que hay originalmente
			orig_tile = map_buff [15 * y + x];
			// Pintamos el incono del objeto
			draw_coloured_tile (VIEWPORT_X + x + x, VIEWPORT_Y + y + y, 16 + hotspots [n_pant].tipo);
		}
	} else if (hotspots [n_pant].act == 0) {
		// Aleatoriamente, ponemos una recarga de vida si no hay objeto activo.	
		if (rand () % 3 == 2) {
			// Sacamos la posici�n a nivel de tiles del objeto
			x = (hotspots [n_pant].xy >> 4);
			y = (hotspots [n_pant].xy & 15);
			// Convertimos la posici�n almacenada en p�xels
			hotspot_x = x << 4;
			hotspot_y = y << 4;
			// Guardamos el tile que hay originalmente
			orig_tile = map_buff [15 * y + x];
			// Pintamos el incono del objeto
			draw_coloured_tile (VIEWPORT_X + x + x, VIEWPORT_Y + y + y, 16);	
		}
	}
	
	// Borramos los cerrojos abiertos
	for (i = 0; i < MAX_CERROJOS; i ++) {
		if (cerrojos [i].np == n_pant && !cerrojos [i].st) {
			draw_coloured_tile (VIEWPORT_X + cerrojos [i].x + cerrojos [i].x, VIEWPORT_Y + cerrojos [i].y + cerrojos [i].y, 0);
			location = 15 * cerrojos [i].y + cerrojos [i].x;
			map_attr [location] = 0;
			map_buff [location] = 0;
		}
	}
	
	// Movemos y cambiamos a los enemigos seg�n el tipo que tengan
	enoffs = n_pant * 3;
	
	for (i = 0; i < 3; i ++) {
		en_an [i].frame = 0;
		en_an [i].count = 0;
		switch (malotes [enoffs + i].t) {
			case 0:
				//sp_MoveSprAbs (sp_moviles [i], spritesClip, 0, 22, 0, 0, 0);
				break;
			case 1:
				en_an [i].next_frame = spr_8;
				break;
			case 2:
				en_an [i].next_frame = spr_10;
				break;
			case 3:
				en_an [i].next_frame = spr_12;
				break;
			case 4:
				en_an [i].next_frame = spr_14;
		}
	}

	cpc_ResetTouchedTiles ();	
	cpc_ShowTileMap (0);
	if (n_pant < 20)
		set_inks (0);
	else
		set_inks (1);

}

void mueve_bicharracos () {
	// Vamos a mover un frame todos los bicharracos activos.
	
	unsigned char i, enoffsmasi, x, y, xx, yy;
	player.gotten = 0;
	
	for (i = 0; i < 3; i ++) {
		enoffsmasi = enoffs + i;
		if (malotes [enoffsmasi].t != 0) {
			malotes [enoffsmasi].x += malotes [enoffsmasi].mx;
			malotes [enoffsmasi].y += malotes [enoffsmasi].my;
						
			en_an [i].count ++; 
			if (en_an [i].count == 4) {
				en_an [i].count = 0;
				en_an [i].frame = !en_an [i].frame;
				
				switch (malotes [enoffsmasi].t) {
					case 1:
						en_an [i].next_frame = en_an [i].frame ? spr_8 : spr_9;
						break;
					case 2:
						en_an [i].next_frame = en_an [i].frame ? spr_10 : spr_11;
						break;
					case 3:
						en_an [i].next_frame = en_an [i].frame ? spr_12 : spr_13;
						break;
					case 4:
						en_an [i].next_frame = en_an [i].frame ? spr_14 : spr_15;
				}	
			}
			
			// Arrastrar plataforma:
			x = player.x >> 6;
			y = player.y >> 6;
			if (malotes [enoffsmasi].t == 4) {
				xx = player.x >> 10;
				// Vertical
				if (malotes [enoffsmasi].my < 0) {
					// Subir.
					if (x >= malotes [enoffsmasi].x - 15 && x <= malotes [enoffsmasi].x + 15 && y >= malotes [enoffsmasi].y - 16 && y <= malotes [enoffsmasi].y - 11 && player.vy >= -(PLAYER_INCR_SALTO)) {
						player.gotten = 1;
						player.y = (malotes [enoffsmasi].y - 16) << 6;
						player.vy = 0;						
						yy = player.y >> 10;
						// No nos estaremos metiendo en un tile �no?
						if (player.y > 1024)
							if (attr (xx, yy) > 1 || ((x & 15) != 0 && attr (xx + 1, yy) > 1)) {
								// ajustamos:
								player.y = (yy + 1) << 10;
							}
					}
				} else if (malotes [enoffsmasi].my > 0) {
					// bajar
					if (x >= malotes [enoffsmasi].x - 15 && x <= malotes [enoffsmasi].x + 15 && y >= malotes [enoffsmasi].y - 20 && y <= malotes [enoffsmasi].y - 14 && player.vy >= 0) {
						player.gotten = 1;
						player.y = (malotes [enoffsmasi].y - 16) << 6;
						player.vy = 0;
						yy = player.y >> 10;
						// No nos estaremos metiendo en un tile �no?
						if (player.y < 9216)
							if (attr (xx, yy + 1) > 1 || ((x & 15) != 0 && attr (xx + 1, yy + 1) > 1)) {
								// ajustamos:
								player.y = yy << 10;
							}
					}
				}
				y = player.y >> 6;
				yy = player.y >> 10;
				// Horizontal
				if (malotes [enoffsmasi].mx != 0 && x >= malotes [enoffsmasi].x - 15 && x <= malotes [enoffsmasi].x + 15 && y >= malotes [enoffsmasi].y - 16 && y <= malotes [enoffsmasi].y - 11 && player.vy >= 0) {
					player.gotten = 1;
					player.y = (malotes [enoffsmasi].y - 16) << 6;
					yy = player.y >> 10;
					x = x + malotes [enoffsmasi].mx;
					player.x = x << 6;
					xx = player.x >> 10;
					if (malotes [enoffsmasi].mx < 0) {
						if (attr (xx, yy) > 1 || ((y & 15) != 0 && attr (xx, yy + 1) > 1)) {
							// paramos y ajustamos:
							player.vx = 0;
							player.x = (xx + 1) << 10;
						}
					} else if (malotes [enoffsmasi].mx > 0) {
						if (attr (xx + 1, yy) > 1 || ((y & 15) != 0 && attr (xx + 1, yy + 1) > 1)) {
							// paramos y ajustamos:
							player.vx = 0;
							player.x = xx << 10;
						}
					}					
				}
			// Colisi�n matadora
			} else if (malotes [enoffsmasi].x >= x - 15 && malotes [enoffsmasi].x <= x + 15 && malotes [enoffsmasi].y >= y - 15 && malotes [enoffsmasi].y <= y + 15) {
				if (player.life > 0) {
					peta_el_beeper (1);
					player.life --;	
					draw_life ();
					// Repulsi�n: Empuja en la direcci�n mx, my del movimiento del malote
					// incrementando vy con PLAYER_MAX_VX con el signo correcto.
					if (malotes [enoffsmasi].mx > 0) player.vx = (PLAYER_MAX_VX + PLAYER_MAX_VX);
					if (malotes [enoffsmasi].mx < 0) player.vx = -(PLAYER_MAX_VX + PLAYER_MAX_VX);
					if (malotes [enoffsmasi].my > 0) player.vy = (PLAYER_MAX_VX + PLAYER_MAX_VX);
					if (malotes [enoffsmasi].my < 0) player.vy = -(PLAYER_MAX_VX + PLAYER_MAX_VX);
				}
			}
			
			// L�mites de trayectoria.
			
			if (malotes [enoffsmasi].x == malotes [enoffsmasi].x1 || malotes [enoffsmasi].x == malotes [enoffsmasi].x2)
				malotes [enoffsmasi].mx = -malotes [enoffsmasi].mx;
			if (malotes [enoffsmasi].y == malotes [enoffsmasi].y1 || malotes [enoffsmasi].y == malotes [enoffsmasi].y2)
				malotes [enoffsmasi].my = -malotes [enoffsmasi].my;
		}
	}
}
