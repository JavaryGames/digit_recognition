
#include "digit_recognition.h"

#include "core/project_settings.h"

#include "data/mnist_mlp.knw.gen.h"

#include "kann/kann.h"

void DigitRecognition::_bind_methods() {
    ClassDB::bind_method(D_METHOD("recognize", "pixels"), &DigitRecognition::recognize);
}

int DigitRecognition::recognize(const PoolByteArray &p_pixels) {

    ERR_FAIL_COND_V(p_pixels.size() != 28 * 28, -1);

    float input[28 * 28];

    for (int i = 0; i < p_pixels.size(); i++) {
        input[i] = (((255 - p_pixels[i]) / 255.0 * 0.99) + 0.01); //make input from range 0.01-0.99;
    }

    int n_out = kann_dim_out(ann);

    const float* out = kann_apply1(ann, input);
    unsigned char answer_index = -1;
    float max = 0.0f;
    for (int j = 0; j < n_out; j++) {
        if (out[j] > max) {
            max = out[j];
            answer_index = j;
        }
    }

    return answer_index;
}

DigitRecognition::DigitRecognition() {
    ann = kann_load_mem(MLP_KNOWLEDGE);
}
