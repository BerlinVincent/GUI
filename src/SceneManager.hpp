#pragma once

#include "Scenes.hpp"
#include <vector>

class SceneManager {
    // Scene Stack
    std::vector<std::unique_ptr<Scene>> sceneStack;

public:
    /**
     * @brief templated arguments for scene construction
     * @param T is the type of the scene
     * @param Args is a pack of argument types
     */
    template<typename T, typename... Args>

    /**
     * @brief push a new scene to the scene stack
     * @param Args is a pack of argument types
     * @param args is a pack of argument values
     * @example sceneManager.pushScene<MenuScene>("MainMenu");
     */
    auto pushScene(Args&&... args) -> void {
        static_assert(std::is_base_of<Scene, T>::value, "T must derive from Scene");
        sceneStack.push_back(std::make_unique<T>(this, std::forward<Args>(args)...));
    }

    /**
     * @brief delete the top scene from the scene stack if it is not empty
     */
    auto popScene() -> void {
        if (!sceneStack.empty()) sceneStack.pop_back();
    }

    /**
     * @brief getter for current top scene
     * @returns nullptr if scene stack is empty
     */
    auto getCurrentScene() -> Scene *{
        return sceneStack.empty() ? nullptr : sceneStack.back().get();
    }

    /**
     * @brief forward update to current scene
     */
    auto update() -> void{
        if (auto* s = getCurrentScene()) s->update();
    }

    /**
     * @brief forward drawing to current scene
     */
    auto draw() -> void{
        if (auto* s = getCurrentScene()) s->draw();
    }

    /**
     * @brief check if scene stack is empty
     */
    auto empty() const -> bool {
        return sceneStack.empty();
    }

    auto run() -> void {
        while (!empty() && !WindowShouldClose()) {  // check if there are scenes
            Scene* current = getCurrentScene();

            update();   // update logic
            draw();     // draw visuals

            if (current->running() == false) popScene();    // pop scene if it's not supposed to run
        }
    }

    SceneManager() = default;
};