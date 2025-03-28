// Basic Euler angles manipulations.
// 2024-12-22

#ifndef _ATTITUDE_EULER_H_
#define _ATTITUDE_EULER_H_

typedef enum
{
  EULER_XYX = 121,
  EULER_XYZ = 123,
  EULER_XZX = 131,
  EULER_XZY = 132,
  EULER_YXY = 212,
  EULER_YXZ = 213,
  EULER_YZX = 231,
  EULER_YZY = 232,
  EULER_ZXY = 312,
  EULER_ZXZ = 313,
  EULER_ZYX = 321,
  EULER_ZYZ = 323
} euler_seq_t;

#endif // euler.h
