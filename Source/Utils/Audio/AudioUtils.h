//
//  AudioUtils.h
//  GameDevFramework
//
//  Created by Bradley Flood on 12-09-05.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef AUDIO_UTILS_H
#define AUDIO_UTILS_H

namespace AudioUtils
{
  void loadAudioData(const char* filename, void** audioData, unsigned int* audioDataSize, int* audioDataFormat, int* audioDataSampleRate);
}

#endif
