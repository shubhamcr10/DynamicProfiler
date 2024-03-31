// Copyright (c) 2022 Jangoo US - https://www.jangoo.us

#ifndef ISP_FRAME_INFO_H
#define ISP_FRAME_INFO_H

#include <cstdint>

#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
// #include <opencv2/imgproc/types_c.hpp>
// #include "sumat_serializer.hpp"
#include <opencv2/core/mat.hpp>

#define MAX_VIDEO_FRAMES 10

struct ISPFrameInfo_t
{
    // vsdk::SerializedSUMat_t mSUMatData{};
    // cv::Mat                 mSUMatData;
    uint8_t                 mSUMatData[3072000];
    uint32_t                width{0U};
    uint32_t                height{0U};
    uint64_t                receivedTime_ms{0U};
    uint32_t                sequenceNumber{0U};
};

#endif // ISP_FRAME_INFO_H
