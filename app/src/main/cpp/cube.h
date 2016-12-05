//
// Created by lastkgb on 2016-11-23.
//

#ifndef MTRACKER_CUBE2_H
#define MTRACKER_CUBE2_H

#include <GLES2/gl2.h>

class Cube {
public:
	GLfloat vertices[108];
	GLbyte indices[6];
public:
	Cube(GLfloat[], GLbyte[]);
	void draw(GLuint);
	virtual ~Cube();
};

#endif //MTRACKER_CUBE2_H
