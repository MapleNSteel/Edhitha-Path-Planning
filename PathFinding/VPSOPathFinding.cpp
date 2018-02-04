/*
 * VPSO.h
 *
 *  Created on: Jan 31, 2016
 *      Author: arjun
 */
#include <cstdlib>
#include <iostream>
#include <tbb/tbb.h>

#include "boost/random.hpp"
#include "boost/generator_iterator.hpp"

#ifndef VPSO_H_
#define VPSO_H_

#include "../Utility Classes/General/Vec3.hpp"

#include "SwarmParticle2D.h"

using namespace tbb;
using namespace std;

namespace VPSOPathFinding{
template <typename T> inline void setVelocity(Vec3<T> destination,Vec3<T> * velocity, Vec3<T> position, Vec3<T> localPosition, Vec3<T> gBestPosition, T inertia, T socialFactor, T globalFactor, T maxSpeed){

	boost::mt19937 rng(time(0));
	boost::random::uniform_real_distribution< > uniformDistribution( 0.0, 1.0 );
	boost::variate_generator< boost::mt19937, boost::random::uniform_real_distribution< > >random(rng, uniformDistribution);

	Vec3<T>gravity=(destination-position);

	long double magnitude = gravity.getMagnitude();
	if(!isnan(magnitude))
		gravity*=1+(8000/(magnitude*magnitude));//<-contributes little
	else
		gravity*=maxSpeed;
	*velocity=gravity+*velocity*(inertia)+
			(localPosition-position) *(socialFactor*random())+
			(gBestPosition-position)*(globalFactor*random());

	if(maxSpeed!=0){

		if(velocity->getMagnitude()>= maxSpeed){
			velocity->normalise();
			*velocity = (*velocity)*maxSpeed;
		}
	}
}
template <typename T> inline Vec3<T> setPosition(Vec3<T> position, Vec3<T> * velocity,T deltaTime){

	return position + (*velocity)*deltaTime;
}
template <typename T> inline void update(SwarmParticle2D * particleSystem,SwarmParticle2D * localParticles,SwarmParticle2D * &gBest,Vec3 <T> destination,T inertia, T socialFactor, T globalFactor,int MAX_PARTICLES,int deltaTime,long double (*objectiveFunction) (Vec3<long double>),T maxSpeedGBest,T maxSpeedNeighbours,bool maximise){
	parallel_for(0,MAX_PARTICLES,[&](unsigned short i) {
		particleSystem[i].setParticlePosition(VPSOPathFinding::setPosition(particleSystem[i].getPosition(),particleSystem[i].getParticleVelocity(),((long double)deltaTime/1000)));
		particleSystem[i].setParticleValue(objectiveFunction(particleSystem[i].getPosition()));
	});
	// Iterate over all particles in the vector
	parallel_for(0,MAX_PARTICLES,[&](unsigned short i) {
		if((!maximise&&(particleSystem[i].getParticleValue() < localParticles[i].getParticleValue())) || (maximise&&(particleSystem[i].getParticleValue() > localParticles[i].getParticleValue()))){
			localParticles[i]=particleSystem[i];
		}
		if((!maximise&&(localParticles[i].getParticleValue()<gBest->getParticleValue()))||((maximise)&&(localParticles[i].getParticleValue() > gBest->getParticleValue())))
			gBest=&(localParticles[i]);//particleSystem yields guaranteed solution.
	});
	parallel_for(0,MAX_PARTICLES,[&](unsigned short i) {
		// ...update the particle position position and draw it.
		if(!(&particleSystem[i]==gBest)){
			VPSOPathFinding::setVelocity(destination,particleSystem[i].getParticleVelocity(),particleSystem[i].getPosition(),
					localParticles[i].getPosition(),gBest->getPosition(),inertia
					,((long double)(socialFactor*1000/deltaTime)),((long double)(globalFactor*1000/deltaTime)),maxSpeedNeighbours);
		}
		else{
			VPSOPathFinding::setVelocity(destination,particleSystem[i].getParticleVelocity(),particleSystem[i].getPosition(),
					localParticles[i].getPosition(),gBest->getPosition(),inertia
					,((long double)(socialFactor*1000/deltaTime)),((long double)(globalFactor*1000/deltaTime)),maxSpeedGBest);
		}
	}); // End of iterator loop
}
}
#endif /* VPSO_H_ */



