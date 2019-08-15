#include "digit_recognition.h"
#include "draw_rect.h"

#include "core/class_db.h"
#include "core/engine.h"

DigitRecognition *dr_singleton;

void register_digit_recognition_types() {
    ClassDB::register_virtual_class<DigitRecognition>();
    dr_singleton = memnew(DigitRecognition);

    Engine::get_singleton()->add_singleton(Engine::Singleton("DigitRecognition", dr_singleton));

    ClassDB::register_class<DrawRect>();
}

void unregister_digit_recognition_types() {
    if (dr_singleton) {
        memfree(dr_singleton);
    }
}
