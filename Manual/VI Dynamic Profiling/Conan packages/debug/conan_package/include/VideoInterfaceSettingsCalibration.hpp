// Copyright (c) 2022 Jangoo Technology - https://www.jangootech.com

#ifndef VIDEO_INTERFACE_SETTINGS_CALIBRATION_H
#define VIDEO_INTERFACE_SETTINGS_CALIBRATION_H

#include <cstdint>
#include <unordered_map>

#include "ISPSettings.hpp"
#include "CommunicationDefs.hpp"

namespace RADS
{
class VideoInterfaceSettingsCalibration
{
  public:
    // NOLINTNEXTLINE
    VideoInterfaceSettingsCalibration(bool isLiveVideo = true, bool isCameraSwitchingOn = false)
        : mIsLiveVideo{isLiveVideo} {}; // Add fields here as required to change class variables

    // global recovery attempt trials (maintained by Supervisot to restart video interface app on error recovery)
    uint32_t mRestartGlobalRecoveryTrials{5};

    // needed to recover from VisionErrorStatus_e::AR0143_INIT_ERROR
    uint32_t mInitErrorRecoveryTrials{1};
    uint32_t mInitErrorWindowSize{1};
    uint32_t mInitErrorTransientsLimit{0};

    // needed to recover from VisionErrorStatus_e::AR0143_POP_FRAME_ERROR
    uint32_t mPopFrameErrorRecoveryTrials{100};
    uint32_t mPopFrameErrorWindowSize{3};
    uint32_t mPopFrameErrorTransientsLimit{1};

    // needed to recover from VisionErrorStatus_e::AR0143_PUSH_FRAME_ERROR
    uint32_t mPushFrameErrorRecoveryTrials{100};
    uint32_t mPushFrameErrorWindowSize{3};
    uint32_t mPushFrameErrorTransientsLimit{1};

    // needed to recover from VisionErrorStatus_e::AR0143_ISP_ERROR
    uint32_t mISPErrorRecoveryTrials{1};
    uint32_t mISPErrorWindowSize{1};
    uint32_t mISPErrorTransientsLimit{0};

    // Logging interval
    uint32_t mVideoInterfaceLoggingInterval_ms{5000}; // in millseconds

    // Contrast enhancement toggle
    bool mIsApplicationBasedDynamicISPOn{true};

    bool mIsLiveVideo{false};

    bool mIsIlluminationBasedDynamicISPOn{false};

    std::unordered_map<uint32_t, ISPSettings_t> mAR0143ISPSettings{}; ///< Stores the ISP settings
    std::unordered_map<uint32_t, ISPSettings_t> mAR0147ISPSettings{}; ///< Stores the ISP settings
};

} // namespace RADS
#endif // VIDEO_INTERFACE_SETTINGS_CALIBRATION_H

