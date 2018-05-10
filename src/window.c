#include "window.h"

static void WindowUpdateTexture( window_t * w ) {
	Uint32 * dst;
	int row, col;
	void * pixels;
	int pitch;
	if ( SDL_LockTexture( w->texture, NULL, &pixels, &pitch ) < 0 ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock texture: %s\n", SDL_GetError() );
		SDL_Quit();
	}
	Uint8 * ptr = w->framebuffer;
	for ( row = 0; row < w->height; ++row ) {
		dst = (Uint32*)( (Uint8*)pixels + row * pitch );
		for ( col = 0; col < w->width; ++col ) {
			Uint8 b = *ptr++;
			Uint8 g = *ptr++;
			Uint8 r = *ptr++;
			Uint8 a = *ptr++;
			*dst++ = ( ( b << 24 ) | ( g << 16 ) | ( r << 8 ) | a );
		}
	}
	SDL_UnlockTexture( w->texture );
}

static Uint8 * WindowInitFramebuffer( window_t * w ) {
	size_t sz = w->width * w->height * w->bpp * sizeof( Uint8 );
	Uint8 * buffer = (Uint8*)malloc( sz );
	if ( buffer == NULL) return NULL;
	memset( buffer, 0, sz );
	return buffer;
}

window_t * WindowInit( int width, int height, int bpp ) {

	if ( SDL_Init( SDL_INIT_VIDEO ) != 0 ) {
		printf( "SDL_Init Error: %s", SDL_GetError() );
		return NULL;
	}

	SDL_Window * sdlwindow = SDL_CreateWindow(	"Software OpenGL renderer",
												SDL_WINDOWPOS_CENTERED,
												SDL_WINDOWPOS_CENTERED,
												width, height,
												SDL_WINDOW_ALLOW_HIGHDPI );

	if ( sdlwindow == NULL ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't create SDL window: %s", SDL_GetError() );
		SDL_DestroyWindow( sdlwindow );
		SDL_Quit();
		return NULL;
	}

	SDL_Renderer * renderer = SDL_CreateRenderer( sdlwindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

	if ( renderer == NULL ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s", SDL_GetError() );
		SDL_DestroyRenderer( renderer );
		SDL_Quit();
		return NULL;
	}

	SDL_Texture * texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_STREAMING, width, height );

	if ( texture == NULL ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture: %s\n", SDL_GetError() );
		SDL_DestroyTexture( texture );
		SDL_Quit();
		return NULL;
	}

	window_t * mainwindow = (window_t*)malloc( sizeof( window_t ) );

	if ( mainwindow == NULL ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't allocate window\n" );
		SDL_Quit();
		return NULL;
	}

	mainwindow->width	= width;
	mainwindow->height	= height;
	mainwindow->bpp		= bpp;
	mainwindow->pitch	= width * bpp;

	Uint8 * framebuffer = WindowInitFramebuffer( mainwindow );

	if ( framebuffer == NULL ) {
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't allocate framebuffer: %s\n", SDL_GetError() );
		SDL_Quit();
		return NULL;
	}

	mainwindow->framebuffer = framebuffer;
	mainwindow->sdlwindow	= sdlwindow;
	mainwindow->renderer	= renderer;
	mainwindow->texture		= texture;

	return mainwindow;
}

void WindowDestroy( window_t * w ) {
	SDL_DestroyRenderer( w->renderer );
	SDL_DestroyTexture( w->texture );
	SDL_DestroyWindow( w->sdlwindow );
	free( w->framebuffer );
	SDL_Quit();
}

void WindowUpdate( window_t * w ) {
	SDL_RenderClear( w->renderer );
	SDL_RenderCopy( w->renderer, w->texture, NULL, NULL );
	SDL_RenderPresent( w->renderer );
	WindowUpdateTexture( w );
}

void WindowDrawPoint( window_t * w, int x, int y, Uint8 r, Uint8 g, Uint8 b ) {
	if((x<0) | (x>w->width) | (y<0) | (y>w->height)){
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Out of bounds trying to draw x:%d y:%d\n", x, y);
		SDL_Quit();
		exit(-1);
	}
	else{
		w->framebuffer[w->bpp*(y * w->width + x)] = (char) b;
		w->framebuffer[w->bpp*(y * w->width + x) + 1] = (char) g;
		w->framebuffer[w->bpp*(y * w->width + x) + 2] = (char) r;
		w->framebuffer[w->bpp*(y * w->width + x) + 3] = 0xFF;
	}
}

void WindowDrawClearColor( window_t * w, Uint8 r, Uint8 g, Uint8 b ) {
	for (int j = 0; j < w->height; j++){
		for (int i = 0; i < w->width; i++){
			WindowDrawPoint(w, i, j, r, g, b);
		}
	}
}

void WindowDrawLine( window_t * w, float *zbuff, float z, int x0, int y0, int x1, int y1, float intens, int tx0, int ty0, int tx1, int ty1, unsigned char *Texture, int imgwidth, int imgheigth, int comp ) {
	int dx,dy,i,xinc,yinc,cumul,x,y,tx,ty ;
  x = x0 ;
  y = y0 ;
	tx = tx0 ;
	ty = ty0 ;

  dx = x1 - x0 ;
  //dy = y1 - y0 ;
  xinc = ( dx > 0 ) ? 1 : -1 ;
  //yinc = ( dy > 0 ) ? 1 : -1 ;
  dx = abs(dx) ;
  //dy = abs(dy) ;
	if(zbuff[y * w->width + x] <= z){
		zbuff[y * w->width + x] = z;
	  WindowDrawPoint(w, x, y, int(Texture[comp*(ty*imgwidth+tx)]*intens), int(Texture[comp*(ty*imgwidth+tx)+1]*intens), int(Texture[comp*(ty*imgwidth+tx)+2]*intens)) ;
	}
  //if ( dx > dy ) {
    //cumul = dx / 2 ;
    for ( i = 1 ; i <= dx ; i++ ) {
      x += xinc ;

			tx = (tx1*i+(dx+1-i)*tx0)/(dx+1);
			ty = (ty1*i+(dx+1-i)*ty0)/(dx+1);
			/*
      cumul += dy ;
      if ( cumul >= dx ) {
        cumul -= dx ;
        y += yinc ;
			}
			*/
			if(zbuff[y * w->width + x] <= z){
				zbuff[y * w->width + x] = z;
			  WindowDrawPoint(w, x, y, int(Texture[comp*(ty*imgwidth+tx)]*intens), int(Texture[comp*(ty*imgwidth+tx)+1]*intens), int(Texture[comp*(ty*imgwidth+tx)+2]*intens)) ;
			}
		//}
	}
	/*
  else {
    cumul = dy / 2 ;
    for ( i = 1 ; i <= dy ; i++ ) {
      y += yinc ;
      cumul += dx ;
      if ( cumul >= dy ) {
        cumul -= dy ;
        x += xinc ;
			}
			if(zbuff[y * w->width + x] < z){
				zbuff[y * w->width + x] = z;
			  WindowDrawPoint(w, x, y, r, g, b) ;
			}
		}
	}
	*/
}

void WindowDrawTriangle( window_t * w, float *zbuff, float z, int x0, int y0, int x1, int y1, int x2, int y2, float intens, int tx0, int ty0, int tx1, int ty1, int tx2, int ty2, unsigned char *Texture, int imgwidth, int imgheigth, int comp) {
	//parametres equation droite A
	int xa = x2 - x0;
	int ya = y2 - y0;
	double a = ( (ya!=0) ? ((double)xa/(double)ya) : (xa) );


	//parametres equation droite B1
	int xb1 = x1 - x0;
	int yb1 = y1 - y0;
	double b1 = ( (yb1!=0) ? ((double)xb1/(double)yb1) : (xb1) );


	//parametres equation droite B2
	int xb2 = x2 - x1;
	int yb2 = y2 - y1;
	double b2 = ( (yb2!=0) ? ((double)xb2/(double)yb2) : (xb2) );


	for(int i=0; i<ya; i++){
		//B1
		if(i<yb1){
			WindowDrawLine(w, zbuff, z, x0 + (int)(i*a), y0 + i, x0 + (int)(i*b1), y0 + i, intens, (i*tx2+(ya-i)*tx0)/ya, (i*ty2+(ya-i)*ty0)/ya, (i*tx1+(yb1-i)*tx2)/yb1, (i*ty1+(yb1-i)*ty0)/yb1, Texture, imgwidth, imgheigth, comp);
		}
		//B2
		else{
			WindowDrawLine(w, zbuff, z, x0 + (int)(i*a), y0 + i, x1 + (int)((i-yb1)*b2), y0 + i, intens, (i*tx2+(ya-i)*tx0)/ya, (i*ty2+(ya-i)*ty0)/ya, ((i-yb1)*tx2+(yb2-i+yb1)*tx1)/yb2, ((i-yb1)*ty2+(yb2-i+yb1)*ty1)/yb2, Texture, imgwidth, imgheigth, comp);
		}
	}
}
