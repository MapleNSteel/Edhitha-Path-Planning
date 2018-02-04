/*
 * SwarmParticle2D.h
 *
 *  Created on: Jan 24, 2016
 *      Author: arjun
 */
#ifndef SWARMPARTICLE2D_H_
#define SWARMPARTICLE2D_H_
#include "../Utility Classes/General/Vec3.hpp"
#include "../Utility Classes/General/Colour4f.hpp"

#include <GL/glew.h>      // Include the extension wrangler to have access to enhanced OpenGL features, like pointsprites

class SwarmParticle2D
{
	protected:
	Vec3<long double> position;  // The current location of the particle
	Vec3<long double> velocity;     // The current speed of the particle

	Colour4f colour;       // The colour of the particle

	int size;              // The size of the particle i.e. how big the pointsprite will be drawn

	long double particleValue;

	public:

	SwarmParticle2D();
	SwarmParticle2D(Vec3<long double> initialLocation);
	SwarmParticle2D(Vec3<long double> initialLocation, Vec3<long double> initialSpeed);
	void initialise();

	long double getRed();
	long double getGreen();
	long double getBlue();
	long double getAlpha();

	void setAlpha(long double alpha);

	void setColour(const Colour4f &theColour);

	void interpolateColourWith(const Colour4f &theColour, long double theMixFactor);

	void setInterpolatedColour(const Colour4f &sourceColour, const Colour4f &destinationColour, long double mixFactor);

	static void setBestValue(long double bestValue);
	void setParticleValue(long double particleValue);
	void setParticleVelocity(Vec3<long double> velocity);
	void setParticlePosition(Vec3<long double> position);

	long double getParticleValue();
	Vec3<long double>* getParticleVelocity();
	Vec3<long double> getPosition();

	long double getXLocation();
	long double getYLocation();
	long double getZLocation();

	int getSize();

			// ---------- Main Methods To Define Particle Behaviour -------------------------
	void draw(Vec3<long double> referenceFrame);
};


#endif /* SWARMPARTICLE2D_H_ */
