#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "SDL2/SDL.h"

#include "geometry.h"

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
 * Dessine une ligne color�e dans la fen�tre avec l'algorithme de Bresenham
 */
void  WindowDrawLineB		( window_t * w, int x0, int y0, int x1, int y1, Uint8 r, Uint8 g, Uint8 b );

/**
 * Dessine une ligne horizontal color�e dans la fen�tre
 */
void	WindowDrawLine		( window_t * w, float *zbuff, int x0, int y0, float z0, int x1, int y1, float z1, float intens, int tx0, int ty0, int tx1, int ty1, vec3f_t *norm0, vec3f_t *norm1, unsigned char *Texture, int imgwidth, int imgheigth, int comp, float lumx, float lumy, float lumz);

/**
 * Dessine un triangle colorie suivant les sommets, les texture, les normales et la lumiere
 */
void  WindowDrawTriangle  ( window_t * w, float *zbuff, int x0, int y0, float z0, int x1, int y1, float z1, int x2, int y2, float z2, int tx0, int ty0, int tx1, int ty1, int tx2, int ty2, vec3f_t *norm0, vec3f_t *norm1, vec3f_t *norm2, unsigned char *Texture, int imgwidth, int imgheigth, int comp,  float lumx, float lumy, float lumz);
#endif //__WINDOW_H__
