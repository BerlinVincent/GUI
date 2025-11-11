#pragma once

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define FRAMERATE 60

#include <raylib.h>
#include <string>
#include <functional>

enum class Align {
    left, center, right
};

enum TextFontSize {
    tiny, small, medium, large, giant
};

/**
 * @brief A storage wrapper for rectangle features
 * @author TheRobotFox
 */
struct Rect {

    int left, top, width, height;

	// conversion operator for easy Rectangle instantiation
    operator ::Rectangle() const {
        return {(float)left, (float)top, (float)width, (float)height};
    }
};

struct Tile {
    // more Tile Data to be implemented
    // ...
    
    std::string f_texturePath;
    Texture2D * t_tileSet = nullptr;
    Vector2 m_tileSetCoordinates;

    Tile()
    : f_texturePath("../../Textures/basic_tileset_and_assets_standard/terrain_tiles_v2.png"), m_tileSetCoordinates({-1, -1}) {}

    Tile(Vector2 coords, std::string texturePath = "../../Textures/basic_tileset_and_assets_standard/terrain_tiles_v2.png")
    : m_tileSetCoordinates(coords), f_texturePath(texturePath.c_str()) {}

    Tile(int x, int y, std::string texturePath = "../../Textures/basic_tileset_and_assets_standard/terrain_tiles_v2.png")
    : m_tileSetCoordinates({(float)x, (float)y}), f_texturePath(texturePath.c_str()) {}

    Tile(std::string texturePath)
    : f_texturePath(texturePath.c_str()), m_tileSetCoordinates({-1, -1}) {}
};

/**
 * @brief A function that scales font size with window size based on a readable size
 * @returns The font size for the text
 * @author BerlinVincent
 */
static auto font_size(TextFontSize tfSize = medium) -> int {
    if (tfSize == tiny) return GetFontDefault().baseSize * 2 * GetScreenHeight() / SCREEN_HEIGHT;
    else if (tfSize == small) return GetFontDefault().baseSize * 4 * GetScreenHeight() / SCREEN_HEIGHT;
    else if (tfSize == large) return GetFontDefault().baseSize * 16 * GetScreenHeight() / SCREEN_HEIGHT;
    else if (tfSize == giant) return GetFontDefault().baseSize * 32 * GetScreenHeight() / SCREEN_HEIGHT;
    else return GetFontDefault().baseSize * 8 * GetScreenHeight() / SCREEN_HEIGHT;
}

/**
 * @brief An abstract base interface for common UI component behaviour
 * @author TheRobotFox
 */
struct Element {
	
	/**
	 * @brief A function for keypress handling
	 * @param _ pressed key
	 */
    virtual void handleKey(int _) {};

	/**
	 * @brief A function for input handling
	 * @param _ input char
	 */
    virtual void handleChar(int _) {};

	/**
	 * @brief A function for ENTER and left-click handling
	 */
    virtual void handleSelect() {};

	/**
	 * @brief A draw function for Element
	 * @param box The background box
	 * @param highlight Wether the Element should be drawn as highlighted
	 * @note Elements use Rectangles as backdrops so that they tile. This is useful for mouse input.
	 */
    virtual void draw(Rect box, bool highlight) = 0;

	/**
	 * @brief A deconstructor
	 */
    virtual ~Element() = default;
};

/**
 * @brief A UI button Element
 * @implements Element
 * @authors TheRobotFox, BerlinVincent
 */
class Button : public Element {

protected:

	/**
	 * @brief The button's label, drawn as text
	 * @example "Start Game", "Quit"
	 */
    std::string m_label;

	// The member function for the action that the button performs
    std::function<void()> m_command;

    // Wether the button is left, center or right aligned
    Align m_alignment;

    // How large the Button should draw its label text
    TextFontSize m_tfSize;

public:

    auto getLabel() -> std::string {
        return m_label;
    }
    auto getCommand() -> std::function<void()> {
        return m_command;
    }

	/**
	 * @brief The button's function for ENTER and left-click handling, executing `m_command()`
	 */
    void handleSelect() override { m_command(); }

	/**
	 * @brief The button's draw function
	 * @param box The background box
	 * @param highlight Wether the button should be drawn as highlighted
	 */
    void draw(Rect box, bool highlight) override;

	/**
	 * @brief A templated constructor, allowing passing any callable to button and flexible behaviour
	 * @param label The button's `m_label`
	 * @param fn The button's `m_command`
	 */
    template <typename Fn>
    Button(std::string label, Fn fn, Align al, TextFontSize tfs = medium)
    : m_label(std::move(label)), m_command(fn), m_alignment(al), m_tfSize(tfs) {}
};

class ScrollableGrid : public Element {
    std::pair<Button, Button> ui_scrollButtons;
    std::vector<Element> ui_gridElements;

    void handleSelect() override;
    void draw(Rect box, bool highlight) override;
};