#include <glm.hpp>
#pragma once
//Helper
glm::ivec2 NdcToGrid(glm::vec2 ndc, int cellAmount) {
	int gx = int((ndc.x + 1.0f) * 0.5f * cellAmount);
	int gy = int((1.0f - ndc.y) * 0.5f * cellAmount);

	if (gx < 0) gx = 0;
	if (gy < 0) gy = 0;
	if (gx >= cellAmount) gx = cellAmount - 1;
	if (gy >= cellAmount) gy = cellAmount - 1;

	return glm::ivec2(gx, gy);
}
glm::vec2 GridToNdc(glm::ivec2 grid, int cellAmount) {
	float ndcX = (grid.x + 0.5f) * (2.0f / cellAmount) - 1.0f;
	float ndcY = 1.0f - (grid.y + 0.5f) * (2.0f / cellAmount);
	return { ndcX, ndcY };
}
glm::vec2 ScreenToNdc(glm::vec2 screen, glm::vec2 resolution) {
	float ndcX = (2.0f * screen.x / resolution.x) - 1.0f;
	float ndcY = 1.0f - (2.0f * screen.y / resolution.y);
	return { ndcX, ndcY };
}

//Converts this->gridPosition to screenPosition
glm::vec2 ScreenPosition(glm::vec2 gridPosition, float size) {
	return glm::vec2((gridPosition * size) * 0.5f);
}
void ClampNdc(glm::vec2& ndc) {
	ndc.x = glm::clamp(ndc.x, -1.0f, 1.0f);
	ndc.y = glm::clamp(ndc.y, -1.0f, 1.0f);
}