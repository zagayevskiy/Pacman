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
	virtual bool action(Action act, float x, float y);
	virtual EngineEvent getEvent() const;
	virtual void render(double elapsedTime);
	virtual void free();
	virtual ~Menu();

protected:
	List<Control*> controls;
	Control* lastActingControl;

	void init(){
		lastActingControl = NULL;
	}
};

#endif /* MENU_H_ */
