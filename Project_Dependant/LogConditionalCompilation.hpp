#ifndef _LOG_CONDITIONAL_COMPILATION_HPP
#define _LOG_CONDITIONAL_COMPILATION_HPP

#define _LOG_ENABLE_
#define _LOG_ENABLE_BMP_

#ifdef _LOG_ENABLE_

  //  Physics vars of player ship
  //#define _LOG_ENABLE_PHYSICS_PLAYER_POS
  //#define _LOG_ENABLE_PHYSICS_PLAYER_FORCE
  #define _LOG_ENABLE_PHYSICS_PLAYER_TORQUE
  #define _LOG_ENABLE_PHYSICS_PLAYER_TORQUE_SPLINE
  #define _LOG_ENABLE_PHYSICS_PLAYER_TORQUE_DRAG
  //#define _LOG_ENABLE_LAPSED_TIME
  //#define _LOG_ENABLE_PHYSICS_PLAYER_POINTING_VECTOR

  //  Input of player
  #define _LOG_ENABLE_MOUSE_INCREMENT  
  
  //#define _LOG_ENABLE_CAMERA_TARGET

  //  World creation
  //#define _LOG_ENABLE_WORLD_POINTS_IN_MOUNTAIN
  #define _LOG_ENABLE_WORLD_MOUNTAIN

  

#endif

#ifdef _LOG_ENABLE_BMP_
  //  World Terrain
  #define _LOG_BMP_MOUNTAIN_STEPS_
#endif


#endif