// kuma.h

#ifndef KUMA_H
#define KUMA_H

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <gl/glut.h>
#include <gl/gl.h>
#endif

#include <fstream>
#include <iostream>

#define Ted 1

class Teddy{

public:	 
	void count();	// read the file "teddy.obj"
	enum dest{
	up, down, right, left
	};
	Teddy( int, int, dest );	// make call list of character
	void Draw(bool isgameover = false);
	void set_dest( dest );
	bool Move();	// update x, y position of chararcter and status
	dest getDest();
	int getMovingStatus();
private:
	float x, y;	// position of the character
	int status;	// move status
	dest where;
	GLdouble **vertex;
	int **face;
	int face_num;
};

#endif
