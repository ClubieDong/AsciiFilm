#include "Video.hpp"
#include <opencv2/videoio.hpp>

Video::Info::Info(json &option, const std::string &location,
                  double aspectRatio, double fps)
    : AspectRatio(aspectRatio), FPS(fps)
{
    using jt = json::value_t;
    static auto validator =
        JsonValidator()
            .AddOptional("id", jt::string, "")
            .AddRequired("filePath", jt::string)
            .AddOptional("width", jt::number_unsigned, 0)
            .AddOptional("height", jt::number_unsigned, 0);
    validator.Validate(option, location);
    ID = option["id"].get<std::string>();
    FilePath = option["filePath"].get<std::string>();
    Width = option["width"].get<unsigned int>();
    Height = option["height"].get<unsigned int>();
    if (ID.empty())
        ID = FilePath.filename();
    if (Width == 0 && Height == 0)
        throw std::invalid_argument("Expect at least one non-zero value "
                                    "between \"width\" and \"height\" fields in " +
                                    location);
}

void Video::LoadFromFile(Info &&info)
{
    auto video = cv::VideoCapture(info.FilePath);
    if (!video.isOpened())
        throw std::invalid_argument("OpenCV Error: Fail to open video!");
    auto frameCount = video.get(cv::CAP_PROP_FRAME_COUNT);
    auto ratio = video.get(cv::CAP_PROP_FPS) / info.FPS;
    cv::Mat frame;
    for (auto i = 0.5 * ratio; i < frameCount; i += ratio)
    {
        video.set(cv::CAP_PROP_POS_FRAMES, i);
        video >> frame;
        if (frame.empty())
            throw std::invalid_argument("OpenCV Error: Fail to load video!");
        auto width = info.Width, height = info.Height;
        FitAspectRatio(width, height, info.AspectRatio,
                       static_cast<double>(frame.size[0]) / frame.size[1]);
        _Data.emplace_back(std::move(frame), width, height);
    }
    if (_Data.empty())
        throw std::invalid_argument("Video is too short!");
}
