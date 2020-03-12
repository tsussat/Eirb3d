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
	//Verification que le point est bien dans la fenetre
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

//Trace une ligne avec la methode de Bresenham
void WindowDrawLineB( window_t * w, int x0, int y0, int x1, int y1, Uint8 r, Uint8 g, Uint8 b ) {
	int dx,dy,i,xinc,yinc,cumul,x,y ;
  x = x0 ;
  y = y0 ;
  dx = x1 - x0 ;
  dy = y1 - y0 ;
  xinc = ( dx > 0 ) ? 1 : -1 ;
  yinc = ( dy > 0 ) ? 1 : -1 ;
  dx = abs(dx) ;
  dy = abs(dy) ;
  WindowDrawPoint(w, x , y, r, g, b) ;
  if ( dx > dy ) {
    cumul = dx / 2 ;
    for ( i = 1 ; i <= dx ; i++ ) {
      x += xinc ;
      cumul += dy ;
      if ( cumul >= dx ) {
        cumul -= dx ;
        y += yinc ;
			}
      WindowDrawPoint(w, x , y, r, g, b) ;
		}
	}
  else {
    cumul = dy / 2 ;
    for ( i = 1 ; i <= dy ; i++ ) {
      y += yinc ;
      cumul += dx ;
      if ( cumul >= dy ) {
        cumul -= dy ;
        x += xinc ;
			}
      WindowDrawPoint(w, x , y, r, g, b) ;
		}
	}
}

//Trace une ligne horizontal
void WindowDrawLine( window_t * w, float *zbuff, int x0, int y0, float z0, int x1, int y1, float z1, int tx0, int ty0, int tx1, int ty1, vec3f_t *norm0, vec3f_t *norm1, unsigned char *Texture, int imgwidth, int imgheigth, int comp , float lumx, float lumy, float lumz) {
	int dx,i,xinc,x,y,tx,ty;
	float intens;
	float z;

  x = x0 ;
  y = y0 ;
	z = z0 ;
	tx = tx0 ;
	ty = ty0 ;

	vec3f_t norm = Vec3fNormalize(*norm0);

	//calcul de l'intensite
	intens = lumx*norm.x + lumy*norm.y + lumz*norm.z;
	if (intens < 0){
		intens = 0;
	}
  dx = x1 - x0 ;
  xinc = ( dx > 0 ) ? 1 : -1 ;
  dx = abs(dx) ;

	if(z  >= zbuff[y * w->width + x] ){
		zbuff[y * w->width + x] = z ;
  	WindowDrawPoint(w, x, y, (int)(Texture[comp*(ty*imgwidth+tx)]*intens), (int)(Texture[comp*(ty*imgwidth+tx)+1]*intens), (int)(Texture[comp*(ty*imgwidth+tx)+2]*intens)) ;
	}

	//affichage de chaque point de la ligne
  for ( i = 1 ; i <= dx ; i++ ) {
    x += xinc ;

		//calucl des caractéristiques du prochain point
		tx = (tx1*i+(dx-i)*tx0)/dx;
		ty = (ty1*i+(dx-i)*ty0)/dx;
		z = i*(z1-z0)/(dx+1)+z0;
		norm.x = i*(norm1->x - norm0->x)/dx+norm0->x;
		norm.y = i*(norm1->y - norm0->y)/dx+norm0->y;
		norm.z = i*(norm1->z - norm0->z)/dx+norm0->z;
		norm = Vec3fNormalize(norm);
		intens = lumx*norm.x + lumy*norm.y + lumz*norm.z;
		if (intens < 0){
			intens = 0;
		}

		//affichage du point
		if(zbuff[y * w->width + x] <= z ){
			zbuff[y * w->width + x] = z;
		  WindowDrawPoint(w, x, y, (int)(Texture[comp*(ty*imgwidth+tx)]*intens), (int)(Texture[comp*(ty*imgwidth+tx)+1]*intens), (int)(Texture[comp*(ty*imgwidth+tx)+2]*intens)) ;
		}
	}
}

//Trace un triangle
void WindowDrawTriangle( window_t * w, float *zbuff, int x0, int y0, float z0, int x1, int y1, float z1, int x2, int y2, float z2, int tx0, int ty0, int tx1, int ty1, int tx2, int ty2, vec3f_t *norm0, vec3f_t *norm1, vec3f_t *norm2, unsigned char *Texture, int imgwidth, int imgheigth, int comp, float lumx, float lumy, float lumz) {
	//parametres equation droite A
	int xa = x2 - x0;
	int ya = y2 - y0;
	float za = z2 - z0;
	double a = ( (ya!=0) ? ((double)xa/(double)ya) : (xa) );
	double az = ( (ya!=0) ? ((double)za/(double)ya) : (za) );

	//parametres equation droite B1
	int xb1 = x1 - x0;
	int yb1 = y1 - y0;
	float zb1 = z1 - z0;
	double b1 = ( (yb1!=0) ? ((double)xb1/(double)yb1) : (xb1) );
	double b1z = ( (yb1!=0) ? ((double)zb1/(double)yb1) : (zb1) );

	//parametres equation droite B2
	int xb2 = x2 - x1;
	int yb2 = y2 - y1;
	float zb2 = z2 - z1;
	double b2 = ( (yb2!=0) ? ((double)xb2/(double)yb2) : (xb2) );
	double b2z = ( (yb2!=0) ? ((double)zb2/(double)yb2) : (zb2) );

	vec3f_t norml0 = Vec3f(0,0,0); //normal du point de depart de la ligne
	vec3f_t norml1 = Vec3f(0,0,0); //normal du point d'arrive de la ligne

	for(int i=0; i<ya; i++){
		//B1
		if(i<yb1){
			norml0.x = i*(norm2->x - norm0->x)/ya+norm0->x;
			norml0.y = i*(norm2->y - norm0->y)/ya+norm0->y;
			norml0.z = i*(norm2->z - norm0->z)/ya+norm0->z;

			norml1.x = i*(norm1->x - norm0->x)/yb1+norm0->x;
			norml1.y = i*(norm1->y - norm0->y)/yb1+norm0->y;
			norml1.z = i*(norm1->z - norm0->z)/yb1+norm0->z;
			WindowDrawLine(w, zbuff, x0 + (int)(i*a), y0 + i, z0 + (float)(i*az), x0 + (int)(i*b1), y0 + i, z0 + (float)(i*b1z), (i*tx2+(ya-i)*tx0)/ya, (i*ty2+(ya-i)*ty0)/ya, (i*tx1+(yb1-i)*tx0)/yb1, (i*ty1+(yb1-i)*ty0)/yb1, &norml0, &norml1, Texture, imgwidth, imgheigth, comp, lumx, lumy, lumz);
		}
		//B2
		else{
			norml0.x = i*(norm2->x - norm0->x)/ya+norm0->x;
			norml0.y = i*(norm2->y - norm0->y)/ya+norm0->y;
			norml0.z = i*(norm2->z - norm0->z)/ya+norm0->z;

			norml1.x = (i-yb1)*(norm2->x - norm1->x)/yb2+norm1->x;
			norml1.y = (i-yb1)*(norm2->y - norm1->y)/yb2+norm1->y;
			norml1.z = (i-yb1)*(norm2->z - norm1->z)/yb2+norm1->z;
			WindowDrawLine(w, zbuff, x0 + (int)(i*a), y0 + i, z0 + (float)(i*az), x1 + (int)((i-yb1)*b2), y0 + i, z1 + (float)((i-yb1)*b2z), (i*tx2+(ya-i)*tx0)/ya, (i*ty2+(ya-i)*ty0)/ya, ((i-yb1)*tx2+(yb2-i+yb1)*tx1)/yb2, ((i-yb1)*ty2+(yb2-i+yb1)*ty1)/yb2, &norml0, &norml1, Texture, imgwidth, imgheigth, comp, lumx, lumy, lumz);
		}
	}
}
