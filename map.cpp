// map.cpp

#include "map.h"

extern int level;

void Map::initMap()	// it is used as Constructor
{
	// memory allocation and initialzing
	tile = new Tile*[MAP_HEIGHT];
	for(int i = 0; i < MAP_HEIGHT; i++) {
		tile[i] = new Tile[MAP_WIDTH];
		for(int j = 0; j < MAP_WIDTH; j++) {
			tile[i][j].isflash = false;
			tile[i][j].isred = false;
			tile[i][j].ismoving = false;
			tile[i][j].movestate = MAX_TILE_MOVESTATE;
			tile[i][j].type = Tile::EMPTY;
			tile[i][j].removestate = MAX_REMOVESTATE;
		}
	}
	max_combo = 0;
}

Map::~Map()
{
	if(tile != 0) {
		for(int i = 0; i < MAP_HEIGHT; i++)
			if(tile[i] != 0) delete[] tile[i];
		delete[] tile;
	}
}

Tile::Type Map::getTile(int x, int y)
{
	return tile[y][x].type;
}

void Map::setTile(int x, int y, Tile::Type type)
{
	tile[y][x].type = type;
}

void Map::moveTile(int src_x, int src_y, int dst_x, int dst_y)
{
	if(src_x < 0 || src_x >= MAP_WIDTH) return;
	if(dst_x < 0 || dst_x >= MAP_WIDTH) return;
	if(src_y < 0 || src_y >= MAP_HEIGHT) return;
	if(dst_y < 0 || dst_y >= MAP_HEIGHT) return;

	tile[dst_y][dst_x] = tile[src_y][src_x];	// 타일을 덮어 씀
	
	// 원본 타일은 초기화, except type
	tile[src_y][src_x].isflash = false;
	tile[src_y][src_x].ismoving = false;
	tile[src_y][src_x].isred = false;
	tile[src_y][src_x].movestate = MAX_TILE_MOVESTATE;
	tile[src_y][src_x].type = Tile::GOING;	// something going from herew
	tile[src_y][src_x].removestate = MAX_REMOVESTATE;
}

bool Map::isFlash(int x, int y)
{
	return tile[y][x].isflash;
}

bool Map::isRed(int x, int y)
{
	return tile[y][x].isred;
}

bool Map::isMoving(int x, int y)
{
	return tile[y][x].ismoving;
}

Tile::Direction Map::getDirection(int x, int y)
{
	return tile[y][x].direction;
}

int Map::getMoveState(int x, int y)
{
	return tile[y][x].movestate;
}

void Map::setDirection(int x, int y, Tile::Direction direction)
{
	int src_x = x, src_y = y;
	switch(direction) {
	case Tile::UP:
		tile[y][x].type = (Tile::Type)(tile[y][x].type - 1);
		if(tile[y][x].type < Tile::SPHERE)
			tile[y][x].type = Tile::CUBE;
		y++;
		break;
	case Tile::DOWN:
		tile[y][x].type = (Tile::Type)(tile[y][x].type + 1);
		if(tile[y][x].type > Tile::CUBE)
			tile[y][x].type = Tile::SPHERE;
		y--;
		break;
	case Tile::LEFT:
		tile[y][x].type = (Tile::Type)(tile[y][x].type - 1);
		if(tile[y][x].type < Tile::SPHERE)
			tile[y][x].type = Tile::CUBE;
		x--;
		break;
	case Tile::RIGHT:
		tile[y][x].type = (Tile::Type)(tile[y][x].type + 1);
		if(tile[y][x].type > Tile::CUBE)
			tile[y][x].type = Tile::SPHERE;
		x++;
		break;
	}
	moveTile(src_x, src_y, x, y);
	tile[y][x].direction = direction;
	tile[y][x].ismoving = true;
}

bool Map::Update()
{
	bool flag = false;	// return value
	int i;
	addscore = 0;	// score to add

	for(i = 0; i < MAP_HEIGHT; i++)
		for(int j = 0; j < MAP_WIDTH; j++) {
			if(tile[i][j].ismoving == true) {
				tile[i][j].movestate--;
				if(tile[i][j].movestate <= 0) {
					tile[i][j].movestate = MAX_TILE_MOVESTATE;
					tile[i][j].ismoving = false;
					switch(tile[i][j].direction) {
					case Tile::UP:
						tile[i-1][j].type = Tile::EMPTY;
						break;
					case Tile::LEFT:
						tile[i][j+1].type = Tile::EMPTY;
						break;
					case Tile::DOWN:
						tile[i+1][j].type = Tile::EMPTY;
						break;
					case Tile::RIGHT:
						tile[i][j-1].type = Tile::EMPTY;
						break;
					}
				}
				else flag = true;
			}
		}

	for(i = 0; i < MAP_HEIGHT; i++)
		for(int j = 0; j < MAP_WIDTH; j++) {
			int tempnumber = 1;
			if(tile[i][j].type != Tile::EMPTY && tile[i][j].type != Tile::GOING && 
				AdjacencyTest(j, i, tile[i][j].type, Tile::NONE, tempnumber) == true)
				tile[i][j].isflash = true;	// 같은 도형이 4개가 인접할 경우

			if(tile[i][j].isflash == true) {
				tile[i][j].isred = !(tile[i][j].isred);
				tile[i][j].removestate--;
			}

			if(tile[i][j].removestate - level/2 <= 0) {	// remove figures
				tile[i][j].isflash = false;
				tile[i][j].isred = false;
				tile[i][j].ismoving = false;
				tile[i][j].movestate = MAX_TILE_MOVESTATE;
				tile[i][j].type = Tile::EMPTY;
				tile[i][j].removestate = MAX_REMOVESTATE;
				addscore++;
				is_combo = COMBO_TERM;
				combo++;
				if( combo > max_combo ) max_combo = combo;
			}
		}

	is_combo--;
	if( is_combo == 0 ) combo = 0;
	return flag;
}

bool Map::AdjacencyTest(int x, int y, Tile::Type type, Tile::Direction from, int& count)
{
	if(tile[y][x].type == Tile::EMPTY || tile[y][x].type == Tile::GOING)
		return false;
	if(tile[y][x].ismoving == true) return false;
	if(count >= 4) return true;

	if(from != Tile::RIGHT && (x < MAP_WIDTH-1) && tile[y][x+1].type == type && tile[y][x+1].ismoving == false) {
		count++;
		AdjacencyTest(x+1, y, type, Tile::LEFT, count);
	}
	if(from != Tile::UP && (y < MAP_HEIGHT-1) && tile[y+1][x].type == type && tile[y+1][x].ismoving == false) {
		count++;
		AdjacencyTest(x, y+1, type, Tile::DOWN, count);
	}
	if(from != Tile::LEFT && (x > 0) && tile[y][x-1].type == type && tile[y][x-1].ismoving == false) {
		count++;
		AdjacencyTest(x-1, y, type, Tile::RIGHT, count);
	}
	if(from != Tile::DOWN && (y > 0) && tile[y-1][x].type == type && tile[y-1][x].ismoving == false) {
		count++;
		AdjacencyTest(x, y-1, type, Tile::UP, count);
	}

	if(count >= 4) return true;
	else return false;
}

int Map::ScoreUpdate()
{
	return addscore;
}

int Map::show_combo(){
	return combo;
}
int Map::show_combomx(){
	return max_combo;
}
