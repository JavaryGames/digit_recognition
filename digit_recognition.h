#ifndef DIGIT_RECOGNITION_H
#define DIGIT_RECOGNITION_H

#include "core/reference.h"

#include "lib/include/NeuralNetwork.h"

class DigitRecognition : public Reference {
    GDCLASS(DigitRecognition, Reference);

private:

    NeuralNetwork nn;
    String knowledge_path;

protected:
    static void _bind_methods();

public:

    // Recognize an image from a linear array of 28x28 grayscale pixels
    int recognize(const PoolByteArray &p_pixels);

    String get_knowledge_path() const;
    void set_knowledge_path(const String &p_path);

    DigitRecognition();
};

#endif // DIGIT_RECOGNITION_H
