#include "Elements.hpp"

void Button::draw(Rect box, bool highlight) {
    Color textColor = highlight ? BLACK : RAYWHITE;
    Color backgroundColor = highlight ? SKYBLUE : DARKGRAY;
	
    if (m_alignment == Align::left) {
        DrawRectangle(box.left, box.top, box.width, box.height, backgroundColor);
        DrawText(m_label.c_str(),
                 box.left + (box.height - font_size(m_tfSize)) / 2,
                 box.top + (box.height - font_size(m_tfSize)) / 2,
                 font_size(m_tfSize),
                 textColor);
    } else if (m_alignment == Align::center) {
        DrawRectangle(box.left, box.top, box.width, box.height, backgroundColor);
        DrawText(m_label.c_str(),
                 box.left + (box.width - MeasureText(m_label.c_str(), font_size(m_tfSize))) / 2,
                 box.top + (box.height - font_size(m_tfSize)) / 2,
                 font_size(m_tfSize),
                 textColor);
    } else {
        DrawRectangle(box.left, box.top, box.width, box.height, backgroundColor);
        DrawText(m_label.c_str(),
                 box.left + box.width - MeasureText(m_label.c_str(), font_size(m_tfSize)) - (box.height - font_size(m_tfSize)) / 2,
                 box.top + (box.height - font_size(m_tfSize)) / 2,
                 font_size(m_tfSize),
                 textColor);
    }        
}