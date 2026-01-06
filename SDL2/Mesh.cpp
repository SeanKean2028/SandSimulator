#include "Mesh.h"
Mesh::Mesh() {

}
Mesh::Mesh(float* _vertices, int _vertexCount, ShaderProgram _shaderProgram) {
	vertices = _vertices;
	vertexCount = _vertexCount;

	shaderProgram = _shaderProgram;
	GenerateArrayObject();
	SetAttributePointers();
}
Mesh::Mesh(float* _vertices, int _vertexCount, int* _indices, int _indicesCount, ShaderProgram _shaderProgram, bool flag) {
	vertices = _vertices;
	indices = _indices;
	indicesCount = _indicesCount;
	shaderProgram = _shaderProgram;
	floatsPerVertex = 8;
	vertexCount = _vertexCount / floatsPerVertex;
	if (_indicesCount > 0)
		GenerateElementBufferObject();
	else
		GenerateArrayObject();
	GenerateElementBufferObject();
	SetAttributePointers();
	SetInstancedAttributePointers();
}
Mesh::Mesh(float* _vertices, int _vertexCount, int* _indices, int _indicesCount, ShaderProgram _shaderProgram) {
	vertices = _vertices;
	vertexCount = _vertexCount;
	indices = _indices;
	indicesCount = _indicesCount;
	shaderProgram = _shaderProgram;
	GenerateElementBufferObject();
	SetAttributePointers();
}

//Indices Used
void Mesh::GenerateElementBufferObject() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
		
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * floatsPerVertex * sizeof(float), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

void Mesh::GenerateArrayObject() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);
}

void Mesh::SetInstancedAttributePointers() {
	glBindVertexArray(VAO);
	glGenBuffers(1, &instanceModelVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceModelVBO);
	// Allocate space for max particles (MAX_PARTICLES must be defined in your code)
	glBufferData(GL_ARRAY_BUFFER, 10000 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

	for (int i = 0; i < 4; i++) {
		glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
		glEnableVertexAttribArray(3 + i);
		glVertexAttribDivisor(3 + i, 1);
	}
	glGenBuffers(1, &instanceColorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceColorVBO);
	glBufferData(GL_ARRAY_BUFFER, 10000 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(7);
	glVertexAttribDivisor(7, 1);

	glBindVertexArray(0);
}

void Mesh::UpdateInstancedVBO(glm::mat4* modelMatrices, int modelCount, glm::vec4* colors, int colorCount) {
	glBindVertexArray(VAO);

	// Update Model Matrix VBO
	glBindBuffer(GL_ARRAY_BUFFER, instanceModelVBO);
	// Use glBufferSubData for frequent updates (or glBufferData if count changes significantly)
	glBufferSubData(GL_ARRAY_BUFFER, 0, modelCount * sizeof(glm::mat4), modelMatrices);

	// Update Color VBO
	glBindBuffer(GL_ARRAY_BUFFER, instanceColorVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, colorCount * sizeof(glm::vec4), colors);

	glBindVertexArray(0);
}
void Mesh::DrawMeshInstanced(int instanceCount) {
	// Shader must be used by the caller (Grid::Draw)
	glBindBuffer(GL_ARRAY_BUFFER, instanceModelVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceColorVBO);
	glBindVertexArray(VAO);

	if (indicesCount > 0) {
		// Draw using indices
		glDrawElementsInstanced(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0, instanceCount);
	}
	else {
		// Draw without indices
		glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, instanceCount);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void Mesh::SetAttributePointers() {
	glBindVertexArray(VAO);
	// Position attribute (location = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
		
	// Normal attribute (location = 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
		
	// TexCoord attribute (location = 2)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}
	
void Mesh::DrawMesh() {		

	shaderProgram.use();
	//Set Up Textures
	if (texture.id != 0)
		glBindTexture(GL_TEXTURE_2D, texture.id);

	glBindVertexArray(VAO);		
	if (indicesCount > 0) {
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);	
	}
	glBindVertexArray(0);
}
	
void Mesh::Delete() {
	if (EBO) glDeleteBuffers(1, &EBO);
	if (VBO) glDeleteBuffers(1, &VBO);
	if (VAO) glDeleteVertexArrays(1, &VAO);
	
}