// game.cpp

#include <cmath>

#include "game.h"

// initialize static members
Game::State Game::state = INIT;
bool Game::work = true;
bool Game::figure_moving = false;
bool Game::teddy_moving = false;
int Game::cursor_x = 0;
int Game::cursor_y = 0;
unsigned Game::score = 0;
double Game::playtime = 0;
unsigned Game::oldtime = 0;
Map Game::map;
int Game::window_w;
int Game::window_h;
double Game::eye_x;
double Game::eye_y;
double Game::eye_z;
double Game::theta = 3.9375;
double Game::phi = 0.7875;
bool Game::static_view = true;
Teddy *Game::kuma;

int level = 20;

void Game::Display()
{
	glClearColor(56.0/255.0, 123.0/255.0, 177.0/255.0, 0.0);	// set Background color
	glClear( GL_COLOR_BUFFER_BIT );
	glLoadIdentity();

	double teddy_x = 0, teddy_y = 0;	// character centered view 일 때 시점 계산하는 변수
	double teddy_status = kuma->getMovingStatus();
	if(teddy_moving == true) {
		switch(kuma->getDest()) {
		case Teddy::up:
			teddy_y = teddy_status / MAX_TILE_MOVESTATE - 1.0;
			break;
		case Teddy::down:
			teddy_y = -teddy_status / MAX_TILE_MOVESTATE + 1.0;
			break;
		case Teddy::left:
			teddy_x = -teddy_status / MAX_TILE_MOVESTATE + 1.0;
			break;
		case Teddy::right:
			teddy_x = teddy_status / MAX_TILE_MOVESTATE - 1.0;
			break;
		}
	}
	gluLookAt( eye_x + (static_view == false)*(FIGURE_SIZE * (cursor_x - MAP_WIDTH/2.0 + teddy_x)),
		eye_y + (static_view == false)*(FIGURE_SIZE * (cursor_y - MAP_WIDTH/2.0 + teddy_y)),
		eye_z,
		0.0 + (static_view == false)*(FIGURE_SIZE * (cursor_x - MAP_WIDTH/2.0 + teddy_x)),
		0.0 + (static_view == false)*(FIGURE_SIZE * (cursor_y - MAP_WIDTH/2.0 + teddy_y)),
		0.0,
		0.0, 0.0, 1.0 );
	
	if(state == START) {
		DrawMap();
		kuma->Draw();
	}
	else if(state == END) {
		DrawMap();
		kuma->Draw(true);
	}

	Write_score();
	Write_combo();

	glutSwapBuffers();
}

void write_number( int score ){
	int temp = score;
	int temp2;
	int i = 0;
	int j;

	while( temp/10 != 0 ) temp /= 10, i++;
	temp = score;

	for( ; i >= 0 ; i-- ){
		j = i;
		temp2 = temp;
		while( j != 0 ) temp/= 10, --j;
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24 , char( '0' + temp ) );
		temp = temp2 - temp*pow(10, i);
	}
}

void write_words( char *string ){
	while( *string != NULL ){
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24 , *string );
		string++;
	}
}

void Game::Write_combo(){

	glLoadIdentity();
	if( state == Game::END ) glColor3f( ( rand()%256 / 255.0 ),( rand()%256 / 255.0 ),( rand()%256 / 255.0 ) );
	else glColor3f( 0.0, 0.0, 0.0 );
	
	glRasterPos2f( -2, 8 );
	write_words( "Max combo : " );
	write_number( map.show_combomx() );
	if( map.show_combo() < 2 ) return;
	
	if( map.show_combo() >= 10 && map.show_combo() < 20 ) glColor3f( 251/255.0, 216/255.0, 51/255.0 );
	else if( map.show_combo() >= 20 && map.show_combo() < 30) glColor3f( 255/255.0, 128/255.0, 0/255.0 );
	else if( map.show_combo() >= 30 ) glColor3f( 1.0, 0.0, 0.0 );
	else glColor3f( 0.0, 0.0, 0.0 );

	glRasterPos2f( -2, 9 );
	write_number( map.show_combo() ); 
	write_words( "Combo!!" );
}

void Game::Write_score(){
	static bool start = false;
	glLoadIdentity();
	if( work == false && state != Game::END){
		glColor3f( ( rand()%256 / 255.0 ),( rand()%256 / 255.0 ),( rand()%256 / 255.0 ) );
		glRasterPos2f( -4.0, 0.0 );

		if( start == false )write_words( "Press Space Bar to Start!! " );
		else write_words( "PAUSE - Press SpaceBar to Continue" );
	}
	if( start == false && work == true ) start = true;
	if( state == Game::END ){
		glColor3f( ( rand()%256 / 255.0 ),( rand()%256 / 255.0 ),( rand()%256 / 255.0 ) );
		glRasterPos2f( -2.0, 0.0 );

		write_words( "GAME OVER !! " );
	}
	else glColor3f( 0.5, 0.2, 0.0 );

	//glRasterPos2f( -2, -8 );
	//write_words( "Score : " ); 
	//write_number( score );

	glRasterPos2f( -3, -9 );
	write_words(" Play Time : ");
	write_number((int)playtime);

	glColor3f( 0.5, 0.9, 0.3 );
	glRasterPos2f( -8, -8 );
	write_words( "LEVEL : " ); 

	if( level<20 ) write_number( level );
	else write_words( "HELL MODE" );
}

	

void Game::DrawTeddy(){
	kuma = new Teddy( cursor_x, cursor_y, Teddy::down ); 
}

void Game::Idle()
{
	if(work == false && state != END ) {
		Display();
		return;
	}
	
	switch(state) {
	case INIT:
		Initialize();
		Display();
		break;
	case START:
		RunGame();
		break;
	case END:
		Display();
		break;
	}
}

void Game::Reshape(int w, int h)
{
	window_w = w;
	window_h = h;

	glViewport(0, 0, w, h);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho(-VIEW_PIX, VIEW_PIX, -VIEW_PIX, VIEW_PIX, -VIEW_PIX, VIEW_PIX);
	
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity();
}

void Game::SpecialKey(int key, int x, int y)
{
	if(work == false) return;
	if( teddy_moving == true) return;	// while moving, ignore input
	
	switch(key) {
	case GLUT_KEY_UP:
		if(cursor_y >= MAP_HEIGHT-1) break;	// bound check
		if(map.getTile(cursor_x, cursor_y + 1) == Tile::EMPTY){	// figure collision check
			map.setDirection(cursor_x, cursor_y, Tile::UP);
			figure_moving = true;
		}
		kuma->set_dest( Teddy::up );
		teddy_moving = true;
		cursor_y++;
		break;
	case GLUT_KEY_DOWN:
		if(cursor_y <= 0) break;
		if(map.getTile(cursor_x, cursor_y - 1) == Tile::EMPTY){
			map.setDirection(cursor_x, cursor_y, Tile::DOWN);
			figure_moving = true;
		}
		kuma->set_dest( Teddy::down );
		teddy_moving = true;
		cursor_y--;
		break;
	case GLUT_KEY_LEFT:
		if(cursor_x <= 0) break;
		if(map.getTile(cursor_x - 1, cursor_y) == Tile::EMPTY){
			map.setDirection(cursor_x, cursor_y, Tile::LEFT);
			figure_moving = true;
		}
		kuma->set_dest( Teddy::left );
		teddy_moving = true;
		cursor_x--;
		break;
	case GLUT_KEY_RIGHT:
		if(cursor_x >= MAP_WIDTH-1) break;
		if(map.getTile(cursor_x + 1, cursor_y) == Tile::EMPTY){
			map.setDirection(cursor_x, cursor_y, Tile::RIGHT);
			figure_moving = true;
		}
		kuma->set_dest( Teddy::right );
		cursor_x++;
		teddy_moving = true;
		break;
	}

	Idle();
}

void Game::KeyInput(unsigned char key, int x, int y)
{
	if(key == ' ') work = !work;

	else if(key == 'v' || key == 'V') static_view = !static_view;

	else if(key == 'a' || key == 'A') theta += VIEW_THETA;
	else if(key == 'd' || key == 'D') theta -= VIEW_THETA;
	else if(key == 'w' || key == 'W') phi += VIEW_PHI;
	else if(key == 's' || key == 'S') phi -= VIEW_PHI;
	if(phi >= LIMIT_PHI) phi = LIMIT_PHI;
	else if(phi <= -LIMIT_PHI) phi = -LIMIT_PHI;

	eye_x = cos(theta) * cos(phi);
	eye_y = sin(theta) * cos(phi);
	eye_z = sin(phi);

	Reshape(window_w, window_h);
	
	Idle();
}

void Game::Initialize()
{
	int x, y;
	srand((unsigned)timeGetTime());

	map.initMap();

	for(int i = 0; i < INIT_FIGURES; i++) {
		do {
			x = rand() % MAP_WIDTH;
			y = rand() % MAP_HEIGHT;
		} while(map.getTile(x, y) != Tile::EMPTY);
		map.setTile(x, y, (Tile::Type)(rand() % 3 + 1));
	}

	cursor_x = x;	// set teddy's initial position
	cursor_y = y;

	eye_x = cos(theta) * cos(phi);
	eye_y = sin(theta) * cos(phi);
	eye_z = sin(phi);

	oldtime = timeGetTime();
	state = START;
	work = false;	// to start with space bar
	DrawTeddy();
}

void Game::RunGame()
{
	unsigned time = timeGetTime();
	if((time - oldtime) < TIMEFACTOR) return;
	oldtime = time;

	figure_moving = map.Update();
	if( teddy_moving == true ) teddy_moving = kuma->Move();
	
	int temp_score = map.ScoreUpdate();
	score += map.show_combo() * map.show_combo() * temp_score * temp_score;
	playtime += TIMEFACTOR / 1000.0;

	if(checkGameOver() == true) {
		state = END;
		return;
	}

	// generate firuges
	int x, y, loop_limit = (MAP_WIDTH * MAP_HEIGHT * 2);
	//level = score / 5000;

	if(level > 20) level = 20;
	if(!(rand() % ( (INIT_DIFFICULTY - level*400 )/100))) {
		do {
			x = rand() % MAP_WIDTH;
			y = rand() % MAP_HEIGHT;
			loop_limit--;	// to avoid too many loops
		} while(map.getTile(x, y) != Tile::EMPTY && loop_limit > 0);
		if(loop_limit > 0) map.setTile(x, y, (Tile::Type)(rand() % 3 + 1));
	}

	Display();
}

void Game::DrawMap()
{
	glPushMatrix();
	glTranslatef(-(MAP_WIDTH * FIGURE_SIZE) / 2.0, -(MAP_HEIGHT * FIGURE_SIZE) / 2.0, 0);
	for(int i = 0; i < MAP_HEIGHT; i++) {
		for(int j = 0; j < MAP_WIDTH; j++) {
			glPushMatrix();
			setMatrixProperties(j, i);	// set the position and color
			switch(map.getTile(j, i)) {
			case Tile::SPHERE:
				glutWireSphere(FIGURE_SIZE/2.0, 10, 10);
				break;
			case Tile::CONE:
				glPushMatrix();
				glTranslatef(0, 0, -FIGURE_SIZE/2.0);
				glutWireCone(FIGURE_SIZE/2.0, FIGURE_SIZE, 10, 10);
				glPopMatrix();
				break;
			case Tile::CUBE:
				glutWireCube(FIGURE_SIZE);
				break;
			case Tile::EMPTY:
				glPushMatrix();
				glTranslatef(0, 0, -FIGURE_SIZE/8.0);
				glutSolidCube(FIGURE_SIZE/2.0);
				glPopMatrix();
				break;
			}
			glPopMatrix();
		}
	}
	glPopMatrix();
}

void Game::setMatrixProperties(int x, int y)
{
	glTranslatef(FIGURE_SIZE * (x + 0.5), FIGURE_SIZE * (y + 0.5), 0);
	if(map.isFlash(x, y) == true && map.isRed(x, y) == true)
		glColor3f(1, 0, 0);		// red
	else glColor3f(0, 0, 0);	// black
	if(map.isMoving(x, y) == true) {
		switch(map.getDirection(x, y)) {
		case Tile::UP:
			glTranslatef(0, -1.0 * map.getMoveState(x, y) * FIGURE_SIZE / MAX_TILE_MOVESTATE, 0);
			break;
		case Tile::LEFT:
			glTranslatef(map.getMoveState(x, y) * FIGURE_SIZE / MAX_TILE_MOVESTATE, 0, 0);
			break;
		case Tile::DOWN:
			glTranslatef(0, map.getMoveState(x, y) * FIGURE_SIZE / MAX_TILE_MOVESTATE, 0);
			break;
		case Tile::RIGHT:
			glTranslatef(-1.0 * map.getMoveState(x, y) * FIGURE_SIZE / MAX_TILE_MOVESTATE, 0, 0);
			break;
		}
	}
}

bool Game::checkGameOver()
{
	if( map.getTile( cursor_x, cursor_y ) == Tile::EMPTY )	return true;	// character밑에 firgure가 없을 때
			
	for(int i = 0; i < MAP_HEIGHT; i++)
		for(int j = 0; j < MAP_WIDTH; j++)
			if(map.getTile(j, i) == Tile::EMPTY || map.getTile(j, i) == Tile::GOING || map.isFlash(j, i) == true)
				return false;	// 가득찼을 때
	return true;
}
