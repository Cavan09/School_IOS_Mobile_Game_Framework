//
//  GameConstants.h
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

enum
{
    GameLoadStepInitial = 0,
    GameLoadStepWorld,
    GameLoadStepPlayer,
    GameLoadStepEnemies,
    GameLoadStepFinal,
	GameLoadStepCount
};
typedef unsigned int GameLoadSteps;


extern const float GAME_GRAVITY_X;
extern const float GAME_GRAVITY_Y;

extern const char* GAME_PHYSICS_EDITOR_FILENAME;
extern const float GAME_PHYSICS_PIXELS_TO_METERS_RATIO;
extern const bool GAME_PHYSICS_CONTINUOUS_SIMULATION;
extern const int GAME_PHYSICS_VELOCITY_ITERATIONS;
extern const int GAME_PHYSICS_POSITION_ITERATIONS;

#endif
