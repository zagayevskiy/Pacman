/*
 * IRenderable.h
 *
 *  Created on: 03.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef IRENDERABLE_H_
#define IRENDERABLE_H_

class IRenderable {
public:

	void virtual render(double elapsedTime) = 0;

	virtual ~IRenderable();
};

#endif /* IRENDERABLE_H_ */
