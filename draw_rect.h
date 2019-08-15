#ifndef RECON_DRAW_RECT_H
#define RECON_DRAW_RECT_H

#include "scene/gui/texture_rect.h"

class DrawRect : public TextureRect {
    GDCLASS(DrawRect, TextureRect);

protected:

    int thickness;

    Ref<Image> image;
    Rect2 rect;
    bool drawing;
    Vector2 last_point;
    Variant extents_max;
    Variant extents_min;

    static void _bind_methods();
    virtual void _gui_input(const Ref<InputEvent> &p_event);

public:

    void init_white();
    void reset_last_point();
    void update_extents(const Vector2 &p_point);
    void draw(const Vector2 &p_point);
    void draw_line_to(const Vector2 &p_final_point);
    void on_parent_resized();

    void set_thickness(int p_thickness) { thickness = p_thickness; }
    int get_thickness() const { return thickness; }

    void set_extents_min(Variant p_extents) { extents_min = p_extents; }
    Variant get_extents_min() const { return extents_min; }
    void set_extents_max(Variant p_extents) { extents_max = p_extents; }
    Variant get_extents_max() const { return extents_max; }

    DrawRect();
};

#endif // RECON_DRAW_RECT_H
