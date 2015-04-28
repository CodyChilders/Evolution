#include <cmath>
#include <vector>
#include <glut.h>
#include <glm.hpp>

#define SCALEFACTOR	0.75f
#define BLOCKSCALEFACTOR 4.5f
#define SMOOTHSCALEFACTOR 2.0f

using namespace std;

static vector<vector<float>> world;

static const int width = 100, height = 100;

static float angle = 0;

static bool blocky = true;

template<typename T>
inline void constrain(T& curr, T min, T max){
	if(curr > max){
		curr = max;
		return;
	}
	if(curr < min){
		curr = min;
		return;
	}
	return;
}

template<typename T>
inline T* negateVector(T* v){
	int i = 0;
	while(v[i] != NULL)
		v[i++] *= -1;
	return v;
}

//next 3 functions from:
//http://www.dreamincode.net/forums/topic/66480-perlin-noise/
//perlin noise generator
inline double findnoise2(double x,double y){
	int n=(int)x+(int)y*57;
	n=(n<<13)^n;
	int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
	return 1.0-((double)nn/1073741824.0);
}

inline double interpolate(double a,double b,double x){
	double ft=x * 3.1415927;
	double f=(1.0-cos(ft))* 0.5;
	return a*(1.0-f)+b*f;
}

inline double noise(double x,double y){
	double floorx=(double)((int)x);//This is kinda a cheap way to floor a double integer.
	double floory=(double)((int)y);
	double s,t,u,v;//Integer declaration
	s=findnoise2(floorx,floory);
	t=findnoise2(floorx+1,floory);
	u=findnoise2(floorx,floory+1);//Get the surrounding pixels to calculate the transition.
	v=findnoise2(floorx+1,floory+1);
	double int1=interpolate(s,t,x-floorx);//Interpolate between the values.
	double int2=interpolate(u,v,x-floorx);//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
	return interpolate(int1,int2,y-floory);//Here we use y-floory, to get the 2nd dimension.
}

//scale the noise to values that we want
inline float scaleNoise(int i, int j){
	double result = noise(i, j);
	result *= SCALEFACTOR;//scale it
	return static_cast<float>(result);
}

//convert 2 index i/j coordinates into a single unique index
inline int convert(int i, int j, int a){
	return a * i + j + 1;
}

void blockyRender(){
	glColor3f(0.75f, 0, 0);
	for(unsigned i = 0; i < world.size(); i++){
		for(unsigned j = 0; j < world[i].size(); j++){
			glPushMatrix();{
				glTranslatef(i, 0, j);
				glScalef(1, world[i][j] * BLOCKSCALEFACTOR, 1);
				glutSolidCube(1);
			}glPopMatrix();
		}
	}
}

float* calculateNormal(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2){
	glm::vec3 v0(p0.x, p0.y, p0.z);
	glm::vec3 v1(p1.x, p1.y, p1.z);
	glm::vec3 v2(p2.x, p2.y, p2.z);
	//grab 2 vectors of the triangle
	glm::vec3 u = v2 - v0;
	glm::vec3 v = v2 - v1;

	float x = u.y * v.z - u.z * v.y;
	float y = u.z * v.x - u.x * v.z;
	float z = u.x * v.y - u.y * v.x;

	float* n = new float[3];
	n[0] = x;
	n[1] = y;
	n[2] = z;

	return n;//glm::vec3(x, y, z);
}

void smoothRender(){
	glColor3f(0, 0.75f, 0);
	for(unsigned i = 0; i < world.size() - 1; i++){
		for(unsigned j = 0; j < world[i].size() - 1; j++){
			glBegin(GL_TRIANGLES);
			{
				//first triangle
				{
					glm::vec3 a (i, world[i][j] * SMOOTHSCALEFACTOR, j);
					glm::vec3 b (i + 1, world[i+1][j] * SMOOTHSCALEFACTOR, j);
					glm::vec3 c (i + 1, world[i + 1][j + 1] * SMOOTHSCALEFACTOR, j + 1);
					glNormal3fv(calculateNormal(a, b, c));
					glVertex3f(a.x, a.y, a.z);
					glVertex3f(b.x, b.y, b.z);
					glVertex3f(c.x, c.y, c.z);
				}
				//second triangle
				{
					glm::vec3 a (i, world[i][j] * SMOOTHSCALEFACTOR, j);
					glm::vec3 b (i + 1, world[i + 1][j + 1] * SMOOTHSCALEFACTOR, j + 1);
					glm::vec3 c (i, world[i][j + 1] * SMOOTHSCALEFACTOR, j + 1);
					glNormal3fv(negateVector(calculateNormal(a, b, c)));
					glVertex3f(a.x, a.y, a.z);
					glVertex3f(b.x, b.y, b.z);
					glVertex3f(c.x, c.y, c.z);
				}
			}
			glEnd();
		}
	}
}

void createWorld(){
	//create world, calculate low and high values at the same time
	float min = INT_MAX, max = INT_MIN;
	for(int i = 0; i < width; i++){
		vector<float> row;
		world.push_back(row);
		for(int j = 0; j < width; j++){
			float val = scaleNoise(i, j);
			world[i].push_back(val);
			if(val < min)
				min = val;
			if(val > max)
				max = val;
		}
	}
	//rescale
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			world[i][j] -= min;
			world[i][j] /= (max - min);
		}
	}
}

void render(){
	/*glPushMatrix();*/{
	//	angle += 0.1;
	//	glTranslatef(width/2, 0, height/2);
		//glRotatef(angle, 0, 1, 0);
		gluLookAt(0, 50, 0, //eye
				  50, 0, 50, //target
				  0, 1, 0); //up
	}//glPopMatrix();
	if(blocky){
		blockyRender();
	}else{
		smoothRender();
	}
}

void toggleBlocky(){
	blocky = !blocky;
}

float getHeightAtPoint(float x, float z){
	return world[x][z];
}

float currentRenderHeight(float x, float z){
	return getHeightAtPoint(x, z) * (blocky ? BLOCKSCALEFACTOR : SMOOTHSCALEFACTOR );
}