#ifndef COLOUR4F_HPP
#define COLOUR4F_HPP

#include <cstdlib>
class Colour4f
{
	private:
		float red, green, blue, alpha;

	public:
		Colour4f()
		{
			red   = 1.0f;
			green = 1.0f;
			blue  = 1.0f;
			alpha = 1.0f;
		}

		Colour4f(float redValue, float greenValue, float blueValue, float alphaValue = 1.0f)
		{
			red   = redValue;
			green = greenValue;
			blue  = blueValue;
			alpha = alphaValue;
		}

		// Getters
		float getRed()   { return red;   }
		float getGreen() { return green; }
		float getBlue()  { return blue;  }
		float getAlpha() { return alpha; }

		void setColour(const Colour4f &theColour)
		{
			red   = theColour.red;
			green = theColour.green;
			blue  = theColour.blue;
			alpha = theColour.alpha;
		}

		void setAlpha(long double alphaValue){
			alpha=alphaValue;
		}

		void fullyRandomise()
		{
			red   = ((float)rand()/(float)RAND_MAX);
			green = ((float)rand()/(float)RAND_MAX);
			blue  = ((float)rand()/(float)RAND_MAX);
			alpha = 0.8*((float)rand()/(float)RAND_MAX);
		}

		void opaqueRandomise()
		{
			red   = 0.0;
			green = 1.0;
			blue  = 1.0;
			alpha = 1.0f;
		}

		// Method to interpolate the current colour with another specified colour with relation to a mix factor
		// 0.0 means don't mix at all, 1.0 means fully replace colour with destination colour
		void interpolateWith(const Colour4f &destination, const float mixFactor)
		{
			red = red * mixFactor + (destination.red * (1.0f - mixFactor));
			if (red > 1.0f)
				red = 1.0f;

			green = green * mixFactor + (destination.green * (1.0f - mixFactor));
			if (green > 1.0f)
				green = 1.0f;

			blue = blue * mixFactor + (destination.blue * (1.0f - mixFactor));
			if (blue > 1.0f)
				blue = 1.0f;

			alpha = alpha * mixFactor + (destination.alpha * (1.0f - mixFactor));
			if (alpha > 1.0f)
				alpha = 1.0f;
		}

		// Method to set the current colour to be somewhere between a source and destination colour based on the mix factor
		// where 0.0 is fully source, and 1.0 is fully destination. Also caps colour components to 1.0 to avoid crazyness.
		void setInterpolatedColour(const Colour4f &source, const Colour4f &destination, const float mixFactor)
		{
			red = source.red * mixFactor + (destination.red * (1.0f - mixFactor));
			if (red > 1.0f)
				red = 1.0f;

			green = source.green * mixFactor + (destination.green * (1.0f - mixFactor));
			if (green > 1.0f)
				green = 1.0f;

			blue = source.blue * mixFactor + (destination.blue * (1.0f - mixFactor));
			if (blue > 1.0f)
				blue = 1.0f;

			alpha = source.alpha * mixFactor + (destination.alpha * (1.0f - mixFactor));
			if (alpha > 1.0f)
				alpha = 1.0f;
		}

};

#endif
