#pragma once
#include <string>
#include "stdafx.h"

class Terrain
{
	//size of the mesh forming the terrain
	int gridWidth,gridDepth;
	int numVerts;
	//size of the terrain in world_coords
	float terrWidth,terrDepth;

	typedef  GLfloat vector[3];
	//array of vertices for the grid(this will be a triangle list)
	//I know, very inefficient, but let's run before we walk
	vector *vertices;
	vector *colors;
	vector *texCoords;
	vector *normals;

	float getHeight(float x, float y);
	float getHeightWithFile(float x, float y);
	void setPoint(vector, float, float,float);

	sf::Image image;
	bool wireframe;
	

	
public:
	Terrain(void);
	~Terrain(void);

	void Init();
	void InitWithFileName(std::string name);
	void NormalVector(GLfloat[3] , GLfloat[3] , GLfloat[3], GLfloat[3]);
	void Draw();
	float tallestPoint;
	void swapWireFrame();
};

