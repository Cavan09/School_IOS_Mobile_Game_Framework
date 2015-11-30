//
//  OpenGLTextureLoader.cpp
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "OpenGLTextureLoader.h"
#include "OpenGLTexture.h"
#include "MathUtils.h"
#include <string>


namespace OpenGLTextureLoader
{
  void loadTextureFromPath(const char* aPath, OpenGLTextureInfo** aTextureInfo)
  {
    //Convert the c string to a NSString
    NSString *path = [[NSString alloc] initWithCString:aPath encoding:NSUTF8StringEncoding];
    
    //Create a UIImage from the image path
    UIImage *image = [[UIImage alloc] initWithContentsOfFile:path];
    
    //Release the path NSString object
    [path release];
    
    //Load the UIImage into an OpenGL Texture
    OpenGLTextureLoader::loadTextureFromImage(image, aTextureInfo);
    
    //Release the UIImage
    [image release];
    image = nil;
  }
  
  void loadTextureFromImage(void* aImage, OpenGLTextureInfo** aTextureInfo)
  {
    //Get the UIImage's CGImageRef and load the OpenGLTextureInfo.
    CGImageRef cgImage = [(UIImage *)aImage CGImage];
    
    //Texture info struct
    OpenGLTextureInfo* textureInfo = *aTextureInfo;
    
    //Load the texture data, and get the OpenGL texture id.
    GLuint textureId = textureInfo->textureId;
    if(textureId == 0)
    {
      //Get the width and height of the image.
      unsigned int sourceWidth = CGImageGetWidth(cgImage);
      unsigned int sourceHeight = CGImageGetHeight(cgImage);
      
      //Get the power of two width and height.
      GLuint textureWidth = MathUtils::nextPowerOf2(sourceWidth);
      GLuint textureHeight = MathUtils::nextPowerOf2(sourceHeight);

      //Create a temporary buffer to hold our image data.
      size_t bytesPerPixel = 4;
      size_t size = textureWidth * textureHeight * bytesPerPixel;
      GLubyte *imageData = (GLubyte *) malloc(size);
      memset(imageData, 0, size); 
      
      //Uses the bitmatp creation function provided by the Core Graphics framework. 
      CGContextRef cgContext = CGBitmapContextCreate(imageData, textureWidth, textureHeight, 8, textureWidth * bytesPerPixel, CGImageGetColorSpace(cgImage), kCGImageAlphaPremultipliedLast);

      //After you create the context, you can draw the image to the context.
      CGContextDrawImage(cgContext, CGRectMake(0.0, 0.0, (float)sourceWidth, (float)sourceHeight), cgImage);
      
      //You don't need the context anymore, release it.
      CGContextRelease(cgContext);
      
      //Use OpenGL ES to generate a name for the texture.
      glGenTextures(1, &textureId);
      
      //Bind the texture name. 
      glBindTexture(GL_TEXTURE_2D, textureId);
      
      //Does the texture options sepcify if the texture has mipmaps?
      GLint mipmapLevel = 0;//[[aOptions objectForKey:@"GLImageMipmapLevel"] intValue];

      //Set the texture parameters to use a minifying filter and a linear filer (weighted average)
      if(mipmapLevel > 0)
      {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      }
      else
      {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      }
      
      //Specify a 2D texture image, provideing the a pointer to the image data in memory
      glTexImage2D(GL_TEXTURE_2D, mipmapLevel, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);			
              
      //Free the image data buffer.
      free(imageData);
    }

    //If the texture name isn't zero, set the image info struct.
    if(textureId != 0)
    {
      //Is there only a subsection of the image to use? If not use 0,0 as the image origin.
      GLuint sourceX = textureInfo->sourceX;
      GLuint sourceY = textureInfo->sourceY;
    
      //Is there only a subsection of the image to use? If not use the image's native width.
      GLuint sourceWidth = textureInfo->sourceWidth;
      if(sourceWidth == 0)
      {
        sourceWidth = CGImageGetWidth(cgImage);
      }
      
      //Is there only a subsection of the image to use? If not use the image's native height.
      GLuint sourceHeight = textureInfo->sourceHeight;
      if(sourceHeight == 0)
      {
        sourceHeight = CGImageGetHeight(cgImage);
      }
    
      //Set the texture info struct data
      textureInfo->textureId = textureId;
      textureInfo->sourceX = sourceX;
      textureInfo->sourceY = sourceY;
      textureInfo->sourceWidth = sourceWidth;
      textureInfo->sourceHeight = sourceHeight;
      textureInfo->textureWidth = MathUtils::nextPowerOf2(CGImageGetWidth(cgImage));
      textureInfo->textureHeight = MathUtils::nextPowerOf2(CGImageGetHeight(cgImage));
      textureInfo->textureFormat = GL_RGBA;
    }
  }
  
  void loadTextureFromAtlas(const char* aPngPath, const char* aPlistPath, const char* aAtlasKey, OpenGLTextureInfo** aTextureInfo)
  {
    //Create the plistPath and atlasKey and rootDictionary
    NSString *plistPath = [[NSString alloc] initWithCString:aPlistPath encoding:NSUTF8StringEncoding];
    NSString *atlasKey = [[NSString alloc] initWithCString:aAtlasKey encoding:NSUTF8StringEncoding];
    NSDictionary *rootDictionary = [[NSDictionary alloc] initWithContentsOfFile:plistPath];
    
    //Load the frames, metadata and atlas dictionaries
    NSDictionary *framesDictionary = [rootDictionary objectForKey:@"frames"];
    NSDictionary *metadataDictionary = [rootDictionary objectForKey:@"metadata"];
    NSDictionary *atlasDictionary = [framesDictionary objectForKey:atlasKey];
    
    //Get the atlas image's frame
    CGRect atlasFrame = CGRectFromString([atlasDictionary objectForKey:@"frame"]);
    
    //Get the texture size
    CGSize textureSize = CGSizeFromString([metadataDictionary objectForKey:@"size"]);
    
    //Set the texture info position and size
    OpenGLTextureInfo* textureInfo = *aTextureInfo;
    textureInfo->sourceWidth = atlasFrame.size.width;
    textureInfo->sourceHeight = atlasFrame.size.height;
    textureInfo->sourceX = atlasFrame.origin.x;
    textureInfo->sourceY = textureSize.height - (atlasFrame.origin.y + atlasFrame.size.height);

    //Load the texture with the updated source position and size
    loadTextureFromPath(aPngPath, aTextureInfo);
    
    //Release the atlas key, plist path and atlas dictionary
    [plistPath release];
    [atlasKey release];
    [rootDictionary release];
  }
    
  void loadAnimatedTextureFromPath(const char* aPngPath, const char* aPlistPath, OpenGLAnimatedTextureInfo** aAnimatedTextureInfo)
  {
    //Create the plistPath and atlasKey and rootDictionary
    NSString *plistPath = [[NSString alloc] initWithCString:aPlistPath encoding:NSUTF8StringEncoding];
    NSDictionary *rootDictionary = [[NSDictionary alloc] initWithContentsOfFile:plistPath];
  
    //Load the frames, metadata and atlas dictionaries
    NSDictionary *framesDictionary = [rootDictionary objectForKey:@"frames"];
    NSDictionary *metadataDictionary = [rootDictionary objectForKey:@"metadata"];
    
    //Get the texture size
    CGSize textureSize = CGSizeFromString([metadataDictionary objectForKey:@"size"]);
    
    //Dereference the Animated texture struct
    OpenGLAnimatedTextureInfo* animatedTextureInfo = *aAnimatedTextureInfo;
    animatedTextureInfo->frameCount = [framesDictionary count];
  
    //Load the full animated texture image
    OpenGLTextureLoader::loadTextureFromPath(aPngPath, &animatedTextureInfo->textureInfo);
    
    //Create the animated texture frames array
    animatedTextureInfo->frames = new OpenGLTexture *[animatedTextureInfo->frameCount];
    
    //Cycle through and setup each frame in the animation 
    for(int index = 0; index < animatedTextureInfo->frameCount; index++)
    {
      //Cache the frame's atlas dictionary
      NSArray *atlasKeys = [framesDictionary allKeys];
      NSArray *atlasValues = [framesDictionary allValues];
      NSDictionary *atlasDictionary = [atlasValues objectAtIndex:index];
      NSString *atlasKey = [atlasKeys objectAtIndex:index];
    
      //Allocate the frameInfo struct and copy the textureInfo struct
      OpenGLTextureInfo* frameInfo = (OpenGLTextureInfo*)malloc(sizeof(OpenGLTextureInfo));
      memset(frameInfo, 0, sizeof(OpenGLTextureInfo));
      memcpy(frameInfo, animatedTextureInfo->textureInfo, sizeof(OpenGLTextureInfo));
    
      //Get the atlas image's frame
      CGRect atlasFrame = CGRectFromString([atlasDictionary objectForKey:@"frame"]);
      
      //Set the frame info's position and size
      frameInfo->sourceWidth = atlasFrame.size.width;
      frameInfo->sourceHeight = atlasFrame.size.height;
      frameInfo->sourceX = atlasFrame.origin.x;
      frameInfo->sourceY = textureSize.height - (atlasFrame.origin.y + atlasFrame.size.height);

      //Set the frame's key
      const char* key = [atlasKey cStringUsingEncoding:NSUTF8StringEncoding];
      frameInfo->textureFilename = (const char*)malloc(strlen(key)+1);
      memcpy((void*)frameInfo->textureFilename, key, strlen(key)+1);
      
      //Set the frame in the animated texture info frames array
      animatedTextureInfo->frames[index] = new OpenGLTexture(frameInfo);
      
      //Free the frame info
      free(frameInfo);
      frameInfo = NULL;
    }
  
    //Release the atlas key, plist path and atlas dictionary
    [plistPath release];
    [rootDictionary release];
  }
}

