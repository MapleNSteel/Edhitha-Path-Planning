#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include <vector>
#include <deque>
#include <unistd.h>
#include <limits>

#include <exception>

#include <GL/glew.h>      // Include the extension wrangler to have access to enhanced OpenGL features, like pointsprites
#include <GL/glfw.h>      // Include OpenGL Framework library
#include <GL/glut.h>

#include "Utility Classes/General/Vec3.hpp"

#include "PathFinding/Obstacles/Sphere/Sphere.h"
#include "PathFinding/Obstacles/Cylinder/Cylinder.h"
#include "PathFinding/SwarmParticle2D.h" // Include our custom Particle2D class
#include "PathFinding/VPSOPathFinding.cpp"

#include "Utility Classes/Ramer-Douglas-Peucker/RamerDouglasPeucker3d.hpp"
#include "Utility Classes/GPSCoordinate.h"

#define ILUT_USE_OPENGL	  // This MUST be defined before calling the DevIL headers or we don't get OpenGL functionality
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

// Specify default namespace for commonly used elements
using std::string;
using std::cout;
using std::endl;
using std::vector;

GLint windowWidth   = 1200;             // Width of our window
GLint windowHeight  = 640;              // Heightof our window

GLint swarmMidX    = 20; // Middle of the Swarm horizontally
GLint swarmMidY    = 20; // Middle of the Swarm vertically

// Specify a limit for the size of our particle vector
const float INACTIVE_PARTICLE_ALPHA=0.9;

const int MAX_OBSTRUCTIONS = 2;

const int MAX_PARTICLES = 64;
const int MAX_ITERATIONS = 1000;

// Vector of Swarm Particles
SwarmParticle2D particleSystem[MAX_PARTICLES];
SwarmParticle2D localParticles[MAX_PARTICLES];
SwarmParticle2D * gBest;

deque<GPSCoordinate> inputWayPoints;
vector<GPSCoordinate> wayPoints;

vector<SwarmParticle2D> bestPath;

//#define SPHERE

#ifdef SPHERE
Sphere sphere[MAX_OBSTRUCTIONS];
#endif
#ifndef SPHERE
vector<Cylinder> cylinder;
#endif

GPSCoordinate home;
GPSCoordinate destination;

Vec3<long double> referenceFrame = Vec3<long double>(600,320,0);
//38.1478790422 -76.4310500305 -84.2180292137

Geodesic wgs84(Constants::WGS84_a(), Constants::WGS84_f());

#define MIN
#define VISUAL

// Unique identifier for the texture we'll apply to our pointsprites
int textureId;
long double objectiveFunction(Vec3<long double> position);
void initGL(){
	// ----- Report GLFW an OpenGL version info -----
	//cout << "Controls: Use WSAD and the mouse to move around!" << endl;

	// Frame counter and window settings variables
	int redBits    = 8, greenBits = 8,    blueBits    = 8;
	int alphaBits  = 8, depthBits = 24,   stencilBits = 0;

	// ----- Intialise GLFW -----

	// Initialise GLFW
	if (!glfwInit() )
	{
		std::cout << "Failed to initialise GLFW!" << endl;
		glfwTerminate();
		return;
	}
	// Create a window
	if( !glfwOpenWindow(windowWidth, windowHeight, redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, GLFW_WINDOW))
	{
		std::cout << "Failed to open window!" << std::endl;
		glfwTerminate();
		return ;
	}

	glfwSetWindowTitle("Particle Swarm Optimisation");

	// Create pointers to ints so we can get the GLFW and OpenGL versions
	int *major = new int;
	int *minor = new int;
	int *revision = new int;

	glfwGetVersion(major, minor, revision);
	std::cout << "Using GLFW version " << *major << "." << *minor << "." << *revision << std::endl;

	// Note: We must have created an OpenGL window before we can query the GL version
	glfwGetGLVersion(major, minor, revision);
	std::cout << "Using OpenGL version " << *major << "." << *minor << "." << *revision << std::endl;

	delete major;
	delete minor;
	delete revision;

	//  ----- Initialise GLEW -----

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "GLEW initialisation error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}
	std::cout << "GLEW intialised successfully. Using GLEW version: " << glewGetString(GLEW_VERSION) << std::endl;

	// ----- GLFW Settings -----

	glfwSwapInterval(1); // Enable vsync

	// ----- Window and Projection Settings -----

	// Setup our viewport to be the entire size of the window
	glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);

	// Change to the projection matrix, reset the matrix and set up our projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Specify orthographic projection (2D - no size attentuation) with the origin on the bottom left
	glOrtho(0, windowWidth, 0, windowHeight, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// ----- Initialize DevIL libraries -----

	// DevIL sanity check
	if ( (iluGetInteger(IL_VERSION_NUM) < IL_VERSION) || (iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION) || (ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION) )
	{
		std::cout << "DevIL versions are different... Exiting." << std::endl;
		exit(-1);
	}
	else
	{
		std::cout << "Devil initialised successfully." << std::endl;
	}

	// Initialise all DevIL functionality
	ilInit();
	iluInit();
	ilutInit();

	// ----- OpenGL settings -----

	glClearColor(0.0, 0.0f, 0.0f, 1.0f); // Set our clear colour to black, full alpha

	// Specify the origin of the point sprite
	glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_UPPER_LEFT); // Default - only other option is GL_LOWER_LEFT

	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);

	// Specify the drawing mode for point sprites
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	// Enable 2D Textures
	glEnable(GL_TEXTURE_2D);

	// Specify linear filtering for minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Specify that textures are clamped to edges
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
vector<Vec3<long double> > inline initialisePositions(){
	vector<Vec3<long double> > positions;
	/*for(short i=0;i<4;i++){
		for(short j=0;j<4;j++){
			for(short k=0;k<4;k++){
				positions.push_back(Vec3<long double>((long double)5*i,(long double)5*j,(long double)5*k));
			}
		}
	}*/
	boost::mt19937 rng(time(0));
	boost::random::uniform_real_distribution< > uniformDistribution( 0.0, 1.0 );
	boost::variate_generator< boost::mt19937, boost::random::uniform_real_distribution< > >random(rng, uniformDistribution);

	for(short i=0;i<MAX_PARTICLES;i++){
		double radius = 5;
		double theta = 360*random();
		double phi = 360*random();

		positions.push_back(Vec3<long double>((long double)radius*cos(theta),(long double)radius*sin(theta)*cos(phi),(long double)radius*sin(theta)*sin(phi)));
	}

	return positions;
}
void inline initialiseLocalPositions(vector<Vec3<long double> > positions){
	for(short i=0;i<MAX_PARTICLES;i++){
		localParticles[i]= SwarmParticle2D(positions[i],Vec3<long double>(0,0,0));
		localParticles[i].setParticleValue(objectiveFunction(positions[i]));
	}
}
long double objectiveFunction(Vec3<long double> position){
	//Function to be Optimised.
	if(GPSCoordinate(position+home.getCartesian()).getZ()<=0)
		return 1000000000.0;
	bool collided = false;
	for(unsigned short i=0; i< cylinder.size(); i++)
#ifdef SPHERE
		collided=collided||sphere[i].isColliding(position,home.getCartesian());
#endif
#ifndef SPHERE
		collided=collided||cylinder[i].isColliding(position,home.getCartesian());
#endif

	if(collided)
		return 1000000000.0;
	else{
		return position.getSquaredDistance(position,destination.getCartesian()-home.getCartesian());
	}
}
void inline initialiseParticleSwarm(vector<Vec3<long double> > positions){

	boost::mt19937 rng(time(0));
	boost::random::uniform_real_distribution< > uniformDistribution( 0.0, 1.0 );
	boost::variate_generator< boost::mt19937, boost::random::uniform_real_distribution< > >random(rng, uniformDistribution);

	for(short i=0;i<MAX_PARTICLES;i++){
		particleSystem[i]=SwarmParticle2D(positions[i],Vec3<long double>(0,0,0));
		//particleSystem[i].getPosition().display();
		particleSystem[i].setAlpha(INACTIVE_PARTICLE_ALPHA);
		particleSystem[i].setParticleValue(objectiveFunction(particleSystem[i].getPosition()));
	}
}
// Function to draw our scene
void updateAndDraw(int deltaTime,long double inertia, long double socialFactor,long double globalFactor)
{
	// Clear the screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// Reset the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Enable 2D textures and point sprites
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_POINT_SPRITE);
	long double (*obj) (Vec3<long double>) = objectiveFunction;
	// ...update the particle position position and draw it.
	VPSOPathFinding::update<long double>(particleSystem,localParticles,gBest,destination.getCartesian()-home.getCartesian(),inertia,socialFactor,globalFactor
			,MAX_PARTICLES,deltaTime,obj,200,400,false);
	//draw Obstacles()
	for(unsigned short i=0; i< cylinder.size(); i++)
#ifdef SPHERE
		sphere[i].draw(referenceFrame,home,getCartesian()-GPSCoordinate(GPSCoordinate::homeReference));
#endif
#ifndef SPHERE
		cylinder[i].draw(referenceFrame,GPSCoordinate(GPSCoordinate::homeReference));
#endif
	for(short i=0; i<MAX_PARTICLES ; i++)
		if(&particleSystem[i]!=gBest)
			particleSystem[i].draw(referenceFrame+home.getCartesian()-GPSCoordinate(GPSCoordinate::homeReference));

	gBest->setAlpha(1.0);//set
	gBest->draw(referenceFrame+home.getCartesian()-GPSCoordinate(GPSCoordinate::homeReference));
	bestPath.push_back(*gBest);
	gBest->setAlpha(INACTIVE_PARTICLE_ALPHA);
	//And end.
	// Disable 2D textures and point sprites
	glDisable(GL_POINT_SPRITE);
	glDisable(GL_TEXTURE_2D);

	// ----- Stop Drawing Stuff! ------

	glfwSwapBuffers(); // Swap the buffers to display the scene (so we don't have to watch it being drawn!)
}
void drawPath(vector<SwarmParticle2D> bestPath){

	glClear(GL_COLOR_BUFFER_BIT);
	for(unsigned short i=0; i<cylinder.size() ; i++)
#ifdef SPHERE
		sphere[i].draw(referenceFrame,GPSCoordinate::homeReference);
#endif
#ifndef SPHERE
		cylinder[i].draw(referenceFrame,GPSCoordinate::homeReference);
#endif

	for(std::vector<SwarmParticle2D>::iterator it = bestPath.begin() ;it!=bestPath.end();it++){
		it->setAlpha(1.0);
		it->draw(referenceFrame-GPSCoordinate::homeReference);
	}
	glDisable(GL_POINT_SPRITE);
	glDisable(GL_TEXTURE_2D);

	// ----- Stop Drawing Stuff! ------

	glfwSwapBuffers(); // Swap the buffers to display the scene (so we don't have to watch it being drawn!)
}
int inline pathFindingLoop(long double inertia,long double socialFactor,long double globalFactor){

	// Main loop

	int oldTimeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	int initialTime=oldTimeSinceStart;
	int count=0;
	//usleep(4000000);//initial delay

	gBest = &particleSystem[0];


	// Flag to keep our main loop running
	bool running = true;

	while (running&&(count++<MAX_ITERATIONS)){
		usleep(20000);
	    int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	    int deltaTime = timeSinceStart - oldTimeSinceStart;

	    oldTimeSinceStart = timeSinceStart;
		// Draw our scene
	    if(inertia > 0){
	   	    inertia=10 - (10)*exp(-(gBest->getParticleValue()/100));

	   		//cout<<"Inertia: "<<inertia<<endl;
	    }
	   	else
	   	    inertia=0;
		updateAndDraw(10,inertia,socialFactor,globalFactor);
		// Exit if ESC was pressed or window was closed
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);

		if(gBest->getParticleValue() <= 5)
			break;
	}

	cout<<"Elapsed Time:	"<<((double)(glutGet(GLUT_ELAPSED_TIME)-initialTime))<<endl;
	return count;
}
vector<SwarmParticle2D> removeDuplicates(vector<SwarmParticle2D> bestPath,long double maxDistance){
	vector<SwarmParticle2D>simplifiedPath;
	simplifiedPath.push_back(bestPath[0]);
	for(unsigned int i=0;i<bestPath.size()-1;i++){
		for(unsigned int j=i+1;j<bestPath.size()-1;j++){
			if(bestPath[i].getPosition().getDistance(bestPath[i].getPosition(),bestPath[j].getPosition())<=maxDistance)
				continue;
			else{
				simplifiedPath.push_back(bestPath[j]);
				i=j;
				break;
			}
		}
	}
	simplifiedPath.push_back(bestPath[bestPath.size()-1]);
	return simplifiedPath;
}
vector<SwarmParticle2D> simplifyPath(vector<SwarmParticle2D> bestPath,double epsilon){

	if(bestPath.size()<3)
		return bestPath;

	vector<SwarmParticle2D>simplifiedPath;
	simplifiedPath=removeDuplicates(bestPath,epsilon*2);


	vector <SwarmParticle2D> tempPath;
	tempPath.push_back(simplifiedPath[0]);

	for(unsigned short i=1;i<simplifiedPath.size()-1;i++){
		Vec3<long double> temp=(simplifiedPath[i-1].getPosition()+simplifiedPath[i].getPosition()+simplifiedPath[i+1].getPosition())/3;
		if(objectiveFunction(tempPath[i].getPosition())==1000000000)
			continue;

		tempPath.push_back(simplifiedPath[i]);
		tempPath.back().setParticlePosition(temp);
	}
	tempPath.push_back(simplifiedPath[simplifiedPath.size()-1]);

	tempPath= RamerDouglasPeucker3D::simplifyPath(tempPath,epsilon);

	return tempPath;
}
void inline displayWayPoints(){
	cout<<"No. of WayPoints:"<<wayPoints.size()<<endl;
	cout<<endl;
	// Clean up GLFW and exit

	cout<<"QGC WPL 110"<<endl;
	cout<<"0\t1\t0\t16\t0\t0\t0\t0\t";
	GPSCoordinate::homeReference.display();
	unsigned short num=1;
	for (std::vector<GPSCoordinate>::iterator it = wayPoints.begin() ; it != wayPoints.end(); ++it){
		cout<<num++<<"\t0\t0\t16\t0\t0\t0\t0\t";
		std::cout.precision(15);
		it->display();
	}
}
void inline findPath(){
	vector< Vec3<long double> > positions=initialisePositions();
	initialiseParticleSwarm(positions);
	initialiseLocalPositions(positions);

	long double inertia = 1;
	long double socialFactor = 1.61803398875;
	long double globalFactor = 1.61803398875;

	int count = pathFindingLoop(inertia,socialFactor,globalFactor);

	cout<<"Count:"<<count<<endl;
}
void inline addWayPoints(vector<SwarmParticle2D> simplifiedPath){
	wayPoints.clear();
	for (std::vector<SwarmParticle2D>::iterator it = simplifiedPath.begin() ; it != simplifiedPath.end(); ++it){
		wayPoints.push_back(GPSCoordinate(it->getPosition()));
	}
}
void kelsaMaadi(){
	wayPoints.push_back(inputWayPoints.front());
	vector<SwarmParticle2D> tempPath;
	while(inputWayPoints.size()!=1){
		cout<<"Home:"<<endl;
		home = inputWayPoints.front();
		std::cout.precision(15);
		home.display();
		inputWayPoints.pop_front();
		cout<<"Destination:"<<endl;
		destination = inputWayPoints.front();
		destination.display();

		findPath();
		vector<SwarmParticle2D> simplifiedPath;

		cout<<"Raw Path size:"<<bestPath.size()<<endl;
		simplifiedPath = simplifyPath(bestPath,10);
		cout<<"Simplified Path size:"<<simplifiedPath.size()<<endl;

		for (std::vector<SwarmParticle2D>::iterator it = simplifiedPath.begin() ; it != simplifiedPath.end(); ++it){
			SwarmParticle2D temp(*it);
			temp.setParticlePosition(temp.getPosition()+home.getCartesian());
			tempPath.push_back(temp);
		}
		bestPath.clear();
	}
	tempPath=removeDuplicates(tempPath,20);
	addWayPoints(tempPath);
	cout<<"Temp Path Length:"<<tempPath.size()<<endl;
	drawPath(tempPath);
	displayWayPoints();
}
// Fire it up...
int main(int argc, char **argv)
{
	GPSCoordinate::homeReference = Vec3<long double>(13.080799,77.759773,865.539978);
	GPSCoordinate::localCartesian = LocalCartesian(GPSCoordinate::homeReference.getX(), GPSCoordinate::homeReference.getY(), GPSCoordinate::homeReference.getZ(), GeographicLib::Geocentric::WGS84());

	inputWayPoints.push_back(GPSCoordinate(13.080828,77.759773,100.000000));
	inputWayPoints.push_back(GPSCoordinate(13.082306,77.759933,100.000000));
	inputWayPoints.push_back(GPSCoordinate(13.0829034,77.7603346,100.000000));

#ifdef SPHERE
	sphere[0] = Sphere(-35.3666207,149.1506910,100,45,2);
	sphere[1] = Sphere(-35.3656233,149.1506052,100,45,2);
	sphere[2] = Sphere(-35.3655304,149.1517060,100,45,2);
#endif
#ifndef SPHERE
	cylinder.push_back(Cylinder(13.0833658,77.7628720,0,40,5,750,10));
	cylinder.push_back(Cylinder(13.0817356,77.7594817,0,40,7,750,10));
#endif

	// Call our initGL function to set up our OpenGL options
	initGL();

	wayPoints.clear();

	kelsaMaadi();
	Geodesic geodesic(Constants::WGS84_a(), Constants::WGS84_f());

	usleep(5000000);

	glfwTerminate();

	bestPath.clear();
	return 0;
}
