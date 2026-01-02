#pragma once
#include <iostream>
#include <cmath>
#include <glm.hpp>
#include <numbers> 

#include "Mesh.h"
#include "Shader.h"
using namespace std;

class Circle {
	//Screen Space
	int amountOfSteps;
	float anglePerSteps;
	
	//OpenGL Screen Space
	vector<float> vertices;
	vector<int> indices;
	const double PI = atan(1) * 4;
	glm::mat4 model;
	Mesh circleMesh;
	ShaderProgram shaderProgram;
public :
	float radius;
	glm::vec2 pos;
	Circle(){}
	Circle(glm::vec2 _center, float _radius, float _steps);
		
	void init(glm::vec2 _center, float _radius, float _steps);
	//Helper function to add vertex to vertices list with only 4 parameter pos(vec2), texcoords(vec2)
	void AddVertex(glm::vec2 pos, glm::vec2 texCoords);

	//Sets vertices, and indices generating using the fan method
	void GenerateCircle();
	void DrawCircle(glm::vec2 mousePos, glm::vec2 resolution);
	void Delete();
};
