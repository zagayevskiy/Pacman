/*
 * Statistics.h
 *
 *  Created on: 20.01.2013
 *      Author: Denis
 */

#ifndef STATISTICS_H_
#define STATISTICS_H_

#include <stdlib.h>

#include "managers/Store.h"

class Statistics {
public:
	static void enterLevel(const char* name);

	inline static void pauseLevel(){event(PAUSE_LEVEL);}
	inline static void resumeLevel(){event(RESUME_LEVEL);};
	inline static void leaveLevel(){event(LEAVE_LEVEL);};
	inline static void winLevel(){event(WIN_LEVEL);};

	inline static void eatFood(){event(PACMAN_EAT_FOOD);};
	inline static void incLifes(){lifesToChangeCount = 1; event(PACMAN_LIFES_COUNT_CHANGED);};
	inline static void decLifes(){lifesToChangeCount = -1; event(PACMAN_LIFES_COUNT_CHANGED);};

	inline static bool isScoreChanged(bool reset = true){
		if(reset){
			bool b = scoreChanged;
			scoreChanged = false;
			return b;
		}
		return scoreChanged;
	};

	inline static bool isEatenFoodCountChanged(bool reset = true){
		if(reset){
			bool b = eatenFoodCountChanged;
			eatenFoodCountChanged = false;
			return b;
		}
		return eatenFoodCountChanged;
	};
	inline static bool isLifesCountChanged(bool reset = true){
		if(reset){
			bool b = lifesCountChanged;
			lifesCountChanged = false;
			return b;
		}
		return lifesCountChanged;
	};

	inline static int getScore(){return score;};
	inline static int getEatenFoodCount(){return eatedFoodCount;};
	inline static int getLifesCount(){return lifesCount;};

	static inline int getLevelRecord(){
		return levelName ? levelRecord : 0;
	}

private:

	enum StatisticsState{
		IDLE,
		LEVEL_ENTERED,
		LEVEL_PAUSED
	};

	enum StatisticsEvent{
		NONE = 0,
		ENTER_LEVEL = 1,
		LEAVE_LEVEL = 2,
		WIN_LEVEL = 3,
		PAUSE_LEVEL = 4,
		RESUME_LEVEL = 5,
		PACMAN_EAT_FOOD = 6,
		PACMAN_LIFES_COUNT_CHANGED = 7
	};

	static StatisticsState state;

	static char* levelToEnterName;
	static char* levelName;

	static int levelRecord;
	static int score;
	static int eatedFoodCount;
	static int lifesCount;
	static int lifesToChangeCount;

	static bool scoreChanged;
	static bool lifesCountChanged;
	static bool eatenFoodCountChanged;

	static void event(StatisticsEvent e);

};

#endif /* STATISTICS_H_ */
