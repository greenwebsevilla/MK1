// MTE MK1 (la Churrera) v3.99.99 (final)
// Copyleft 2010-2017 by the Mojon Twins

// mainloop.h
// Churrera copyleft 2011 by The Mojon Twins.

void saca_a_todo_el_mundo_de_aqui (void) {
	sp_MoveSprAbs (sp_player, spritesClip, 0, VIEWPORT_Y + 30, VIEWPORT_X + 20, 0, 0);				
	for (gpit = 0; gpit < 3; gpit ++)
		sp_MoveSprAbs (sp_moviles [gpit], spritesClip, 0, VIEWPORT_Y + 30, VIEWPORT_X + 20, 0, 0);
	#ifdef PLAYER_CAN_FIRE
		for (gpit = 0; gpit < MAX_BULLETS; gpit ++)
			sp_MoveSprAbs (sp_bullets [gpit], spritesClip, 0, -2, -2, 0, 0);
	#endif
}

void main (void) {

	// Install ISR
	
	#asm
		di
	#endasm
	
	#ifdef MODE_128K
		sp_InitIM2(0xf1f1);
		sp_CreateGenericISR(0xf1f1);
		sp_RegisterHook(255, ISR);
		
		#asm
			ei
		#endasm

		wyz_init ();
	#endif

	cortina ();
	
	// splib2 initialization
	sp_Initialize (7, 0);
	sp_Border (BLACK);
	sp_AddMemory(0, NUMBLOCKS, 14, AD_FREE);
	
	// Load tileset
	allpurposepuntero = tileset;
	for (itj = 0; itj < 256; itj++) {
		sp_TileArray (itj, allpurposepuntero);
		allpurposepuntero += 8;
	}

	// Clipping rectangle
	spritesClipValues.row_coord = VIEWPORT_Y;
	spritesClipValues.col_coord = VIEWPORT_X;
	spritesClipValues.height = 20;
	spritesClipValues.width = 30;
	spritesClip = &spritesClipValues;
	
	// Sprite creation
	#ifdef NO_MASKS
		sp_player = sp_CreateSpr (sp_OR_SPRITE, 3, sprite_2_a, 1, TRANSPARENT);
		sp_AddColSpr (sp_player, sprite_2_b, TRANSPARENT);
		sp_AddColSpr (sp_player, sprite_2_c, TRANSPARENT);
		pcurrent_frame = pnext_frame = sprite_2_a;
		
		for (gpit = 0; gpit < 3; gpit ++) {
			sp_moviles [gpit] = sp_CreateSpr(sp_OR_SPRITE, 3, sprite_9_a, 1, TRANSPARENT);
			sp_AddColSpr (sp_moviles [gpit], sprite_9_b, TRANSPARENT);
			sp_AddColSpr (sp_moviles [gpit], sprite_9_c, TRANSPARENT);	
			en_an_current_frame [gpit] = sprite_9_a;
		}
	#else
		sp_player = sp_CreateSpr (sp_MASK_SPRITE, 3, sprite_2_a, 1, TRANSPARENT);
		sp_AddColSpr (sp_player, sprite_2_b, TRANSPARENT);
		sp_AddColSpr (sp_player, sprite_2_c, TRANSPARENT);
		pcurrent_frame = pnext_frame = sprite_2_a;
		
		for (gpit = 0; gpit < 3; gpit ++) {
			sp_moviles [gpit] = sp_CreateSpr(sp_MASK_SPRITE, 3, sprite_9_a, 2, TRANSPARENT);
			sp_AddColSpr (sp_moviles [gpit], sprite_9_b, TRANSPARENT);
			sp_AddColSpr (sp_moviles [gpit], sprite_9_c, TRANSPARENT);	
			en_an_current_frame [gpit] = en_an_next_frame [gpit] = sprite_9_a;
		}
	#endif

	#ifdef PLAYER_CAN_FIRE
		for (gpit = 0; gpit < MAX_BULLETS; gpit ++) {
			#ifdef MASKED_BULLETS
				sp_bullets [gpit] = sp_CreateSpr (sp_MASK_SPRITE, 2, sprite_19_a, 1, TRANSPARENT);
			#else		
				sp_bullets [gpit] = sp_CreateSpr (sp_OR_SPRITE, 2, sprite_19_a, 1, TRANSPARENT);
			#endif
			sp_AddColSpr (sp_bullets [gpit], sprite_19_b, TRANSPARENT);
		}
	#endif

	while (1) {
		// Here the title screen
		sp_UpdateNow();
		blackout ();
		#ifdef MODE_128K
			// Resource 0 = title.bin
			get_resource (0, 16384);
		#else		
			#asm
				ld hl, _s_title
				ld de, 16384
				call depack
			#endasm
		#endif
		#ifdef MODE_128K
			//wyz_play_music (0);
		#endif
		select_joyfunc ();
		#ifdef MODE_128K
			//wyz_stop_sound ();
		#endif

		#ifdef ENABLE_CHECKPOINTS
			sg_submenu ();
		#endif

		#ifdef COMPRESSED_LEVELS
			mlplaying = 1;
			#ifdef ENABLE_CHECKPOINTS
				if (sg_do_load) level = sg_level; else level = 0;
			#else
				level = 0;
			#endif				
			#ifndef REFILL_ME
				plife = PLAYER_LIFE;
			#endif
			while (mlplaying) {
				prepare_level (level);			
				blackout_area ();

				level_str [7] = 49 + level;
				print_str (12, 12, 71, level_str);
				sp_UpdateNow ();
				#ifdef MODE_128K
					wyz_play_sound (3);
				#else			
					peta_el_beeper (1);
				#endif

				espera_activa (100);
		#endif
					
		#ifndef DIRECT_TO_PLAY
			// Clear screen and show game frame
			cortina ();
			sp_UpdateNow();
			#ifdef MODE_128K
				// Resource 1 = marco.bin
				get_resource (1, 16384);
			#else		
				#asm
					ld hl, _s_title
					ld de, 16384
					call depack
				#endasm
			#endif
		#endif

		// Let's do it.

		playing = 1;
		init_player ();

		#ifndef COMPRESSED_LEVELS		
			init_hotspots ();
		#endif

		#ifndef COMPRESSED_LEVELS		
			#ifndef DEACTIVATE_KEYS
				init_cerrojos ();
			#endif
		#endif

		#if defined(PLAYER_KILLS_ENEMIES) || defined (PLAYER_CAN_FIRE)
			#ifndef COMPRESSED_LEVELS
				init_malotes ();
			#endif
		#endif

		#ifdef PLAYER_CAN_FIRE
			init_bullets ();
		#endif	

		#ifndef COMPRESSED_LEVELS	
			n_pant = SCR_INICIO;
		#endif		

		maincounter = 0;
		
		#ifdef ACTIVATE_SCRIPTING		
			script_result = 0;
			msc_init_all ();
		#endif

		#ifdef ACTIVATE_SCRIPTING
			// Entering game
			script = e_scripts [MAP_W * MAP_H];
			run_script ();
		#endif
				
		#ifdef PLAYER_KILLS_ENEMIES 	
			#ifdef SHOW_TOTAL
			// Show total of enemies next to the killed amount.

			sp_PrintAtInv (KILLED_Y, 2 + KILLED_X, 71, 15);
			sp_PrintAtInv (KILLED_Y, 3 + KILLED_X, 71, 16 + BADDIES_COUNT / 10);
			sp_PrintAtInv (KILLED_Y, 4 + KILLED_X, 71, 16 + BADDIES_COUNT % 10);
			#endif
		#endif

		pkillme = success = half_life = 0;
			
		objs_old = keys_old = life_old = killed_old = 0xff;
		#ifdef MAX_AMMO 	
			ammo_old = 0xff;
		#endif
		#if defined(TIMER_ENABLE) && defined(PLAYER_SHOW_TIMER)
			timer_old = 0;
		#endif
		
		#ifdef PLAYER_CHECK_MAP_BOUNDARIES		
			#ifdef MODE_128K
				x_pant = n_pant % level_data->map_w;
				y_pant = n_pant / level_data->map_w;
			#else
				x_pant = n_pant % MAP_W; y_pant = n_pant / MAP_W;
			#endif
		#endif

		#ifdef ENABLE_CHECKPOINTS
			if (sg_do_load) {
				mem_load ();
			}
		#endif		

		#ifdef MODE_128K
				// Play music
			#ifdef COMPRESSED_LEVELS		
				//wyz_play_music (levels [level].music_id);
			#else
				//wyz_play_music (1);
			#endif		
		#endif

		#ifdef MSC_MAXITEMS
			display_items ();
		#endif

		o_pant = 0xff;
		while (playing) {
			pad0 = (joyfunc) (&keys);

			if (o_pant != n_pant) {
				draw_scr (); 
				o_pant = n_pant;
			}
			
			#ifdef TIMER_ENABLE
				// Timer
				if (ctimer.on) {
					ctimer.count ++;
					if (ctimer.count == ctimer.frames) {
						ctimer.count = 0;
						ctimer.t --;
						if (ctimer.t == 0) ctimer.zero = 1;
					}
				}

				#if defined(TIMER_SCRIPT_0) && defined(ACTIVATE_SCRIPTING)
					if (ctimer.zero) {
						ctimer.zero = 0;
						#ifdef SHOW_TIMER_OVER
							saca_a_todo_el_mundo_de_aqui ();
							time_over ();
						#endif
						script = e_scripts [MAP_W * MAP_H + 2];
						run_script ();	
					}	
				#endif

				#ifdef TIMER_KILL_0
					if (ctimer.zero) {
						#ifdef SHOW_TIMER_OVER
							#ifndef TIMER_SCRIPT_0
								saca_a_todo_el_mundo_de_aqui ();
								time_over ();
							#endif
						#endif				
						ctimer.zero = 0;
						#ifdef TIMER_AUTO_RESET 			
							ctimer.t = TIMER_INITIAL;
						#endif
						
						#ifdef MODE_128K
							pkillme = 7;
						#else
							pkillme = 4;
						#endif

						#ifdef PLAYER_FLICKERS
							pestado = EST_PARP;
							pct_estado = 50;
						#endif

						#if defined(TIMER_WARP_TO_X) && defined(TIMER_WARP_TO_Y)
							px = TIMER_WARP_TO_X << 10;
							py = TIMER_WARP_TO_Y << 10;
						#endif

						#ifdef TIMER_WARP_TO
							n_pant = TIMER_WARP_TO;
							draw_scr ();
						#endif
					}
				#endif
			#endif

			#ifndef DEACTIVATE_OBJECTS
				if (pobjs != objs_old) {
					draw_objs ();
					objs_old = pobjs;
				}
			#endif
			
			if (plife != life_old) {
				print_number2 (LIFE_X, LIFE_Y, plife);
				life_old = plife;
			}
			
			#ifndef DEACTIVATE_KEYS
				if (pkeys != keys_old) {
					print_number2 (KEYS_X, KEYS_Y, pkeys);
					keys_old = pkeys;
				}
			#endif

			#if defined(PLAYER_KILLS_ENEMIES) || defined(PLAYER_CAN_FIRE)
				#ifdef PLAYER_SHOW_KILLS
					if (pkilled != killed_old) {
						print_number2 (KILLED_X, KILLED_Y, pkilled);
						killed_old = pkilled; 
					}
				#endif
			#endif

			#ifdef MAX_AMMO 	
				if (pammo != ammo_old) {
					print_number2 (AMMO_X, AMMO_Y, pammo);
					ammo_old = pammo;
				}
			#endif

			#if defined(TIMER_ENABLE) && defined(PLAYER_SHOW_TIMER)
				if (ctimer.t != timer_old) {
					print_number2 (TIMER_X, TIMER_Y, ctimer.t);
					timer_old = ctimer.t;
				}
			#endif

			maincounter ++;
			half_life = !half_life;
			
			// Move player
			move ();
			
			// Move enemies
			mueve_bicharracos ();

			if (pkillme) kill_player (pkillme);

			#ifdef PLAYER_CAN_FIRE
				// Move bullets 			
				mueve_bullets ();
			#endif

			#ifdef ENABLE_TILANIMS
				do_tilanims ();
			#endif

			// Render		
			for (gpit = 0; gpit < 3; gpit ++) {
				#if defined (RANDOM_RESPAWN) || defined (ENABLE_CUSTOM_TYPE_6)
					if (en_an_fanty_activo [gpit] || malotes [enoffs + gpit].t == 6) {
						rdx = en_an_x [gpit] >> 6;
						rdy = en_an_y [gpit] >> 6;
					} else
				#endif
				{
					rdx = malotes [enoffs + gpit].x;
					rdy = malotes [enoffs + gpit].y;
				}

				sp_MoveSprAbs (sp_moviles [gpit], spritesClip, en_an_next_frame [gpit] - en_an_current_frame [gpit], VIEWPORT_Y + (rdy >> 3), VIEWPORT_X + (rdx >> 3), rdx & 7, rdy & 7);
				en_an_current_frame [gpit] = en_an_next_frame [gpit];
			}

					
			#ifdef ACTIVATE_SCRIPTING
				#ifdef ENABLE_FIRE_ZONE
					if (f_zone_ac == 1) {
						if (gpx >= fzx1 && gpx <= fzx2 && gpy >= fzy1 && gpy <= fzy2) {
							run_fire_script ();
						}	
					}
				#endif
			#endif			
			
			if ( !(pestado & EST_PARP) || !(half_life) )
				sp_MoveSprAbs (sp_player, spritesClip, pnext_frame - pcurrent_frame, VIEWPORT_Y + (gpy >> 3), VIEWPORT_X + (gpx >> 3), gpx & 7, gpy & 7);
			else
				sp_MoveSprAbs (sp_player, spritesClip, pnext_frame - pcurrent_frame, -2, -2, 0, 0);
			
			pcurrent_frame = pnext_frame;
			
			#ifdef PLAYER_CAN_FIRE
				for (gpit = 0; gpit < MAX_BULLETS; gpit ++) {
					if (bullets [gpit].estado == 1) {
						sp_MoveSprAbs (sp_bullets [gpit], spritesClip, 0, VIEWPORT_Y + (bullets [gpit].y >> 3), VIEWPORT_X + (bullets [gpit].x >> 3), bullets [gpit].x & 7, bullets [gpit].y & 7);
					} else {
						sp_MoveSprAbs (sp_bullets [gpit], spritesClip, 0, -2, -2, 0, 0);
					}
				}
			#endif			
			
			// Update to screen
			sp_UpdateNow();
			
			#ifdef PLAYER_CAN_FIRE
				for (gpit = 0; gpit < 3; gpit ++)
					if (en_an_morido [gpit] == 1) {
					#ifdef MODE_128K
						wyz_play_sound (7);
					#else
						peta_el_beeper (1);
					#endif
					en_an_morido [gpit] = 0;
				}	
			#endif

			#ifdef PLAYER_FLICKERS
				// Flickering
				if (pestado == EST_PARP) {
					pct_estado --;
					if (pct_estado == 0)
						pestado = EST_NORMAL; 
				}
			#endif			
			
			// Hotspot interaction.
			//if (x >= hotspot_x - 15 && x <= hotspot_x + 15 && y >= hotspot_y - 15 && y <= hotspot_y + 15) {
			if (collide (gpx, gpy, hotspot_x, hotspot_y)) {
				// Deactivate hotspot
				draw_coloured_tile (VIEWPORT_X + (hotspot_x >> 3), VIEWPORT_Y + (hotspot_y >> 3), orig_tile);
				gpit = 0;
				#ifndef USE_HOTSPOTS_TYPE_3
					// Was it an object, key or life boost?
					if (hotspots [n_pant].act == 0) {
						plife += PLAYER_REFILL;
						if (plife > PLAYER_LIFE)
							plife = PLAYER_LIFE;
						hotspots [n_pant].act = 2;
						#ifdef MODE_128K
							wyz_play_sound (5);
						#else
							peta_el_beeper (8);
						#endif
					} else {					
						switch (hotspots [n_pant].tipo) {
							#ifndef DEACTIVATE_OBJECTS						
								case 1:
									#ifdef ONLY_ONE_OBJECT
										if (pobjs == 0) {
											pobjs ++;
											#ifdef MODE_128K
												wyz_play_sound (3);
											#else
												peta_el_beeper (9);
											#endif 
										} else {
											#ifdef MODE_128K
												wyz_play_sound (5);
											#else
												peta_el_beeper (4); 
											#endif
											draw_coloured_tile (VIEWPORT_X + (hotspot_x >> 3), VIEWPORT_Y + (hotspot_y >> 3), 17);
											gpit = 1;
										}
									#else
										pobjs ++;
										#ifdef OBJECT_COUNT
											flags [OBJECT_COUNT] = pobjs;
										#endif
										#ifdef MODE_128K
											wyz_play_sound (3);
										#else
											peta_el_beeper (9);
										#endif
									#endif
									break;
							#endif

							#ifndef DEACTIVATE_KEYS
								case 2:
									pkeys ++;
									#ifdef MODE_128K
										wyz_play_sound (3);
									#else
										peta_el_beeper (7);
									#endif
									break;
							#endif

							#ifdef MAX_AMMO
								case 4:
									if (MAX_AMMO - pammo > AMMO_REFILL)
										pammo += AMMO_REFILL;
									else 
										pammo = MAX_AMMO;
									#ifdef MODE_128K
										wyz_play_sound (3);
									#else
										peta_el_beeper (9);
									#endif
									break;
							#endif

							#ifdef TIMER_ENABLE
								case 5:
									if (99 - ctimer.t > TIMER_REFILL)
										ctimer.t += TIMER_REFILL;
									else
										ctimer.t = 99;
									#ifdef MODE_128K
										wyz_play_sound (3);
									#else
										peta_el_beeper (7);
									#endif
									break;
							#endif

							#ifdef ENABLE_CHECKPOINTS
								case 6:
									mem_save ();
									#ifdef MODE_128K
										wyz_play_sound (3);
									#else
										peta_el_beeper (7);
									#endif
									break;						
							#endif
						}
						hotspots [n_pant].act = gpit;
					}
					hotspot_x = hotspot_y = 240;

				#else

					// Modificaci?n para que los hotspots de tipo 3 sean recargas directas
					// Was it an object, key or life boost?
					if (hotspots [n_pant].act) {
						hotspots [n_pant].act = 0;
						switch (hotspots [n_pant].tipo) {
							#ifndef DEACTIVATE_OBJECTS					   
								case 1:
									#ifdef ONLY_ONE_OBJECT
										if (pobjs == 0) {
											pobjs ++;
											#ifdef MODE_128K
												wyz_play_sound (3);
											#else
												peta_el_beeper (9);
											#endif
										} else {
											#ifdef MODE_128K
												wyz_play_sound (5);
											#else
												peta_el_beeper (4);
											#endif
											draw_coloured_tile (VIEWPORT_X + (hotspot_x >> 3), VIEWPORT_Y + (hotspot_y >> 3), 17);
											hotspots [n_pant].act = 1;
										}
									#else
										++ pobjs;
										#ifdef OBJECT_COUNT
											flags [OBJECT_COUNT] = pobjs;
										#endif

										#ifdef MODE_128K
											wyz_play_sound (5);
										#else
											peta_el_beeper (9);
										#endif

										#ifdef GET_X_MORE
											if (level_data.max_objs > pobjs) {
												print_str (10, 11, 79, spacer);
												getxmore [5] = '0' + level_data.max_objs - pobjs;
												print_str (10, 12, 79, getxmore);
												print_str (10, 13, 79, spacer);
												sp_UpdateNow ();
												sp_WaitForNoKey ();
												espera_activa (100);
												draw_scr_background ();
											}
										#endif							
									#endif
									break;
							#endif

							#ifndef DEACTIVATE_KEYS
								case 2:
									pkeys ++;
									#ifdef MODE_128K
										wyz_play_sound (3);
									#else
										peta_el_beeper (7);
									#endif
									break;
							#endif

							case 3:
								plife += PLAYER_REFILL;
								if (plife > PLAYER_LIFE)
									plife = PLAYER_LIFE;
								#ifdef MODE_128K
									wyz_play_sound (5);
								#else	
									peta_el_beeper (8);
								#endif
								break;

							#ifdef MAX_AMMO
								case 4:
									if (MAX_AMMO - pammo > AMMO_REFILL)
										pammo += AMMO_REFILL;
									else
										pammo = MAX_AMMO;
									#ifdef MODE_128K
										wyz_play_sound (3);
									#else
										peta_el_beeper (9);
									#endif
									break;
							#endif

							#ifdef TIMER_ENABLE
								case 5:
									if (99 - ctimer.t > TIMER_REFILL)
										ctimer.t += TIMER_REFILL;
									else
										ctimer.t = 99;
									#ifdef MODE_128K
										wyz_play_sound (3);
									#else
										peta_el_beeper (7);
									#endif
									break;
							#endif

							#ifdef ENABLE_CHECKPOINTS
								case 6:
									mem_save ();
									#ifdef MODE_128K
										wyz_play_sound (3);
									#else
										peta_el_beeper (7);
									#endif
									break;						
							#endif
						}
						
					}
					hotspot_x = hotspot_y = 240;

				#endif
			}

			// Flick screen checks and scripting related stuff
			
			#ifdef ACTIVATE_SCRIPTING

				// Select object
				#ifdef MSC_MAXITEMS
					if (sp_KeyPressed (KEY_Z)) {
						if (!key_z_pressed) {
							#ifdef MODE_128K
								wyz_play_sound (0);
							#else
								peta_el_beeper (2);
							#endif
							flags [FLAG_SLOT_SELECTED] = (flags [FLAG_SLOT_SELECTED] + 1) % MSC_MAXITEMS;
							display_items ();
						}
						key_z_pressed = 1;
					} else {
						key_z_pressed = 0;
					}
				#endif			


				#ifdef SCRIPTING_KEY_M			
					if (sp_KeyPressed (KEY_M))
				#endif
				#ifdef SCRIPTING_DOWN
					if ((pad0 & sp_DOWN) == 0)
				#endif
				#ifdef SCRIPTING_KEY_FIRE
					if ((pad0 & sp_FIRE) == 0)
				#endif
				{
					// Any scripts to run in this screen?
					run_fire_script ();
				}
			#endif

			#ifdef PAUSE_ABORT
				// Pause/Abort handling
				if (sp_KeyPressed (KEY_H)) {
					sp_WaitForNoKey ();
					#ifdef MODE_128K
						wyz_stop_sound ();
						wyz_play_sound (1);
					#endif				
					saca_a_todo_el_mundo_de_aqui ();
					pause_screen ();
					while (!sp_KeyPressed (key_h));
					sp_WaitForNoKey ();
					draw_scr ();
					#ifdef MODE_128K
						#ifdef COMPRESSED_LEVELS
							//wyz_play_music (levels [level].music_id);
						#else
							//wyz_play_music (1);
						#endif
					#endif				
				}			
				if (sp_KeyPressed (KEY_Y)) {
					playing = 0;
				}
			#endif

			// Change screen				
			#ifdef PLAYER_CHECK_MAP_BOUNDARIES		
				if (gpx == 0 && pvx < 0 && x_pant > 0) {
					n_pant --;
					x_pant --;
					gpx = 224
					px = 14336;
				}

				#if defined (MODE_128K) && defined (COMPRESSED_LEVELS)
					if (gpx == 224 && pvx > 0 && x_pant < (level_data->map_w - 1))
				#else			
					if (gpx == 224 && pvx > 0 && x_pant < (MAP_W - 1))
				#endif
				{
					n_pant ++;
					x_pant ++;					
					gpx = px = 0;
				}

				if (gpy == 0 && pvy < 0 && y_pant > 0) {
					#if defined (MODE_128K) && defined (COMPRESSED_LEVELS)
						n_pant -= level_data->map_w;
					#else				
						n_pant -= MAP_W;
					#endif
					y_pant --;					
					gpy = 144;
					py = 9216;	
				}

				#if defined (MODE_128K) && defined (COMPRESSED_LEVELS)
					if (gpy == 144 && pvy > 0 && y_pant < (level_data->map_h - 1)) {
						n_pant += level_data->map_w;
				#else			
					if (gpy == 144 && pvy > 0 && y_pant < (MAP_H - 1)) {
						n_pant += MAP_W;
				#endif
					y_pant ++;					
					gpy = py = 0;
					if (pvy > 256) pvy = 256;	
				}
			#else			
				#ifdef PLAYER_AUTO_CHANGE_SCREEN
					if (gpx == 0 && pvx < 0) {
						n_pant --;
						gpx = 224; px = 14336;
					}
					if (gpx == 224 && pvx > 0) {
						n_pant ++;
						gpx = px = 0;
					}
				#else
					if (gpx == 0 && ((pad0 & sp_LEFT) == 0)) {
						n_pant --;
						gpx = 224; px = 14336;
					}
					if (gpx == 224 && ((pad0 & sp_RIGHT) == 0)) {		// 14336 = 224 * 64
						n_pant ++;
						gpx = px = 0;
					}
				#endif

				#if defined (MODE_128K) && defined (COMPRESSED_LEVELS)
					if (gpy == 0 && pvy < 0 && n_pant >= level_data->map_w) {
						n_pant -= level_data->map_w;
				#else
					if (gpy == 0 && pvy < 0 && n_pant >= MAP_W) {
						n_pant -= MAP_W;
				#endif
					gpy = 144;
					py = 9216;	
				}

				if (gpy == 144 && pvy > 0) {				// 9216 = 144 * 64
					#if defined (MODE_128K) && defined (COMPRESSED_LEVELS)
						if (n_pant < level_data->map_w * (level_data->map_h - 1)) {
							n_pant += level_data->map_w;
					#else
						if (n_pant < MAP_W * MAP_H - MAP_W) {
							n_pant += MAP_W;
					#endif				
						gpy = py = 0;
						if (pvy > 256) pvy = 256;
					}
					#ifdef MAP_BOTTOM_KILLS
						else {
							pvy = -PLAYER_MAX_VY_CAYENDO; 
							{
								#ifdef MODE_128K
									pkillme = 1;
								#else
									pkillme = 4;
								#endif
							}
						}
					#endif
				}
			#endif			

			// Win game condition
			
			#ifdef ACTIVATE_SCRIPTING
				if (pobjs == PLAYER_NUM_OBJETOS || script_result == 1)
			#else			
				if (pobjs == PLAYER_NUM_OBJETOS)
			#endif
			{
				if (
					(n_pant == pant_final && ((px >> 10) == PLAYER_FIN_X && (py >> 10) == PLAYER_FIN_Y)) ||
					pant_final == 99
				) {
					success = 1;
					playing = 0;					
				}
			}
			
			// Game over condition
			if (plife == 0
				#ifdef ACTIVATE_SCRIPTING
					|| script_result == 2
				#endif
				#if defined(TIMER_ENABLE) && defined(TIMER_GAMEOVER_0)
					|| ctimer.zero
				#endif
			) {
				playing = 0;				
			}
		}
		sp_WaitForNoKey ();
		
		#ifdef MODE_128K		
			wyz_stop_sound ();
		#endif

		saca_a_todo_el_mundo_de_aqui ();
		sp_UpdateNow ();
		
		#ifdef COMPRESSED_LEVELS
			if (success) {
				/*
				wyz_play_music (6);
				print_str (10, 11, 79, spacer);
				print_str (10, 12, 79, " ZONE CLEAR ");
				print_str (10, 13, 79, spacer);
				sp_UpdateNow ();
				sp_WaitForNoKey ();
				espera_activa (250);			
				*/
				level ++;
				if (level == MAX_LEVELS) {
					game_ending ();
					mlplaying = 0;
				}
			} else {
				#ifdef MODE_128K
					//wyz_play_music (8);
				#endif
				#if defined(TIMER_ENABLE) && defined(TIMER_GAMEOVER_0) && defined(SHOW_TIMER_OVER)
					if (ctimer.zero) time_over (); else game_over ();
				#else
					game_over ();
				#endif
				mlplaying = 0;
				#ifdef MODE_128K
					wyz_stop_sound ();
				#endif
			}
		}
		cortina ();
		#else
			if (success) {
				game_ending (); 
			} else {
				//wyz_play_music (8);
				game_over ();
			}
			cortina ();
		#endif
	}
}
