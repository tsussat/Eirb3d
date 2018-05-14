#ifndef __MODEL_H__
#define __MODEL_H__

#include "vector.h"
#include "geometry.h"

/**
 * D�finition des prototypes de fonctions
 */

/**
 * Retourne la liste des sommets du mod�le
 */
vector_t	*	ModelVertices		();

/**
 * Retourne la liste des normales du mod�le
 */
vector_t	*	ModelNormals		();

/**
 * Retourne la liste des coordonn�es de texture du mod�le
 */
vector_t	*	ModelTexcoords		();

/**
 * Retourne la liste des faces du mod�le
 */
vector_t	*	ModelFaces		();

/**
 * Retourne le sommet du mod�le � l'index sp�cifi�
 */
vec3f_t			ModelGetVertex		( int idx );

/**
 * Retourne la normale du mod�le � l'index sp�cifi�
 */
vec3f_t			ModelGetNormal		( int idx );

/**
 * Retourne les coordonn�s de texture du mod�le � l'index sp�cifi�
 */
vec3f_t			ModelGetTexcoord	( int idx );

/**
 * Retourne la face du mod�le � l'index sp�cifi�
 */
face_t			ModelGetFace		( int idx );

/**
 * Charge un mod�le 3D � partir du fichier sp�cifi�
 */
bool			ModelLoad		( char * objfilename );

#endif // __MODEL_H__
