/*
 * Statistics.cpp
 *
 *  Created on: 20.01.2013
 *      Author: Denis
 */

#include "Statistics.h"
#include "logic/actors/Pacman.h"

Statistics::StatisticsState Statistics::state = IDLE;

int Statistics::levelNumber = 0;
int Statistics::levelToEnterNumber = 0;

int Statistics::levelRecord = 0;
int Statistics::score = 0;
int Statistics::lifesCount = 0;
int Statistics::eatedFoodCount = 0;
int Statistics::lifesToChangeCount = 0;
int Statistics::foodCost = 10;

bool Statistics::scoreChanged = true;
bool Statistics::eatenFoodCountChanged = true;
bool Statistics::lifesCountChanged = true;
bool Statistics::foodCostChanged = DEFAULT_FOOD_COST;
bool Statistics::levelPassedWithRecord = false;

double Statistics::elapsedTime = 0.0;

#define NAME_STATE "Stats_state"
#define NAME_LEVEL_NUMBER "Stats_lvl_number"
#define NAME_SCORE "Stats_score"
#define NAME_LIFES_COUNT "Stats_lfs_cnt"
#define NAME_EATED_FOOD_COUNT "Stats_eated_food_cnt"
#define NAME_LIFES_TO_CHANGE_COUNT "Stats_lfs2chng_cnt"
#define NAME_FOOD_COST "Stats_food_cost"

#define NAME_LEVEL_PASSED_WITH_RECORD "Stats_lvl_pssd_rec"

#define NAME_ELAPSED_TIME "Stats_etime"

void Statistics::save(){
	Store::saveInt(NAME_STATE, state);
	Store::saveInt(NAME_LEVEL_NUMBER, levelNumber);
	Store::saveInt(NAME_SCORE, score);
	Store::saveInt(NAME_LIFES_COUNT, lifesCount);
	Store::saveInt(NAME_EATED_FOOD_COUNT, eatedFoodCount);
	Store::saveInt(NAME_LIFES_TO_CHANGE_COUNT, lifesToChangeCount);
	Store::saveInt(NAME_FOOD_COST, foodCost);

	Store::saveBool(NAME_LEVEL_PASSED_WITH_RECORD, levelPassedWithRecord);

	Store::saveFloat(NAME_ELAPSED_TIME, static_cast<float>(elapsedTime));

}

void Statistics::load(){
	state = static_cast<StatisticsState>(Store::loadInt(NAME_STATE, state));
	levelNumber = Store::loadInt(NAME_LEVEL_NUMBER, levelNumber);
	score = Store::loadInt(NAME_SCORE, score);
	lifesCount = Store::loadInt(NAME_LIFES_COUNT, lifesCount);
	eatedFoodCount = Store::loadInt(NAME_EATED_FOOD_COUNT, eatedFoodCount);
	lifesToChangeCount = Store::loadInt(NAME_LIFES_TO_CHANGE_COUNT, lifesToChangeCount);
	foodCost = Store::loadInt(NAME_FOOD_COST, foodCost);

	scoreChanged = true;
	eatenFoodCountChanged = true;
	lifesCountChanged = true;
	foodCostChanged = true;
	levelPassedWithRecord = Store::loadBool(NAME_LEVEL_PASSED_WITH_RECORD, levelPassedWithRecord);

	elapsedTime = static_cast<double>(Store::loadFloat(NAME_ELAPSED_TIME, elapsedTime));
}

void Statistics::step(double time){
	if(foodCost > MIN_FOOD_COST && state == LEVEL_ENTERED){
		elapsedTime += time;
		if(elapsedTime >= FOOD_COST_CHANGE_TIME_DELTA){
			elapsedTime = 0.0;
			foodCost -= FOOD_COST_DELTA;
			if(foodCost < MIN_FOOD_COST){
				foodCost = MIN_FOOD_COST;
			}
			foodCostChanged = true;
		}
	}
}

void Statistics::event(StatisticsEvent e){
	LOGI("Statistics::event(%d)", e);
	switch(state){
		case IDLE:{
			if(e == ENTER_LEVEL){
				levelNumber = levelToEnterNumber;
				char* levelName = Art::getLevel(levelToEnterNumber)->name;
				score = eatedFoodCount = 0;
				lifesCount = Pacman::DEFAULT_LIFES_COUNT;
				scoreChanged = lifesCountChanged = eatenFoodCountChanged = true;
				levelRecord = Store::loadInt(levelName, 0);
				foodCost = DEFAULT_FOOD_COST;
				state = LEVEL_ENTERED;
				LOGI("Statistics::state=LEVEL_ENTERED");
			}
		}break;

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

				case WIN_LEVEL:{
					state = IDLE;
					score += lifesCount*DEFAULT_LIFE_COST;
					scoreChanged = true;
					char* levelName = Art::getLevel(levelNumber)->name;
					if(score > Store::loadInt(levelName, 0)){
						LOGI("Statistics: New Record! Level:%s, Score:%d", levelName, score);
						levelPassedWithRecord = true;
						Store::saveInt(levelName, score);
						levelRecord = score;
					}else{
						levelPassedWithRecord = false;
					}
					LOGI("Statistics::state=IDLE");
				}break;

				case PACMAN_EAT_FOOD:
					++eatedFoodCount;
					score += foodCost;
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


void Statistics::enterLevel(int number){
	levelToEnterNumber = number;

	event(ENTER_LEVEL);
}
