#include "Scenes.hpp"
#include "SceneManager.hpp"

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

    // movement input
    if (playerPos.x == nextPos.x && playerPos.y == nextPos.y) {
        if      (IsKeyDown(KEY_UP))     (nextPos.y - tileSize < 0) ? : nextPos.y -= tileSize;
        else if (IsKeyDown(KEY_DOWN))   (nextPos.y + tileSize > (m_tileMap.size() - 1) * tileSize) ? : nextPos.y += tileSize;
        else if (IsKeyDown(KEY_LEFT))   (nextPos.x - tileSize < 0) ? : nextPos.x -= tileSize;
        else if (IsKeyDown(KEY_RIGHT))  (nextPos.x + tileSize > (m_tileMap.size() - 1) * tileSize) ? : nextPos.x += tileSize;
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
    camera.target = {playerPos.x + tileSize / 2.0f, playerPos.y + tileSize / 2.0f};
}

void WorldScene::draw() {
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(camera);

    // draw from buffer so as to not draw the entire map every time
    DrawTextureRec(m_worldBuffer.texture, {0, 0, (float)m_worldBuffer.texture.width, -(float)m_worldBuffer.texture.height}, {0, 0}, WHITE);
    
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

    // draw player (currently a red rectangle)
    DrawRectangle((int)playerPos.x, (int)playerPos.y, tileSize, tileSize, RED);

    EndMode2D();
    EndDrawing();
}

MainMenu::MainMenu(SceneManager *manager) : MenuScene(manager, "Main Menu", {
    new Button("Start Game", [this](){ m_manager->pushScene<WorldScene>(1.0f); }, Align::center),
    new Button("Options", [](){}, Align::center),
    new Button("Exit", [this](){ killScene(); }, Align::center)
}) {}