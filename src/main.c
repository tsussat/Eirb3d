#include "window.h"
#include "events.h"
#include "vector.h"
#include "geometry.h"
#include "model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main( int argc, char ** argv ) {

	ModelLoad("./bin/data/diablo.obj");


	vector_t * g_vertex = ModelVertices();
	vector_t * g_norm = ModelNormals();
  vector_t * g_texcoord = ModelTexcoords();
	vector_t * g_face = ModelFaces();
	//vec3f_t * v = (vec3f_t *) VectorGetFromIdx( g_vertex, 0 );

	//TEST chargement objet
	//printf("%f\n",v->y);
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

		//TEST affichage ligne et point
/*
		// Dessin d'un point blanc au milieu de le fenêtre
		WindowDrawPoint( mainwindow, width/2 , height/2 , 255, 0, 255 );

		// Dessin d'une ligne
		WindowDrawLine( mainwindow, 0, 0, width-1, height-1, 255, 255, 255);
*/

		for(int i = 0; i<VectorGetLength(g_face); i++){
			face_t * face = (face_t *) VectorGetFromIdx(g_face, i);
			vec3f_t * point0 = (vec3f_t *) VectorGetFromIdx(g_vertex, face->v[0]-1);
			int x0 = (point0->x + 1) / 2 * width;
			int y0 = height - (point0->y + 1) / 2 * height;
			vec3f_t * point1 = (vec3f_t *) VectorGetFromIdx(g_vertex, face->v[1]-1);
			int x1 = (point1->x + 1) / 2 * width;
			int y1 = height - (point1->y + 1) / 2 * height;
			vec3f_t * point2 = (vec3f_t *) VectorGetFromIdx(g_vertex, face->v[2]-1);
			int x2 = (point2->x + 1) / 2 * width;
			int y2 = height - (point2->y + 1) / 2 * height;


			WindowDrawLine(mainwindow, x0, y0, x1, y1, 255, 255, 255);
			WindowDrawLine(mainwindow, x0, y0, x2, y2, 255, 255, 255);
			WindowDrawLine(mainwindow, x1, y1, x2, y2, 255, 255, 255);
		}

		// Mise à jour de la fenêtre
		WindowUpdate( mainwindow );


	}

	// Fermeture de la fenêtre
	WindowDestroy( mainwindow );





	return 1;
}
