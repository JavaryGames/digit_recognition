#include "digit_recognition.h"

#include "core/project_settings.h"

#include <vector>

void DigitRecognition::_bind_methods() {
    ClassDB::bind_method(D_METHOD("recognize", "pixels"), &DigitRecognition::recognize);
    ClassDB::bind_method(D_METHOD("get_knowledge_path"), &DigitRecognition::get_knowledge_path);
    ClassDB::bind_method(D_METHOD("set_knowledge_path", "path"), &DigitRecognition::set_knowledge_path);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "knowledge_path"), "set_knowledge_path", "get_knowledge_path");
}

int DigitRecognition::recognize(const PoolByteArray &p_pixels) {

    ERR_FAIL_COND_V(knowledge_path.empty(), -1);

    std::vector<double> input;
    input.resize(p_pixels.size());

    for (int i = 0; i < p_pixels.size(); i++) {
        input[i] = (((255 - p_pixels[i]) / 255.0 * 0.99) + 0.01); //make input from range 0.01-0.99;
        // print_line("digit " + itos(i) + " is " + itos(p_pixels[i]) + " and " + rtos(input[i]));
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

    print_line("Trying to recognize " + itos(p_pixels.size()));
    return answerIndex;
}

String DigitRecognition::get_knowledge_path() const {
    return knowledge_path;
}

void DigitRecognition::set_knowledge_path(const String &p_path) {
    knowledge_path = p_path;
    String globalized = ProjectSettings::get_singleton()->globalize_path(p_path);
    // print_line("globalized is " + globalized);
    nn.deserialize(std::string(globalized.utf8().get_data()));
}

DigitRecognition::DigitRecognition() {}
