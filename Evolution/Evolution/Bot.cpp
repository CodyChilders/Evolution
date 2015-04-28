#include <cstdlib>
#include <cstdio>
#include <glut.h>
#include "Bot.h"
#include "TerrainGen.h"

Bot::Bot(){
	x = rand() % 100;
	z = rand() % 100;
	y = getHeightAtPoint(x, z);
	shouldMoveBack = false;
}

Bot::~Bot(){

}

void Bot::newMove(){
	//hold on to current height for comparison
	float height = y;
	bool moveX = rand() % 2 == 0 ? true : false;
	float newDir = rand() % 2 ? -1 : 1;
	if(moveX){
		x += newDir;
	}else{
		y += newDir;
	}
	//update height
	y = getHeightAtPoint(x, z);
	if(y < height){
		shouldMoveBack = true;
		xdir = moveX ? newDir : 0;
		zdir = moveX ? 0 : newDir;
	}else{
		shouldMoveBack = false;
	}
}

void Bot::undoMove(){
	x -= xdir;
	z -= zdir;
	shouldMoveBack = false;
}

void Bot::update(){
	shouldMoveBack ? undoMove() : newMove();
}

void Bot::render(){
	glPushMatrix();{
		glColor3f(0, 0, 1);
		glTranslatef(x, currentRenderHeight(x, z), z);
		glutSolidSphere(0.75, 30, 30);
	}glPopMatrix();
}