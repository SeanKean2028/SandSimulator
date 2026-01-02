#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <vector>
#include <string>
#include "Shader.h"
using namespace std;
struct Texture {
	unsigned int id;
	string type;
	string path;
};
class Mesh {
	//Holds vertex floats pos 3, normals 3, and texcoords 2
	float* vertices;
	//count of vertices
	int vertexCount = 0;
	//floats contained in each vertex
	int floatsPerVertex = 8;
	//ID
	GLuint VAO,VBO,EBO = 0;
	int* indices;
	int indicesCount;

	Texture texture;
	//Shader used to draw object
	ShaderProgram shaderProgram;
	//Indices Used
	void GenerateElementBufferObject();
	//No Indices
	void GenerateArrayObject();
	void SetAttributePointers();
public : 
	Mesh();
	Mesh(float* _vertices, int _vertexCount, int* _indices, int _indicesCount, ShaderProgram _shaderProgram);

	Mesh(float* _vertices, int _vertexCount, ShaderProgram _shaderProgram);
	void DrawMesh();
	void Delete();
};