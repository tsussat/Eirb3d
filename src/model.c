#include "model.h"

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

bool ModelLoad( char * objfilename ) {
	// Fonction à implementer
	vec3f_t v3;
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

	while( fgets(buffer, 1024, f) != NULL){
		if (sscanf(buffer, "v %f %f %f", &v3.x, &v3.y, &v3.z)){
			VectorAdd(g_vertex, &v3);
		}
		if (sscanf(buffer, "vn %f %f %f", &v3.x, &v3.y, &v3.z)){
			VectorAdd(g_norm, &v3);
		}
		if (sscanf(buffer, "vt %f %f ", &v2.x, &v2.y)){
			VectorAdd(g_texcoord, &v2);
		}
		if (sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", &face.v[0], &face.vt[0], &face.vn[0], &face.v[1], &face.vt[1], &face.vn[1], &face.v[2], &face.vt[2], &face.vn[2] )){
			VectorAdd(g_face, &face);
		}
	}

	return true;
}
