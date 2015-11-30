//
//  AudioUtils.mm
//  GameDevFramework
//
//  Created by Bradley Flood on 12-09-05.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "AudioUtils.h"
#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AudioToolbox/ExtendedAudioFile.h>

namespace AudioUtils
{
  void loadAudioData(const char* aFilename, void** aAudioData, unsigned int* aAudioDataSize, int* aAudioDataFormat, int* aAudioDataSampleRate)
  {
    //
  	void* audioData = NULL;
    UInt64 audioDataSize = 0;
    AudioStreamBasicDescription audioDataInfo;
    UInt32 audioDataInfoSize = sizeof(audioDataInfo);
    AudioFileID audioDataId = 0;
    OSStatus error = noErr;	
    BOOL abort = NO;
    
    //Get the path for the filename, always of type wav
    NSString *filename = [[NSString alloc] initWithCString:aFilename encoding:NSUTF8StringEncoding];
    NSString *path = [[NSBundle mainBundle] pathForResource:filename ofType:@"wav"];
    NSURL *patlUrl = [[NSURL alloc] initFileURLWithPath:path];
    
    //Open the audio file
    error = AudioFileOpenURL((CFURLRef)patlUrl, kAudioFileReadPermission, 0, &audioDataId);
    if(error != noErr) 
    { 
      abort = YES;
    }
    
    //Release the pathUrl and the filename NSStrings 
    [patlUrl release];
    [filename release];
    
    
    //Get the audio data properties.
    if(abort == NO)
    {
      error = AudioFileGetProperty(audioDataId, kAudioFilePropertyDataFormat, &audioDataInfoSize, &audioDataInfo);
      if(error != noErr) 
      { 
        abort = YES;
      }
    }
    
    if(abort == NO)
    {
      if(audioDataInfo.mChannelsPerFrame > 2)  
      { 
        abort = YES;
      }
    }
    
    if(abort == NO)
    {
      if((audioDataInfo.mFormatID != kAudioFormatLinearPCM) || (!TestAudioFormatNativeEndian(audioDataInfo))) 
      { 
        abort = YES;
      }
    }
    
    if(abort == NO)
    {
      if ((audioDataInfo.mBitsPerChannel != 8) && (audioDataInfo.mBitsPerChannel != 16)) 
      { 
        abort = YES;
      }
    }
    
    if(abort == NO)
    {
      audioDataInfoSize = sizeof(audioDataSize);
      error = AudioFileGetProperty(audioDataId, kAudioFilePropertyAudioDataByteCount, &audioDataInfoSize, &audioDataSize);
      if(error != noErr) 
      { 
        abort = YES;
      }
    }
  
    if(abort == NO)
    {
      //Read the audio data into memory.
      UInt32 dataSize = (UInt32)audioDataSize;
      audioData = malloc(audioDataSize);
      if(audioData != NULL)
      {
        AudioFileReadBytes(audioDataId, false, 0, &dataSize, audioData);
        if(error == noErr)
        {
          //Success, set the aAudioData
          *aAudioData = audioData;
            
          //And then set the aAudioDataSize
          *aAudioDataSize = dataSize;
          
          //Set the aAudioDataFormat based on its format.
          if(audioDataInfo.mBitsPerChannel == 16) 
          { 
            *aAudioDataFormat = (audioDataInfo.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
          } 
          else 
          {
            *aAudioDataFormat = (audioDataInfo.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;	
          }	
          
          //Set the aAudioDataSampleRate.
          *aAudioDataSampleRate = audioDataInfo.mSampleRate;
        }
        else 
        { 
          //Failed, free the data.
          free (audioData);
          audioData = NULL;
        }	
      }
    }
    
    //Close the ExtAudioFileRef, it is no longer needed.
    if(audioDataId)
    {
      AudioFileClose(audioDataId);
    }
  }
}