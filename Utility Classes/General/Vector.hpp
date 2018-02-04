#ifndef VectorN_HPP
#define VectorN_HPP

#include <iostream>
#include<stdarg.h>
#include <math.h>
#include <stdlib.h>

#include <tbb/tbb.h>

template <typename T,short size> class Vector
{
	private:
		// A Vector<T,size>N simply has two properties called x and y
		T component[size];

	public:

		// ------------ Constructors ------------

		// Default constructor
		Vector(...)
		{
			va_list ap;
			va_start(ap, size);

			for(unsigned short i=0;i<size;i++)
				component[i]=va_arg(ap,int);
		}

		// ------------ Getters and setters ------------

		void set(...)
		{
			va_list ap;
			va_start(ap, size);

			for(unsigned short i=0;i<size;i++)
				component[i]=va_arg(ap,int);
		}

		T getComponent(short index) const { return component[index]; }

		void setComponent(const T &x,short index)
		{
			component[index]=x;
		}
		// ------------ Helper methods ------------

		// Method to reset a vector to zero
		void zero()
		{
			tbb::parallel_for(0,size+0,[&](int i){
				component[i]=0;
			});
		}
		T getMagnitude(){
			T sum=0;

			tbb::parallel_for(0,size+0,[&](int i){
				sum+=component[i];
			});

			return (T)(sqrt(sum));
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
				tbb::parallel_for(0,size+0,[&](int i){
					component[i]/=magnitude;
				});
			}
		}
		static T dotProduct(const Vector<T,size> &vec1, const Vector<T,size> &vec2)
		{
			T dotProduct=0;
			tbb::parallel_for(0,size+0,[&](int i){
				dotProduct+=vec1.component[i]*vec2.component[i];
			});

			return dotProduct;
		}
		T dotProduct(const Vector<T,size> &vec) const
		{
			T dotProduct=0;
			tbb::parallel_for(0,size+0,[&](int i){
				dotProduct+=this->component[i]*vec.component[i];
			});

			return dotProduct;
		}
		T getManhattanDistance(Vector<T,size> v)
		{
			T manhattanDistance=0;

			tbb::parallel_for(0,size+0,[&](int i){
				dotProduct+=this->component[i]-v.component[i];
			});

			return manhattanDistance;
		}
		static T getSquaredDistance(const Vector<T,size> &v1, const Vector<T,size> &v2)
		{
			return (v1-v2).dotProduct(v1-v2);
		}

		static T getDistance(const Vector<T,size> &v1, const Vector<T,size> &v2)
		{
			return sqrt((v1-v2).dotProduct(v1-v2));
		}

		// Method to display the vector so you can easily check the values
		void display()

		{
			for(short i=0;i<size;i++)
				std::cout<<"x"<<i<<":"<<this->component[i]<<"\t";
		}

		// ------------ Overloaded operators ------------

		// Overloaded addition operator to add Vector<T,size>Ns together
		Vector<T,size> operator+(const Vector<T,size> &vector) const
		{
			Vector<T,size> sum=Vector<T,size>();
			tbb::parallel_for(0,size+0,[&](int i){
				sum.component[i]=this->component[i]+vector.component[i];
			});
			return sum;
		}

		// Overloaded add and asssign operator to add Vector<T,size>Ns together
		void operator+=(const Vector<T,size> &vector)
		{
			tbb::parallel_for(0,size+0,[&](int i){
				this->component[i]+=vector.component[i];
			});
		}

		// Overloaded subtraction operator to subtract a Vector<T,size>N from another Vector<T,size>N
		Vector<T,size> operator-(const Vector<T,size> &vector) const
		{
			Vector<T,size> sum=Vector<T,size>();
			tbb::parallel_for(0,size+0,[&](int i){
				sum.component[i]=this->component[i]-vector.component[i];
			});
			return sum;
		}

		// Overloaded subtract and asssign operator to subtract a Vector<T,size>N from another Vector<T,size>N
		void operator-=(const Vector<T,size> &vector)
		{
			Vector<T,size> sum=Vector<T,size>();
			tbb::parallel_for(0,size+0,[&](int i){
				this->component[i]-=vector.component[i];
			});
		}

		// Overloaded multiply operator to multiply a vector by a scalar
		Vector<T,size> operator*(const T &value) const
		{
			Vector<T,size> temp=Vector<T,size>();

			tbb::parallel_for(0,size+0,[&](int i){
				temp.component[i]*=value;
			});
			return temp;
		}

		// Overloaded multiply and assign operator to multiply a vector by a scalar
		void operator*=(const T &value)
		{
			tbb::parallel_for(0,size+0,[&](int i){
				this->component[i]*=value;
			});
		}
		bool operator==(const Vector<T,size> & vector)
		{
			tbb::parallel_for(0,size+0,[&](int i){
				if(this->component[i]==vector.component[i])
					return false;
			});

			return true;
		}
		// Overloaded multiply operator to multiply a vector by a scalar
		Vector<T,size> operator/(const T &value) const
		{
			Vector<T,size> temp=Vector<T,size>();

			tbb::parallel_for(0,size+0,[&](int i){
				temp->component[i]/=value;
			});
			return temp;
		}

		// Overloaded multiply and assign operator to multiply a vector by a scalar
		void operator/=(const T &value)
		{
			tbb::parallel_for(0,size+0,[&](int i){
				this->component[i]/=value;
			});
		}
};

#endif
