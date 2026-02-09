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
	private :
		glm::vec2 gridPosition;
		glm::vec2 ndc;
	
		CellType cellType;

		//Move to vectors that hold our data instead of per model
		glm::mat4 model;
		glm::vec4 color = glm::vec4(1, 1, 1, 1);
		
		//Store in flags
		float lifeTime = 1;
		int density = 0;
		bool isSpreadedFire = false;
		int unchangedFrames = 0;
		bool active = true;
		bool deleting = false;
		bool moved = false;
	public :
		Cell(){}
		Cell(glm::vec2 gridPos, int _cellAmount, glm::vec2 cellSize, glm::vec2 _resolution, CellType type);
	
		float GetLifeTime() { return lifeTime; }
		CellType GetCellType() { return cellType; }

		bool GetFlamable() { return isSpreadedFire; }
		bool GetActive() { return active; }
		float GetUnchangedFrames() { return unchangedFrames; }
		glm::vec2 GetGridPos() { return gridPosition; }
		float GetDensity() { return density; }
		bool GetMoved() { return moved; }
		glm::mat4 GetModel() { return model; }
		glm::vec4 GetColor() { return color; }

		void SetGridPos(glm::vec2 _pos) { this->gridPosition = _pos; }
		void SetLifeTime(float _lifeTime) { this->lifeTime = _lifeTime;}
		void SetActive(bool _active) { this->active = _active; }
		void SetModel(glm::mat4 _model) { this->model = _model; }
		void SetUnchangedFrames(float _frames) { this->unchangedFrames = _frames; }
		void SetCellType(CellType _cellType) { this->cellType = _cellType; }
		void SetDensity(float _density) { this->density = _density; }
		void SetMoved(bool _state) { this->moved = _state; }
		void SetColor(glm::vec4 _color) { this->color = _color; }
		void IncrementUnchangedFrames() { this->unchangedFrames++; }
		void ChangeLifeTime(float _delta) { this->lifeTime += _delta; }
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
	
		ShaderProgram gridShader;
		ShaderProgram cellShader;
		Mesh gridMesh;
		
		int cellAmount;
		glm::vec2 resolution;
		glm::vec2 cellSize;

		vector<glm::mat4> instanceModels;
		vector<glm::vec4> instanceColors;
		
		std::vector<Cell*> toActivate;
		vector<PendingCell> pendingSpawns;
		vector<PendingCell> pendingCellDeletion;

		const int MAX_QUEUE = 555;
		
		float fireSpreadChance = 1.0f;
		float m_deltaTime;
		
	public:
		//--- Instantiation
		Grid();
		Grid(GridInitArgs _gridInitArgs);
		void init(GridInitArgs _gridInitArgs);

		/* @Brief: Intializes Cell in memory.
		*  
		*  Set's cell's parameters, updates model, 
		*  and set's random variables.
		*
		*  @Param: glm::vec2 Pos for grid position, and CellType for fire, wood, etc.
		*  @Return: pointer to the instantiated cell
		*/
		Cell* CreateCell(glm::vec2 pos, CellType type);

		/* @Brief: Initializes a collection of cells using a circles bounds
		*
		*  Create's Cells within the grid using the bounds of a circles radius
		*  set's cells type
		*
		*  @Param: Circle* _circle for radius and position; CellType _type for physics
		*  @Return: void
		*/
		void CreateCellsFromCircle(Circle* _circle, CellType _type);
		//---
		void DeleteCellsFromCircle(Circle* _circle);
		/* @Brief: Checks bound of passed ints in Grid
		*
		*  @Param: int x, int y flipped in cells[y][x]
		*  @Return: bool true if in, false if not
		*/
		bool InBounds(int x, int y);
				
		//--- Physics

		/* @Brief: Finds the new Position of a falling Cell
		*
		*  Adds +1 to the y position of the passed in glm::vec2
		*
		*  @Param: glm::vec2 gridPosition, position of the falling cell
		*  @Return: glm::vec2 newPosition of the falling cell
		*/
		glm::vec2 GravityPos(glm::vec2 gridPosition);
		
		/* @Brief: Finds the new Position of a falling spreading cell
		*
		*  Adds +1 to the y position, and +1 or -1 to the x, of the passed in glm::vec2
		*
		*  @Param: glm::vec2 gridPosition, position of the falling spreading cell
		*  @Return: glm::vec2 newPosition of falling spreading cell
		*/
		vector<glm::vec2> GravityDispersalPos(glm::vec2 gridPosition);
		
		/* @Brief: Finds the new Position of a spreading cell
		*
		*  Adds +1 or -1 to the x, of the passed in glm::vec2
		*
		*  @Param: glm::vec2 gridPosition, int leftRight for bias in movement 
		*  @Return: glm::vec2 newPosition of spreading cell
		*/
		vector<glm::vec2> DispersalPos(glm::vec2 gridPosition, int );
		
		/* @Brief: Wakes up Cells in surrounding area
		*
		*  Adds dirs[i] to x,y of our passed in coordinates
		*  checks if inactive and reactivates.
		*  Dirs[i] is a static ivec2 for delta changes to locate neighbors.
		* 
		*  @Param: int x,y as coordinates of cell waking neighbors 
		*  @Return: void
		*/
		void WakeNeighbors(int x, int y);
		//Todo Seperate Physics, and rendering into two seperate .cpp files
		//Todo Remove One or all of these
		
		/* @Brief: Moves a cell to a position
		*
		*  Moves cell c to a new position nullifying it's old, updating its model, 
		*  and changing the cells[][].
		*
		*  @Param: Cell* c, moved cell, going to glm::ivec2 newPos
		*  @Return: void
		*/
		void Move(Cell* c, glm::ivec2 newPos);

		/* @Brief: Checks if a cell can move to a new position
		*
		*  Checks bounds, checks if cells[y][x] == nullptr
		*  DOESN'T MOVE ACTUAL CELL IN GRID
		*  @Param: Cell* c, going to glm::ivec2 newPos
		*  @Return: bool returns true if can move, false if can't
		*/
		bool MoveCell(glm::vec2 _newPos);

		/* @Brief: Checks if cell is surrounded
		*
		*  Checks if above, below, left, and right are 
		*  @Param: Cell* c, going to glm::ivec2 newPos
		*  @Return: bool returns true if can move, false if can't
		*/
		bool IsBoxed(Cell* c);

		/* @Brief: Checks if cell is surrounded
		*
		*  Checks if above, below, left, and right are
		*  @Param: Cell* c, going to glm::ivec2 newPos
		*  @Return: bool returns true if can move, false if can't
		*/
		bool CheckNewFirePos(glm::vec2 _newPos);

		void SwitchCellType(glm::vec2 _newPos, CellType type);
		void SwapCells(glm::vec2 c1Pos, glm::vec2 c2Pos);

		void UpdateSand(Cell* _cell);
		void UpdateWood(Cell* _cell);
		void UpdateFire(Cell* _cell);
		void UpdateWater(Cell* _cell);
		void UpdateSmoke(Cell* _cell);
		void ActivateCells();
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
	};