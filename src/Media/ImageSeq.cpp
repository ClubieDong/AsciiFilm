#include "ImageSeq.hpp"
#include <opencv2/videoio.hpp>

ImageSeq::Info::Info(json &option, const std::string &location, double aspectRatio)
    : AspectRatio(aspectRatio)
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

void ImageSeq::LoadFromFile(Info &&info)
{
    auto imgSeq = cv::VideoCapture(info.FilePath);
    if (!imgSeq.isOpened())
        throw std::invalid_argument("OpenCV Error: Fail to open image sequence!");
    cv::Mat frame;
    auto frameCount = imgSeq.get(cv::CAP_PROP_FRAME_COUNT);
    for (auto i = 0; i < frameCount; ++i)
    {
        imgSeq >> frame;
        if (frame.empty())
            throw std::invalid_argument("OpenCV Error: Fail to load image sequence!");
        auto width = info.Width, height = info.Height;
        FitAspectRatio(width, height, info.AspectRatio,
                       static_cast<double>(frame.size[0]) / frame.size[1]);
        _Data.emplace_back(std::move(frame), width, height);
    }
}
