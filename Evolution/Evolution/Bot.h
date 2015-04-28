#pragma once

class Bot{
public:
	Bot();
	~Bot();
	void update();
	void render();
private:
	float x, y, z;
	bool shouldMoveBack;
	float xdir, zdir;
	void newMove();
	void undoMove();
};