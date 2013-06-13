// kuma.cpp

#include "kuma.h"
#include "game.h"

Teddy::Teddy( int X, int Y, dest D ){
	x = X;
	y = Y;
	where = D;
	count();
	status = 0;

	// make callist
	glNewList( Ted, GL_COMPILE);
		int i;
		glBegin( GL_LINES );
				glColor3f(1.0, 0.0, 0.0);
		for (i = 0; i < face_num; i++) {
			glVertex3dv(vertex[face[i][0]]);
			glVertex3dv(vertex[face[i][1]]);
			glVertex3dv(vertex[face[i][1]]);
			glVertex3dv(vertex[face[i][2]]);
			glVertex3dv(vertex[face[i][2]]);
			glVertex3dv(vertex[face[i][0]]);
		}
		glEnd();
	glEndList();
}

bool Teddy::Move(){
	if( where == up ) y += 1.0/MAX_TILE_MOVESTATE;
	if( where == down ) y -= 1.0/MAX_TILE_MOVESTATE;
	if( where == left ) x -= 1.0/MAX_TILE_MOVESTATE;
	if( where == right ) x += 1.0/MAX_TILE_MOVESTATE;

	status++;
	if( status == MAX_TILE_MOVESTATE ){
		status = 0;
		return false;
	}
	return true;
}

void Teddy::set_dest( dest New ){
	where = New;
}

void Teddy::Draw(bool isgameover){
	glTranslatef(-(MAP_WIDTH * FIGURE_SIZE) / 2.0, -(MAP_HEIGHT * FIGURE_SIZE) / 2.0, 0);
	if( where == down ){
		glRotatef( 90, 1, 0 ,0 );
		glTranslatef(FIGURE_SIZE * (x + 0.5), FIGURE_SIZE, -FIGURE_SIZE * (y + 0.5) );
	}
	else if( where == up ){
		glRotatef( 90, 1, 0, 0 );
		glRotatef( 180, 0, 1, 0 );
		glTranslatef(-FIGURE_SIZE * (x + 0.5), FIGURE_SIZE, FIGURE_SIZE * (y + 0.5) );
	}

	else if( where == left ){
		glRotatef( 90, 1, 0, 0 );
		glRotatef( -90, 0, 1, 0 );
		glTranslatef(-FIGURE_SIZE * (y + 0.5), FIGURE_SIZE, -FIGURE_SIZE * (x + 0.5) );
	}

	else{
		glRotatef( 90, 1, 0, 0 );
		glRotatef( 90, 0, 1, 0 );
		glTranslatef(FIGURE_SIZE * (y + 0.5), FIGURE_SIZE, FIGURE_SIZE * (x + 0.5) );
	}

	if(isgameover == true) {
		static int windmill = 0;
		windmill += 5;
		if(windmill >= 360) windmill = 0;
		glRotatef(windmill, 0, 1, 0);
	}

	glCallList( Ted );
}

void Teddy::count(){

	std::ifstream obj;
	int number=0;
	char check;
	double temp;
	int temp2;

	obj.open("teddy.obj", std::ios::in);
	if(!obj) {
		//MessageBox(NULL, "Cannot find teddy.obj", "Error", MB_ICONERROR | MB_OK);
		std::cerr << "Cannot find teddy.obj" << std::endl;
		exit(0);
	}

	//count the number of vertex
	while(1){
		obj>>check;
		if( check == 'v' ){
			number++;
			obj>>temp;
			obj>>temp;
			obj>>temp;
		}

		else break;
	}

	obj.close();

	obj.open( "teddy.obj", std::ios::in );

	vertex = new GLdouble * [number];

	//store the position of vertex
	int i;
	for( i = 0; i < number ; i++ ){

		vertex[i] = new GLdouble [3];
		obj>>check;
		obj>>vertex[i][0];
		obj>>vertex[i][1];
		obj>>vertex[i][2];
	}

	number = 0;

	//do same thing on the face also
	while(1){
		if( obj>>check ){
			number++;
			obj>>temp2;
			obj>>temp2;
			obj>>temp2;
		}
		else break;
	}

	obj.close();

	obj.open( "teddy.obj", std::ios::in );

	face = new int * [number];

	while(1){
		obj>>check;
		if( check == 'f' )
			break;
		else{
			obj>>temp;
			obj>>temp;
			obj>>temp;
		}
	}

	for( i = 0; i < number ; i++ ){

		face[i] = new int [3];

		obj>>face[i][0];
		obj>>face[i][1];
		obj>>face[i][2];
		obj>>check;
		face[i][0]--;
		face[i][1]--;
		face[i][2]--;
	}
	face_num = number;

	obj.close();
}

Teddy::dest Teddy::getDest()
{
	return where;
}

int Teddy::getMovingStatus()
{
	return status;
}
