// Config.h
// Generado por GameConfig de la churrera
// Copyleft 2010 The Mojon Twins

// ***********************************************
// Datos sobre el movimiento del jugador principal
// ***********************************************

// Movimiento vertical: 
// ====================
//
// 1.- PLAYER_G siempre se suma a la velocidad vertical del jugador y le hace caer.
// 2.- Cuando se pulsa la tecla de salto, primero se hace que la velocidad vertical del juga-
//     dor sea PLAYER_VY_INICIAL_SALTO, y en cada frame mientras se sigue pulsando la tecla
//     se le suma PLAYER_INCR_SALTO hasta que se alcanza PLAYER_MAX_VY_SALTANDO.

#define PLAYER_MAX_VY_CAYENDO	512		// Velocidad m�xima cuando cae (512/64 = 8 p�xels/frame)
#define PLAYER_G				32		// Aceleraci�n de la gravedad (32/64 = 0.5 p�xeles/frame^2)

#define PLAYER_VY_INICIAL_SALTO	64		// Velocidad inicial al saltar (64/64 = 1 p�xel/frame)
#define PLAYER_MAX_VY_SALTANDO	320		// Velocidad m�xima al saltar (320/64 = 5 p�xels/frame)
#define PLAYER_INCR_SALTO 		48		// Aceleraci�n al pulsar "salto" (48/64 = 0.75 p�xeles/frame^2)

// Movimiento horizontal:
// ======================
//
// 1.- Si el jugador pulsa IZQ o DER, la velocidad horizontal del jugador se va incrementando
//     seg�n PLAYER_AX hasta que se llega a PLAYER_MAX_VX, a partir de entonces sigue constante.
// 2.- Si no se pulsa IZQ o DER, la velocidad horizontal del jugador se va decrementando seg�n
//     PLAYER_RX hasta que se llega a 0.

#define PLAYER_MAX_VX			192		// Velocidad m�xima horizontal (192/64 = 3 p�xels/frame)
#define PLAYER_AX				24		// Aceleraci�n horizontal (24/64 = 0,375 p�xels/frame^2)
#define PLAYER_RX				32		// Fricci�n horizontal (32/64 = 0,5 p�xels/frame^2)

// Valores generales
// =================

#define PLAYER_LIFE				128		// Vida m�xima (con la que empieza, adem�s)
#define PLAYER_REFILL			10		// Recarga de vida.

// ****************************************
// Datos sobre la configuraci�n de pantalla
// ****************************************

#define VIEWPORT_X				0		// En CPC SIEMPRE 0
#define VIEWPORT_Y				0		// Posici�n de la ventana de juego (en car�cteres)
#define LIFE_X					7		//
#define LIFE_Y					1		// Posici�n del marcador de vida (en car�cteres)
#define OBJECTS_X				18		//
#define OBJECTS_Y				1		// Posici�n del marcador de objetos (en car�cteres)
#define KEYS_X					28		//
#define KEYS_Y					1		// Posici�n del marcador de llaves (en car�cteres)

// *************************************
// Datos sobre la configuraci�n del mapa
// *************************************

#define MAP_W 					20		//
#define MAP_H					2		// Dimensiones del mapa, en pantallas.
#define SCR_INICIO				0		// Pantalla de inicio
#define PLAYER_INI_X			1		//
#define PLAYER_INI_Y			1		// Coordenadas de inicio del jugador, a nivel de tiles
#define SCR_FIN					99		// Pantalla del final. 99 si da igual.
#define PLAYER_FIN_X			99		//
#define PLAYER_FIN_Y			99		// Posici�n del jugador para terminar, a nivel de tiles
#define PLAYER_NUM_OBJETOS		24		// N�mero de objetos para terminar el juego

// ****************************
// Comportamiento de cada tile.
// ****************************

// Indica el comportamiento de cada uno de los 16 tiles. Recuerda que el tile n� 15 (el �ltimo)
// es el cerrojo y debe ser un obst�culo, porque si no, vaya mierda de cerrojo.

// 0 = traspasable.
// 1 = traspasable y mata.
// 2 = obst�culo

unsigned char comportamiento_tiles [] = {
0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 1, 2, 2
};
