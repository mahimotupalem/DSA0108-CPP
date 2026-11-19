#include "GameMenu.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow) {
    qc::GameMenu game;
    return game.run(hInstance, nCmdShow);
}
