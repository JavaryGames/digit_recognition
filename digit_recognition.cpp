#include "digit_recognition.h"

#include "core/project_settings.h"

#include "data/knowledge.knw.gen.h"

#include <vector>
#include <string>
#include <sstream>

void DigitRecognition::_bind_methods() {
    ClassDB::bind_method(D_METHOD("recognize", "pixels"), &DigitRecognition::recognize);
}

int DigitRecognition::recognize(const PoolByteArray &p_pixels) {

    ERR_FAIL_COND_V(p_pixels.size() != 28 * 28, -1);

    std::vector<double> input;
    input.resize(p_pixels.size());

    for (int i = 0; i < p_pixels.size(); i++) {
        input[i] = (((255 - p_pixels[i]) / 255.0 * 0.99) + 0.01); //make input from range 0.01-0.99;
    }

    Matrix<double> results = nn.query(input);

    // Find the answer output by neural net
    double max = results[0][0];
	size_t answerIndex = 0;
	for (size_t i = 0; i < results.size(); i++) {
		if (results[i][0] > max) {
			answerIndex = (size_t) i;
			max = results[i][0];
		}
	}

    return answerIndex;
}

DigitRecognition::DigitRecognition() {
    using namespace std;
    stringstream stream(KNOWLEDGE);
    nn.deserialize(stream);
}
