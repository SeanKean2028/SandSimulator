#pragma once
#include <SDL3/SDL.h>
#include "Grid.h"
class Game;

class InputHandler {
private:
    InputHandler();
    ~InputHandler();
    static InputHandler* s_pInstance;
    Game* m_pGame = nullptr;
    const bool* m_keyStates;
public:
    static InputHandler* Instance();

    bool init(Game* game);
    void update();
    bool isKeyDown(SDL_Scancode key);
    void clean();

    bool mouseDown = false;
    CellType spawningCellType = sand;



};
typedef InputHandler TheInputHandler;