/*
 * Cylinder.cpp
 *
 *  Created on: 18 Apr 2016
 *      Author: arjun
 */
#include "Cylinder.h"

Cylinder::Cylinder():
Obstacle(){
	// TODO Auto-generated destructor stub
	this->radius=0;
	this->radiusAdmission=0;
	this->height=0;
	this->heightAdmission=0;
}

Cylinder::Cylinder(Vec3<long double> initialLocation,double radius,double radiusAdmission,double height,double heightAdmission):
Obstacle(initialLocation){
	// TODO Auto-generated constructor stub
	this->radius=radius;
	this->radiusAdmission=radiusAdmission;
	this->height=height;
	this->heightAdmission=heightAdmission;

	setNormal();
}

Cylinder::Cylinder(long double latitude, long double longitude, long double altitude,double radius, double radiusAdmission,double height,double heightAdmission):
Obstacle(latitude,longitude,altitude){
	this->radius=radius;
	this->radiusAdmission=radiusAdmission;
	this->height=height;
	this->heightAdmission=heightAdmission;

	setNormal();
}
void Cylinder::setNormal(){

	GPSCoordinate temp;

	temp=GPSCoordinate(temp.getX(),temp.getY(),temp.getZ()+1);
	temp=temp.getCartesian()-this->position.getCartesian();
	temp.normalise();

	this->normal = temp;
}
Cylinder::~Cylinder() {
	// TODO Auto-generated destructor stub
}

double Cylinder::getRadius(){
	return radius;
}

double Cylinder::getRadiusAdmission(){
	return radiusAdmission;
}
double Cylinder::getHeight(){
	return height;
}
bool Cylinder::isColliding(Vec3<long double> position,Vec3<long double> home){

	Vec3<long double> projectionVector=getProjectionVector(position,home);
	return projectionVector.getX() <= radius+radiusAdmission && projectionVector.getZ() <= height+heightAdmission;
}
Vec3<long double> Cylinder::getProjectionVector(Vec3<long double> position,GPSCoordinate home){

	Vec3<long double> temp;
	temp=position-(this->position.getCartesian()-home.getCartesian());//vector from base.

	long double heightProjection=temp.dotProduct(this->normal);
	long double radialDistance = (temp-this->normal*heightProjection).getMagnitude();

	temp=Vec3<long double>(radialDistance,0,heightProjection);

	return temp;//radialDistance, angle(Unnecessary, height.
}
void Cylinder::draw(Vec3<long double> referenceFrame, GPSCoordinate home){

	float x = size*(position.getCartesian()-home.getCartesian()).getX()+referenceFrame.getX();
	float y = size*(position.getCartesian()-home.getCartesian()).getY()+referenceFrame.getY();

	glColor3f(1.0,0.0,0.0);

	glBegin(GL_LINE_LOOP);
	for(double d = 0; d < 2*3.141592654; d += 0.01) {
		glVertex2f(cos(d) * (size*radius) + x, sin(d) * (size*radius) + y);
    }
	glEnd();
}
