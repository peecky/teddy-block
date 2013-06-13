// map.h

#ifndef __pi_piano_MAP_H__
#define __pi_piano_MAP_H__

#define MAP_WIDTH 6
#define MAP_HEIGHT 6
#define MAX_TILE_MOVESTATE 10
#define MAX_REMOVESTATE 120

#define COMBO_TERM 200

struct Tile {
	enum Type {EMPTY = 0, SPHERE = 1, CONE = 2, CUBE = 3, GOING = 4};
	Type type;
	bool isflash;	// 반짝이는가?
	bool isred;	// red/black Toggle
	bool ismoving;
	enum Direction {UP, LEFT, DOWN, RIGHT, NONE};
	Direction direction;
	int movestate;
	int removestate;
};

class Map {
public:
	void initMap();	// be used for constructor
	~Map();
	Tile::Type getTile(int x, int y);
	bool isFlash(int x, int y);
	bool isRed(int x, int y);
	bool isMoving(int x, int y);
	Tile::Direction getDirection(int x, int y);
	int getMoveState(int x, int y);
	void setTile(int x, int y, Tile::Type type);
	void setDirection(int x, int y, Tile::Direction);
	void moveTile(int src_x, int src_y, int dst_x, int dxt_y);

	bool Update();	/* update move state, remove state, score, combo, is_combo
	if moving is end then return false */
	int ScoreUpdate();
	int show_combo();
	int show_combomx();
private:
	Tile **tile;
	int addscore;
	int combo;
	int is_combo;
	int max_combo;
	bool AdjacencyTest(int x, int y, Tile::Type, Tile::Direction from, int& count);	// 같은 도형 4개 인접성 체크
};

#endif