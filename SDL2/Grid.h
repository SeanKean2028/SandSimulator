	#pragma once
	#include <GL/glew.h>
	#include <glm.hpp>
	#include <vector>
	#include "Mesh.h"
	#include "Shader.h"
	enum CellType {
		none,
		sand,
		water,
		fire,
		wood

	};

	using namespace std;
	class Cell {
		Mesh cellMesh;
		glm::vec2 size;
		glm::vec2 resolution;
		int cellAmount;
		vector<float> vertices;
		vector<int> cellIndices{
			0, 1, 2,
			2, 3, 0
		};
	public : 
		ShaderProgram shader;
		glm::vec2 gridPosition;
		CellType cellType = none;
		glm::mat4 model;
		Cell() {

		}
		Cell(glm::vec2 gridPos, ShaderProgram _shader, int _cellAmount, glm::vec2 cellSize, glm::vec2 _resolution, CellType type)
		{
			gridPosition = gridPos;
			size = cellSize;
			resolution = _resolution;
			shader = _shader;
			cellType = type;
			cellAmount = _cellAmount;
			vertices = {
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
			 cellMesh = Mesh(vertices.data(), vertices.size(), cellIndices.data(), cellIndices.size(), shader);
			 shader.use();
			 shader.setVec4("color", glm::vec4(0.97f, 0.98f, 0.62f, 1));
		}
		//Converts this->gridPosition to screenPosition
		glm::vec2 ScreenPosition() {
			return glm::vec2((gridPosition * size) * 0.5f);
		}
		//New pos = y+1, x, simulates the space right below our cell!
		glm::vec2 GravityPos() {
			int nGridPosY = gridPosition.y + 1;
			int nGridPosX = gridPosition.x;
			if (nGridPosY >= cellAmount)   // STOP at bottom
				return gridPosition;
			return glm::vec2(nGridPosX, nGridPosY);
		}
		//New positions = y+1, x+1; y+1, x-1; simulates the spreading affect of sand!
		vector<glm::vec2> GravityDispersalPos() {
			int nGridPosY = gridPosition.y + 1;
			
			int nGridPosX1 = gridPosition.x + 1;
			int nGridPosX2 = gridPosition.x - 1;
			
			glm::vec2 pos1 = glm::vec2(nGridPosX1, nGridPosY);
			glm::vec2 pos2 = glm::vec2(nGridPosX2, nGridPosY);
			
			vector<glm::vec2> _vec2 = { pos1, pos2 };
			return _vec2;
		}
		glm::mat4 UpdatedModel(glm::vec2 newPos) {
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

			shader.use(); 
			glm::mat4 model = glm::mat4(1.0f);
			glm::vec3 translation = glm::vec3(ndcX, ndcY, 0);
			model = glm::translate(glm::mat4(1.0f), translation);
			model = glm::scale(model, glm::vec3(cellNdcW, cellNdcH, 1.0f));

			return model;
		}

		void Draw() {
			shader.use();
			cellMesh.DrawMesh();
		}
		void Delete() {
			cellMesh.Delete();
		}
	};
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
	class Grid {
		vector<vector<Cell*>> cells;
		vector<Cell*> activeCells;

	public : 
		ShaderProgram gridShader;
		ShaderProgram cellShader;
		Mesh gridMesh;

		glm::vec2 resolution;
		glm::vec2 cellSize;
	
		int cellAmount;
		Grid() {

		}
		Grid(int _resX, int _resY, int _cells){	
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
		
			gridMesh = Mesh(GridVertices, sizeof(GridVertices), GridIndices, sizeof(GridIndices), gridShader);
		
			//Generate CellShader
			VertexShader cellVertex("Shaders/cellVertex.glsl", GL_VERTEX_SHADER);
			FragmentShader cellFragment("Shaders/cellFragment.glsl", GL_FRAGMENT_SHADER);

			cellShader = ShaderProgram(cellVertex, cellFragment);
		}
		void CreateCell(glm::vec2 pos, CellType type) {
			Cell* newCell = new Cell(
				pos,
				cellShader,
				cellAmount,
				cellSize,
				resolution,
				type
			);
			activeCells.push_back(newCell);

			// Store the pointer in the grid array
			cells[(int)pos.y][(int)pos.x] = newCell;
		}
		bool MoveCell(glm::vec2 _newPos) {
			if (_newPos.x < 0 || _newPos.y < 0)
				return false;

			if (_newPos.x >= cellAmount || _newPos.y >= cellAmount)
				return false;

			return cells[_newPos.y][_newPos.x] == nullptr;
		}
		//Updates our Sand Cell's model to translate it's position to simulate falling
		void UpdateSand(Cell *_cell) {
			glm::vec2 newPos = _cell->GravityPos();
			//Try fall
			if (_cell->gridPosition.y >= cellAmount - 1) {
				return;
			}
			if (MoveCell(newPos)) {
				glm::mat4 model = _cell->UpdatedModel(newPos);
				_cell->model = model;
				cells[_cell->gridPosition.y][_cell->gridPosition.x] = nullptr;
				_cell->shader.setMat4("model", model);
				_cell->gridPosition = newPos;
				cells[newPos.y][newPos.x] = _cell;
			}
			
			else {
				//Try fall Diagnol
				for (glm::vec2 _newPos : _cell->GravityDispersalPos()) {
					if (MoveCell(_newPos)) {
						cells[_cell->gridPosition.y][_cell->gridPosition.x] = nullptr;
						glm::mat4 _model = _cell->UpdatedModel(_newPos);
						_cell->model = _model;
						_cell->shader.setMat4("model", _model);
						_cell->gridPosition = _newPos;
						cells[_newPos.y][_newPos.x] = _cell;
						break;
					}
				}
			}
		}
		//Draws our grid mesh, updates all active cells within the map, and spits out
		void Update() {
			gridMesh.DrawMesh();
			for (int y = cells.size() - 2; y >= 0; --y) {
				for (int x = 0; x < cells[0].size(); ++x) {
					Cell* _cell = cells[y][x];
					if (_cell != nullptr) {
						switch (_cell->cellType) {
						case sand:
							UpdateSand(_cell);
							break;
						}
					}
				}
			}
		}
		void Draw() {
			gridMesh.DrawMesh();
			Update();
			for (Cell* _cell : activeCells) {
				glm::mat4 model = _cell->UpdatedModel(_cell->gridPosition);

				_cell->shader.use();
				_cell->shader.setMat4("model", _cell->model);

				_cell->Draw();
			}
		}
		void Delete() {
			gridMesh.Delete();
			gridShader.Delete();
			cellShader.Delete();
			for (Cell *_cell : activeCells) {
				_cell->Delete();
				delete _cell;
			}
		}
	};