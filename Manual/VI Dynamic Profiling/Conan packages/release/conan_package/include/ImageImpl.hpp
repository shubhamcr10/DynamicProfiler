// #ifndef IMAGE_IMPL_H
// #define IMAGE_IMPL_H
#include "RADSInterApplicationTypes.hpp"

class ImageImpl
{
  public:
    ImageImpl()          = default;
    ~ImageImpl() = default;

    // Copyquare 
    ImageImpl(const ImageImpl&) = delete;
    ImageImpl& operator=(const ImageImpl&) = delete;

    // Move
    ImageImpl(ImageImpl&&) = delete;
    ImageImpl& operator=(ImageImpl&&) = delete;

    size_t         width() const                                                    ;
    size_t         height() const                                                   ;
    const uint8_t* getPixelsArray() const                                           ;
    void           imageImplInit(const RADS::ImageSupport::ImageReady_t& imageReady);
};

// #endif // #ifndef IMAGE_IMPL_H
