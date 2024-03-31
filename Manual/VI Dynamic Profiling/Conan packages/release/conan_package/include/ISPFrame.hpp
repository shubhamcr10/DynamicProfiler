// Copyright (c) 2022 Jangoo US - https://www.jangoo.us

#ifndef ISP_FRAME_H
#define ISP_FRAME_H

#include <cstdint>

#include "ISPFrameInfo.hpp"
// #include "oal_utils.hpp"
// #include "sdi.hpp"

struct ISPFrame_t
{
    // SDI_Frame      videoFrame{};
    // SDI_Frame      embeddedFrame{};
    ISPFrameInfo_t videoFrameInfo{};
};

// namespace RADS
// {
// inline void* getOALAddress(SDI_Frame frame)
// {
//     return reinterpret_cast<void*>( // NOLINT -- casts warns
//         OAL_GetReturnAddress(static_cast<OAL_MemoryHandle*>(frame.mUMat.u->handle), OAL_ACCESS_PHY));
// }
// } // namespace RADS

#endif // ISP_FRAME_H
