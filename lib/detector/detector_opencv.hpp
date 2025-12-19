#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>
#include <string>
#include <vector>

struct YuNet {
private:
    cv::Ptr<cv::FaceDetectorYN> detector;
public:
    YuNet(
        const char* model_path,
        const cv::Size& input_size,
        const float conf_threshold,
        const float nms_threshold,
        const int top_k)
    {
        detector = cv::FaceDetectorYN::create(model_path, "",
            input_size,
            conf_threshold,
            nms_threshold,
            top_k,
            cv::dnn::DNN_BACKEND_OPENCV,
            cv::dnn::DNN_TARGET_CPU);
    }
public:
    void setInputSize(const cv::Size& input_size) {
        detector->setInputSize(input_size);
    }
    cv::Mat infer(const cv::Mat& image) {
        cv::Mat result;
        detector->detect(image, result);
        return result;
    }
};

struct face_detector {
private:
    cv::Mat detect_faces(
        const cv::Mat& input_image,
        const cv::Mat& faces,
        std::vector<cv::Rect>& input_image_rects)
    {
        static const cv::Scalar box_color{0, 0, 255};
        static const cv::Size gausiann_kernel(25, 25);

        const double ratio = 0.5;
        const int new_height = static_cast<int>(input_image.rows * ratio);
        const int new_width  = static_cast<int>(input_image.cols * ratio);
        auto target_size = cv::Size(new_width, new_height);
        cv::Mat output_image = cv::Mat::zeros(target_size, CV_8UC3);

        // resize the original image, maintaining aspect ratio
        cv::Mat resize_out;
        cv::resize(input_image, resize_out, cv::Size(new_width, new_height), cv::INTER_LINEAR);

        // determine top left corner in resized dimensions
        const int top  = std::max(0, target_size.height - new_height) / 2;
        const int left = std::max(0, target_size.width  - new_width ) / 2;

        // copy resized image
        const cv::Rect roi = cv::Rect(cv::Point(left, top), cv::Size(new_width, new_height));
        cv::Mat out_sub_image = output_image(roi);
        resize_out.copyTo(out_sub_image);

        for (int i = 0; i < faces.rows; ++i) {
            const int x0 = static_cast<int>(faces.at<float>(i, 0)) + left;
            const int y0 = static_cast<int>(faces.at<float>(i, 1)) + top;
            const int w0 = static_cast<int>(faces.at<float>(i, 2));
            const int h0 = static_cast<int>(faces.at<float>(i, 3));

            const int x1 = static_cast<int>(faces.at<float>(i, 0) * ratio) + left;
            const int y1 = static_cast<int>(faces.at<float>(i, 1) * ratio) + top;
            const int w1 = static_cast<int>(faces.at<float>(i, 2) * ratio);
            const int h1 = static_cast<int>(faces.at<float>(i, 3) * ratio);

            input_image_rects.push_back(cv::Rect(x0, y0, w0, h0));

            // apply chain of decorators
            cv::Mat decorated;
            cv::Rect face(x1, y1, w1, h1);

            cv::Mat face_region = output_image(face);

            // gaussian blur
            cv::GaussianBlur(face_region, decorated, gausiann_kernel, 0);
            decorated.copyTo(output_image(face));  

            // bounding box
            cv::rectangle(output_image, face, box_color, 2);
        }
        return output_image;
    }
public:
    cv::Mat detect_faces(
        const char* model_path,
        const char* input_image_path,
        std::vector<cv::Rect>& input_image_rects)
    {
        // YuNet
        static const float detect_threshold   = 0.9;
        static const float nms_threshold      = 0.3;
        static const int top_k_bounding_boxes = 5000;

        auto face_detector = YuNet(
            model_path,
            cv::Size(320, 320),
            detect_threshold,
            nms_threshold,
            top_k_bounding_boxes
        );

        // detect faces in input image
        cv::Mat input_image = cv::imread(input_image_path);
        face_detector.setInputSize(input_image.size());
        cv::Mat input_faces = face_detector.infer(input_image);
        cv::Mat output_image = detect_faces(input_image, input_faces, input_image_rects);

        return output_image;
    }
};
