/*
 * LifeBonus.cpp
 *
 *  Created on: 09.01.2013
 *      Author: Denis
 */

#include "LifeBonus.h"

LifeBonus::LifeBonus(Game* _game, float _x, float _y, GLuint program): game(_game){
	x = _x + 0.5f;
	y = _y + 0.5f;
	radius = 0.5f;
	float tileSize = game->getTileSize();
	pulse = new Pulsation((x - radius + 0.1)*tileSize + game->getShiftX(), (y - radius + 0.1)*tileSize + game->getShiftY(), tileSize*0.8,
							Art::getTexture(Art::TEXTURE_GREEN_GOO), 1.0f);
	//renderX = (x - radius)*tileSize + game->getShiftX();
	//renderY = (y - radius)*tileSize + game->getShiftY();
}

bool LifeBonus::apply(Pacman* pacman){
	pacman->incLifes();
	Audio::playSound(Art::SOUND_LIFE);
	return true;
}

void LifeBonus::render(double elapsedTime){
	pulse->render(elapsedTime);
}

LifeBonus::~LifeBonus() {
	if(pulse){
		delete pulse;
	}
}

