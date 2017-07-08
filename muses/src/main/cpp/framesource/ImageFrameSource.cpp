//
// Created by ranqingguo on 7/8/17.
//

#include "ImageFrameSource.h"

ImageFrameSource::ImageFrameSource(long startMills, long endMills, const std::string &&imagePath)
        : IFrameSource(startMills, endMills), mImagePath(imagePath) {

}



