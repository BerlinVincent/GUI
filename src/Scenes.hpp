#pragma once

#include <raylib.h>

#include <cmath>
#include <memory>
#include <string>
#include <vector>
#include <optional>
#include <map>

#include "Elements.hpp"

// forward declarations

class SceneManager;

// helper functions

/**
 * @brief Abstract base class for all other scenes
 */
class Scene {
    
    protected:
    bool m_running;                     // wether scene is supposed to be running or not
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
    
    protected:
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
    
    protected:
    // the map in tiles
    std::vector<std::vector<Tile>> m_tileMap;
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
    
    // the set of textures tiles render as
    std::unordered_map<std::string, Texture2D> m_textureCache;
    // texture for the player
    Texture2D * t_Player;
    // draw static textures here once and reuse
    RenderTexture2D worldBuffer;
    // wether the world buffer needs to be updated
    bool worldBufferValid;
    
public:
    void preload_textures();
    void update() override;
    virtual void drawStatic();
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
        
        
        // initialize a standard map
        
        m_tileMap = std::vector<std::vector<Tile>>(15, std::vector<Tile>(15, Tile("../../Textures/Tileset Stone.png")));
        
        // load the textures
        
        preload_textures();
        
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
        preload_textures();
    }

    ~WorldScene() {
        // unload cached textures
        for (auto &[path, texture] : m_textureCache) {
            UnloadTexture(texture);
        }
        UnloadRenderTexture(worldBuffer);
        m_textureCache.clear();
    }
};

class WorldEditor : public WorldScene {

protected:
    // std::vector<std::vector<Tile>> m_tileMap;
    Tile m_currentTile;
    // editor menu elements
    std::vector<Button> ui_leftSideBar;
    std::vector<Rectangle> ui_bottomBar;
    std::vector<Tile> ui_tilePalette;
    // position of the cursor
    Vector2 lastMouse;

public:
    void update() override;
    void drawStatic() override;
    void draw() override;

    bool isWalkable(int x, int y) {
        x = x / tileSize;
        y = y / tileSize;

        if (x < 0 || x >= m_tileMap[y].size() || y < 0 || y >= m_tileMap.size()) return false;
        return true;        
    }

    WorldEditor(SceneManager *manager) : WorldScene(manager) {

        ui_leftSideBar = {
            Button("Plants",    [](){}, Align::center, TextFontSize::small),
            Button("Props",     [](){}, Align::center, TextFontSize::small),
            Button("Struct",    [](){}, Align::center, TextFontSize::small),
            Button("Grass",     [](){}, Align::center, TextFontSize::small),
            Button("Stone",     [](){}, Align::center, TextFontSize::small),
            Button("Walls",     [](){}, Align::center, TextFontSize::small)
        };
    }
};

struct MainMenu : public MenuScene {
    MainMenu(SceneManager *manager);
};