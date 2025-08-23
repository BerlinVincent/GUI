#pragma once

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define FRAMERATE 60

#include <raylib.h>
#include <functional>

enum class Align {
    left, center, right
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

    Vector2 m_tileSetCoordinates;

    Tile(Vector2 coords) : m_tileSetCoordinates(coords) {}
};

/**
 * @brief A function that scales font size with window size based on a readable size
 * @returns The font size for the text
 * @author BerlinVincent
 */
static auto font_size() -> int {
    return GetFontDefault().baseSize * 8 *  GetScreenHeight() / SCREEN_HEIGHT;
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

	/**
	 * @brief The button's label, drawn as text
	 * @example "Start Game", "Quit"
	 */
    std::string m_label;

	// The member function for the action that the button performs
    std::function<void()> m_command;

    // Wether the button is left, center or right aligned
    Align m_alignment;

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
    void draw(Rect box, bool highlight) override {
        Color textColor = highlight ? BLACK : RAYWHITE;
        Color backgroundColor = highlight ? SKYBLUE : DARKGRAY;
		
        if (m_alignment == Align::left) {
            DrawRectangle(box.left, box.top, box.width, box.height, backgroundColor);
            DrawText(m_label.c_str(),
                     box.left + (box.height - font_size()) / 2,
                     box.top + (box.height - font_size()) / 2,
                     font_size(),
                     textColor);
        } else if (m_alignment == Align::center) {
            DrawRectangle(box.left, box.top, box.width, box.height, backgroundColor);
            DrawText(m_label.c_str(),
                     box.left + (box.width - MeasureText(m_label.c_str(), font_size())) / 2,
                     box.top + (box.height - font_size()) / 2,
                     font_size(),
                     textColor);
        } else {
            DrawRectangle(box.left, box.top, box.width, box.height, backgroundColor);
            DrawText(m_label.c_str(),
                     box.left + box.width - MeasureText(m_label.c_str(), font_size()) - (box.height - font_size()) / 2,
                     box.top + (box.height - font_size()) / 2,
                     font_size(),
                     textColor);
        }        
    }

	/**
	 * @brief A templated constructor, allowing passing any callable to button and flexible behaviour
	 * @param label The button's `m_label`
	 * @param fn The button's `m_command`
	 */
    template <typename Fn>
    Button(std::string label, Fn fn, Align al) : m_label(std::move(label)), m_command(fn), m_alignment(al) {}
};