// main.cpp

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>

#include "game.h"

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("TEDDY BLOCK");
	glutReshapeFunc(Game::Reshape);
	glutIdleFunc(Game::Idle);
	glutDisplayFunc(Game::Display);
	glutSpecialFunc(Game::SpecialKey);
	glutKeyboardFunc(Game::KeyInput);
	
	glutMainLoop();

	return 0;
}
