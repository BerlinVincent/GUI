#include "Scenes.hpp"
#include "SceneManager.hpp"
#include "TestRoom.hpp"

void MenuScene::update() {
    int key = GetKeyPressed();
    
    switch (key) {
        case KEY_DOWN: {
            highlight = (highlight == m_elements.size() - 1) ? 0 : highlight + 1;
            break;
        }
        case KEY_UP: {
            highlight = (highlight == 0) ? m_elements.size() - 1 : highlight - 1;
            break;
        }
        case KEY_ENTER: {
            m_elements[highlight]->handleSelect();
            break;
        }
        case KEY_ESCAPE: {
            killScene();
        }
    }
}

void MenuScene::draw() {
    Vector2 mousePos = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    
    int width = GetScreenWidth();
    int height = GetScreenHeight();
    float padding = 0.2;
    float border = 0.1;
    
    // coordinate box for menu elements to be drawn on
    Rect box{
        .left = (int)(width * border / 2),
        .top = (int)(height * border * 2),
        .width = (int)(width * (1 - border)),
        .height = (int)((height - box.top) * (1 - border) / (m_elements.size() + 1))
    };
    
    BeginDrawing();
    ClearBackground(BLACK);
    
    // draw the menu title
    DrawText(
        m_menuTitle.c_str(),
        (width - MeasureText(m_menuTitle.c_str(), height * border)) / 2,
        height * border / 2,
        height * border,
        YELLOW
    );
    
    // coordinate box shift amount
    int deltaY = box.height * (1 + padding);

    // tell all menu elements to draw themselves in their coordinate box
    // and wether they are selected
    for (int i = 0; i < (int)m_elements.size(); i++) {
        m_elements[i]->draw(box, i == highlight);
        
        // handle mpuse interaction
        if (CheckCollisionPointRec(mousePos, box)) {
            if (mousePos.x != lastMouse.x || mousePos.y != lastMouse.y) {
                highlight = i;
                lastMouse = mousePos;
            }
            if (click)
            m_elements[highlight]->handleSelect();
        }
        box.top += deltaY;  // shift coordinate box
    }
    
    EndDrawing();
}

void WorldScene::preload_textures() {
    // we only use cleaned up stuff
    m_textureCache.clear();

    // load the player texture
    m_textureCache["Player"] = LoadTexture("../../Textures/Player.png");
    t_Player = &m_textureCache["Player"];

    // load the textures that tiles use
    for (auto &row : m_tileMap) {
        for (auto &tile :row) {
            const std::string &path = tile.f_texturePath;

            if (path.empty()) continue;

            // if a texture isn't cached yet, load it into cache
            if (!m_textureCache.contains(path)) {
                m_textureCache[path] = LoadTexture(path.c_str());
            }
        
            // reference the correct texture for every tile
            tile.t_tileSet = &m_textureCache[path];
        }
    }
}

void WorldScene::update() {
    int key = GetKeyPressed();
    if (key == KEY_ESCAPE) killScene();
    
    // movement input & direction detection
    if (playerPos.x == nextPos.x && playerPos.y == nextPos.y) {
        if (IsKeyDown(KEY_UP)) {
            if (isWalkable(nextPos.x, nextPos.y - tileSize)) {
                nextPos.y -= tileSize;
            }
            lastMoveDirection = 2;
        }
        else if (IsKeyDown(KEY_DOWN)) {
            if (isWalkable(nextPos.x, nextPos.y + tileSize)) {
                nextPos.y += tileSize;
            }
            lastMoveDirection = 0;
        }
        else if (IsKeyDown(KEY_LEFT)) {
            if (isWalkable(nextPos.x - tileSize, nextPos.y)) {
                nextPos.x -= tileSize;
            }
            lastMoveDirection = 1;
        }
        else if (IsKeyDown(KEY_RIGHT)) {
            if (isWalkable(nextPos.x + tileSize, nextPos.y)) {
                nextPos.x += tileSize;
            }
            lastMoveDirection = 3;
        }
    } 
    else // player movement
    {
        Vector2 delta = {nextPos.x - playerPos.x, nextPos.y - playerPos.y};
        float dist = sqrtf(powf(delta.x, 2) + powf(delta.y, 2));
        
        if (dist <= moveSpeed) playerPos = nextPos;
        else {
            playerPos.x += moveSpeed * (delta.x / dist);
            playerPos.y += moveSpeed * (delta.y / dist);
        }
    }
    
    // adjust camera position
    camera.target = {playerPos.x + tileSize / 2, playerPos.y + tileSize / 2};
}

void WorldScene::draw() {
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(camera);

    Rect src, dest;

    for (int y = 0; y < m_tileMap.size(); y++) {
        for (int x = 0; x < m_tileMap[y].size(); x++) {
            Vector2 tileIndex = m_tileMap[y][x].m_tileSetCoordinates;

            if (tileIndex.x + tileIndex.y >= 0) {
                src = {
                    .left = (int)tileIndex.x * tileSize,
                    .top = (int)tileIndex.y * tileSize,
                    .width = tileSize,
                    .height = tileSize
                };
                dest = {
                    .left = y * tileSize,
                    .top = x * tileSize,
                    .width = tileSize,
                    .height = tileSize
                };

                DrawTexturePro(m_textureCache[m_tileMap[x][y].f_texturePath], src, dest, (Vector2){0, 0}, 0, WHITE);
            }
        }
    }
    
    /* // Draw debug grid
    for (int y = 0; y < m_tileMap.size(); y++) {
        for (int x = 0; x < m_tileMap[y].size(); x++) {
            int px = x * tileSize;
            int py = y * tileSize;

            // Top border
            DrawLine(px, py, px + tileSize, py, GREEN);
            // Left border
            DrawLine(px, py, px, py + tileSize, GREEN);
        }
    } */

    // draw Player Shadow

    src = {
        .left = 3 * tileSize - 3,
        .top = 2,
        .width = tileSize,
        .height = tileSize * 2
    };

    dest = {
        .left = (int)playerPos.x,
        .top = (int)playerPos.y - tileSize,
        .width = tileSize,
        .height = tileSize * 2
    };

    DrawTexturePro(*t_Player, src, dest, (Vector2){0, 0}, 0, {255, 255, 255, 127});

    // select player sprite based on last move direction

    if (lastMoveDirection == 0) {
        src = {
            .left = 0,
            .top = 0,
            .width = tileSize,
            .height = tileSize * 2
        };
    }
    if (lastMoveDirection == 1) {
        src = {
            .left = 2 * tileSize - 1,
            .top = 0,
            .width = tileSize,
            .height = tileSize * 2
        };
    }
    if (lastMoveDirection == 2) {
        src = {
            .left = 1 * tileSize,
            .top = 0,
            .width = tileSize,
            .height = tileSize * 2
        };
    }
    if (lastMoveDirection == 3) {
        src = {
            .left = 2 * tileSize,
            .top = 0,
            .width = -tileSize,
            .height = tileSize * 2
        };
    }

    // draw Player

    DrawTexturePro(*t_Player, src, dest, (Vector2){0, 0}, 0, WHITE);

    EndMode2D();

    // draw debug info

    DrawRectangle(10, 10, 100, 75, Fade(DARKGRAY, 0.8f));

    DrawText(std::to_string((int)playerPos.x).c_str(), 15, 15, 20, YELLOW);
    DrawText(std::to_string((int)playerPos.y).c_str(), 15, 35, 20, YELLOW);
    DrawText(std::to_string((int)nextPos.x).c_str(), 70, 15, 20, YELLOW);
    DrawText(std::to_string((int)nextPos.y).c_str(), 70, 35, 20, YELLOW);
    DrawText(std::to_string(lastMoveDirection).c_str(), 15, 60, 20, YELLOW);

    EndDrawing();
}

void WorldEditor::update() {
    int key = GetKeyPressed();
    if (key == KEY_ESCAPE) killScene();

    // movement input & direction detection
    if (playerPos.x == nextPos.x && playerPos.y == nextPos.y) {
        if (IsKeyDown(KEY_UP)) {
            if ((nextPos.y - tileSize) / tileSize >= 0) {
                nextPos.y -= tileSize;
            }
            lastMoveDirection = 2;
        }
        else if (IsKeyDown(KEY_DOWN)) {
            if ((nextPos.y + tileSize) / tileSize < m_tileMap.size()) {
                nextPos.y += tileSize;
            }
            lastMoveDirection = 0;
        }
        else if (IsKeyDown(KEY_LEFT)) {
            if ((nextPos.x - tileSize) / tileSize >= 0) {
                nextPos.x -= tileSize;
            }
            lastMoveDirection = 1;
        }
        else if (IsKeyDown(KEY_RIGHT)) {
            if ((nextPos.x + tileSize) / tileSize < m_tileMap[nextPos.y / tileSize].size()) {
                nextPos.x += tileSize;
            }
            lastMoveDirection = 3;
        }
    } 
    else // player movement
    {
        Vector2 delta = {nextPos.x - playerPos.x, nextPos.y - playerPos.y};
        float dist = sqrtf(powf(delta.x, 2) + powf(delta.y, 2));

        if (dist <= moveSpeed) playerPos = nextPos;
        else {
            playerPos.x += moveSpeed * (delta.x / dist);
            playerPos.y += moveSpeed * (delta.y / dist);
        }
    }

    // adjust camera position
    camera.target = {playerPos.x + tileSize / 2, playerPos.y + tileSize / 2};
}

void WorldEditor::draw() {
    BeginDrawing();

    // measurements for the "game window"
    int viewX = GetScreenWidth() / 6;
    int viewY = 0;
    int viewWidth = GetScreenWidth() - viewX;
    int viewHeight = GetScreenHeight() / 4 * 3;
    
    static RenderTexture2D worldTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    BeginTextureMode(worldTexture);
    ClearBackground(BLACK);
    
    // this helps with subpixel errors
    camera.target.x = roundf(camera.target.x);
    camera.target.y = roundf(camera.target.y);

    BeginMode2D(camera);
    
    // Draw underlying grid, accounts for subpixel errors
    for (int i = 0; i <= m_tileMap.size(); i++) {
        float pos = (float)i * tileSize + 0.5f;
        DrawLineEx({pos, 0.5f}, {pos, (float)m_tileMap.size() * tileSize + 0.5f}, 1.0f, GREEN);
        DrawLineEx({0.5f, pos}, {(float)m_tileMap.size() * tileSize + 0.5f, pos}, 1.0f, GREEN);
    }

    Rect src, dest;

    // draw all tiles
    for (int y = 0; y < m_tileMap.size(); y++) {
        for (int x = 0; x < m_tileMap[y].size(); x++) {
            Vector2 tileIndex = m_tileMap[y][x].m_tileSetCoordinates;

            if (tileIndex.x + tileIndex.y >= 0) {
                src = {
                    .left = (int)tileIndex.x * tileSize,
                    .top = (int)tileIndex.y * tileSize,
                    .width = tileSize,
                    .height = tileSize
                };
                dest = {
                    .left = y * tileSize,
                    .top = x * tileSize,
                    .width = tileSize,
                    .height = tileSize
                };

                DrawTexturePro(*m_tileMap[y][x].t_tileSet, src, dest, (Vector2){0, 0}, 0, WHITE);
            }
        }
    }

    // draw Player Shadow

    src = {
        .left = 3 * tileSize - 3,
        .top = 2,
        .width = tileSize,
        .height = tileSize * 2
    };

    dest = {
        .left = (int)playerPos.x,
        .top = (int)playerPos.y - tileSize,
        .width = tileSize,
        .height = tileSize * 2
    };

    DrawTexturePro(*t_Player, src, dest, (Vector2){0, 0}, 0, {255, 255, 255, 127});

    // select player sprite based on last move direction

    if (lastMoveDirection == 0) {
        src = {
            .left = 0,
            .top = 0,
            .width = tileSize,
            .height = tileSize * 2
        };
    }
    if (lastMoveDirection == 1) {
        src = {
            .left = 2 * tileSize - 1,
            .top = 0,
            .width = tileSize,
            .height = tileSize * 2
        };
    }
    if (lastMoveDirection == 2) {
        src = {
            .left = 1 * tileSize,
            .top = 0,
            .width = tileSize,
            .height = tileSize * 2
        };
    }
    if (lastMoveDirection == 3) {
        src = {
            .left = 2 * tileSize,
            .top = 0,
            .width = -tileSize,
            .height = tileSize * 2
        };
    }

    // draw Player

    DrawTexturePro(*t_Player, src, dest, (Vector2){0, 0}, 0, WHITE);

    EndMode2D();
    EndTextureMode();

    // draw the "game window" into the right place in the UI
    BeginScissorMode(viewX, viewY, viewWidth, viewHeight);

    Rectangle srcRec = {0, 0, (float)worldTexture.texture.width, -(float)worldTexture.texture.height};
    Rectangle destRec = { (float)viewX, (float)viewY, (float)viewWidth, (float)viewHeight};
    DrawTexturePro(worldTexture.texture, srcRec, destRec, (Vector2){0, 0}, 0, WHITE);

    EndScissorMode();

    // draw editor UI
    
    Vector2 mousePos = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    int width = GetScreenWidth();
    int height = GetScreenHeight();
    float padding = 0.2;
    float border = 0.1;

    // left sidebar

    DrawRectangle(0, 0, GetScreenWidth() / 6, GetScreenHeight() / 4 * 3, GRAY);
    DrawRectangle(5, 5, GetScreenWidth() / 6 - 10, GetScreenHeight() / 4 * 3, LIGHTGRAY);

    Rect box{
        .left = 10,
        .top = 10,
        .width = GetScreenWidth() / 6 - 20,
        .height = GetScreenHeight() / 24 * 3 - 20
    };

    int deltaY = box.height * (1 + 0.2);
    bool highlight = false;

    for (int i = 0; i < (int)ui_leftSideBar.size(); i++) {
        if (CheckCollisionPointRec(mousePos, box)) {
            highlight = true;
            if (mousePos.x != lastMouse.x || mousePos.y != lastMouse.y) {
                lastMouse = mousePos;
            }
            if (click) ui_leftSideBar[i].handleSelect();
        }
        ui_leftSideBar[i].draw(box, highlight);
        highlight = false;
        box.top += deltaY;
    }

    // bottom bar

    DrawRectangle(0, GetScreenHeight() / 4 * 3, GetScreenWidth(), GetScreenHeight() / 4, GRAY);
    DrawRectangle(5, GetScreenHeight() / 4 * 3 + 5, GetScreenWidth() - 10, GetScreenHeight() / 4 - 10, LIGHTGRAY);
    
    // Tile Palette
    
    for (size_t i = 0; i < ui_tilePalette.size(); i++) {
        
    }

    // draw debug info

    DrawRectangle(10, 10, 100, 75, Fade(DARKGRAY, 0.8f));

    DrawText(std::to_string((int)playerPos.x).c_str(), 15, 15, 20, YELLOW);
    DrawText(std::to_string((int)playerPos.y).c_str(), 15, 35, 20, YELLOW);
    DrawText(std::to_string((int)nextPos.x).c_str(), 70, 15, 20, YELLOW);
    DrawText(std::to_string((int)nextPos.y).c_str(), 70, 35, 20, YELLOW);
    DrawText(std::to_string(lastMoveDirection).c_str(), 15, 60, 20, YELLOW);

    EndDrawing();
}

MainMenu::MainMenu(SceneManager *manager) : MenuScene(manager, "Main Menu", {
    new Button("Start Game", [this](){ m_manager->pushScene<WorldScene>(maps::TestRoom2()); }, Align::center),
    new Button("Options", [](){}, Align::center),
    new Button("Create World", [this](){ m_manager->pushScene<WorldEditor>(); }, Align::center),
    new Button("Exit", [this](){ killScene(); }, Align::center)
}) {}