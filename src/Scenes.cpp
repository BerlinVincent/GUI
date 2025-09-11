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
        float dist = sqrtf(delta.x * delta.x + delta.y * delta.y);

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
        for ( int x = 0; x < m_tileMap[y].size(); x++) {
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

                DrawTexturePro(m_tileSet, src, dest, (Vector2){0, 0}, 0, WHITE);
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

    DrawTexturePro(m_playerSprite, src, dest, (Vector2){0, 0}, 0, WHITE);

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

    DrawTexturePro(m_playerSprite, src, dest, (Vector2){0, 0}, 0, WHITE);

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

MainMenu::MainMenu(SceneManager *manager) : MenuScene(manager, "Main Menu", {
    new Button("Start Game", [this](){ m_manager->pushScene<WorldScene>(maps::TestRoom()); }, Align::center),
    new Button("Options", [](){}, Align::center),
    new Button("Exit", [this](){ killScene(); }, Align::center)
}) {}