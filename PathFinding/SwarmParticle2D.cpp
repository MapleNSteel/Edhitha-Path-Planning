#include "SwarmParticle2D.h"
	// ---------- Constructors -------------------------
	// Constructor that takes an initial position as a Vec3
	SwarmParticle2D::SwarmParticle2D(){
		particleValue=0;

		initialise();
	}

	SwarmParticle2D::SwarmParticle2D(Vec3<long double> initialLocation)
		{
			particleValue=0;
			position = initialLocation; // Set the particle location

			initialise();
		}

		// Constructor that takes a position and a speed
	SwarmParticle2D::SwarmParticle2D(Vec3<long double> initialLocation, Vec3<long double> initialSpeed)
		{
			particleValue=0;
			position = initialLocation;
			velocity  = initialSpeed;

			initialise();
		}

		// ---------- Helper Methods and Getters & Setters -------------------------

		// Method to setup other properties of the particle
		void SwarmParticle2D::initialise()
		{
			// Set an entirely random colour (including random alpha value)
			colour.opaqueRandomise();

			// Randomise the size of the particle
			size = 1;
		}

		long double SwarmParticle2D::getRed()   { return colour.getRed();	 }
		long double SwarmParticle2D::getGreen() { return colour.getGreen(); }
		long double SwarmParticle2D::getBlue()  { return colour.getBlue();  }
		long double SwarmParticle2D::getAlpha() { return colour.getAlpha(); }

		void SwarmParticle2D::setAlpha(long double alpha){ colour.setAlpha(alpha);}

		void SwarmParticle2D::setColour(const Colour4f &theColour)
		{
			colour.setColour(theColour);
		}

		void SwarmParticle2D::interpolateColourWith(const Colour4f &theColour, long double theMixFactor)
		{
			colour.interpolateWith(theColour, theMixFactor);
		}

		void SwarmParticle2D::setInterpolatedColour(const Colour4f &sourceColour, const Colour4f &destinationColour, long double mixFactor)
		{
			colour.setInterpolatedColour(sourceColour, destinationColour, mixFactor);
		}
		void SwarmParticle2D::setParticleValue(long double particleValue){
			this->particleValue = particleValue;
		}

		void SwarmParticle2D::setParticleVelocity(Vec3<long double> velocity){
			this->velocity=velocity;
		}

		void SwarmParticle2D::setParticlePosition(Vec3<long double> position){
			this->position=position;
		}

		long double SwarmParticle2D::getParticleValue(){
			return particleValue;
		}
		Vec3<long double>* SwarmParticle2D::getParticleVelocity(){
			return &velocity;
		}
		Vec3<long double> SwarmParticle2D::getPosition()  { return position;        }
		long double SwarmParticle2D::getXLocation()       { return position.getX(); }
		long double SwarmParticle2D::getYLocation()       { return position.getY(); }
		long double SwarmParticle2D::getZLocation()       { return position.getZ(); }

		int SwarmParticle2D::getSize()              { return size;            }

		// ---------- Main Methods To Define Particle Behaviour -------------------------

		void SwarmParticle2D::draw(Vec3<long double> referenceFrame)
		{
			// Set the size of the point and draw it
			glPointSize(size);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBegin(GL_POINTS);
			// Set the colour and draw the particle
			glColor4f( colour.getRed(), colour.getGreen(), colour.getBlue(), colour.getAlpha() );
			glVertex2f(size*position.getX()+referenceFrame.getX(), size*position.getY()+referenceFrame.getY() );

			glEnd();
		}
