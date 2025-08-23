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
    // the map in tiles
    std::vector<std::vector<Tile>> m_tileMap;
    // the set of textures tiles render as
    Texture2D m_tileSet;
    // map gets drawn here once and is then reused
    RenderTexture2D m_worldBuffer;
    int tileSize;
    
    Camera2D camera;
    // where the player is
    Vector2 playerPos;
    // where the player should be
    Vector2 nextPos;
    // how fast the player goes
    float moveSpeed;

public:
    void update() override;
    void draw() override;

    WorldScene(SceneManager *manager, float baseMoveSpeed) 
        : Scene(manager), tileSize(32) {
        startScene();

        // initialize the tile map and load the tile set

        m_tileMap = std::vector<std::vector<Tile>>(9, std::vector<Tile>(9, Tile({3, 3})));   // set size of map
        m_tileSet = LoadTexture("../../Pixel Art Top Down - Basic/Texture/TX Tileset Stone Ground.png");

        // initialize the world buffer with the tile map

        m_worldBuffer = LoadRenderTexture(
            (int)m_tileMap[0].size() * tileSize,
            (int)m_tileMap.size() * tileSize
        );

        // initialize the position data

        playerPos = {4.0f * tileSize, 4.0f * tileSize}; // player is initially in the middle of the map
        nextPos = playerPos;                            // and isn't moving
        moveSpeed = tileSize / 8;

        // initialize the camera

        camera.target = {playerPos.x + tileSize / 2.0f, playerPos.y + tileSize / 2.0f}; // camera is centered on the player
        camera.offset = {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};    // and is places in the middle of the screen
        camera.rotation = 0.0f;                                                         // is not rotated
        camera.zoom = 2.0f;                                                             // and is zoomed in so everything looks big

        // draw the map into the buffer

        BeginTextureMode(m_worldBuffer);
        ClearBackground(BLACK);

        for (int y = 0; y < m_tileMap.size(); y++) {        // all rows
           for (int x = 0; x < m_tileMap[y].size(); x++) {  // all columns
                Vector2 tileIndex = m_tileMap[y][x].m_tileSetCoordinates;

                // where texture comes from in the tile set
                Rect src {
                   .left = (int)tileIndex.x * tileSize,
                   .top = (int)tileIndex.y * tileSize,
                   .width = tileSize,
                   .height = tileSize
                };
                // where the texture goes in the tile map
                Rect dest {
                   .left = x * tileSize,
                   .top = y * tileSize,
                   .width = tileSize,
                   .height = tileSize
                };

                // draw the texture to the tile map

                DrawTexturePro(m_tileSet, src, dest, {0, 0}, 0.0f, WHITE);
            }
        }

        EndTextureMode();
    }

    ~WorldScene() {
        UnloadTexture(m_tileSet);
        UnloadRenderTexture(m_worldBuffer);
    }
};

struct MainMenu : public MenuScene {
    MainMenu(SceneManager *manager);
};