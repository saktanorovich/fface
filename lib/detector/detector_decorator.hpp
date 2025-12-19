#include <opencv2/opencv.hpp>
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct face_decorator {
    virtual void operator()(cv::Mat& image, const cv::Rect& roi) const = 0;
    virtual std::string name() const = 0;
    virtual ~face_decorator() = default;
};

struct blur_effect : public face_decorator {
private:
    cv::Size gausiann_kernel;
public:
    explicit blur_effect(const cv::Size& gausiann_kernel)
        : gausiann_kernel(gausiann_kernel) {
    }
    std::string name() const override {
        return "blur effect";
    }
    void operator()(cv::Mat& image, const cv::Rect& roi) const override {
        cv::Mat region = image(roi);
        cv::Mat blurred;
        cv::GaussianBlur(region, blurred, gausiann_kernel, 0);
        blurred.copyTo(image(roi));
    }
};

struct pixelization_effect : public face_decorator {
private:
    int block_size;
public:
    explicit pixelization_effect(const int& block_size)
        : block_size(block_size) {
    }
    std::string name() const override {
        return "pixelization effect";
    }
    void operator()(cv::Mat& image, const cv::Rect& roi) const override {
        cv::Mat region = image(roi);
        cv::Mat small, pixelated;
        cv::resize(region, small,
            cv::Size(
                std::max(1, region.cols / block_size),
                std::max(1, region.rows / block_size)),
            0, 0, cv::INTER_LINEAR);
        cv::resize(small, pixelated, region.size(), 0, 0, cv::INTER_NEAREST);
        pixelated.copyTo(image(roi));
    }
};

struct bounding_box_effect : public face_decorator {
private:
    cv::Scalar color;
    int thickness;
public:
    explicit bounding_box_effect(const cv::Scalar& color, const int& thickness)
        : color(color), thickness(thickness) {
    }
    std::string name() const override {
        return "bounding box effect";
    }
    void operator()(cv::Mat& image, const cv::Rect& roi) const override {
        cv::rectangle(image, roi, color, thickness);
    }
};
