; ******************************************************
; **       Librer�a de rutinas para Amstrad CPC       **
; **	   Ra�l Simarro, 	  Artaburu 2007           **
; ******************************************************

; [na_th_an] Modificada por Mojon Twins - eliminamos la "cabecera" de los sprites.
; [na_th_an] Gracias a Fran Gallego y al c�digo de la CPCTelera, rescrito para usar LUTs
; [na_th_an] Esta nueva vesi�n de la rutina imprime al pixel en m1
; Necesita una LUT en $FE00

XLIB cpc_PutTrSp8x16TileMap2bPxM1

XREF tiles_tocados
XREF pantalla_juego		
XREF posiciones_super_buffer
XREF ancho_pantalla_bytes 
XREF posicion_inicial_superbuffer

.cpc_PutTrSp8x16TileMap2bPxM1

	;seg�n las coordenadas x,y que tenga el sprite, se dibuja en el buffer 
    ex de,hl	;4
    LD IXH,d	;9
    LD IXL,e	;9 

	;lo cambio para la rutina de multiplicar 
    ld  e, (ix+8)		; X - Cuidado ! va a pixel (0-255)
    ld  a, e 			; Save for later!
    srl e 				; X a bytes
    ld  h, (ix+9)		; Y

.pasa_bloque_a_sbuffer

.transferir_sprite
	ld  d, 0

	; Multiplico HL = H * 64, offset "Y"
	ld  l, d
	srl h
	rr  l
	srl h
	rr  l
	
	add hl, de 		; sumo offset "X"

	ld  de, posicion_inicial_superbuffer
	add hl,de

	;hl apunta a la posici�n en buffer (destino)
			
	ld e,(ix+0)
    ld d,(ix+1)	;HL apunta al sprite
	
.sp_buffer_mask
	ld ixh,16
	ex de,hl 		; de -> bg
	ld b, h
	ld c, l 		; bc -> sprite

	; Remember A = X in pixels
	; Select routine based upon number of rotations
	; would write self-modifying code for short but too complicated
	and 3
	jr  z, loop_alto_map_sbuffer	
	cp  1
	jr  z, loop_alto_map_sbuffer_shift1
	cp  2
	jp  z, loop_alto_map_sbuffer_shift2
	jp  loop_alto_map_sbuffer_shift3

.loop_alto_map_sbuffer
	ld h, $FE		; hl -> LUT

	; El ancho est� desenrollado: Hay que procesar y copiar 4 bytes.

	ld a, (bc) 		; Get sprite
	ld l, a 		; copy to L to index LUT
	ld a, (de) 		; Get bg
	and (hl) 		; make a hole
	or l  			; draw pixels
	ld (de), a 		; save BG
	inc de
	inc bc

	ld a, (bc) 		; Get sprite
	ld l, a 		; copy to L to index LUT
	ld a, (de) 		; Get bg
	and (hl) 		; make a hole
	or l  			; draw pixels
	ld (de), a 		; save BG
	inc de
	inc bc

	ld a, (bc) 		; Get sprite
	ld l, a 		; copy to L to index LUT
	ld a, (de) 		; Get bg
	and (hl) 		; make a hole
	or l  			; draw pixels
	ld (de), a 		; save BG
	inc de
	inc bc

	ld a, (bc) 		; Get sprite
	ld l, a 		; copy to L to index LUT
	ld a, (de) 		; Get bg
	and (hl) 		; make a hole
	or l  			; draw pixels
	ld (de), a 		; save BG
	inc de
	inc bc
	
	;*************************************************		
		
	dec ixh
	ret z

	; de += 60 (next line in bg)
	ld hl, 60
	add hl, de
	ex de, hl

	jp loop_alto_map_sbuffer

.loop_alto_map_sbuffer_shift1

	; El ancho est� desenrollado. Procesamos 4 bytes que copiamos en 5:
	; 1: -> A'A; byte 1 = A'A AND 0x77 = 0A
	; 2: -> B'B; byte 2 = (B'B AND 0x77) OR (A'A AND 0x88) = A'B
	; 3: -> C'C; byte 3 = (C'C AND 0x77) OR (B'B AND 0x88) = B'C
	; 4: -> D'D; byte 4 = (D'D AND 0x77) OR (C'C AND 0x88) = C'D
	; 5:       ; byte 5 = D'D AND 0x88  

	jp loop_alto_map_sbuffer

.loop_alto_map_sbuffer_shift2

	; El ancho est� desenrollado. Procesamos 4 bytes que copiamos en 5:
	; 1: -> A'A; byte 1 = A'A AND 0x33 = 0A
	; 2: -> B'B; byte 2 = (B'B AND 0x33) OR (A'A AND 0xCC) = A'B
	; 3: -> C'C; byte 3 = (C'C AND 0x33) OR (B'B AND 0xCC) = B'C
	; 4: -> D'D; byte 4 = (D'D AND 0x33) OR (C'C AND 0xCC) = C'D
	; 5:       ; byte 5 = D'D AND 0xCC  

	jp loop_alto_map_sbuffer

.loop_alto_map_sbuffer_shift3

	; El ancho est� desenrollado. Procesamos 4 bytes que copiamos en 5:
	; 1: -> A'A; byte 1 = A'A AND 0x11 = 0A
	; 2: -> B'B; byte 2 = (B'B AND 0x11) OR (A'A AND 0xEE) = A'B
	; 3: -> C'C; byte 3 = (C'C AND 0x11) OR (B'B AND 0xEE) = B'C
	; 4: -> D'D; byte 4 = (D'D AND 0x11) OR (C'C AND 0xEE) = C'D
	; 5:       ; byte 5 = D'D AND 0xEE  

	jp loop_alto_map_sbuffer



	jp loop_alto_map_sbuffer_shifted

