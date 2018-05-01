#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "SDL2/SDL.h"

/**
 * D�finition des types
 */
typedef struct window {
	SDL_Window	*	sdlwindow;
	SDL_Renderer	*	renderer;
	SDL_Texture	*	texture;
	unsigned char	*	framebuffer;
	int			width;
	int			height;
	int			bpp;
	int			pitch;
}window_t;

/**
 * D�finition des prototypes de fonctions
 */

/**
 * Initialise et ouvre une nouvelle fen�tre
 */
window_t	*	WindowInit		( int width, int height, int bpp );

/**
 * Ferme et detruit une f�netre
 */
void			WindowDestroy		( window_t * w );

/**
 * Efface une fen�tre avec la couleur souha�t�e
 */
void			WindowDrawClearColor	( window_t * w, unsigned char r, unsigned char g, unsigned char b );

/**
 * Met � jour le contenu de la fen�tre
 */
void			WindowUpdate		( window_t * w );

/**
 * Dessine un point color� dans la fen�tre
 */
void			WindowDrawPoint		( window_t * w, int x, int y, Uint8 r, Uint8 g, Uint8 b );

/**
 * Dessine une ligne color�e dans la fen�tre
 */
void			WindowDrawLine		( window_t * w, int x0, int y0, int x1, int y1, Uint8 r, Uint8 g, Uint8 b );

void      WindowDrawTriangle( window_t * w, int x0, int y0, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b );
#endif //__WINDOW_H__
