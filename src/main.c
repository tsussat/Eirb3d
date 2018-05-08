#include "window.h"
#include "events.h"
#include "vector.h"
#include "geometry.h"
#include "model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <float.h>

int main( int argc, char ** argv ) {

	ModelLoad("./bin/data/diablo.obj");
	int imgwidth,imgheigth,comp;
	unsigned char *Texture = stbi_load("./bin/data/diablo_diffuse.tga", &imgwidth, &imgheigth, &comp, STBI_rgb_alpha);

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
		int buffsize = height * width;
		float zbuff[buffsize];
		for(int i = 0; i< buffsize; i++){
			zbuff[i]=-FLT_MAX;
		}

		for(int i = 0; i<VectorGetLength(g_face); i++){

			face_t * face = (face_t *) VectorGetFromIdx(g_face, i);
			vec3f_t * point0 = (vec3f_t *) VectorGetFromIdx(g_vertex, face->v[0]-1);
			int x0 = (point0->x + 1) / 2 * width;
			int y0 = height - (point0->y + 1) / 2 * height;
			float z0 = point0->z;

			vec3f_t * point1 = (vec3f_t *) VectorGetFromIdx(g_vertex, face->v[1]-1);
			int x1 = (point1->x + 1) / 2 * width;
			int y1 = height - (point1->y + 1) / 2 * height;
			float z1 = point1->z;

			vec3f_t * point2 = (vec3f_t *) VectorGetFromIdx(g_vertex, face->v[2]-1);
			int x2 = (point2->x + 1) / 2 * width;
			int y2 = height - (point2->y + 1) / 2 * height;
			float z2 = point2->z;

			//picks highest z value
			float z = (z0>z1)?z0:(z1>z2)?z1:z2;

			//vt loading
			vec2f_t * tex0 =(vec2f_t *) VectorGetFromIdx(g_texcoord, face->v[0]-1);
			int tx0 = (int)tex0->x * imgwidth;
			int ty0 = (int)tex0->y * imgheigth;
			vec2f_t * tex1 =(vec2f_t *) VectorGetFromIdx(g_texcoord, face->v[1]-1);
			int tx1 = (int)tex1->x * imgwidth;
			int ty1 = (int)tex1->y * imgheigth;;
			vec2f_t * tex2 =(vec2f_t *) VectorGetFromIdx(g_texcoord, face->v[2]-1);
			int tx2 = (int)tex2->x * imgwidth;
			int ty2 = (int)tex2->y * imgheigth;;

			//tri sommets
			if(y2 < y1){
				swapf(&z1,&z2); swap(&y1,&y2); swap(&x1,&x2); swap(&tx1, &tx2); swap(&ty1, &ty2);
			}
			if(y1 < y0){
				swapf(&z1,&z0); swap(&y1,&y0); swap(&x1,&x0); swap(&tx1, &tx0); swap(&ty1, &ty0);
			}
			if(y2 < y1){
				swapf(&z1,&z2); swap(&y1,&y2); swap(&x1,&x2); swap(&tx1, &tx2); swap(&ty1, &ty2);
			}
			//printf("try %d, %d %d %d %d %d %d\n", i, x0, y0, x1, y1, x2, y2);

			//lumiere
			vec3f_t lum = Vec3f(0, 0, 1);

			//normale
			vec3f_t norm = Vec3fNormalize(Vec3fCross(Vec3fSub(*point0, *point1) , Vec3fSub(*point0, *point2)));

			//intensite lumiere
			float intens = lum.x*norm.x + lum.y*norm.y + lum.z*norm.z;
			if(intens>0){
				WindowDrawTriangle(mainwindow, zbuff, z, x0, y0, x1, y1, x2, y2, intens, tx0, ty0, tx1, ty1, tx2, ty2, Texture, imgwidth, imgheigth, STBI_rgb_alpha);
			}

			/*WindowDrawLine(mainwindow, x0, y0, x1, y1, 255, 255, 255);
			WindowDrawLine(mainwindow, x0, y0, x2, y2, 255, 255, 255);
			WindowDrawLine(mainwindow, x1, y1, x2, y2, 255, 255, 255);
			*/
			//printf("pass %d\n", i);
		}

		// Mise à jour de la fenêtre
		WindowUpdate( mainwindow );


	}

	// Fermeture de la fenêtre
	WindowDestroy( mainwindow );





	return 1;
}
