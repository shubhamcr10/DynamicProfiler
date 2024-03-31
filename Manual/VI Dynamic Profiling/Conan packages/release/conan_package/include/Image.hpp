// Copyright (c) 2022 Jangoo US - https://www.jangoo.us

#ifndef IMAGE_H
#define IMAGE_H

#include <cstddef>
#include <cstdint>

#include "RADSInterApplicationTypes.hpp"
#include "RADSCommon.hpp"
#if defined(QNX) && defined(PRODUCTION)
#include "VideoImageImpl.hpp"
#elif defined(QNX) && (defined(DEVELOPMENT) || defined(COVERAGE))
// #include "HostsimImageImpl.hpp"
#include "VideoImageImpl.hpp"
#include "ImageImpl.hpp"
#else // X86-Builds
// #include "HostsimImageImpl.hpp"
#include "ImageImpl.hpp"
#endif // #if defined(QNX) && (defined(PRODUCTION) || defined(COVERAGE))

class Image : public ImageImpl
{
    uint32_t       mBufferIndex = 0;
    size_t         mWidth       = 0;
    size_t         mHeight      = 0;
    const uint8_t* mPixelsArray = nullptr;
    ImageImpl*     mImageImpl{nullptr};

#if defined(QNX) && defined(PRODUCTION)
    VideoImageImpl mVideoImageImpl;
#elif defined(QNX) && (defined(DEVELOPMENT) || defined(COVERAGE))
    ImageImpl* mHostsimImageImpl;
    VideoImageImpl   mVideoImageImpl;
#else  // X86-Builds
    ImageImpl* mHostsimImageImpl;
#endif // #if defined(QNX) && (defined(PRODUCTION) || defined(COVERAGE))

  public:
    explicit Image(const RADS::ImageSupport::ImageReady_t& imageReady); 
    ~Image() = default;

    // Copy
    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;

    // Move
    Image(Image&&) = delete;
    Image& operator=(Image&&) = delete;

    size_t         width() const;
    size_t         height() const;
    const uint8_t* getPixelsArray() const;
    void           save(const char* filename) const;
    static void    save(const uint8_t* pixels, uint32_t imageWidth, uint32_t imageHeight, const char* filename);
};

#endif // IMAGE_H
