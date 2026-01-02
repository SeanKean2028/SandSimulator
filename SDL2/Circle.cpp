#include "Circle.h"
Circle::Circle(glm::vec2 _center, float _radius, float _steps) {
	radius = _radius;
	amountOfSteps = _steps;

	anglePerSteps = (2 * PI) / _steps;
	GenerateCircle();
}
void Circle::init(glm::vec2 _center, float _radius, float _steps) {
	radius = _radius;
	amountOfSteps = _steps;

	anglePerSteps = (2 * PI) / _steps;
	GenerateCircle();
}
//Helper function to add vertex to vertices list with only 4 parameter pos(vec2), texcoords(vec2)
void Circle::AddVertex(glm::vec2 pos, glm::vec2 texCoords) {
	//Position
	vertices.push_back(pos.x);
	vertices.push_back(pos.y);
	vertices.push_back(0);
	//Normal
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(1);
	//TexCoordinates
	vertices.push_back(texCoords.x);
	vertices.push_back(texCoords.y);
}
//Sets vertices, and indices generating using the fan method
void Circle::GenerateCircle() {
	//Add Center
	AddVertex(glm::vec2(0.0f, 0.0f), glm::vec2(0.5f, 0.5f));
	//Add all Vertices
	for (int i = 0; i < amountOfSteps; i++) {
		double theta = anglePerSteps * i;
		glm::vec2 pos = glm::vec2(radius * cos(theta), radius * sin(theta));
		glm::vec2 texCoords = glm::vec2(0.5f + 0.5f * cos(theta), 0.5f + 0.5f * sin(theta));
		AddVertex(pos, texCoords);
	}
	//Add all Indices
	for (int j = 1; j <= amountOfSteps; j++) {
		indices.push_back(0);
		indices.push_back(j);
		indices.push_back((j % amountOfSteps) + 1);
	}
	VertexShader vertexShader = VertexShader("Shaders/cellVertex.glsl", GL_VERTEX_SHADER);
	FragmentShader fragmentShader = FragmentShader("Shaders/cellFragment.glsl", GL_FRAGMENT_SHADER);
	shaderProgram = ShaderProgram(vertexShader, fragmentShader);

	circleMesh = Mesh(vertices.data(), vertices.size(), indices.data(), indices.size(), shaderProgram);
}
void Circle::DrawCircle(glm::vec2 mousePos, glm::vec2 resolution) {
	pos.x = (2.0f * mousePos.x / resolution.x) - 1.0f;
	pos.y = 1.0f - (2.0f * mousePos.y / resolution.y);

	glm::mat4 model = glm::translate(
		glm::mat4(1.0f),
		glm::vec3(pos, 0.0f)
	);

	shaderProgram.use();
	shaderProgram.setMat4("model", model);
	shaderProgram.setVec4("color", glm::vec4(1, 0, 0, 0.2f));

	circleMesh.DrawMesh();
}
void Circle::Delete() {
	circleMesh.Delete();
	shaderProgram.Delete();
}