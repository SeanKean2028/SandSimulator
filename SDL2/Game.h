#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include "Grid.h"

#include "Circle.h"
using namespace std;
struct GameInitArgs {
	const char* title;
	int width;
	int height;
	int flags;
};
class Game {
	public :
		Game() { cout << "Instantiating: Game \n"; }
		~Game(){}
		bool init(GameInitArgs initArgs);
		void render();
		void update();
		void handleEvents();
		void clean();
		bool running() { return m_bRunning; }
		float getDeltaTime() const { return m_deltaTime; }
		GameInitArgs gameArgs;
		Grid m_Grid;
		Circle m_MouseCircle;
		bool m_bRunning;


	private:
		SDL_Window* m_pWindow;
		SDL_Renderer* m_pRenderer;
		SDL_GLContext m_pContext = {};
		float m_deltaTime = 0.0f;
		Uint64 m_lastCounter = 0;
};