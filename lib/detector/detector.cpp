#include <fface/detector/detector.hpp>
#include <filesystem>
#include <string>
#include <vector>

#include "detector_opencv.hpp"
#include "detector_logger.hpp"

namespace fs = std::filesystem;

extern "C" void detect_faces(const char* root_path, const char* model_path, const char* input_image_path) {
    std::string output_image_path = std::string(input_image_path) + ".jpg";

    // find faces
    face_detector face_detector;
    std::vector<cv::Rect> input_image_rects;
    cv::Mat output_image = face_detector.detect_faces(
        model_path,
        input_image_path,
        input_image_rects);
    cv::imwrite(output_image_path, output_image);
    std::cout
        << std::left
        << std::setw(FACES_HEADER_WIDTH) << input_image_rects.size()
        << std::setw(IMAGE_HEADER_WIDTH) << input_image_path << std::endl;

    // append to json
    fs::path logger_path = fs::path(root_path) / "result.json";
    detector_logger::append(logger_path, output_image_path, input_image_rects);
}
