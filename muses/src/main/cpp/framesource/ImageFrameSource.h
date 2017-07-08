//
// Created by ranqingguo on 7/8/17.
//

#ifndef PBOTEST_IMAGEFRAMESOURCE_H
#define PBOTEST_IMAGEFRAMESOURCE_H


#include <string>
#include "IFrameSource.h"

class ImageFrameSource : public IFrameSource {
public:
    ImageFrameSource(long startMills, long endMills, const std::string &&imagePath);

private:
    std::string mImagePath;

};


#endif //PBOTEST_IMAGEFRAMESOURCE_H
