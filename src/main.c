#include "window.h"
#include "events.h"
#include "vector.h"
#include "geometry.h"
#include "model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main( int argc, char ** argv ) {
	//TEST
	ModelLoad("./bin/data/test.obj");
	vector_t * vec = ModelVertices();
	vec3f_t * v = (vec3f_t *) VectorGetFromIdx( vec, 0 );
	printf("%f\n",v->y);



	const int width		= 1024;
	const int height	= 768;

	// Ouverture d'une nouvelle fenêtre
	window_t * mainwindow = WindowInit( width, height, 4 );

	int done = false;

	// Tant que l'utilisateur de ferme pas la fenêtre
	while ( !done ) {

		// Mise à jour et traitement des evênements de la fenêtre
		done = EventsUpdate( mainwindow );

		// Effacement de l'écran avec une couleur
		WindowDrawClearColor( mainwindow, 64, 64, 64 );

		// Dessin d'un point blanc au milieu de le fenêtre
		WindowDrawPoint( mainwindow, width / 2, height / 2, 255, 255, 255 );

		// Mise à jour de la fenêtre
		WindowUpdate( mainwindow );

	}

	// Fermeture de la fenêtre
	WindowDestroy( mainwindow );





	return 1;
}
