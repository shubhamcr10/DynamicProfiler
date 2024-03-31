// // Copyright (c) 2022 Jangoo US - https://www.jangoo.us

// #ifndef ISP_GRAPH_H
// #define ISP_GRAPH_H

// #include "ISPFrame.hpp"
// #include "VisionErrorStatus.hpp"
// // This interface represents ISP Graph
// // There could be many different subclasses from this class for different ISP Graphs
// // Each ISP Graph should be associated with specific Camera that is identified by CameraIds_e value
// // The ISPGraphManager will be responsible for managing the loading/unloading different ISP Graphs based on Camera Ids.
// class VisionErrorRecovery;
// class ISPGraph
// {
//   protected:
//     uint32_t mFrameSequenceNumber{};

//   public:
//     ISPGraph()          = default;
//     virtual ~ISPGraph() = default;

//     // Copy
//     ISPGraph(const ISPGraph&) = delete;
//     ISPGraph& operator=(const ISPGraph&) = delete;

//     // Move
//     ISPGraph(ISPGraph&&) = delete;
//     ISPGraph&    operator=(ISPGraph&&) = delete;
//     virtual void init()                = 0;
//     virtual void restart()             = 0;

//     virtual void close() = 0;

//     virtual bool     pop(ISPFrame_t& ispFrame)            = 0;
//     virtual bool     push(ISPFrame_t& ispFrame)           = 0;
//     virtual uint32_t filterISPFrame(ISPFrame_t& ispFrame) = 0;
//     // true if there is non-transient error
//     // we need this pull approach for checking non-transient errors. Not all ISPGraph methods throw exceptions
//     // Some functions are also async functions like CSI SeqEventHandler
//     virtual bool checkVisionError() = 0;

//     // return the detected non-transient error. This should let users calls its recover() as long as more recovery
//     // trails are possible for that error.
//     virtual VisionErrorRecovery& getVisionError() = 0;

//     // clear current vision error if recovery was successful
//     virtual void clearVisionError() = 0;

//     // Handle internal errors in the ISP Graph
//     virtual void handleInternalError() = 0; // NOLINT -- virtual warns

//     /// Enables ISP settings required for optimum performance of autohitch feature.
//     /// This includes Contrast Enhancement turned ON and Sharpening Gain reduced
//     /// <br>Errors: None
//     /// @param[in] None
//     /// @return None
//     /// @exception None
//     virtual void enableAutoHitchISPSettings() = 0;

//     /// Enables ISP settings required for optimum performance of deepTAD feature.
//     /// This includes Contrast Enhancement turned OFF and Sharpening Gain increase
//     /// <br>Errors: None
//     /// @param[in] None
//     /// @return None
//     /// @exception None
//     virtual void enableOtherConsumerISPSettings() = 0;

//     /// Checks whether the frame sequence number is less than max count
//     /// <br>Errors: None
//     /// @param[in] uint32_t Max frame count
//     /// @param[in] ISPFrame_t frame containing sequence number
//     /// @return bool true if frame sequence number less than argument max count
//     /// @exception None
//     static bool isSequenceNumberLessThanLimit(uint32_t maxCount, const ISPFrame_t& currentISPFrame);
// };

// #endif // ISP_GRAPH_H
