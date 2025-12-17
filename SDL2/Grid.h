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
		}
		glm::vec2 ScreenPosition() {
			return glm::vec2((gridPosition * size) * 0.5f);
		}
		glm::vec2 newPosition() {
			int nGridPosY = gridPosition.y + 1;
			int nGridPosX = gridPosition.x;
			return glm::vec2(nGridPosX, nGridPosY);
		}
		glm::mat4 UpdatePosition() {
			int nGridPosY = gridPosition.y + 1;
			int nGridPosX = gridPosition.x;
			if (nGridPosY > cellAmount)
				return glm::mat4(1.0f);
			//Grid → Screen (center)
			float cellPixelW = resolution.x / cellAmount;
			float cellPixelH = resolution.y / cellAmount;

			float screenX = (gridPosition.x + 0.5f) * cellPixelW;
			float screenY = (gridPosition.y + 0.5f) * cellPixelH;

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

			vector<vector<Cell*>> temp(cellAmount + 1, vector<Cell*>(cellAmount + 1));
			cells = temp;

			//Generate GridShader
			VertexShader gridVertex("Shaders/gridVertex.glsl", GL_VERTEX_SHADER);
			FragmentShader gridFragment("Shaders/gridFragment.glsl", GL_FRAGMENT_SHADER);

			glm::mat4 model = glm::mat4(1.0f);
		
			gridShader = ShaderProgram(gridVertex, gridFragment);
			gridShader.use();
			gridShader.setMat4("model", model);
			gridShader.setVec4("params", glm::vec4(1, _cells, 0, 0));
			gridShader.setVec4("color", glm::vec4(1, 0, 1, 0.5));
		
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
			if (_newPos.y >= cells.size() || _newPos.x >= cells[0].size())
				return false;
			if (cells[_newPos.y][_newPos.x] == nullptr) {
				return true;
			}
			return false;
		}
		void Draw() {
			gridMesh.DrawMesh();
			for (Cell *_cell : activeCells) {
				glm::mat4 model = _cell->UpdatePosition();
				glm::vec2 newPos = _cell->newPosition();
				if (MoveCell(newPos)) {
					cells[_cell->gridPosition.y][_cell->gridPosition.x] = nullptr;
					_cell->shader.setMat4("model", model);
					_cell->gridPosition = newPos;
					cells[newPos.y][newPos.x] = _cell;
				}
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