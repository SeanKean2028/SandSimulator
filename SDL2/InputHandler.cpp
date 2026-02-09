#include "InputHandler.h"
#include <string>
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
        if (mouseDown[0]) {
            m_pGame->getGrid().CreateCellsFromCircle(
                &m_pGame->getMouseCircle(), spawningCellType);
        }
        if (mouseDown[1]) {
            m_pGame->getGrid().DeleteCellsFromCircle(
                &m_pGame->getMouseCircle());
        }
        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN ) {
            if (static_cast<int>(event.button.button) == 1) {
                m_pGame->getGrid().CreateCellsFromCircle(
                    &m_pGame->getMouseCircle(), spawningCellType);
                mouseDown[0] = true;
            }
            if (static_cast<int>(event.button.button) == 3) {
                m_pGame->getGrid().DeleteCellsFromCircle(
                    &m_pGame->getMouseCircle());
                mouseDown[1] = true;
            }
        }
        if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
            for (int i = 0; i < mouseDown.size(); i++ )
                mouseDown[i] = false;
    
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