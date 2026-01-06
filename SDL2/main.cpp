#include<SDL3/SDL.h>
#include "InputHandler.h"
//Sean Made Headers
#include "Game.h"


int main(int argc, char* argv[])
{
    //--- SetUp Window
    int resX = 1000; int resY = 1000; const char* name = "Sean's Sand Sim"; 
    GameInitArgs gameInitArgs{
        name, resX, resY,
        SDL_WINDOW_OPENGL
    };
    //InputHandler inputHandler();
    Game game{};
    InputHandler::Instance()->init(&game);
    game.init(gameInitArgs);
    game.update();
    InputHandler::Instance()->clean();
    return 0;
}