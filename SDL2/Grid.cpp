#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <vector>
#include <algorithm>
#include <random>

#include "Mesh.h"
#include "Shader.h"
#include "Circle.h"
#include "Grid.h"
#include "Coordinate.h"
using namespace std;
#pragma region Cell Mesh Data
vector<float> vertices = {

	// bottom-left		
	-0.5f, -0.5f, 0.0f,
	0.0f,  0.0f, 1.0f,
	0.0f,  0.0f,


	// bottom-right
	0.5f, -0.5f, 0.0f,
	0.0f,  0.0f, 1.0f,
	1.0f,  0.0f,

	// top-right
	 0.5f,  0.5f, 0.0f,
	 0.0f,  0.0f, 1.0f,
	 1.0f,  1.0f,

	 // top-left
	 -0.5f,  0.5f, 0.0f,
	  0.0f,  0.0f, 1.0f,
	  0.0f,  1.0f

};
vector<int> cellIndices{
	0, 1, 2,
	2, 3, 0
};
#pragma endregion
#pragma region  Colors
vector<glm::vec4> sandColors = {
	glm::vec4(1, 0.992f, 0.592f, 1),
	glm::vec4(1,1,1,1),
	glm::vec4(0.89f, 0.878f, 0.133f,1),
	glm::vec4(0.85f, 0.855f, 0.49f, 1),
};

vector<glm::vec4> woodColors = {
	glm::vec4(0.43f, 0.31f, 0.17f, 1),
	glm::vec4(0.631f, 0.416f, 0.106f, 1),
	glm::vec4(0.91f, 0.71f, 0.42f, 1),
	glm::vec4(0.6f, 0.4f, 0.106f, 1),
};

vector<glm::vec4> fireColors = {
	glm::vec4(1,0.714f, 0, 1),
	glm::vec4(1,0,0,1),
	glm::vec4(1,1,0,1),
	glm::vec4(1,0.204f, 0, 1)
 };
vector<glm::vec4> waterColors = {
	glm::vec4(0.737f, 1, 0.984f, 1),
	glm::vec4(0, 0.604f, 1,1),
	glm::vec4(0.282f, 0.251f, 1,1),
	glm::vec4(0,0.573f, 1,1)
};
vector<glm::vec4> ashColors = {
	glm::vec4(0.31f, 0.271f, 0.259f, 1),
	glm::vec4(0.388f, 0.125f, 0.125f,1),
	glm::vec4(0.078f, 0.078f, 0.078f,1),
	glm::vec4(0,0,0,1)
};
vector<glm::vec4> smokeColors = {
	glm::vec4(0.31f, 0.271f, 0.259f, 0.25f),
	glm::vec4(0.388f, 0.125f, 0.125f,0.1f),
	glm::vec4(0.078f, 0.078f, 0.078f,0.3f),
	glm::vec4(0,0,0,0.4f)
};
int densities[] = {
	0, 3, 2, 0, 0,3,1
};
#pragma endregion
#pragma region SharedCellMesh
struct SharedCellMesh {
	public: static Mesh* mesh;
		  static ShaderProgram* shader;
};

Mesh* SharedCellMesh::mesh = nullptr;
ShaderProgram* SharedCellMesh::shader = nullptr;

#pragma endregion
//Todo: Make own header for importing Random int, Random float etc..
int Random(int _min, int _max) {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(_min, _max);

	return(dist(gen));
}
#pragma region Cell
Cell::Cell(glm::vec2 gridPos, int _cellAmount, glm::vec2 cellSize, glm::vec2 _resolution, CellType type)
{
	gridPosition = gridPos;
	cellType = type;
	int random = -1;	
	int random2 = 0;
	switch (cellType) {
		case sand:
			random = Random(0, sandColors.size() - 1);
			color = sandColors[random];
			break;
		case wood:
			random = Random(0, woodColors.size() - 1);
			color = woodColors[random];
			break;	
		case fire:
			random = Random(0, fireColors.size() - 1);
			color = fireColors[random];
			break;
		case water:
			random = Random(0, waterColors.size() - 1);
			color = waterColors[random];
			random2 = Random(1, 2);
			
			if (random2 == 1)
				random = -1;
			else
				random = 1;
			break;
		case ash:
			random = Random(0, ashColors.size() - 1);
			color = ashColors[random];
			break;
		case smoke:
			random = Random(0, smokeColors.size() - 1);
			color = smokeColors[random];
			break;
	}	
	density = densities[cellType];
}
void Cell::Delete() {
	SharedCellMesh::mesh->Delete();
}
#pragma endregion
#pragma region Grid Mesh Data
float GridVertices[] = {
	-1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
	-1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,    0.0f, 1.0f
};
int GridIndices[] = {
	0, 1, 2,
	2, 3, 0
};
#pragma endregion 
#pragma region Grid Initialization
Grid::Grid() {
}
	
Grid::Grid(GridInitArgs gridInitArgs) {
	init(gridInitArgs);
}
void Grid::init(GridInitArgs gridInitArgs) {
	//Set Resolution
	resolution = glm::vec2(gridInitArgs._resX, gridInitArgs._resY);

	//Set Cell Amount
	cellAmount = gridInitArgs._cells;
	cellSize = glm::vec2(resolution.x / cellAmount, resolution.y / cellAmount);

	vector<vector<Cell*>> temp(cellAmount, vector<Cell*>(cellAmount));
	cells = temp;

	// These vectors are used to collect data for the GPU upload every frame.
	instanceModels.reserve(1000);
	instanceColors.reserve(1000);

	// Generate CellShader
	VertexShader cellVertex("Shaders/cellVertex.glsl", GL_VERTEX_SHADER);
	FragmentShader cellFragment("Shaders/cellFragment.glsl", GL_FRAGMENT_SHADER);
	const size_t VERTEX_FLOAT_COUNT = 8; // 3 Pos + 3 Norm + 2 TexCoord
	const size_t TOTAL_VERTICES = vertices.size() / VERTEX_FLOAT_COUNT; // Should be 4
	const size_t VERTEX_BUFFER_SIZE_BYTES = vertices.size() * sizeof(float);
	if (!SharedCellMesh::shader) {
		SharedCellMesh::shader = new ShaderProgram(cellVertex, cellFragment);
	}

	if (!SharedCellMesh::mesh) {
		SharedCellMesh::mesh = new Mesh(
			vertices.data(),
			VERTEX_BUFFER_SIZE_BYTES,
			cellIndices.data(), 
			cellIndices.size(),
			*SharedCellMesh::shader,true
		);
	}

	// Generate GridShader
	VertexShader gridVertex("Shaders/gridVertex.glsl", GL_VERTEX_SHADER);
	FragmentShader gridFragment("Shaders/gridFragment.glsl", GL_FRAGMENT_SHADER);

	glm::mat4 model = glm::mat4(1.0f);

	gridShader = ShaderProgram(gridVertex, gridFragment);
	gridShader.use();
	gridShader.setMat4("model", model);
	gridShader.setVec4("params", glm::vec4(1, gridInitArgs._cells, 0, 0));
	gridShader.setVec4("color", glm::vec4(1, 0, 1, 0.25f));

	gridMesh = Mesh(GridVertices, sizeof(GridVertices), GridIndices, sizeof(GridIndices), gridShader);
}
#pragma endregion
#pragma region Cell Instantation
Cell* Grid::CreateCell(glm::vec2 pos, CellType type) {
	if (pos.x < 0 || pos.y < 0 || pos.x >= cellAmount || pos.y >= cellAmount)
		return nullptr;
	if (cells[pos.y][pos.x] != nullptr)
			return nullptr;
	Cell* cell = new Cell(
		pos,
		cellAmount,
		cellSize,
		resolution,
		type
	);
	cells[(int)pos.y][(int)pos.x] = cell;	
	if (cell->cellType == fire) {
		if (cell->isSpreadedFire)
		{
			cell->lifeTime = 15.0f;
			cell->active = true;
			cells[(int)pos.y][(int)pos.x] = cell;
			return cell;
		}
		float random = Random(11, 90);
		float time = random / 100;
		cell->lifeTime = time;
		cells[(int)pos.y][(int)pos.x] = cell;
	}
	else if (cell->cellType == smoke) {
		float random = Random(200, 400);
		float time = random / 100;
		cell->lifeTime = time;
		cells[(int)pos.y][(int)pos.x] = cell;
	}
	cell->active = true;
	cell->model = UpdatedModel(pos);
	activeCells.push_back(cell);
	return cell;
}
void Grid::CreateCellsFromCircle(Circle* _circle, CellType _type) {
	glm::vec2 max = glm::vec2(_circle->pos.x + _circle->radius, _circle->pos.y + _circle->radius);
	glm::vec2 min = glm::vec2(_circle->pos.x - _circle->radius, _circle->pos.y - _circle->radius);
	ClampNdc(max);
	ClampNdc(min);

	glm::vec2 percentageOfGridMin = glm::vec2((min.x + 1) / 2, 1.0f - ((max.y + 1) / 2));
	glm::vec2 percentageOfGridMax = glm::vec2((max.x + 1) / 2, 1.0f - ((min.y + 1) / 2));

	glm::ivec2 gridMin = glm::vec2(cellAmount * percentageOfGridMin.x, cellAmount * percentageOfGridMin.y);
	glm::ivec2 gridMax = glm::vec2(cellAmount * percentageOfGridMax.x, cellAmount * percentageOfGridMax.y);

	for (int y = gridMin.y; y < gridMax.y; ++y) {
		for (int x = gridMin.x; x < gridMax.x; ++x) {
			if (!InBounds(x, y))
				continue;
			if (cells[y][x] == nullptr && pendingSpawns.size() < MAX_QUEUE)
				pendingSpawns.push_back({ glm::ivec2(x,y),_type });
		}
	}

}
#pragma endregion
#pragma region Cell Physics Helpers
void Grid::WakeNeighbors(int x, int y) {
	static const glm::ivec2 dirs[] = { {0,-1},{-1,-1},{1,-1}, {0,1}, {1,0}, {1,1}, {-1,1}, {-1,0} };
	for (auto d : dirs) {
		int nx = x + d.x, ny = y + d.y;
		if (InBounds(nx, ny) && cells[ny][nx] && !cells[ny][nx]->active) {
			cells[ny][nx]->active = true;
			toActivate.push_back(cells[ny][nx]);
			cells[ny][nx]->unchangedFrames = 0;
		}
	}
}

bool Grid::InBounds(int x, int y) {
	if (x < 0 || y < 0)
		return false;
	if (x >= cellAmount || y >= cellAmount)
		return false;
	return true;
}
bool Grid::MoveCell(glm::vec2 _newPos) {
	if (_newPos.x < 0 || _newPos.y < 0)
		return false;
	if (_newPos.x >= cellAmount || _newPos.y >= cellAmount)
		return false;
	return cells[_newPos.y][_newPos.x] == nullptr;
}
	
glm::mat4 Grid::UpdatedModel(glm::vec2 newPos) {
	float cellNdcW = 2.0f / cellAmount;
	float cellNdcH = 2.0f / cellAmount;

	glm::vec2 ndc = GridToNdc(newPos, cellAmount);
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 translation = glm::vec3(ndc.x, ndc.y, 0);
	
	model = glm::translate(glm::mat4(1.0f), translation);
	model = glm::scale(model, glm::vec3(cellNdcW, cellNdcH, 1.0f));
	return model;
}
//New pos = y+1, x, simulates the space right below our cell!
glm::vec2 Grid::GravityPos(glm::vec2 gridPosition) {
	int nGridPosY = gridPosition.y + 1;
	int nGridPosX = gridPosition.x;
	if (nGridPosY >= cellAmount)   // STOP at bottom
		return gridPosition;
	return glm::vec2(nGridPosX, nGridPosY);
}
vector<glm::vec2> Grid::GravityDispersalPos(glm::vec2 gridPosition) {
	vector<glm::vec2> _vec2;
	int nGridPosY = gridPosition.y + 1;
	if (nGridPosY >= cellAmount) return _vec2;

	int nGridPosX1 = gridPosition.x + 1;
	int nGridPosX2 = gridPosition.x - 1;

	if (gridPosition.y < cellAmount - 1)
		_vec2.push_back(glm::vec2(nGridPosX1, nGridPosY));
	if (gridPosition.x > 0)
		_vec2.push_back(glm::vec2(nGridPosX2, nGridPosY));

	return _vec2;
}
vector<glm::vec2> Grid::DispersalPos(glm::vec2 gridPosition, int leftRight)
{
	vector<glm::vec2> _vec2;
	//Generate Right
	int nGridPosX = gridPosition.x + 1;
	if (nGridPosX < cellAmount)
		_vec2.push_back(glm::vec2(nGridPosX, gridPosition.y));

	//Generate Left
	int nGridPosX2 = gridPosition.x - 1;
	if (nGridPosX2 >= 0)
		_vec2.push_back(glm::vec2(nGridPosX2, gridPosition.y));
	vector<glm::vec2> _vec;
	if (_vec2.size() == 2) {
		if (leftRight == -1) {
			_vec.push_back(_vec2[1]);
			_vec.push_back(_vec2[0]);
			return _vec;
		}
		else {
			_vec.push_back(_vec2[0]);
			_vec.push_back(_vec2[1]);
			return _vec;
		}

	}
	return _vec2;
}

void Grid::Move(Cell* c, glm::ivec2 newPos) {
	cells[c->gridPosition.y][c->gridPosition.x] = nullptr;
	c->gridPosition = newPos;
	c->model = UpdatedModel(newPos);
	c->unchangedFrames = 0;
	cells[newPos.y][newPos.x] = c;
}    
bool Grid::IsBoxed(Cell* c) {
	glm::ivec2 p = c->gridPosition;
	return !MoveCell({ p.x,p.y + 1 }) && !MoveCell({ p.x,p.y - 1 }) && !MoveCell({ p.x - 1,p.y }) && !MoveCell({ p.x + 1,p.y });
}
int Grid::CountEmptyHoriz(glm::ivec2 pos, int dir) {
	int count = 0;
	while (true) {
		pos.x += dir;
		if (!InBounds(pos.x, pos.y)) break;
		if (cells[pos.y][pos.x] != nullptr) break;
		count++;
	}
	return count;
}
bool Grid::CheckNewFirePos(glm::vec2 _newPos) {
	if (_newPos.x < 0 || _newPos.y < 0)
		return false;
	if (_newPos.x >= cellAmount || _newPos.y >= cellAmount)
		return false;
	if (cells[_newPos.y][_newPos.x] == nullptr)
		return false;
	return cells[_newPos.y][_newPos.x]->cellType == wood;
}
void Grid::SwitchCellType(glm::vec2 _newPos, CellType type) {
	cells[_newPos.y][_newPos.x]->cellType = type;
	switch (type) {
		case fire:
			cells[_newPos.y][_newPos.x]->lifeTime = float(Random(250, 750)/100);
			break;
	}
}
void Grid::SwapCells(glm::vec2 p1, glm::vec2 p2)
{
	Cell*& a = cells[p1.y][p1.x];
	Cell*& b = cells[p2.y][p2.x];

	if (!a || !b) return;
	if (a->density <= b->density) return;
	if (a->density == 0 || b->density == 0) return;
	std::swap(a, b);

	WakeNeighbors(p1.x, p1.y);
	// Update internal state (THIS WAS MISSING)
	a->gridPosition = p1;
	b->gridPosition = p2;

	a->model = UpdatedModel(p1);
	b->model = UpdatedModel(p2);

	a->moved = true;
	b->moved = true;
}
#pragma endregion
#pragma region Cell Update Function
//Updates our Sand Cell's model to translate it's position to simulate falling
void Grid::UpdateSand(Cell* _cell) {
	if (_cell->moved == true)
		return;
	_cell->moved = true;
	glm::vec2 newPos = GravityPos(_cell->gridPosition);
	//Try fall
	if (_cell->gridPosition.y >= cellAmount - 1) {
		return;
	}
	if (MoveCell(newPos)) {
		glm::mat4 model = UpdatedModel(newPos);
		WakeNeighbors(_cell->gridPosition.x, _cell->gridPosition.y);
		_cell->model = model;
		cells[_cell->gridPosition.y][_cell->gridPosition.x] = nullptr;
		_cell->gridPosition = newPos;
		cells[newPos.y][newPos.x] = _cell;
		_cell->unchangedFrames = 0;
		return;
	}
	if(cells[newPos.y][newPos.x]->density < _cell->density) {
		SwapCells(_cell->gridPosition, newPos);
		_cell->unchangedFrames = 0;
		WakeNeighbors(newPos.x, newPos.y);
		return;
	}

		//Try fall Diagnol
		for (glm::vec2 _newPos : GravityDispersalPos(_cell->gridPosition)) {
			if (MoveCell(_newPos)) {
				WakeNeighbors(_cell->gridPosition.x, _cell->gridPosition.y);
				cells[_cell->gridPosition.y][_cell->gridPosition.x] = nullptr;
				glm::mat4 _model = UpdatedModel(_newPos);
				_cell->model = _model;
				_cell->gridPosition = _newPos;
				cells[_newPos.y][_newPos.x] = _cell;
				_cell->unchangedFrames = 0;
				return;
			}
		}
		_cell->unchangedFrames++;
		if (_cell->unchangedFrames > 120)
			_cell->active = false;
	
}
void Grid::UpdateWood(Cell* _cell) {

}
void Grid::UpdateFire(Cell* cell) {
	static const glm::ivec2 dirs[6] = {
		{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {-1,-1}, {1,-1}
	};
	cell->lifeTime -= m_deltaTime;

	if (cell->lifeTime <= 0.0f) {
		PendingCell pendingDeletion{
			cell->gridPosition, fire
		};
		PendingCell pendingAshSpawn{
			cell->gridPosition, ash
		};
		int smokingY = cell->gridPosition.y;
		int increment = 0;
		glm::vec2 newPos = glm::vec2(cell->gridPosition.x, smokingY);
		while (!MoveCell(newPos) && increment < 20) {
			newPos = glm::vec2(newPos.x, smokingY--);
			increment++;
		}
		PendingCell pendingSmokeSpawn{
			newPos, smoke
		};
		pendingCellDeletion.push_back(pendingDeletion);
		pendingSpawns.push_back(pendingAshSpawn);
		if(InBounds(newPos.x, newPos.y))
			pendingSpawns.push_back(pendingSmokeSpawn);

		WakeNeighbors(cell->gridPosition.x, cell->gridPosition.y);
		return;
	}

	for (int i = 0; i < 4; ++i) {
		glm::ivec2 newPos = glm::ivec2(cell->gridPosition.x + dirs[i].x, cell->gridPosition.y + dirs[i].y);
		if (CheckNewFirePos(newPos)) {
			SwitchCellType(newPos, fire);
			WakeNeighbors(newPos.x, newPos.y);
			break; 
		}
	}
}
void Grid::UpdateWater(Cell* c) {
	if (c->moved == true)
		return;
	c->moved = true;
	glm::ivec2 pos = c->gridPosition;
	while (true) {
		glm::ivec2 below(pos.x, pos.y + 1);
		if (!InBounds(below.x, below.y) || !MoveCell(below))
			break;
		Move(c, below);
		pos = below;
		WakeNeighbors(below.x, below.y);
	}
	
	int spreadDistance = 10; 
	int dir = Random(0, 1) == 0 ? -1 : 1;

	for (int i = 1; i <= spreadDistance; ++i) {
		glm::ivec2 newPos(pos.x + i * dir, pos.y);
		if (!InBounds(newPos.x, newPos.y) || !MoveCell(newPos))
			break;
		Move(c, newPos);
		WakeNeighbors(newPos.x, newPos.y);
	}

	dir *= -1;
	for (int i = 1; i <= spreadDistance; ++i) {
		glm::ivec2 newPos(pos.x + i * dir, pos.y);
		if (!InBounds(newPos.x, newPos.y) || !MoveCell(newPos))
			break;
		WakeNeighbors(newPos.x, newPos.y);
	}

	c->unchangedFrames++;
	if (c->unchangedFrames > 250) 
		c->active = false;

}
void Grid::UpdateSmoke(Cell* c) {
	if (c->moved == true)
		return;
	c->moved = true;
	glm::ivec2 pos = c->gridPosition;
	c->lifeTime -= m_deltaTime;

	if (c->lifeTime <= 0.0f) {
		PendingCell deletionCell{
			pos, smoke
		};
		pendingCellDeletion.push_back(deletionCell);
		return;
	}
	int up = 0, maxUp = 10;
	while (up < maxUp) {
		glm::ivec2 above(pos.x, pos.y -1);
		if (!InBounds(above.x, above.y) || !MoveCell(above))
			break;
		Move(c, above);
		pos = above;
		up++;
	}

	int spreadDistance = 3;
	int dir = Random(0, 1) == 0 ? -1 : 1;

	for (int i = 1; i <= spreadDistance; ++i) {
		glm::ivec2 newPos(pos.x + i * dir, pos.y);
		if (!InBounds(newPos.x, newPos.y) || !MoveCell(newPos))
			break;
		Move(c, newPos);
	}

	dir *= -1;
	for (int i = 1; i <= spreadDistance; ++i) {
		glm::ivec2 newPos(pos.x + i * dir, pos.y);
		if (!InBounds(newPos.x, newPos.y) || !MoveCell(newPos))
			break;
	}
}

#pragma endregion

#pragma region Update
void Grid::SpawnPendingCells() {
	for (PendingCell pendingCell : pendingSpawns) {
		if (cells[pendingCell.pos.y][pendingCell.pos.x] == nullptr) {
			CreateCell(pendingCell.pos, pendingCell.type);
		}
	}
	pendingSpawns.clear();
}
void Grid::DeletePendingCells() {
	for (PendingCell pendingCell : pendingCellDeletion) {
		int x = pendingCell.pos.x;
		int y = pendingCell.pos.y;

		Cell* cell = cells[y][x];
		if (cell != nullptr) {
			auto it = std::find(activeCells.begin(), activeCells.end(), cell);
			if (it != activeCells.end())
				activeCells.erase(it);

			delete cell;
			cells[y][x] = nullptr;
		}
	}
	pendingCellDeletion.clear();
}
void Grid::ActivateCells() {
	for (Cell* c : toActivate)
		activeCells.push_back(c);
	toActivate.clear();
}
// --- Grid::Update() ---
void Grid::Update() {
	int random = -1;
	for (Cell* c : activeCells) {
		if (!c) continue;
		if (!c->active && c->cellType == water) {
			if(MoveCell(glm::vec2(c->gridPosition.x, c->gridPosition.y  -1)))
				toActivate.push_back(c);
		}
		if (!c->active) continue;
		c->moved = false;
		glm::ivec2 oldPos = c->gridPosition;

		switch (c->cellType) {
			case sand:
				UpdateSand(c);
				break;
			case water:
				UpdateWater(c);
				break;
			case fire:
				random = Random(0, fireColors.size() - 1);
				glm::vec4 color = fireColors[random];
				c->color = color;
				UpdateFire(c);
				break;
			case wood:
				UpdateWood(c); // Sets active=false
				break;
			case ash:
				UpdateSand(c);
				break;
			case smoke:
				UpdateSmoke(c);
				break;
		}
	}
		DeletePendingCells();
		SpawnPendingCells();
		ActivateCells();
		CleanUp(); // Removes inactive cells from activeCells list
}
void Grid::UpdateInstanceBuffers() {
	instanceModels.clear();
	instanceColors.clear();

	for (int y = 0; y < cellAmount; y++) {
		for (int x = 0; x < cellAmount; x++) {
			Cell* c = cells[y][x];
			if (!c) continue;

			instanceModels.push_back(c->model);
			instanceColors.push_back(c->color);
		}
	}

	if (!instanceModels.empty()) {
		SharedCellMesh::mesh->UpdateInstancedVBO(
			instanceModels.data(), instanceModels.size(),
			instanceColors.data(), instanceColors.size()
		);
	}
}
#pragma endregion
// --- Grid::Draw(float deltaTime) ---
void Grid::Draw(float deltaTime) {
	m_deltaTime = deltaTime;

	gridMesh.DrawMesh();
	Update();
	UpdateInstanceBuffers();
	
	if (SharedCellMesh::mesh && SharedCellMesh::shader && !instanceModels.empty()) {
		SharedCellMesh::shader->use();
		SharedCellMesh::mesh->DrawMeshInstanced(instanceModels.size());
	}
}

void Grid::Delete() {
	gridMesh.Delete();
	gridShader.Delete();

	if (SharedCellMesh::mesh) {
		SharedCellMesh::mesh->Delete();
		delete SharedCellMesh::mesh;
		SharedCellMesh::mesh = nullptr;
	}
	if (SharedCellMesh::shader) {
		SharedCellMesh::shader->Delete();
		delete SharedCellMesh::shader;
		SharedCellMesh::shader = nullptr;
	}

	activeCells.clear();
	for (int y = 0; y < cells.size(); ++y) {
		cells[y].clear(); 
	}
	cells.clear();
}
void Grid::CleanUp() {
	activeCells.erase( std::remove_if(activeCells.begin(), activeCells.end(),[](Cell* c) { return !c->active; }), activeCells.end());
}

