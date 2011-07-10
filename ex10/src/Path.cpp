#include "Path.h"
#include <iostream>

#include<float.h>

Path::Path() {
  setFirstControlPoint(ControlPoint());
  setLastControlPoint(ControlPoint());
  setLooped(false);
}

Path::Path(ControlPoint start, ControlPoint end, bool looped) {
  setFirstControlPoint(start);
  setLastControlPoint(end);
  setLooped(looped);
}
    
void Path::setFirstControlPoint(ControlPoint point) {
  point.time = -1;
  mControlPoints[-1] = point;
}

void Path::setLastControlPoint(ControlPoint point) {
  point.time = FLT_MAX;
  mControlPoints[FLT_MAX] = point;
}

void Path::addIntermediateControlPoint(ControlPoint point) {
  mControlPoints[point.time] = point;
}

void Path::setLooped(bool looped) {
  mIsLooped = looped;
}

bool Path::isLooped() {
  return mIsLooped;
}

ControlPoint Path::getPositionForTime(float t) {
  // init return value //
  ControlPoint P(0,0,0,t);

  // TODO: check sanity of t and adapt if neccessary (path might be looped) //

  // XXX: select correct control point segment for t //
  std::map<float, ControlPoint>::iterator bound = mControlPoints.lower_bound(t);

  // XXX: since t is globally defined for the whole path, you need to compute your interpolation step for the current segment //
  ControlPoint P1 = (*bound++).first;
  ControlPoint P2 = (*bound++).first;
  ControlPoint P3 = (*bound++).first;
  ControlPoint P4 = (*bound).first;

  float mat[16] = {
                        -1*(P1.pos[0]) +3*(P2.pos[0]) -3*(P3.pos[0]) +1*(P4.pos[0]),
                        +2*(P1.pos[0]) -5*(P2.pos[0]) +4*(P3.pos[0]) -1*(P4.pos[0]),
                        -1*(P1.pos[0]) +0*(P2.pos[0]) +1*(P3.pos[0]) +0*(P4.pos[0]),
                        +0*(P1.pos[0]) +2*(P2.pos[0]) +0*(P3.pos[0]) +0*(P4.pos[0]),

                        -1*(P1.pos[1]) +3*(P2.pos[1]) -3*(P3.pos[1]) +1*(P4.pos[1]),
                        +2*(P1.pos[1]) -5*(P2.pos[1]) +4*(P3.pos[1]) -1*(P4.pos[1]),
                        -1*(P1.pos[1]) +0*(P2.pos[1]) +1*(P3.pos[1]) +0*(P4.pos[1]),
                        +0*(P1.pos[1]) +2*(P2.pos[1]) +0*(P3.pos[1]) +0*(P4.pos[1]),

                        -1*(P1.pos[2]) +3*(P2.pos[2]) -3*(P3.pos[2]) +1*(P4.pos[2]),
                        +2*(P1.pos[2]) -5*(P2.pos[2]) +4*(P3.pos[2]) -1*(P4.pos[2]),
                        -1*(P1.pos[2]) +0*(P2.pos[2]) +1*(P3.pos[2]) +0*(P4.pos[2]),
                        +0*(P1.pos[2]) +2*(P2.pos[2]) +0*(P3.pos[2]) +0*(P4.pos[2]),

                        -1*(P1.pos[3]) +3*(P2.pos[3]) -3*(P3.pos[3]) +1*(P4.pos[3]),
                        +2*(P1.pos[3]) -5*(P2.pos[3]) +4*(P3.pos[3]) -1*(P4.pos[3]),
                        -1*(P1.pos[3]) +0*(P2.pos[3]) +1*(P3.pos[3]) +0*(P4.pos[3]),
                        +0*(P1.pos[3]) +2*(P2.pos[3]) +0*(P3.pos[3]) +0*(P4.pos[3]),
  };

  float t2 = t*t;
  float t3 = t*t*t;

  // TODO: compute the interpolated point //
  P.pos[0] = 0.5* (t3*mat[0] + t2*mat[1] + t*mat[2] + mat[3]);
  P.pos[1] = 0.5* (t3*mat[4] + t2*mat[5] + t*mat[6] + mat[7]);
  P.pos[2] = 0.5* (t3*mat[8] + t2*mat[9] + t*mat[10] + mat[11]);
  P.pos[3] = 0.5* (t3*mat[12] + t2*mat[13] + t*mat[14] + mat[15]);

  return P;
}
