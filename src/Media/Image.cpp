#include "Image.hpp"
#include <opencv2/imgproc.hpp>

Image::Info::Info(json &option, const std::string &location, double aspectRatio)
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

void Image::LoadFromMat(cv::Mat &&img, unsigned int width, unsigned int height)
{
    _Data.resize(boost::extents[height][width]);
    cv::Size size(width, height);
    auto inter = img.total() > _Data.num_elements() ? cv::INTER_AREA : cv::INTER_CUBIC;
    cv::resize(img, img, size, 0, 0, inter);
    auto destIter = _Data.origin();
    // I know it's vulnerable but it seems to be the only way
    // to distinguish the format of the image read by `cv::imread`.
    if (img.channels() == 3)
        // For BGR
        for (auto srcIter = img.begin<cv::Vec3b>(); srcIter != img.end<cv::Vec3b>();
             ++srcIter, ++destIter)
        {
            // TODO: Improve performance
            destIter->B = (*srcIter)[0];
            destIter->G = (*srcIter)[1];
            destIter->R = (*srcIter)[2];
            destIter->A = 255;
        }
    else
        // For BGRA
        for (auto srcIter = img.begin<cv::Vec4b>(); srcIter != img.end<cv::Vec4b>();
             ++srcIter, ++destIter)
        {
            // TODO: Improve performance
            destIter->B = (*srcIter)[0];
            destIter->G = (*srcIter)[1];
            destIter->R = (*srcIter)[2];
            destIter->A = (*srcIter)[3];
        }
}
