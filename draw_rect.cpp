#include "draw_rect.h"

void DrawRect::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init_white"), &DrawRect::init_white);
    ClassDB::bind_method(D_METHOD("reset_last_point"), &DrawRect::reset_last_point);
    ClassDB::bind_method(D_METHOD("on_parent_resized"), &DrawRect::on_parent_resized);
    ClassDB::bind_method(D_METHOD("update_extents", "point"), &DrawRect::update_extents);
    ClassDB::bind_method(D_METHOD("upddrawate_extents", "point"), &DrawRect::draw);
    ClassDB::bind_method(D_METHOD("draw_line_to", "final_point"), &DrawRect::draw_line_to);
    ClassDB::bind_method(D_METHOD("_gui_input"), &DrawRect::_gui_input);

    ClassDB::bind_method(D_METHOD("get_thickness"), &DrawRect::get_thickness);
    ClassDB::bind_method(D_METHOD("set_thickness", "thickness"), &DrawRect::set_thickness);
    ClassDB::bind_method(D_METHOD("get_extents_min"), &DrawRect::get_extents_min);
    ClassDB::bind_method(D_METHOD("set_extents_min", "extents"), &DrawRect::set_extents_min);
    ClassDB::bind_method(D_METHOD("get_extents_max"), &DrawRect::get_extents_max);
    ClassDB::bind_method(D_METHOD("set_extents_max", "extents"), &DrawRect::set_extents_max);
    ClassDB::bind_method(D_METHOD("get_line_color"), &DrawRect::get_line_color);
    ClassDB::bind_method(D_METHOD("set_line_color", "color"), &DrawRect::set_line_color);
    ClassDB::bind_method(D_METHOD("get_image_binary"), &DrawRect::get_image_binary);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "thickness"), "set_thickness", "get_thickness");
    ADD_PROPERTY(PropertyInfo(Variant::NIL, "extents_min"), "set_extents_min", "get_extents_min");
    ADD_PROPERTY(PropertyInfo(Variant::NIL, "extents_max"), "set_extents_max", "get_extents_max");
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "line_color"), "set_line_color", "get_line_color");

    ADD_SIGNAL(MethodInfo("started_drawing"));
    ADD_SIGNAL(MethodInfo("stopped_drawing"));
    ADD_SIGNAL(MethodInfo("point_drawn"));

}

void DrawRect::_gui_input(const Ref<InputEvent> &p_event) {
    Ref<InputEventMouse> iem = p_event;
    if (iem.is_valid()) {
        Ref<InputEventMouseButton> mb = iem;
        if (mb.is_valid()) {
            drawing = mb->is_pressed();
            if (drawing) {
                emit_signal("started_drawing");
			} else {
				emit_signal("stopped_drawing");
            }
            draw(mb->get_position());
        }
        Ref<InputEventMouseMotion> mm = iem;
        if (drawing && mm.is_valid()) {
            draw(mm->get_position());
            emit_signal("point_drawn");
        }
        if (!drawing || !rect.has_point(iem->get_position())) {
            reset_last_point();
        }
    }
}

void DrawRect::init_white() {
    rect = Rect2(get_position(), get_size() * get_scale());
    image.instance();
    image->create(get_size().x, get_size().y, false, Image::FORMAT_RGBA8);
    image->lock(); 
    image->fill(Color(1, 1, 1, 0));
    image->unlock();

    image_binary.instance();
    image_binary->create(get_size().x, get_size().y, false, Image::FORMAT_RGBA8);
    image_binary->lock(); 
    image_binary->fill(Color(1, 1, 1, 0));
    image_binary->unlock();

    Ref<ImageTexture> texture = get_texture();
    texture->create_from_image(image);

    extents_max = Variant();
    extents_min = Variant();
}

void DrawRect::reset_last_point() {
    last_point = Vector2(-INFINITY, -INFINITY);
}

void DrawRect::update_extents(const Vector2 &p_point) {
    if (extents_min.get_type() == Variant::NIL) {
        extents_min = Vector2(get_rect().size);
        extents_max = Vector2();
    }

    Vector2 new_min(MAX(MIN(extents_min.operator Vector2().x, p_point.x - thickness), rect.get_position().x), MAX(MIN(extents_min.operator Vector2().y, p_point.y - thickness), rect.get_position().y));
    Vector2 new_max(MIN(MAX(extents_max.operator Vector2().x, p_point.x + thickness), rect.get_position().x + rect.get_size().x), MIN(MAX(extents_max.operator Vector2().y, p_point.y + thickness), rect.get_position().y + rect.get_size().y));

    extents_min = new_min;
    extents_max = new_max;
}

void DrawRect::draw(const Vector2 &p_point) {
    if (!rect.has_point(p_point)) {
        return;
    }

    update_extents(p_point);

    if (last_point.x != -INFINITY) {
        draw_line_to(p_point);
    }
    last_point = p_point;

    Rect2 neighboorhood_rect = Rect2(p_point - Vector2(thickness, thickness), Vector2(2*thickness, 2*thickness));

    image->lock();
    image_binary->lock();
    for (int y = 0; y <= neighboorhood_rect.size.y; y++) {
        for (int x = 0; x <= neighboorhood_rect.size.x; x++) {
            Vector2 neighbor_pixel = (neighboorhood_rect.position + Vector2(x, y)) * get_scale();
            if (!rect.has_point(neighbor_pixel)) {
                continue;
            }
            real_t dist = (p_point - neighboorhood_rect.position).distance_to(Vector2(x, y));
            if (dist > thickness) {
                continue;
            }
            Vector2 image_pixel_pos = (neighbor_pixel - get_position()) / get_scale();
            if (image_pixel_pos.x < 0 || image_pixel_pos.y < 0 || image_pixel_pos.x >= image->get_size().x || image_pixel_pos.y >= image->get_size().y) {
                continue;
            }
            image->set_pixelv(image_pixel_pos, line_color);
            image_binary->set_pixelv(image_pixel_pos, Color(0, 0, 0, 1));
        }
    }
    image->unlock();
    image_binary->unlock();

    Ref<ImageTexture> texture = get_texture();
    texture->set_data(image);
}

void DrawRect::draw_line_to(const Vector2 &p_final_point) {
    if ((p_final_point - last_point).length() == 0) {
        return;
    }

    Vector2 normalized_vector = (p_final_point - last_point).normalized() / get_scale();
    Vector2 offset = normalized_vector.tangent() * get_scale() * thickness;

    image->lock();
    image_binary->lock();
    while (offset.distance_to(-normalized_vector.tangent() * get_scale() * thickness) > 1) {
        offset -= normalized_vector.tangent();
		Vector2 delta = last_point;
        while (delta.distance_to(p_final_point) > 1) {
            delta += normalized_vector;
            Vector2 pixel_pos = delta + offset;
            if (!rect.has_point(pixel_pos)) {
                continue;
            }
            Vector2 image_pixel_pos = (pixel_pos - get_position());
            if (pixel_pos.x < 0 || pixel_pos.y < 0 || pixel_pos.x >= image->get_size().x || pixel_pos.y >= image->get_size().y) {
                continue;
            }
			image->set_pixelv(image_pixel_pos, line_color);
            image_binary->set_pixelv(image_pixel_pos, Color(0, 0, 0, 1));
        }
    }
    image->unlock();
    image_binary->unlock();

    Ref<ImageTexture> texture = get_texture();
    texture->set_data(image);
}

void DrawRect::on_parent_resized() {
    Control *parent = Object::cast_to<Control>(get_parent());
    ERR_FAIL_COND(!parent);

    Vector2 parent_size = parent->get_size();
	if (parent_size.x == 0.0 or parent_size.y == 0.0) {
		return;
    }
    Ref<ImageTexture> texture;
    texture.instance();
	set_texture(texture);
	set_size(parent->get_size() / get_scale());
	init_white();
}

DrawRect::DrawRect() {
    drawing = false;
    last_point = Vector2(-INFINITY, -INFINITY);
    extents_max = Variant();
    extents_min = Variant();
    thickness = 20;
    line_color = Color(0,0,0,1.0F);
    image.instance();
    image_binary.instance();
    Ref<ImageTexture> texture;
    texture.instance();
    set_texture(texture);
}