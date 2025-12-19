#include <opencv2/opencv.hpp>
#include <json/json.h>
#include <fstream>
#include <map>
#include <string>

struct detector_logger {
public:
    static void append(const std::string& path, const std::string& image_name, const std::vector<cv::Rect>& rects) {
        Json::Value root;
        std::ifstream infile(path);
        if (infile.is_open()) {
            infile >> root;
            infile.close();
        }
        if (!root.isArray()) {
            root = Json::Value(Json::arrayValue);
        }
        Json::Value image_obj;
        image_obj["image"] = image_name;
        Json::Value faces(Json::arrayValue);
        for (const auto& r : rects) {
            Json::Value rect_json;
            rect_json["left"]   = r.x;
            rect_json["top"]    = r.y;
            rect_json["width"]  = r.width;
            rect_json["height"] = r.height;
            faces.append(rect_json);
        }
        image_obj["faces"] = faces;
        root.append(image_obj);
        std::ofstream outfile(path);
        if (!outfile.is_open()) {
            std::cerr << "Error: could not open " << path << " for writing" << std::endl;
            return;
        }
        Json::StreamWriterBuilder writer;
        writer["indentation"] = "  ";
        std::unique_ptr<Json::StreamWriter> jsonWriter(writer.newStreamWriter());
        jsonWriter->write(root, &outfile);
        outfile.close();
    }
};
