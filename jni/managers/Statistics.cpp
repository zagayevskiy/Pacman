/*
 * Statistics.cpp
 *
 *  Created on: 20.01.2013
 *      Author: Denis
 */

#include "Statistics.h"
#include "logic/actors/Pacman.h"

Statistics::StatisticsState Statistics::state = IDLE;

char* Statistics::levelName = NULL;
char* Statistics::levelToEnterName = NULL;

int Statistics::levelRecord = 0;
int Statistics::score = 0;
int Statistics::lifesCount = 0;
int Statistics::eatedFoodCount = 0;
int Statistics::lifesToChangeCount = 0;

bool Statistics::scoreChanged = true;
bool Statistics::eatenFoodCountChanged = true;
bool Statistics::lifesCountChanged = true;

void Statistics::event(StatisticsEvent e){
	LOGI("Statistics::event(%d)", e);
	switch(state){
		case IDLE:
			if(e == ENTER_LEVEL){
				levelName = levelToEnterName;
				score = eatedFoodCount = 0;
				lifesCount = Pacman::DEFAULT_LIFES_COUNT;
				scoreChanged = lifesCountChanged = eatenFoodCountChanged = true;
				levelRecord = Store::loadInt(levelName, 0);
				state = LEVEL_ENTERED;
				LOGI("Statistics::state=LEVEL_ENTERED");
			}
		break;

		case LEVEL_ENTERED:
			switch(e){
				case PAUSE_LEVEL:
					state = LEVEL_PAUSED;
					LOGI("Statistics::state=LEVEL_PAUSED");
				break;

				case LEAVE_LEVEL:
					state = IDLE;
					LOGI("Statistics::state=IDLE");
				break;

				case WIN_LEVEL:
					state=IDLE;
					score += lifesCount*100;
					scoreChanged = true;
					if(score > Store::loadInt(levelName, 0)){
						LOGI("Statistics: New Record! Level:%s, Score:%d", levelName, score);
						Store::saveInt(levelName, score);
					}
					LOGI("Statistics::state=IDLE");
				break;

				case PACMAN_EAT_FOOD:
					++eatedFoodCount;
					score += 10;
					scoreChanged = eatenFoodCountChanged = true;
				break;

				case PACMAN_LIFES_COUNT_CHANGED:
					lifesCount += lifesToChangeCount;
					lifesCountChanged = true;
				break;

				default: break;
			}
		break;

		case LEVEL_PAUSED:
			switch(e){
				case RESUME_LEVEL:
					state = LEVEL_ENTERED;
					LOGI("Statistics::state=LEVEL_ENTERED");
				break;

				case LEAVE_LEVEL:
					state = IDLE;
					LOGI("Statistics::state=IDLE");
				break;

				default: break;
			}
		break;

		default:
			LOGE("Statistics::state:default");
		break;
	}
}


void Statistics::enterLevel(const char* name){
	if(levelToEnterName){
		delete[] levelToEnterName;
		levelToEnterName = NULL;
	}
	if(name){
		levelToEnterName = new char[strlen(name) + 1];
		strcpy(levelToEnterName, name);
	}

	event(ENTER_LEVEL);
}
