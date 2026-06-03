#pragma once
#include "syati.h"

class RotateActorsSwitch : public NameObj {
public:
    RotateActorsSwitch(const char *pName);

    virtual bool rotater();
    virtual void init(const JMapInfoIter &rIter);
    virtual void control();
    
    LiveActorGroup* mGroup;
    StageSwitchCtrl* mCtrl;
    TVec3f mRot;
    s32 mGenID;
    bool mGo;
};