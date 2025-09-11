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
    // the set of textures tiles render as
    Texture2D m_tileSet;
    Texture2D m_playerSprite;
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

    WorldScene(SceneManager *manager, std::vector<std::vector<Tile>> tileMap) 
        : Scene(manager), m_tileMap(tileMap), tileSize(32) {
        startScene();

        // load the tile set

        m_tileSet = LoadTexture("../../Textures/Tileset Stone.png");
        m_playerSprite = LoadTexture("../../Textures/Player.png");

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

    ~WorldScene() {
        UnloadTexture(m_tileSet);
        UnloadTexture(m_playerSprite);
    }
};

class WorldEditor : public Scene {

private:
    std::vector<std::vector<Tile>> m_tileMap;
    Tile m_currentTile;

    Texture2D Plants;
    Texture2D PlantsShadows;
    Texture2D Player;
    Texture2D Props;
    Texture2D PropsShadow;
    Texture2D Structure;
    Texture2D TilesetGrass;
    Texture2D TilesetStone;
    Texture2D TilesetWall;

public:
    void update() override;
    void draw() override;

    WorldEditor(SceneManager *manager) : Scene(manager) {
        startScene();
    }
};

struct MainMenu : public MenuScene {
    MainMenu(SceneManager *manager);
};