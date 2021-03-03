#include "Image.hpp"
#include <cmath>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

Image::Image(json &option, const std::string &location, double aspectRatio)
{
    // Get property from JSON
    using jt = json::value_t;
    static auto validator =
        JsonValidator()
            .AddRequired("id", jt::string)
            .AddRequired("filePath", jt::string)
            .AddOptional("width", jt::number_unsigned, 0)
            .AddOptional("height", jt::number_unsigned, 0);
    validator.Validate(option, location);
    _ID = option["id"].get<std::string>();
    _FilePath = option["filePath"].get<std::string>();
    _Width = option["width"].get<unsigned int>();
    _Height = option["height"].get<unsigned int>();
    if (_Width == 0 && _Height == 0)
        throw std::invalid_argument("Expect at least one non-zero value "
                                    "between \"width\" and \"height\" fields in " +
                                    location);
    // Load image
    auto img = cv::imread(_FilePath, cv::IMREAD_UNCHANGED);
    if (_Width == 0)
        _Width = std::round(aspectRatio * img.size[1] / img.size[0] * _Height);
    else if (_Height == 0)
        _Height = std::round(1 / aspectRatio * img.size[0] / img.size[1] * _Width);
    _Data.resize(boost::extents[_Height][_Width]);
    cv::Size size(_Width, _Height);
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
