/*
 * Statistics.h
 *
 *  Created on: 20.01.2013
 *      Author: Denis
 */

#ifndef STATISTICS_H_
#define STATISTICS_H_

#include <stdlib.h>

class Statistics {
public:
	static void enterLevel(const char* name);

	inline static void pauseLevel(){event(PAUSE_LEVEL);}
	inline static void resumeLevel(){event(RESUME_LEVEL);};
	inline static void leaveLevel(){event(LEAVE_LEVEL);};

	inline static void eatFood(){event(PACMAN_EAT_FOOD);};
	inline static void incLifes(){lifesToChangeCount = 1; event(PACMAN_LIFES_COUNT_CHANGED);};
	inline static void decLifes(){lifesToChangeCount = -1; event(PACMAN_LIFES_COUNT_CHANGED);};

	inline static bool isEatenFoodCountChaged(bool reset = true){
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

	inline static int getEatenFoodCount(){return eatedFoodCount;};
	inline static int getLifesCount(){return lifesCount;};

private:

	enum StatisticsState{
		IDLE,
		LEVEL_ENTERED,
		LEVEL_FINISHED,
		LEVEL_PAUSED
	};

	enum StatisticsEvent{
		NONE,
		ENTER_LEVEL,
		LEAVE_LEVEL,
		PAUSE_LEVEL,
		RESUME_LEVEL,
		PACMAN_EAT_FOOD,
		PACMAN_LIFES_COUNT_CHANGED
	};

	static StatisticsState state;

	static char* levelToEnterName;
	static char* levelName;

	static int eatedFoodCount;
	static int lifesCount;
	static int lifesToChangeCount;

	static bool lifesCountChanged;
	static bool eatenFoodCountChanged;

	static void event(StatisticsEvent e);

};

#endif /* STATISTICS_H_ */
