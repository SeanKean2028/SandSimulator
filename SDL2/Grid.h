	#pragma once
	#include <GL/glew.h>
	#include <glm.hpp>
	#include <vector>
	#include <algorithm>
	#include <random>

	#include "Mesh.h"
	#include "Shader.h"
	#include "Circle.h"
	
	using namespace std;
		
	enum CellType {
		none,
		sand,
		water,
		fire,
		wood,
		ash,
		smoke
	};
	struct PendingCell {
		glm::ivec2 pos;
		CellType type;
	};
	struct Cell {
	public:
		glm::vec2 gridPosition;
		glm::vec2 ndc;
	
		CellType cellType;

		//Move to vectors that hold our data instead of per model
		glm::mat4 model;
		glm::vec4 color = glm::vec4(1, 1, 1, 1);
		
		//Store in flags
		float lifeTime = 1;
		bool isSpreadedFire = false;
		int unchangedFrames = 0;
		bool active = true;
		bool deleting = false;

		Cell(){}
		Cell(glm::vec2 gridPos, int _cellAmount, glm::vec2 cellSize, glm::vec2 _resolution, CellType type);
		

		void Delete();
	};
	struct GridInitArgs {
		int _resX;
		int _resY;
		int _cells;
	};
	//Todo Redefine whats public vs private data
	class Grid {
		vector<vector<Cell*>> cells;
		vector<Cell*> activeCells;
	public:
		ShaderProgram gridShader;
		ShaderProgram cellShader;
		Mesh gridMesh;
		
		int cellAmount;
		glm::vec2 resolution;
		glm::vec2 cellSize;

		vector<glm::mat4> instanceModels;
		vector<glm::vec4> instanceColors;

		vector<PendingCell> pendingSpawns;
		vector<PendingCell> pendingCellDeletion;

		const int MAX_QUEUE = 555;
		
		float fireSpreadChance = 1.0f;
		
		
		//--- Instantiation
		Grid();
		Grid(GridInitArgs _gridInitArgs);
		void init(GridInitArgs _gridInitArgs);

		//change to work together
		Cell* CreateCell(glm::vec2 pos, CellType type);
		void CreateCellsFromCircle(Circle* _circle, CellType _type);
		//---
		

		bool InBounds(int x, int y);
				
		//--- Physics
		glm::vec2 GravityPos(glm::vec2 gridPosition);
		vector<glm::vec2> GravityDispersalPos(glm::vec2 gridPosition);
		vector<glm::vec2> DispersalPos(glm::vec2 gridPosition, int );
		void WakeNeighbors(int x, int y);
		//Todo Seperate Physics, and rendering into two seperate .cpp files
		//Todo Remove One or all of these
		void Move(Cell* c, glm::ivec2 newPos);
		bool MoveCell(glm::vec2 _newPos);
		bool IsBoxed(Cell* c);
		bool CheckNewFirePos(glm::vec2 _newPos);
		void SwitchCellType(glm::vec2 _newPos, CellType type);

		void UpdateSand(Cell* _cell);
		void UpdateWood(Cell* _cell);
		void UpdateFire(Cell* _cell);
		void UpdateWater(Cell* _cell);
		void UpdateSmoke(Cell* _cell);

		void Update();
		void UpdateInstanceBuffers();
		//---
		
		//--- Rendering
		glm::mat4 UpdatedModel(glm::vec2 newPos);
		//---
		int CountEmptyHoriz(glm::ivec2 pos, int dir);
		void CleanUp();
		void SpawnPendingCells();
		void DeletePendingCells();
		void Draw(float deltaTime);
		void Delete();
	private:
		float m_deltaTime;
	};