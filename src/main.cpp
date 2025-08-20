#include "SceneManager.hpp"

auto main(int argc, char *argv[]) -> int {
    SceneManager sceneManager;

    sceneManager.pushScene<MainMenu>();

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Example");
    SetTargetFPS(FRAMERATE);
    SetExitKey(0);
    sceneManager.run();
}