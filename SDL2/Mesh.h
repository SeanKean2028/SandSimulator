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
private:
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
	// --- New Instancing Buffers ---
	GLuint instanceModelVBO = 0;
	GLuint instanceColorVBO = 0;

	Texture texture;
	//Shader used to draw object
	ShaderProgram shaderProgram;
		
	void GenerateElementBufferObject();
	void GenerateArrayObject();
	void SetAttributePointers();
public : 
	Mesh();
	Mesh(float* _vertices, int _vertexCount, ShaderProgram _shaderProgram);
	Mesh(float* _vertices, int _vertexCount, int* _indices, int _indicesCount, ShaderProgram _shaderProgram, bool flag);
	Mesh(float* _vertices, int _vertexCount, int* _indices, int _indicesCount, ShaderProgram _shaderProgram);

	void UpdateInstancedVBO(glm::mat4* modelMatrices, int modelCount, glm::vec4* colors, int colorCount);
	
	void DrawMesh();
	void DrawMeshInstanced(int instanceCount);
	void SetInstancedAttributePointers();
	void Delete();
};