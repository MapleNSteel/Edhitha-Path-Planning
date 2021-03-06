/*
 * GPSCoordinate.cpp
 *
 *  Created on: 8 Apr 2016
 *      Author: arjun
 */

#include "GPSCoordinate.h"

LocalCartesian GPSCoordinate::localCartesian;
Vec3<long double> GPSCoordinate::homeReference;
GPSCoordinate::GPSCoordinate():
Vec3<long double>(0,0,0){
	cartesianLocation.set(0,0,0);
}
GPSCoordinate::GPSCoordinate(long double latitude, long double longitude, long double altitude):
Vec3<long double>(latitude,longitude,altitude+homeReference.getZ()) {
	// TODO Auto-generated constructor stub

	double x,y,z;
	localCartesian.Forward(latitude, longitude, altitude+homeReference.getZ(), x, y, z);

	cartesianLocation.set((long double)x,(long double)y,(long double)z);
}

GPSCoordinate::GPSCoordinate(Vec3<long double> cartesianCoordinate):Vec3(){

	this->cartesianLocation = cartesianCoordinate;

	double latitude,longitude,altitude,x,y,z;
	x=(double)cartesianCoordinate.getX();
	y=(double)cartesianCoordinate.getY();
	z=(double)cartesianCoordinate.getZ();
	localCartesian.Reverse(x, y, z,latitude, longitude, altitude);

	this->setX(latitude);
	this->setY(longitude);
	this->setZ(altitude);
}

GPSCoordinate::~GPSCoordinate() {
	// TODO Auto-generated destructor stub
}
Vec3<long double> GPSCoordinate::getCartesian(){
	return cartesianLocation;
}
void GPSCoordinate::display(){
	std::cout<< this->getX() << "\t" << this->getY() << "\t" << this->getZ()-homeReference.getZ()<< std::endl;
}
