
; Tabla de instrumentos
TABLA_PAUTAS: DW PAUTA_0,0,PAUTA_2,PAUTA_3,PAUTA_4,PAUTA_5

; Tabla de efectos
TABLA_SONIDOS: DW SONIDO0,SONIDO1,SONIDO2,SONIDO3

;Pautas (instrumentos)
;Instrumento 'Piano'
PAUTA_0:	DB	8,0,8,0,7,0,7,0,6,0,6,0,5,0,4,0,3,0,2,0,131
;Instrumento 'Bajo'
PAUTA_2:	DB	12,0,11,0,11,0,10,0,9,0,9,0,8,0,8,0,7,0,7,0,6,0,6,0,130
;Instrumento 'Glam'
PAUTA_3:	DB	8,0,74,0,11,0,42,0,9,0,8,0,71,0,7,0,39,0,7,0,132
;Instrumento 'Chip-low'
PAUTA_4:	DB	6,0,73,0,10,0,42,0,9,0,72,0,6,0,38,0,6,0,70,0,132
;Instrumento 'Wave'
PAUTA_5:	DB	8,0,10,0,10,0,10,0,10,0,9,0,8,0,7,0,7,0,6,0,5,0,4,0,3,0,129

;Efectos
;Efecto 'Bass drum 1'
SONIDO0:	DB	26,78,0,169,108,0,255
;Efecto 'Drum'
SONIDO1:	DB	231,45,0,217,44,1,100,42,2,184,24,3,255
;Efecto 'Tambourine'
SONIDO2:	DB	29,9,2,255
;Efecto 'Bass drum 2'
SONIDO3:	DB	238,59,0,126,104,0,211,134,2,255

;Frecuencias para las notas
DATOS_NOTAS: DW 0,0
DW 1711,1614,1524,1438,1358,1281,1210,1142,1078,1017
DW 960,906,855,807,762,719,679,641,605,571
DW 539,509,480,453,428,404,381,360,339,320
DW 302,285,269,254,240,227,214,202,190,180
DW 170,160,151,143,135,127,120,113,107,101
DW 95,90,85,80,76,71,67,64,60,57
