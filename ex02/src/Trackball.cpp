#include "Trackball.h"
#include <cmath>
#include <iostream>

Trackball::Trackball() {
  reset();
}

Trackball::~Trackball() {}

void Trackball::reset(void) { 
  mViewOffset = {0.0f, 0.0f, 10.0f};
  mPhi = 2.0f;
  mLastPhi = mPhi;
  mTheta = 1.0f;
  mLastTheta = mTheta;
  mX = 0;
  mY = 0;
  mState = NO_BTN;
}
    
void Trackball::updateMousePos(int x, int y) {
    // implement the angular update given by the current mouse position //
    if(mState == NO_BTN)
        return;

    mLastPhi = mPhi;
    mLastTheta = mTheta;

    mTheta -= (x-mX) * 0.01f;
    mPhi += (y-mY) * 0.01f;
    if(mPhi > 3.14f)
        mPhi = 3.14f;
    if(mPhi < 0)
        mPhi = 0;

    std::cout << "Phi: " << mPhi << ", Theta: " << mTheta << std::endl;

    mX = x;
    mY = y;
}

void Trackball::updateMouseBtn(MouseState state, int x, int y) {
    // whenever this method is called, a mouse button has been pressed or released //
    switch (state) {
        case NO_BTN : {
                          break;
                      }
        case LEFT_BTN : {
                            break;
                        }
        case RIGHT_BTN : {
                             // not used yet //
                             break;
                         }
        default : break;
    }
    mX = x;
    mY = y;
    mState = state;
}

void Trackball::updateOffset(Motion motion) {
  // whenever this method is called, a motion is intended //
  
  switch (motion) {
    case MOVE_FORWARD : {
      // TODO: move STEP_DISTANCE along viewing direction //
      mViewOffset[0] += cos(mTheta) * STEP_DISTANCE;
      mViewOffset[1] += cos(mPhi) * STEP_DISTANCE;
      mViewOffset[2] -= sin(mTheta) * STEP_DISTANCE;
      break;
    }
    case MOVE_BACKWARD : {
      // TODO: move STEP_DISTANCE in opposite of viewing direction //
      mViewOffset[0] -= cos(mTheta) * STEP_DISTANCE;
      mViewOffset[1] -= cos(mPhi) * STEP_DISTANCE;
      mViewOffset[2] += sin(mTheta) * STEP_DISTANCE;
      break;
    }
    case MOVE_LEFT : {
      // TODO: move STEP_DISTANCE to the left on the x-z-plane //
      mViewOffset[0] -= sin(mTheta) * STEP_DISTANCE;
      mViewOffset[2] -= cos(mTheta) * STEP_DISTANCE;
      break;
    }
    case MOVE_RIGHT : {
      // TODO: move STEP_DISTANCE to the right on the x-z-plane //
      mViewOffset[0] += sin(mTheta) * STEP_DISTANCE;
      mViewOffset[2] += cos(mTheta) * STEP_DISTANCE;
      break;
    }
    default : break;
  }
}

void Trackball::rotateView(void) {
    // TODO: use gluLookAt(...) to set up the view for the current view offset and viewing angles //
    // gluLookAt(mViewOffset[0], mViewOffset[1], mViewOffset[2], mViewOffset[0] +sin(mTheta), mViewOffset[1] +cos(mPhi), mViewOffset[2] -1, 0, 1, 0);
    gluLookAt(mViewOffset[0], mViewOffset[1], mViewOffset[2], mViewOffset[0] +cos(mTheta), mViewOffset[1] +cos(mPhi), mViewOffset[2] -sin(mTheta), 0, 1, 0);
}
