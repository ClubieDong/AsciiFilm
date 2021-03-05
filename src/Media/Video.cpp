#include "Video.hpp"
#include <opencv2/videoio.hpp>

void Video::LoadFromFile(Info &&info)
{
    auto video = cv::VideoCapture(info.FilePath);
    if (!video.isOpened())
        throw std::invalid_argument("OpenCV Error: Fail to open video!");
    auto videoAspectRatio = video.get(cv::CAP_PROP_FRAME_HEIGHT) /
                            video.get(cv::CAP_PROP_FRAME_WIDTH);
    FitAspectRatio(info.Width, info.Height, info.AspectRatio, videoAspectRatio);
    auto frameCount = video.get(cv::CAP_PROP_FRAME_COUNT);
    auto ratio = video.get(cv::CAP_PROP_FPS) / info.FPS;
    cv::Mat frame;
    for (auto i = 0.5 * ratio; i < frameCount; i += ratio)
    {
        video.set(cv::CAP_PROP_POS_FRAMES, i);
        video >> frame;
        if (frame.empty())
            throw std::invalid_argument("OpenCV Error: Fail to load video!");
        _Data.emplace_back(std::move(frame), info.Width, info.Height);
    }
    if (_Data.empty())
        throw std::invalid_argument("Video is too short!");
}
