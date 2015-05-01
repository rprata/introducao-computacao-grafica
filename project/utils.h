#ifndef	UTILS_H
#define	UTILS_H

#include <GL/glut.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <jpeglib.h>
#include <jerror.h>

int load_texture (const char * filename, unsigned char * dest, const int format, const unsigned int size);
float * crossProduct(float b[3], float c[3]);

#endif