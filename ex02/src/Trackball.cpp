#include "Trackball.h"
#include <cmath>
#include <iostream>

Trackball::Trackball() {
  reset();
}

Trackball::~Trackball() {}

void Trackball::reset(void) { 
  mViewOffset = {0.0f, 0.0f, 10.0f};
  mPhi = 0.0f;
  mLastPhi = mPhi;
  mTheta = 0.0f;
  mLastTheta = mTheta;
  mX = 0;
  mY = 0;
  mState = NO_BTN;
}
    
void Trackball::updateMousePos(int x, int y) {
	if(mState == LEFT_BTN) {
		mTheta += (0.01 * (x - mX));
		mPhi += (0.01 * (y - mY));
		// wrap theta between 0 and 2 * M_PI
		// and bound phi between 0.001 and M_PI - 0.001 (poles are ambiguous!)
		while(mTheta < 0) mTheta+=(2 * M_PI);
		while(mTheta > 2 * M_PI) mTheta-=(2 * M_PI);
		if(mPhi <= 0) mPhi = 0.001;
		if(mPhi >= M_PI) mPhi = M_PI - 0.001;
		std::cout << "phi: " << mPhi << " theta: " << mTheta << std::endl;
	}
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
    case MOVE_FORWARD:
      mViewOffset[0] += std::sin(mPhi) * std::cos(mTheta) * STEP_DISTANCE;
      mViewOffset[1] += std::cos(mPhi) * STEP_DISTANCE;
      mViewOffset[2] += std::sin(mPhi) * std::sin(mTheta) * STEP_DISTANCE;
      break;
    case MOVE_BACKWARD : {
      mViewOffset[0] -= std::sin(mPhi) * std::cos(mTheta) * STEP_DISTANCE;
      mViewOffset[1] -= std::cos(mPhi) * STEP_DISTANCE;
      mViewOffset[2] -= std::sin(mPhi) * std::sin(mTheta) * STEP_DISTANCE;
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
      mViewOffset[2] += sin(mTheta) * STEP_DISTANCE;
      break;
    }
    default : break;
  }
}

void Trackball::rotateView(void) {
  // TODO: use gluLookAt(...) to set up the view for the current view offset and viewing angles //
  gluLookAt( mViewOffset[0]
           , mViewOffset[1]
           , mViewOffset[2]
           , mViewOffset[0] + std::sin(mPhi) * std::cos(mTheta)
           , mViewOffset[1] + std::cos(mPhi)
           , mViewOffset[2] + std::sin(mPhi) * std::sin(mTheta)
           , 0, 1, 0
           );
}
