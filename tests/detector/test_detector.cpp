#include <fface/detector/detector.hpp>
#include <opencv2/opencv.hpp>
#include <gtest/gtest.h>
#include <filesystem>

TEST(FaceDetectorTest, DetectFace) {
    // clean up old results
    const std::string result_path = "../testsdata/result.json";
    const std::string output_path = "../testsdata/serge.jpg.jpg";
    if (std::filesystem::exists(result_path)) {
        std::filesystem::remove(result_path);
    }
    if (std::filesystem::exists(output_path)) {
        std::filesystem::remove(output_path);
    }

    // run detection
    const char* tests_path = "../testsdata";
    const char* input_path = "../testsdata/serge.jpg";
    const char* model_path = "../model/fd_yunet_2023mar.onnx";
    detect_faces(tests_path, model_path, input_path);

    // load original and detected images
    cv::Mat original = cv::imread(input_path);
    cv::Mat detected = cv::imread(output_path);

    EXPECT_FALSE(original.empty()) << "Failed to load original image";
    EXPECT_FALSE(detected.empty()) << "Failed to load detected image";

    // verify output files exist
    EXPECT_TRUE(std::filesystem::exists("../testsdata/serge.jpg.jpg")) << "Expected detected image file not found";
    EXPECT_TRUE(std::filesystem::exists(result_path)) << "Expected results.json not found";

    // verify dimensions: detected should be half of original
    EXPECT_EQ(detected.rows, original.rows / 2) << "Detected image height should be half of original";
    EXPECT_EQ(detected.cols, original.cols / 2) << "Detected image width  should be half of original";
}
