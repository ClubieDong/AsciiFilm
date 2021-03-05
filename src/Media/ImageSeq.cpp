#include "ImageSeq.hpp"
#include <opencv2/videoio.hpp>

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
