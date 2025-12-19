#pragma once

const int FACES_HEADER_WIDTH = 8;
const int IMAGE_HEADER_WIDTH = 5;

using detect_faces_fn = void (*)(const char*, const char*, const char*);

extern "C" void detect_faces(const char* root, const char* model_path, const char* input_image_path);
