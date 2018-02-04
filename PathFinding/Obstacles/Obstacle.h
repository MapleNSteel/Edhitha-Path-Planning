/*
 * Obstacle.h
 *
 *  Created on: 23 Feb 2016
 *      Author: arjun
 */

#ifndef OBSTACLE_H_
#define OBSTACLE_H_

#include "../../Utility Classes/General/Colour4f.hpp"
#include "../../Utility Classes/General/Vec3.hpp"
#include "../../Utility Classes/GPSCoordinate.h"

#include <math.h>
#include <iostream>

#include <GL/glew.h>      // Include the extension wrangler to have access to enhanced OpenGL features, like pointsprites

class Obstacle {
	protected:
	GPSCoordinate position;  // The current location of the obstacle
	int size;

	public:

	Obstacle();
	Obstacle(Vec3<long double> initialLocation);
	Obstacle(long double latitude, long double longitude, long double altitude);

	virtual ~Obstacle();

	GPSCoordinate getPosition();
	GPSCoordinate getForce();

	// ---------- Main Methods To Define Particle Behaviour -------------------------
	void draw(Vec3<long double> referenceFrame, GPSCoordinate home);

	bool isColliding(Vec3<long double> position,Vec3<long double> home);
};
#endif /* OBSTRUCTION_H_ */
