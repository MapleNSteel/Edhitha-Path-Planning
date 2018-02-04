/*
 * Cylinder.h
 *
 *  Created on: 18 Apr 2016
 *      Author: arjun
 */
#include "../Obstacle.h"
#include "../../../Utility Classes/General/Vec3.hpp"

#ifndef PATHFINDING_OBSTACLES_CYLINDER_CYLINDER_H_
#define PATHFINDING_OBSTACLES_CYLINDER_CYLINDER_H_

class Cylinder: public Obstacle {
private:
	double radius;
	double radiusAdmission;
	double height;
	double heightAdmission;

	Vec3<long double> normal;

public:
	Cylinder();
	Cylinder(Vec3<long double> initialLocation,double radius,double radiusAdmission,double height,double heightAdmission);
	Cylinder(long double latitude, long double longitude, long double altitude,double radius, double radiusAdmission,double height,double heightAdmission);
	virtual ~Cylinder();
	void setNormal();

	double getRadius();
	double getRadiusAdmission();
	double getHeight();

	bool isColliding(Vec3<long double> position,Vec3<long double> home);
	Vec3<long double> getProjectionVector(Vec3<long double> position,GPSCoordinate home);
	void draw(Vec3<long double> referenceFrame, GPSCoordinate home);
};
#endif /* PATHFINDING_OBSTACLES_CYLINDER_CYLINDER_H_ */
