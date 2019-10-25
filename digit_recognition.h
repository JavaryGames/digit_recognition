#ifndef DIGIT_RECOGNITION_H
#define DIGIT_RECOGNITION_H

#include "core/reference.h"

#include "kann/kann.h"

class DigitRecognition : public Reference {
    GDCLASS(DigitRecognition, Reference);

private:

    kann_t* ann;

protected:
    static void _bind_methods();

public:

    // Recognize an image from a linear array of 28x28 grayscale pixels (0-255 range)
    int recognize(const PoolByteArray &p_pixels);

    DigitRecognition();
};

#endif // DIGIT_RECOGNITION_H
