#include "RotateActorsSwitch.h"
using namespace MR;

RotateActorsSwitch::RotateActorsSwitch(const char *pName) : NameObj(pName) {
    mCtrl = NULL;
    mGo = true;
}

bool RotateActorsSwitch::rotater() {
    s32 x = 0; 
    s32 grpSz = mGroup->mNumObjs;
    if (grpSz < 1) return false;

    while (x < grpSz) {
        LiveActor *currAct = mGroup->getActor(x);
        currAct->mRotation += mRot;
        ++x;
    }
    
    if (x == grpSz) return true;
    else {
        OSReport("[RAS] Object amount inconsistent with amount rotated, failing. X: %d, GrpSz: %d\n", x, grpSz);
        return false;
    }
}

void RotateActorsSwitch::init(const JMapInfoIter &rIter) {
    connectToSceneMapObjMovement(this);
    getGeneratorID(rIter, &mGenID);
    getJMapInfoArg0NoInit(rIter, &mRot.x);
    getJMapInfoArg1NoInit(rIter, &mRot.y);
    getJMapInfoArg2NoInit(rIter, &mRot.z);
    mCtrl = createStageSwitchCtrl(this, rIter);

    AllLiveActorGroup* alag = getAllLiveActorGroup();
    s32 alSz = alag->mNumObjs;
    mGroup = new LiveActorGroup("RASGroup", alSz);

    for (s32 i = 0; i < alSz; i++) {
        LiveActor* currActor = alag->getActor(i);
        s16 matchid = currActor->mLinkedInfo.mLinkId;
        s16 checkid = mGenID;
        OSReport("[RAS] Object: %s, mLinkID: %hd vs GenID: %hd\n", currActor->mName, matchid, checkid);
        if (matchid == checkid) {
            mGroup->registerActor(currActor);
            OSReport("[RAS] Added Object %d to mGroup\n", currActor->mName);
        }
    }
}

void RotateActorsSwitch::control() {
    if (mGo && mCtrl->isOnSwitchA()) {
        mGo = false;

        rotater()
        ? OSReport("[RAS] Rotated %d objects with ID %hd\n", mGroup->mNumObjs, mGroup->getActor(0)->mLinkedInfo.mLinkId) 
        , OSReport("[RAS] Rotated by X: %d, Y: %d, Z: %d\n", mRot.x, mRot.y, mRot.z)
        : OSReport("[RAS] Failed Rotate or 0 objects found\n");
    }
}