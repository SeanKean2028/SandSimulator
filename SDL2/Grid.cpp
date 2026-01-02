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
using namespace std;



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
struct SharedCellMesh {
	public: static Mesh* mesh;
		  static ShaderProgram* shader;
};

Mesh* SharedCellMesh::mesh = nullptr;
ShaderProgram* SharedCellMesh::shader = nullptr;

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
void ClampNdc(glm::vec2& ndc) {
	ndc.x = glm::clamp(ndc.x, -1.0f, 1.0f);
	ndc.y = glm::clamp(ndc.y, -1.0f, 1.0f);
}
int Random(int _min, int _max) {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(_min, _max);

	return(dist(gen));
}
Cell::Cell(glm::vec2 gridPos, int _cellAmount, glm::vec2 cellSize, glm::vec2 _resolution, CellType type)
{
	gridPosition = gridPos;
	size = cellSize;
	resolution = _resolution;
	cellType = type;
	cellAmount = _cellAmount;
	model = UpdatedModel(gridPos);

	int random = -1;	
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
			break;

	}	
}
void Cell::init(glm::vec2 gridPos, int _cellAmount, glm::vec2 cellSize, glm::vec2 _resolution, CellType type) {
	gridPosition = gridPos;
	size = cellSize;
	resolution = _resolution;
	cellType = type;
	cellAmount = _cellAmount;
	model = UpdatedModel(gridPos);

	int random = -1;
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
		break;
	}
}
//Converts this->gridPosition to screenPosition
glm::vec2 Cell::ScreenPosition() {
	return glm::vec2((gridPosition * size) * 0.5f);	
}

//New pos = y+1, x, simulates the space right below our cell!
glm::vec2 Cell::GravityPos() {
	int nGridPosY = gridPosition.y + 1;
	int nGridPosX = gridPosition.x;
	if (nGridPosY >= cellAmount)   // STOP at bottom
		return gridPosition;
	return glm::vec2(nGridPosX, nGridPosY);
}
	
//New positions = y+1, x+1; y+1, x-1; simulates the spreading affect of sand!

vector<glm::vec2> Cell::GravityDispersalPos() {
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
vector<glm::vec2> Cell::DispersalPos()
{
	vector<glm::vec2> _vec2;
	//Generate Right
	int nGridPosX = gridPosition.x + 1;
	if (nGridPosX >= cellAmount) return _vec2;
	_vec2.push_back(glm::vec2(nGridPosX, gridPosition.y));

	//Generate Left
	int nGridPosX2 = gridPosition.x - 1;
	if (nGridPosX2 >= cellAmount) return _vec2;
	_vec2.push_back(glm::vec2(nGridPosX2, gridPosition.y));
	return _vec2;
}
glm::mat4 Cell::UpdatedModel(glm::vec2 newPos) {

	int nGridPosY = newPos.y;
	int nGridPosX = newPos.x;
	
	if (nGridPosY > cellAmount)
		return glm::mat4(1.0f);

	//Grid → Screen (center)
	float cellPixelW = resolution.x / cellAmount;
	float cellPixelH = resolution.y / cellAmount;

	float screenX = (newPos.x + 0.5f) * cellPixelW;
	float screenY = (newPos.y + 0.5f) * cellPixelH;
	//Screen → NDC
	float ndcX = (2.0f * screenX / resolution.x) - 1.0f;
	float ndcY = 1.0f - (2.0f * screenY / resolution.y);
	
	float cellNdcW = 2.0f / cellAmount;
	float cellNdcH = 2.0f / cellAmount;
	
	ndc = glm::vec2(ndcX, ndcY);
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 translation = glm::vec3(ndcX, ndcY, 0);
	
	model = glm::translate(glm::mat4(1.0f), translation);
	model = glm::scale(model, glm::vec3(cellNdcW, cellNdcH, 1.0f));
	return model;
}

	
void Cell::Draw() {
	SharedCellMesh::shader->use();
	SharedCellMesh::mesh->DrawMesh();
}
void Cell::Delete() {
	SharedCellMesh::mesh->Delete();
}

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

	
Grid::Grid() {
}
	
Grid::Grid(int _resX, int _resY, int _cells) {

	//Set Resolution
	resolution = glm::vec2(_resX, _resY);

	//Set Cell Amount
	cellAmount = _cells;
	cellSize = glm::vec2(resolution.x / cellAmount, resolution.y / cellAmount);

	vector<vector<Cell*>> temp(cellAmount, vector<Cell*>(cellAmount));
	cells = temp;
	
	//Generate GridShader
	VertexShader gridVertex("Shaders/gridVertex.glsl", GL_VERTEX_SHADER);	
	FragmentShader gridFragment("Shaders/gridFragment.glsl", GL_FRAGMENT_SHADER);

	glm::mat4 model = glm::mat4(1.0f);

	gridShader = ShaderProgram(gridVertex, gridFragment);
	gridShader.use();
	gridShader.setMat4("model", model);
	gridShader.setVec4("params", glm::vec4(1, _cells, 0, 0));
	gridShader.setVec4("color", glm::vec4(1, 0, 1, 0.25f));

	//Generate CellShader
	VertexShader cellVertex("Shaders/cellVertex.glsl", GL_VERTEX_SHADER);
	FragmentShader cellFragment("Shaders/cellFragment.glsl", GL_FRAGMENT_SHADER);
	
	if (!SharedCellMesh::shader)
		SharedCellMesh::shader = new ShaderProgram(cellVertex, cellFragment);;
	gridMesh = Mesh(GridVertices, sizeof(GridVertices), GridIndices, sizeof(GridIndices), gridShader);
	
	if (!SharedCellMesh::mesh) {
		SharedCellMesh::mesh = new Mesh(
		vertices.data(), sizeof(vertices),
		cellIndices.data(), sizeof(cellIndices),
		*SharedCellMesh::shader
		);
	}
}
void Grid::init(int _resX, int _resY, int _cells) {

	//Set Resolution
	resolution = glm::vec2(_resX, _resY);

	//Set Cell Amount
	cellAmount = _cells;
	cellSize = glm::vec2(resolution.x / cellAmount, resolution.y / cellAmount);

	vector<vector<Cell*>> temp(cellAmount, vector<Cell*>(cellAmount));
	cells = temp;

	//Generate GridShader
	VertexShader gridVertex("Shaders/gridVertex.glsl", GL_VERTEX_SHADER);
	FragmentShader gridFragment("Shaders/gridFragment.glsl", GL_FRAGMENT_SHADER);

	glm::mat4 model = glm::mat4(1.0f);

	gridShader = ShaderProgram(gridVertex, gridFragment);
	gridShader.use();
	gridShader.setMat4("model", model);
	gridShader.setVec4("params", glm::vec4(1, _cells, 0, 0));
	gridShader.setVec4("color", glm::vec4(1, 0, 1, 0.25f));

	//Generate CellShader
	VertexShader cellVertex("Shaders/cellVertex.glsl", GL_VERTEX_SHADER);
	FragmentShader cellFragment("Shaders/cellFragment.glsl", GL_FRAGMENT_SHADER);

	if (!SharedCellMesh::shader)
		SharedCellMesh::shader = new ShaderProgram(cellVertex, cellFragment);;
	gridMesh = Mesh(GridVertices, sizeof(GridVertices), GridIndices, sizeof(GridIndices), gridShader);

	if (!SharedCellMesh::mesh) {
		SharedCellMesh::mesh = new Mesh(
			vertices.data(), sizeof(vertices),
			cellIndices.data(), sizeof(cellIndices),
			*SharedCellMesh::shader
		);
	}
}

void Grid::CreateCell(glm::vec2 pos, CellType type) {
	if (pos.x < 0 || pos.y < 0 || pos.x >= cellAmount || pos.y >= cellAmount)
		return;
	if (cells[pos.y][pos.x] != nullptr)
			return;
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
			cout << "Cell \n";
			cell->lifeTime = 15.0f;
			return;
		}
		float random = Random(11, 90);
		float time = random / 100;
		cell->lifeTime = time;
	}
}
Cell* Grid::CreateFireCell(glm::vec2 pos) {
	Cell* cell = new Cell(
		pos,
		cellAmount,
		cellSize,
		resolution,
		fire
	);
	cells[(int)pos.y][(int)pos.x] = cell;
	float random = Random(22, 180);
	float time = random / 100;
	cell->lifeTime = time;
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
			if (cells[y][x] == nullptr && pendingSpawns.size() < MAX_QUEUE)
				pendingSpawns.push_back({ glm::ivec2(x,y),_type });
		}
	}

}
void Grid::WakeNeighbors(int x, int y)
{
	for (int dx = -5; dx <= 5; dx++)
	{
		for (int dy = 0; dy <= 5; dy++)
		{
			int nx = x + dx;
			int ny = y + dy;
			
			if (InBounds(nx, ny))
			{
				if(cells[ny][nx] != nullptr)
				if (cells[ny][nx]->cellType == sand && cells[ny][nx]->active == false) {
					cells[ny][nx]->active = true;
					activeCells.push_back(cells[ny][nx]);
					cells[ny][nx]->unchangedFrames = 0;
					int nny = ny + 1;
					while (InBounds(nx, nny)) {
						if (cells[nny][nx] != nullptr) {
							if (cells[nny][nx]->active == false) {
								cells[ny][nx]->active = true;
								activeCells.push_back(cells[ny][nx]);
								cells[ny][nx]->unchangedFrames = -10;
							}
						}
						nny++;
					}
				}
			}
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
	//Updates our Sand Cell's model to translate it's position to simulate falling
void Grid::UpdateSand(Cell* _cell) {
	glm::vec2 newPos = _cell->GravityPos();
	//Try fall
	if (_cell->gridPosition.y >= cellAmount - 1) {
		return;
	}
	if (MoveCell(newPos)) {
		glm::mat4 model = _cell->UpdatedModel(newPos);
		WakeNeighbors(_cell->gridPosition.x, _cell->gridPosition.y);
		_cell->model = model;
		cells[_cell->gridPosition.y][_cell->gridPosition.x] = nullptr;
		_cell->gridPosition = newPos;
		cells[newPos.y][newPos.x] = _cell;
		_cell->unchangedFrames = 0;
		return;
	}
	else {
		//Try fall Diagnol
		for (glm::vec2 _newPos : _cell->GravityDispersalPos()) {
			if (MoveCell(_newPos)) {
				WakeNeighbors(_cell->gridPosition.x, _cell->gridPosition.y);
				cells[_cell->gridPosition.y][_cell->gridPosition.x] = nullptr;
				glm::mat4 _model = _cell->UpdatedModel(_newPos);
				_cell->model = _model;
				_cell->gridPosition = _newPos;
				cells[_newPos.y][_newPos.x] = _cell;
				_cell->unchangedFrames = 0;
				return;
			}
		}
		_cell->unchangedFrames++;
		if (_cell->unchangedFrames > 80)
			_cell->active = false;
	}
}
void Grid::UpdateWood(Cell* _cell) {
	_cell->active = false;
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
void Grid::UpdateFire(Cell* _cell) {
	//Calculate per frame chance to stop the fire based on one second percentage chance
	_cell->lifeTime -= m_deltaTime;
	if (_cell->lifeTime <= 0.0f) {
		_cell->active = false;
		cells[_cell->gridPosition.y][_cell->gridPosition.x] = nullptr;
		WakeNeighbors(_cell->gridPosition.x, _cell->gridPosition.x);
		return;
	}
	//All 8 cardinal directions n,nw,w,sw,se,e,ne 
	vector<int> directions = {0,1,2,3};
	int maxAttempts = 4;
	int attempts = 0;
	//Test all direction		
	while (attempts < maxAttempts && !directions.empty()) {
		//Try Spread

			glm::vec2 gridVector = glm::vec2(0, 0);
			switch (attempts) {
				case 0:
					gridVector = glm::vec2(0, 1);
				break;
				case 1:
					gridVector = glm::vec2(1, 0);
				break;
				case 2:
					gridVector = glm::vec2(0, -1);
				break;
				case 3:
					gridVector = glm::vec2(-1, 0);
				break;
			}
			glm::vec2 newGridPos = gridVector + _cell->gridPosition;
			
			if (CheckNewFirePos(newGridPos)) {
				glm::mat4 model = _cell->UpdatedModel(newGridPos);
				_cell->model = model;

				
				cells[newGridPos.y][newGridPos.x] = nullptr;
				Cell* cell = CreateFireCell(newGridPos);
				WakeNeighbors(newGridPos.x, newGridPos.y);
				_cell->unchangedFrames = 0;
				return;
			}
		
		attempts++;
	}
}
void Grid::UpdateWater(Cell* _cell) {
	glm::vec2 newPos = _cell->GravityPos();
	//Try fall
	if (_cell->gridPosition.y >= cellAmount - 1) {
		return;
	}
	if (MoveCell(newPos)) {
		glm::mat4 model = _cell->UpdatedModel(newPos);
		WakeNeighbors(_cell->gridPosition.x, _cell->gridPosition.y);
		_cell->model = model;
		cells[_cell->gridPosition.y][_cell->gridPosition.x] = nullptr;
		_cell->gridPosition = newPos;
		cells[newPos.y][newPos.x] = _cell;
		return;
	}
	else {
		//Try fall Diagnol
		for (glm::vec2 _newPos : _cell->GravityDispersalPos()) {
			if (MoveCell(_newPos)) {
				WakeNeighbors(_cell->gridPosition.x, _cell->gridPosition.y);
				cells[_cell->gridPosition.y][_cell->gridPosition.x] = nullptr;
				glm::mat4 _model = _cell->UpdatedModel(_newPos);
				_cell->model = _model;
				_cell->gridPosition = _newPos;
				cells[_newPos.y][_newPos.x] = _cell;
				return;
			}
		}
	}
	//Try Left or Right
	for (glm::vec2 _newPos : _cell->DispersalPos()) {
		if (MoveCell(_newPos)) {
			WakeNeighbors(_cell->gridPosition.x, _cell->gridPosition.y);
			cells[_cell->gridPosition.y][_cell->gridPosition.x] = nullptr;
			glm::mat4 _model = _cell->UpdatedModel(_newPos);
			_cell->model = _model;
			_cell->gridPosition = _newPos;
			cells[_newPos.y][_newPos.x] = _cell;
			return;
		}
	}

	
}
void Grid::CleanUp() {
	activeCells.erase( std::remove_if(activeCells.begin(), activeCells.end(),[](Cell* c) { return !c->active; }), activeCells.end());
}
void Grid::SpawnPendingCells() {
	for (PendingCellSpawn pendingCell : pendingSpawns) {
		if (cells[pendingCell.pos.y][pendingCell.pos.x] == nullptr) {
			CreateCell(pendingCell.pos, pendingCell.type);
		}
	}
	pendingSpawns.clear();
}

//Draws our grid mesh, updates all active cells within the map, and spits out
void Grid::Update() {
	gridMesh.DrawMesh();
	for (int y = cellAmount - 1; y >= 0; --y) {
		for (int x = 0; x < cellAmount; ++x) {
			Cell* c = cells[y][x];
			if (!c || !c->active) continue;
			switch (c->cellType) {
			case sand:
				UpdateSand(c);
				break;
			case wood:
				UpdateWood(c);
				break;
			case fire:
				UpdateFire(c);
				break;
			case water:
				UpdateWater(c);
				break;
			}
		}
	}
	SpawnPendingCells();
	CleanUp();
}
void Grid::Draw(float deltaTime) {
	m_deltaTime = deltaTime;
	gridMesh.DrawMesh();
	Update();
	for (int y = cells.size() - 1; y >= 0; --y) {
		for (int x = 0; x < cells[0].size(); x++) {
			if (cells[y][x] == nullptr)
				continue;
			glm::mat4 model = cells[y][x]->UpdatedModel(cells[y][x]->gridPosition);
			SharedCellMesh::shader->use();
			SharedCellMesh::shader->setVec4("color", cells[y][x]->color);
			SharedCellMesh::shader->setMat4("model", cells[y][x]->model);
			cells[y][x]->Draw();
		}
	}
}
void Grid::Delete() {
	gridMesh.Delete();
	gridShader.Delete();
	cellShader.Delete();
	for (Cell* _cell : activeCells) {
		_cell->Delete();
		delete _cell;
	}
}
