#include "InputHandler.h"
#include "Game.h"
#include "Grid.h"
InputHandler* InputHandler::s_pInstance = nullptr;

InputHandler::InputHandler() {}
InputHandler::~InputHandler() {}

InputHandler* InputHandler::Instance() {
    if (!s_pInstance)
        s_pInstance = new InputHandler();
    return s_pInstance;
}

bool InputHandler::init(Game* game) {
    m_pGame = game;
    return true;
}
bool InputHandler::isKeyDown(SDL_Scancode key) {
    return m_keyStates && m_keyStates[key];
}
void InputHandler::update() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            m_pGame->setRunning(false);
            return;
        }


        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            m_pGame->getGrid().CreateCellsFromCircle(
                &m_pGame->getMouseCircle(), spawningCellType);
            mouseDown = true;
        }
        else if (mouseDown) {
            m_pGame->getGrid().CreateCellsFromCircle(
                &m_pGame->getMouseCircle(), spawningCellType);
        }

        if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
            mouseDown = false;
    
    }
    m_keyStates = SDL_GetKeyboardState(nullptr);
    if (isKeyDown(SDL_SCANCODE_1))
        spawningCellType = sand;
    if (isKeyDown(SDL_SCANCODE_2))
        spawningCellType = wood;
    if (isKeyDown(SDL_SCANCODE_3))
        spawningCellType = fire;
    if (isKeyDown(SDL_SCANCODE_4))
        spawningCellType = water;
    if (isKeyDown(SDL_SCANCODE_5))
        spawningCellType = smoke;
}

void InputHandler::clean() {
    delete s_pInstance;
    s_pInstance = nullptr;
}