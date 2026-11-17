#ifndef COREEVENTS_H
#define COREEVENTS_H


class GameObject;

struct CollisionEvent {
    GameObject& this_;
    GameObject& other_;
};

#endif
