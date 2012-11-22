/*
 * Menu.h
 *
 *  Created on: 10.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef MENU_H_
#define MENU_H_

#include "graphics/ui/Control.h"

#include "templates/list.h"
#include "actions_events.h"

class Menu: public Control {
public:

	static const int LAST_CONTROL_NONE = -1;

	Menu(){
		lastActingControl = NULL;
		lastActingControlNumber = LAST_CONTROL_NONE;
	}

	virtual bool action(Action act, float x, float y);
	virtual EngineEvent getEvent() const;
	virtual void render(double elapsedTime);
	virtual void free();
	virtual ~Menu();

protected:
	List<Control*> controls;
	Control* lastActingControl;
	int lastActingControlNumber;
};

#endif /* MENU_H_ */
