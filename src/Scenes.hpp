#pragma once

#include <raylib.h>

#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include "Elements.hpp"

class SceneManager;

/**
 * @brief Abstract base class for all other scenes
 */
class Scene {

private:
    bool m_running;                     // wether scene is supposed to be running or not

protected:
    SceneManager *m_manager = nullptr;  // back-reference to this scene's manager
    
public:
    virtual ~Scene() = default;
    Scene(SceneManager *manager = nullptr) : m_manager(manager) {}

    // update scene's logic
    virtual void update() = 0;
    // display scene's visuals
    virtual void draw() = 0;

    // setter for m_running = true
    virtual void startScene() { m_running = true; }
    // setter for m_running = false
    virtual void killScene() { m_running = false; }
    // getter for m_running
    virtual bool running() const { return m_running; }
};

class MenuScene : public Scene {

private:
    std::string m_menuTitle;
    std::vector<Element *> m_elements;

    // index for the selected / highlighted element
    size_t highlight = 0;
    // position of the cursor
    Vector2 lastMouse;

public:
    void update() override;
    void draw() override;

    MenuScene(SceneManager *manager, std::string title, std::vector<Element *> elements)
        : Scene(manager), m_menuTitle(title), m_elements(std::move(elements)) {
        startScene();
    }

    ~MenuScene() {
        for (Element *e : m_elements) delete e;
    }
};

class WorldScene : public Scene {

private:
    // the map in tiles
    std::vector<std::vector<Tile>> m_tileMap;
    // player sprite
    Texture2D t_Player;
    int tileSize;
    
    Camera2D camera;
    // where the player is
    Vector2 playerPos;
    // where the player should be
    Vector2 nextPos;
    // how fast the player goes
    float moveSpeed;
    // last direction the player moved in
    int lastMoveDirection;

public:
    void update() override;
    void draw() override;

    bool isWalkable(int y, int x) {
        x = x / tileSize;
        y = y / tileSize;
        // out of bounds coordinates
        if (x < 0 || x >= m_tileMap[y].size() || y < 0 || y >= m_tileMap.size()) return false;
        // non walkable tile in bounds
        if (m_tileMap[y][x].m_tileSetCoordinates.y < 0 || m_tileMap[y][x].m_tileSetCoordinates.x < 0) return false;

        // if nothing should prevent movement, tile is walkable
        return true;
    }

    // standard constructor, only needs a manager
    WorldScene(SceneManager *manager) 
        : Scene(manager), tileSize(32) {
        startScene();

        t_Player = LoadTexture("../../Textures/Player.png");

        // initialize a standard map

        m_tileMap = std::vector<std::vector<Tile>>(15, std::vector<Tile>(15, Tile(-1, -1)));

        // initialize the position data

        playerPos = {2.0f * tileSize, 2.0f * tileSize}; // player is initially in the middle of the map
        nextPos = playerPos;                            // and isn't moving
        moveSpeed = tileSize / 16;                      // but has a nice pace if they start to move
        lastMoveDirection = 0;                          // player initially faces southwards

        // initialize the camera

        camera.target = {playerPos.x + tileSize / 2.0f, playerPos.y + tileSize / 2.0f}; // camera is centered on the player
        camera.offset = {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};    // and is places in the middle of the screen
        camera.rotation = 0.0f;                                                         // is not rotated
        camera.zoom = tileSize / 16;                                                    // and is zoomed in so everything looks big
    }

    // constructor that also takes a custom tile map
    WorldScene(SceneManager *manager, std::vector<std::vector<Tile>> tileMap)
        : WorldScene(manager) {
        m_tileMap = tileMap;
    }
};

class WorldEditor : public WorldScene {

private:
    std::vector<std::vector<Tile>> m_tileMap;
    Tile m_currentTile;
    // the set of textures tiles render as
    Texture2D t_Plants;
    Texture2D t_PlantsShadows;
    Texture2D t_Player;
    Texture2D t_Props;
    Texture2D t_PropsShadow;
    Texture2D t_Structure;
    Texture2D t_Grass;
    Texture2D t_Stone;
    Texture2D t_Walls;

public:
    void update() override;
    void draw() override;

    WorldEditor(SceneManager *manager) : WorldScene(manager) {

        // load the textures

        t_Plants = LoadTexture("../../Textures/t_Plants.png");
        t_PlantsShadows = LoadTexture(("../../Textures/t_Plants Shadows.png"));
        t_Player = LoadTexture(("../../Textures/Player.png"));
        t_Props = LoadTexture(("../../Textures/Props.png"));
        t_PropsShadow = LoadTexture(("../../Textures/Props Shadow.png"));
        t_Structure = LoadTexture(("../../Textures/Structure.png"));
        t_Grass = LoadTexture(("../../Textures/Tileset Grass.png"));
        t_Stone = LoadTexture(("../../Textures/Tileset Stone.png"));
        t_Walls = LoadTexture(("../../Textures/Tileset Walls.png"));
    }

    ~WorldEditor() {
        UnloadTexture(t_Plants);
        UnloadTexture(t_PlantsShadows);
        UnloadTexture(t_Player);
        UnloadTexture(t_Props);
        UnloadTexture(t_PropsShadow);
        UnloadTexture(t_Structure);
        UnloadTexture(t_Grass);
        UnloadTexture(t_Stone);
        UnloadTexture(t_Walls);
    }
};

struct MainMenu : public MenuScene {
    MainMenu(SceneManager *manager);
};