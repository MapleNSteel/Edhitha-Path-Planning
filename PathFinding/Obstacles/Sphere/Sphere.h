/*
 * Sphere.h
 *
 *  Created on: 10 Apr 2016
 *      Author: arjun
 */
#include "../Obstacle.h"
#include "../../../Utility Classes/General/Vec3.hpp"

#ifndef PATHFINDING_OBSTACLES_SPHERE_SPHERE_H_
#define PATHFINDING_OBSTACLES_SPHERE_SPHERE_H_

class Sphere: public Obstacle {
private:
	double radius;
	double admission;

public:
	Sphere();
	Sphere(Vec3<long double> initialLocation,double radius,double admission);
	Sphere(long double latitude, long double longitude, long double altitude,double radius,double admission);
	virtual ~Sphere();

	double getRadius();
	double getAdmission();

	bool isColliding(Vec3<long double> position,Vec3<long double> home);
	long double getDistance(Vec3<long double> position,GPSCoordinate home);

	void draw(Vec3<long double> referenceFrame, GPSCoordinate home);
};

#endif /* PATHFINDING_OBSTACLES_SPHERE_SPHERE_H_ */
