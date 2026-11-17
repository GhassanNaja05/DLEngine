#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H
#include "UserFacing.h"

class EventListener final : public UserFacing {
public:
    EventListener() = default;
    ~EventListener() override = default;
};

#endif
