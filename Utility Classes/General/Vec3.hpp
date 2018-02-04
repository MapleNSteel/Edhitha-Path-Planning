#ifndef Vec3_HPP
#define Vec3_HPP

#include <iostream>
#include <math.h>
#include <stdlib.h>

template <typename T> class Vec3
{
	private:
		// A Vec3 simply has two properties called x and y
		T x, y, z;

	public:

		// ------------ Constructors ------------

		// Default constructor
		Vec3()
		{
			x = y = z = 0;
		}

		// Two parameter constructor
		Vec3(T xValue, T yValue, T zValue)
		{
			x = xValue;
			y = yValue;
			z = zValue;
		}

		// ------------ Getters and setters ------------

		void set(const T &xValue, const T &yValue, const T &zValue)
		{
			x = xValue;
			y = yValue;
			z = zValue;
		}

		T getX() const { return x; }
		T getY() const { return y; }
		T getZ() const { return z; }

		void setX(const T &xValue)
		{
			x = xValue;
		}

		void setY(const T &yValue)
		{
			y = yValue;
		}

		void setZ(const T &zValue)
		{
			z = zValue;
		}

		// ------------ Helper methods ------------

		// Method to reset a vector to zero
		void zero()
		{
			x = y = z = 0;
		}
		T getMagnitude(){

			return (T)(sqrt((x * x) + (y * y) + (z * z)));
		}
		// Method to normalise a vector
		void normalise()
		{
			// Calculate the magnitude of our vector
			T magnitude = getMagnitude();

			// As long as the magnitude isn't zero, divide each element by the magnitude
			// to get the normalised value between -1 and +1
			if (magnitude > 0.0)
			{
				x /= magnitude;
				y /= magnitude;
				z /= magnitude;
			}
		}
		static T dotProduct(const Vec3 &vec1, const Vec3 &vec2)
		{
			return vec1.x * vec2.x + vec1.y * vec2.y +  vec1.z * vec2.z;
		}
		T dotProduct(const Vec3 &vec) const
		{
			return x * vec.x + y * vec.y + z * vec.z;
		}

		void addX(T value)
		{
			x += value;
		}

		void addY(T value)
		{
			y += value;
		}

		void addZ(T value)
		{
			z += value;
		}
		T getManhattanDistance(Vec3 v)
		{
			T dx = abs(v.x - x);
			T dy = abs(v.y - y);
			T dz = abs(v.z - z);

			return dx + dy + dz;
		}

		bool withinManhattanDistance(Vec3 v, T distance)
		{
			T dx = abs(v.x - x);
			T dy = abs(v.y - y);
			T dz = abs(v.z - z);

			return dx + dy + dz < distance;
		}

		static T getSquaredDistance(const Vec3 &v1, const Vec3 &v2)
		{
			T dx = v2.x - v1.x;
			T dy = v2.y - v1.y;
			T dz = v2.z - v1.z;

			return (dx * dx + dy * dy + dz * dz);

		}

		static T getDistance(const Vec3 &v1, const Vec3 &v2)
		{
			T dx = v2.x - v1.x;
			T dy = v2.y - v1.y;
			T dz = v2.z - v1.z;

			return sqrt(dx * dx + dy * dy + dz * dz);

		}

		Vec3<T> getVectorTo(const Vec3 &v)
		{
			Vec3<T> temp(v.x - x, v.y - y, v.z - z);

			temp.normalise();

			return temp;
		}

		// Method to display the vector so you can easily check the values
		void display()

		{
			std::cout<< x << "\t" << y << "\t" << z << std::endl;
		}

		// ------------ Overloaded operators ------------

		// Overloaded addition operator to add Vec3s together
		Vec3 operator+(const Vec3 &vector) const
		{
			return Vec3<T>(x + vector.x, y + vector.y, z + vector.z);
		}

		// Overloaded add and asssign operator to add Vec3s together
		void operator+=(const Vec3 &vector)
		{
			x += vector.x;
			y += vector.y;
			z += vector.z;
		}

		// Overloaded subtraction operator to subtract a Vec3 from another Vec3
		Vec3 operator-(const Vec3 &vector) const
		{
			return Vec3<T>(x - vector.x, y - vector.y, z - vector.z);
		}

		// Overloaded subtract and asssign operator to subtract a Vec3 from another Vec3
		void operator-=(const Vec3 &vector)
		{
			x -= vector.x;
			y -= vector.y;
			z -= vector.z;
		}

		// Overloaded multiplication operator to multiply two Vec3s together
		Vec3 operator*(const Vec3 &vector) const
		{
			return Vec3<T>(x * vector.x, y * vector.y, z * vector.z);
		}

		// Overloaded multiply operator to multiply a vector by a scalar
		Vec3 operator*(const T &value) const
		{
			return Vec3<T>(x * value, y * value, z * value);
		}

		// Overloaded multiply and assign operator to multiply a vector by a scalar
		void operator*=(const T &value)
		{
			x *= value;
			y *= value;
			z *= value;
		}

		// Overloaded multiply and assign operator to multiply a vector by a scalar
		void operator*=(const Vec3 & vector)
		{
			x *= vector.x;
			y *= vector.y;
			z *= vector.z;
			//return Vec3<T>(x * vector.x, y * vector.y);
		}
		bool operator==(const Vec3 & vector)
		{
			return ((x==vector.x)&&(y==vector.y)&&(z==vector.z));
		}
		// Overloaded multiply operator to multiply a vector by a scalar
		Vec3 operator/(const T &value) const
		{
			return Vec3<T>(x / value, y / value, z / value);
		}

		// Overloaded multiply and assign operator to multiply a vector by a scalar
		void operator/=(const T &value)
		{
			x /= value;
			y /= value;
			z /= value;
		}
};

#endif
