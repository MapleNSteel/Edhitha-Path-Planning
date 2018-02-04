/*
 * Sphere.cpp
 *
 *  Created on: 10 Apr 2016
 *      Author: arjun
 */

#include "Sphere.h"

Sphere::Sphere():
Obstacle(){
	// TODO Auto-generated destructor stub
	this->admission=0;
	this->radius=0;
}

Sphere::Sphere(Vec3<long double> initialLocation,double radius,double admission):
Obstacle(initialLocation){
	// TODO Auto-generated constructor stub
	this->admission=admission;
	this->radius=radius;
}

Sphere::Sphere(long double latitude, long double longitude, long double altitude,double radius,double admission):
Obstacle(latitude,longitude,altitude){
	this->admission=admission;
	this->radius=radius;
}

Sphere::~Sphere() {
	// TODO Auto-generated destructor stub
}

double Sphere::getRadius(){
	return radius;
}

double Sphere::getAdmission(){
	return admission;
}
bool Sphere::isColliding(Vec3<long double> position,Vec3<long double> home){
	return getDistance(position,home)<=radius+admission;
}
long double Sphere::getDistance(Vec3<long double> position,GPSCoordinate home){
	return this->position.getDistance(position,(this->position.getCartesian()-home.getCartesian()));
}
void Sphere::draw(Vec3<long double> referenceFrame, GPSCoordinate home){

	float x = size*(position.getCartesian()-home.getCartesian()).getX()+referenceFrame.getX();
	float y = size*(position.getCartesian()-home.getCartesian()).getY()+referenceFrame.getY();

	glColor3f(1.0,0.0,0.0);

	glBegin(GL_LINE_LOOP);
	for(double d = 0; d < 2*3.141592654; d += 0.01) {
		glVertex2f(cos(d) * (size*radius) + x, sin(d) * (size*radius) + y);
    }
	glEnd();
}
