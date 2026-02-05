#pragma once
#include <iostream>
#include <cmath>
#include <glm.hpp>
#include <numbers> 

#include "Mesh.h"
#include "Shader.h"
using namespace std;

struct circleInitArgs {
	float _radius;
	float _steps;
};
class Circle {
private :
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
	float radius;
	glm::vec2 pos;
public :
	Circle(){}
	Circle(circleInitArgs);

	void init(circleInitArgs initArgs);
	//Helper function to add vertex to vertices list with only 4 parameter pos(vec2), texcoords(vec2)
	void AddVertex(glm::vec2 pos, glm::vec2 texCoords);

	//Sets vertices, and indices generating using the fan method
	void GenerateCircle();
	void DrawCircle(glm::vec2 mousePos, glm::vec2 resolution);
	glm::vec2 GetPos() { return pos; }
	void SetRadius(float _radius) { radius = _radius;}
	float GetRadius() { return radius; }
	void Delete();
};
