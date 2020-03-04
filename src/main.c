#include "window.h"
#include "events.h"
#include "vector.h"
#include "geometry.h"
#include "model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <math.h>
#include <time.h>

#define PI 3.14159265

int main( int argc, char ** argv ) {

	float xoffset, yoffset, range;

	ModelLoad("./bin/data/diablo.obj", &xoffset, &yoffset, &range);
	printf("%f %f\n", xoffset, yoffset);
	int imgwidth,imgheigth,comp;
	unsigned char *Texture = stbi_load("./bin/data/diablo_diffuse.tga", &imgwidth, &imgheigth, &comp, STBI_rgb_alpha);

	vector_t * g_vertex = ModelVertices();
	vector_t * g_norm = ModelNormals();
  vector_t * g_texcoord = ModelTexcoords();
	vector_t * g_face = ModelFaces();

	const int width		= 1024;
	const int height	= 768;

	// Ouverture d'une nouvelle fenêtre
	window_t * mainwindow = WindowInit( width, height, 4 );
	int done = false;

	int frames = 0;
	clock_t time_i = clock();
	clock_t time_c;

	int rot = 0;
	int angle = 1;

	// Tant que l'utilisateur de ferme pas la fenêtre
	while ( !done ) {

		// Mise à jour et traitement des evênements de la fenêtre
		done = EventsUpdate( mainwindow );

		// Effacement de l'écran avec une couleur
		WindowDrawClearColor( mainwindow, 64, 64, 64 );

		//lumiere
		vec3f_t lum = Vec3f(0, 0, 1);

		int buffsize = height * width;
		float zbuff[buffsize];
		for(int i = 0; i< buffsize; i++){
			zbuff[i]= -FLT_MAX * lum.z;
		}

		float cosrot = cos(rot*PI/180.0);
		float sinrot = sin(rot*PI/180.0);

		for(int i = 0; i<VectorGetLength(g_face); i++){

			face_t * face = (face_t *) VectorGetFromIdx(g_face, i);

			/*x0 = point0->x*cos(rot*PI/180.0)+point0->z*sin(rot*PI/180.0);
			z0 = -point0->x*sin(rot*PI/180.0)+point0->z*cos(rot*PI/180.0);
			x1 = point1->x*cos(rot*PI/180.0)-point1->z*sin(rot*PI/180.0);
			z1 = point1->x*sin(rot*PI/180.0)+point1->z*cos(rot*PI/180.0);
			x2 = point2->x*cos(rot*PI/180.0)-point2->z*sin(rot*PI/180.0);
			z2 = point2->x*sin(rot*PI/180.0)+point2->z*cos(rot*PI/180.0);*/

			//charge les coordonnées des points
			vec3f_t * point0 = (vec3f_t *) VectorGetFromIdx(g_vertex, face->v[0]-1);
			int x0 = (point0->x*cosrot+point0->z*sinrot - xoffset) / range * width;
			int y0 = height - (point0->y - yoffset) / range * height;
			float z0 = (-point0->x*sinrot+point0->z*cosrot- xoffset)/ range * width;

			vec3f_t * point1 = (vec3f_t *) VectorGetFromIdx(g_vertex, face->v[1]-1);
			int x1 = (point1->x*cosrot+point1->z*sinrot - xoffset) / range * width;
			int y1 = height - (point1->y - yoffset) / range * height;
			float z1 = (-point1->x*sinrot+point1->z*cosrot- xoffset)/ range * width;

			vec3f_t * point2 = (vec3f_t *) VectorGetFromIdx(g_vertex, face->v[2]-1);
			int x2 = (point2->x*cosrot+point2->z*sinrot - xoffset) / range * width;
			int y2 = height - (point2->y - yoffset) / range * height;
			float z2 = (-point2->x*sinrot+point2->z*cos(rot*PI/180.0)- xoffset)/ range * width;

			//charge les textures
			vec2f_t * tex0 =(vec2f_t *) VectorGetFromIdx(g_texcoord, face->vt[0]-1);
			int tx0 = (int)(tex0->x * imgwidth);
			int ty0 = imgheigth-(int)(tex0->y * imgheigth);
			vec2f_t * tex1 =(vec2f_t *) VectorGetFromIdx(g_texcoord, face->vt[1]-1);
			int tx1 = (int)(tex1->x * imgwidth);
			int ty1 = imgheigth-(int)(tex1->y * imgheigth);
			vec2f_t * tex2 =(vec2f_t *) VectorGetFromIdx(g_texcoord, face->vt[2]-1);
			int tx2 = (int)(tex2->x * imgwidth);
			int ty2 = imgheigth-(int)(tex2->y * imgheigth);

			//charge les normales
			vec3f_t * norm0t = (vec3f_t *) VectorGetFromIdx(g_norm, face->vn[0]-1);
			vec3f_t norm0 = Vec3f(norm0t->x, norm0t->y, norm0t->z);

			vec3f_t * norm1t = (vec3f_t *) VectorGetFromIdx(g_norm, face->vn[1]-1);
			vec3f_t norm1 = Vec3f(norm1t->x, norm1t->y, norm1t->z);

			vec3f_t * norm2t = (vec3f_t *) VectorGetFromIdx(g_norm, face->vn[2]-1);
			vec3f_t norm2 = Vec3f(norm2t->x, norm2t->y, norm2t->z);

			//tri des sommets
			if(y2 < y1){
				swapf(&z1,&z2); swap(&y1,&y2); swap(&x1,&x2); swap(&tx1, &tx2); swap(&ty1, &ty2); Vec3fSwap(&norm1,&norm2);
			}
			if(y1 < y0){
				swapf(&z1,&z0); swap(&y1,&y0); swap(&x1,&x0); swap(&tx1, &tx0); swap(&ty1, &ty0); Vec3fSwap(&norm1,&norm0);
			}
			if(y2 < y1){
				swapf(&z1,&z2); swap(&y1,&y2); swap(&x1,&x2); swap(&tx1, &tx2); swap(&ty1, &ty2); Vec3fSwap(&norm1,&norm2);
			}


			WindowDrawTriangle(mainwindow, zbuff, x0, y0, z0, x1, y1, z1, x2, y2, z2, tx0, ty0, tx1, ty1, tx2, ty2, &norm0, &norm1, &norm2, Texture, imgwidth, imgheigth, STBI_rgb_alpha, lum.x, lum.y, lum.z);

			//Trace les lignes de fer
			/*WindowDrawLineB(mainwindow, x0, y0, x1, y1, 255, 255, 255);
			WindowDrawLineB(mainwindow, x0, y0, x2, y2, 255, 255, 255);
			WindowDrawLineB(mainwindow, x1, y1, x2, y2, 255, 255, 255);
			*/

		}
		if(rot>=360) rot-=360;
		else rot+=angle;

		time_c = clock();
		float secs = (time_c-time_i)/CLOCKS_PER_SEC;
		if(secs>=1.0){
			printf("\r%0.1f FPS, %d frames, %d clocks", frames/secs, frames, time_c-time_i);
			fflush(stdout);
			frames=0;
			time_i = clock();
		}
		else frames++;
		// Mise à jour de la fenêtre
		WindowUpdate( mainwindow );

	}
	// Fermeture de la fenêtre
	WindowDestroy( mainwindow );
	return 1;
}
