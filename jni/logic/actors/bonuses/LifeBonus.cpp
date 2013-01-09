/*
 * LifeBonus.cpp
 *
 *  Created on: 09.01.2013
 *      Author: Denis
 */

#include "LifeBonus.h"

LifeBonus::LifeBonus(Game* _game, float _x, float _y, GLuint program): game(_game), animation(NULL) {
	x = _x + 0.5f;
	y = _y + 0.5f;
	radius = 0.5f;
	float tileSize = game->getTileSize();
	animation = new Animation(program, Art::getTexture(Art::TEXTURE_HEART), 4, 2, 2, 800.0, tileSize, tileSize);
	renderX = (x - radius)*tileSize;
	renderY = (y - radius)*tileSize;
}

bool LifeBonus::apply(Pacman* pacman){
	pacman->setLifes(pacman->getLifes() + 1);
	return true;
}

void LifeBonus::render(double elapsedTime){
	animation->render(elapsedTime, renderX, renderY);
}

LifeBonus::~LifeBonus() {
	if(animation){
		delete animation;
	}
}

