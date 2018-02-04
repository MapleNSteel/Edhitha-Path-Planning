/*
 * Obstacle.cpp
 *
 *  Created on: 23 Feb 2016
 *      Author: arjun
 */

#include "../Obstacles/Obstacle.h"

Obstacle::Obstacle():position(0,0,0){
	size=1;
}
Obstacle::Obstacle(long double latitude, long double longitude, long double altitude):position(latitude,longitude,altitude){
	size=1;
}
Obstacle::Obstacle(Vec3<long double> initialPosition):position(initialPosition){
	size=1;
}

Obstacle::~Obstacle() {
	// TODO Auto-generated destructor stub
}

GPSCoordinate Obstacle::getPosition(){
	return this->position;
}
GPSCoordinate Obstacle::getForce(){
	return position;
}
