// Copyright (c) 2022 Jangoo US - https://www.jangoo.us

#ifndef IMAGE_FILE_H
#define IMAGE_FILE_H

#include <cstdint>
#include <cstddef>
#include <vector>

#include "RADSCommon.hpp"

// Used to represent images on file system
class ImageFile
{
  private:
    const char_t*        mFilename = nullptr;
    uint32_t             mWidth    = 0;
    uint32_t             mHeight   = 0;
    std::vector<uint8_t> mPixels;

  public:
    explicit ImageFile(const char_t* imageFilename);
    size_t         getImageWidth() const;
    size_t         getImageHeight() const;
    const uint8_t* getPixelsArray() const;
    const char_t*  getImageFilename() const;

    static constexpr uint32_t MAXLINE{128U};
};

#endif // IMAGE_FILE_H
