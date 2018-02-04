/*
 * RamerDouglasPeucker3d.cpp
 *
 *  Created on: 17 Apr 2016
 *      Author: arjun
 */
#include <vector>
#include "../../PathFinding/SwarmParticle2D.h"

using namespace std;

namespace RamerDouglasPeucker3D{

	const pair<int, double> findMaximumDistanceIndex(vector<SwarmParticle2D>& bestPath){
		Vec3<long double> firstPoint = bestPath[0].getPosition();
		Vec3<long double> lastPoint = bestPath[bestPath.size()-1].getPosition();

		unsigned int index = 0;
		long double maxDistance=-1;

		//distance Calculation
		Vec3<long double> point1 = lastPoint-firstPoint;
		point1.normalise();

		for(unsigned int i=1;i<bestPath.size()-1;i++){

			Vec3<long double> point2 = bestPath[i].getPosition()-firstPoint;

			long double distance = (point2-point1*point2.dotProduct(point1)).getMagnitude();

			if(distance>maxDistance){
				maxDistance=distance;
				index = i;
			}
		}
		return make_pair(index,maxDistance);
	}

	vector<SwarmParticle2D> simplifyPath(vector<SwarmParticle2D>& bestPath, double epsilon){
		if(bestPath.size()<3)
			return bestPath;

		std::pair<unsigned int, double> maxDistance = findMaximumDistanceIndex(bestPath);
		if(maxDistance.second > epsilon){
			int index = maxDistance.first;

			 vector<SwarmParticle2D>::iterator it=bestPath.begin();
			 vector<SwarmParticle2D> path1(it,it+index+1); //new path l1 from 0 to index
			 vector<SwarmParticle2D> path2(it+index,bestPath.end()); // new path l2 from index to last

			 vector<SwarmParticle2D> r1 = simplifyPath(path1,epsilon);
			 vector<SwarmParticle2D> r2 = simplifyPath(path2,epsilon);

			 //Concat simplified path1 and path2 together
			 vector<SwarmParticle2D> rs;
			 for(unsigned int i=0;i<r1.size()-1;i++)
				 rs.push_back(r1[i]);
			 for(unsigned int i=0;i<r2.size();i++)
				 rs.push_back(r2[i]);
			 return rs;
		}
		else { //base case 2, all points between are to be removed.
			vector<SwarmParticle2D> r;
			r.push_back(bestPath[0]);
			r.push_back(bestPath[bestPath.size()-1]);
			return r;
		}
	}
}
