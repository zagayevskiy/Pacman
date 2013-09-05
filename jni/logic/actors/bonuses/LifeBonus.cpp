/*
 * LifeBonus.cpp
 *
 *  Created on: 09.01.2013
 *      Author: Denis
 */

#include "LifeBonus.h"
#include "managers/Store.h"

#define NAME_LIFE_BONUS_EXISTS "LifeBonus_%s_%d_exists"

LifeBonus::LifeBonus(Game* _game, float _x, float _y, GLuint program): game(_game){
	x = _x + 0.5f;
	y = _y + 0.5f;
	radius = 0.5f;
	float tileSize = game->getTileSize();
	pulse = new Pulsation((x - radius)*tileSize + game->getShiftX(), (y - radius)*tileSize + game->getShiftY(), tileSize,
							Art::getTexture(Art::TEXTURE_GREEN_GOO), 1.0f);
}

void LifeBonus::onLevelStart(){
	save(false);
}

void LifeBonus::save(){
	save(true);
}

void LifeBonus::save(bool exists){
	char* buffer = new char[64];

	sprintf(buffer, NAME_LIFE_BONUS_EXISTS, Art::getLevel(game->getLevelNumber())->name, getIndex());
	Store::saveBool(buffer, exists);

	delete[] buffer;
}

bool LifeBonus::shouldBeRemovedAfterLoading(){

	char* buffer = new char[64];
	sprintf(buffer, NAME_LIFE_BONUS_EXISTS, Art::getLevel(game->getLevelNumber())->name, getIndex());

	bool result = !Store::loadBool(buffer, true);

	delete[] buffer;

	return result;
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

