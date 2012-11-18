/*
 * Control.h
 *
 *  Created on: 03.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "graphics/IRenderable.h"
#include "log.h"
#include "actions_events.h"


class Control: public IRenderable {
public:

	virtual bool action(Action act, float x, float y) = 0;
	virtual EngineEvent getEvent() const = 0;
	virtual ~Control();

protected:
	Action lastAction;
};

#endif /* CONTROL_H_ */
