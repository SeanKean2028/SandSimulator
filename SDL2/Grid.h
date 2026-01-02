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
			wood
	};
	struct PendingCellSpawn {
		glm::ivec2 pos;
		CellType type;
	};
	class Cell {
		glm::vec2 size;
		glm::vec2 resolution;
		int cellAmount;
		vector<float> vertices;
	public:
		glm::vec2 gridPosition;
		CellType cellType;
		glm::mat4 model;
		glm::vec2 ndc;
		glm::vec4 color = glm::vec4(1, 1, 1, 1);
		float lifeTime = 1;
		bool isSpreadedFire = false;
		int unchangedFrames = 0;
		bool active = true;
		Cell(){}
		Cell(glm::vec2 gridPos, int _cellAmount, glm::vec2 cellSize, glm::vec2 _resolution, CellType type);
		void init(glm::vec2 gridPos, int _cellAmount, glm::vec2 cellSize, glm::vec2 _resolution, CellType type);

		//Converts this->gridPosition to screenPosition
		glm::vec2 ScreenPosition();
		
		//New pos = y+1, x, simulates the space right below our cell!
		glm::vec2 GravityPos();
		
		//New positions = y+1, x+1; y+1, x-1; simulates the spreading affect of sand!
		vector<glm::vec2> GravityDispersalPos();

		//New positions = x+1, x-1; Left Right
		vector<glm::vec2> DispersalPos();
		
		glm::mat4 UpdatedModel(glm::vec2 newPos);

		void Draw();
		void Delete();
	};
	class Grid {
		vector<vector<Cell*>> cells;
		vector<Cell*> activeCells;

	public:
		ShaderProgram gridShader;
		ShaderProgram cellShader;
		Mesh gridMesh;

		glm::vec2 resolution;
		glm::vec2 cellSize;
		vector<PendingCellSpawn> pendingSpawns;
		const int MAX_QUEUE = 555;
		//Chance of fire to burnout in percentage per second
		float fireSpreadChance = 1.0f;
		int cellAmount;
		Grid();
		Grid(int _resX, int _resY, int _cells);
		void init(int _resX, int _resY, int _cells);

		void CreateCell(glm::vec2 pos, CellType type);
		Cell* CreateFireCell(glm::vec2 pos);
		void CreateCellsFromCircle(Circle* _circle, CellType _type);
		
		bool MoveCell(glm::vec2 _newPos);
		bool CheckNewFirePos(glm::vec2 _newPos);

		void WakeNeighbors(int x, int y);
		bool InBounds(int x, int y);
		//Updates our Sand Cell's model to translate it's position to simulate falling
		void UpdateSand(Cell* _cell);
		void UpdateWood(Cell* _cell);
		void UpdateFire(Cell* _cell);
		void UpdateWater(Cell* _cell);
		void CleanUp();
		void SpawnPendingCells();
		//Draws our grid mesh, updates all active cells within the map, and spits out
		void Update();
		void Draw(float deltaTime);
		void Delete();
	private:
		float m_deltaTime;
	};