#include "model.h"
#include <float.h>
#include <math.h>

vector_t * g_vertex;
vector_t * g_norm;
vector_t * g_texcoord;
vector_t * g_face;

vector_t * ModelVertices() {
	return g_vertex;
}

vector_t * ModelNormals() {
	return g_norm;
}

vector_t * ModelTexcoords() {
	return g_texcoord;
}

vector_t * ModelFaces() {
	return g_face;
}

vec3f_t ModelGetVertex( int index ) {
	vec3f_t v = *(vec3f_t*)VectorGetFromIdx( ModelVertices(), index );
	return v;
}

vec3f_t ModelGetNormal( int index ) {
	vec3f_t t = *(vec3f_t*)VectorGetFromIdx( ModelNormals(), index );
	return t;
}

vec3f_t ModelGetTexcoord( int index ) {
	vec3f_t t = *(vec3f_t*)VectorGetFromIdx( ModelTexcoords(), index );
	return t;
}

face_t ModelGetFace( int index ) {
	face_t f = *(face_t*)VectorGetFromIdx( ModelFaces(), index );
	return f;
}

bool ModelLoad( char * objfilename , float* xoffset, float* yoffset, float* range) {
	// Fonction à implementer
	vec3f_t v3;
	float xmax = -FLT_MAX;
	float ymax = -FLT_MAX;
	float zmax = -FLT_MAX;
	float xmin = FLT_MAX;
	float ymin = FLT_MAX;
	float zmin = FLT_MAX;

	vec2f_t v2;
	face_t face;
	FILE *f = fopen(objfilename, "r");
	if(f == NULL ){
		printf("Obj non valide\n");
		return false;
	}
	char buffer[1024];
	g_vertex = Vector();
	g_norm = Vector();
	g_texcoord = Vector();
	g_face = Vector();

	while( fgets(buffer, 1024, f) != NULL){ //tant qu'il y a des donnees a charger
		//chargement des sommets dans la liste g_vertex
		if (buffer[0] == 'v' && buffer[1] == ' '){
			vec3f_t * v3 = (vec3f_t *) malloc(sizeof(vec3f_t));
			sscanf(buffer, "v %f %f %f", &v3->x, &v3->y, &v3->z);
			VectorAdd(g_vertex, v3);
			if (v3->x < xmin) xmin = v3->x ;
			if (v3->x > xmax) xmax = v3->x ;
			if (v3->y < ymin) ymin = v3->y ;
			if (v3->y > ymax) ymax = v3->y ;
			if (v3->z < zmin) zmin = v3->z ;
			if (v3->z > zmax) zmax = v3->z ;

		}
		//chargement des normales dans la liste g_norm
		if (buffer[0] == 'v' && buffer[1] == 'n'){
			vec3f_t * v3 = (vec3f_t *)malloc(sizeof(vec3f_t));
			sscanf(buffer, "vn %f %f %f", &v3->x, &v3->y, &v3->z);
			VectorAdd(g_norm, v3);
		}
		//chargement des textures dans la liste g_textcoord
		if (buffer[0] == 'v' && buffer[1] == 't'){
			vec2f_t * v2 = (vec2f_t *) malloc(sizeof(vec2f_t));
			sscanf(buffer, "vt %f %f ", &v2->x, &v2->y);
			VectorAdd(g_texcoord, v2);
		}
		//chargement des faces dans la liste g_face
		if (buffer[0] == 'f'){
			face_t *face = (face_t *) malloc(sizeof(face_t));
			sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", &face->v[0], &face->vt[0], &face->vn[0], &face->v[1], &face->vt[1], &face->vn[1], &face->v[2], &face->vt[2], &face->vn[2] );
			VectorAdd(g_face, face);
		}
	}
	float xrange = ((xmax-xmin)>(zmax-zmin)?(xmax-xmin):(zmax-zmin));
	float yrange = (ymax-ymin);
	if(xrange < yrange){
		*range = abs(yrange);
		*xoffset = xmin - (yrange-xrange)/2;
		*yoffset = ymin;
	}
	else{
		*range = abs(xrange);
		*xoffset = xmin;
		*yoffset = ymin - (xrange-yrange)/2;
	}
	return true;
}
