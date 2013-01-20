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
int Statistics::lifesCount = 0;
int Statistics::eatedFoodCount = 0;
int Statistics::lifesToChangeCount = 0;

bool Statistics::eatenFoodCountChanged = false;
bool Statistics::lifesCountChanged = false;

void Statistics::event(StatisticsEvent e){
	switch(state){
		case IDLE:
			if(e == ENTER_LEVEL){
				levelName = levelToEnterName;
				eatedFoodCount = 0;
				lifesCount = Pacman::DEFAULT_LIFES_COUNT;
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
					state = LEVEL_FINISHED;
					LOGI("Statistics::state=LEVEL_FINISHED");
				break;

				case PACMAN_EAT_FOOD:
					++eatedFoodCount;
					eatenFoodCountChanged = true;
					LOGI("Statistics::eatedFoodCount=%d", eatedFoodCount);
				break;

				case PACMAN_LIFES_COUNT_CHANGED:
					lifesCount += lifesToChangeCount;
					lifesCountChanged = true;
					LOGI("Statistics::lifesCount=%d", lifesCount);
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
					state = LEVEL_FINISHED;
					LOGI("Statistics::state=LEVEL_FINISHED");
				break;

				default: break;
			}
		break;

		case LEVEL_FINISHED:

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
}
