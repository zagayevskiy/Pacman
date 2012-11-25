/*
 * Menu.cpp
 *
 *  Created on: 10.11.2012
 *      Author: Denis Zagayevskiy
 */

#include "Menu.h"

bool Menu::action(Action act, float x, float y){
	if(lastActingControl){
		if(lastActingControl->action(act, x, y)){
			return true;
		}
	}else{
		Control* control;
		int n = 0;
		bool exists = controls.getHead(control);
		while(exists){
			if(control->action(act, x, y)){
				lastActingControl = control;
				lastActingControlNumber = n;
				return true;
			}
			exists = controls.getNext(control);
			++n;
		}
	}
	lastActingControl = NULL;
	lastActingControlNumber = LAST_CONTROL_NONE;
	return false;
}

EngineEvent Menu::getEvent() const{
	return lastActingControl ? lastActingControl->getEvent() : EVENT_NONE;
}

void Menu::render(double elapsedTime){
	Control* control;
	bool exists = controls.getHead(control);
	while(exists){
		control->render(elapsedTime);
		exists = controls.getNext(control);
	}
}

void Menu::free(){
	if(!controls.isEmpty()){
		Control* control;
		bool exists = controls.getHead(control);
		while(exists){
			delete control;
		}
		controls.clear();
	}
}

Menu::~Menu() {
	free();
}

