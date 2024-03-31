// Copyright (c) 2022 Jangoo US - https://www.jangoo.us

#ifndef CONSUMER_REGISTER_INFO_H
#define CONSUMER_REGISTER_INFO_H
#include <cstdint>

struct ConsumerRegisterInfo_t
{
    bool     registered;
    uint64_t time_ms; // register or unregister time
    bool     active;
};
#endif // CONSUMER_REGISTER_INFO_H
