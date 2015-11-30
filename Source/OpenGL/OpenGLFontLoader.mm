//
//  OpenGLFontLoader.m
//  GameDevFramework
//
//  Created by Bradley Flood on 2012-09-18.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "OpenGLFontLoader.h"
#include "OpenGLTextureLoader.h"


namespace OpenGLFontLoader
{
  void loadFont(OpenGLFontInfo** aFontInfo)
  {
    //Dereference the fontInfo struct pointer
    OpenGLFontInfo* fontInfo = *aFontInfo;
  
    //Allocate the font name NSString
    NSString *fontName = [[NSString alloc] initWithCString:fontInfo->fontName encoding:NSUTF8StringEncoding];

    //Create the UIFont
    UIFont *font = [UIFont fontWithName:fontName size:fontInfo->fontSize];

    //Release the font name
    [fontName release];
    fontName = nil;
    
    //Allocate the character set NSString
    NSString *characterSet = [[NSString alloc] initWithCString:fontInfo->characterSet encoding:NSUTF8StringEncoding];
    
    //Cache the character set length
    int characterSetLength = [characterSet length];
    
    //Determine how many rows there will be
    int numRows = 1;
    while(numRows * numRows < characterSetLength)
    {
      ++numRows;
    }
    
    //Set the columns to match the rows
    int numColumns = numRows;

    //Calculate how much height and width will be needed
    int maxWidth = 0;
    int maxHeight = 0;
    for(int index = 0; index < characterSetLength; index++)
    {
      NSString *character = [characterSet substringWithRange:NSMakeRange(index, 1)];
      CGSize characterSize = [character sizeWithFont:font];
      maxWidth = fmaxf(maxWidth, characterSize.width);
      maxHeight = fmaxf(maxHeight, characterSize.height);
    }
    
    //Calculate the row and column size
    int rowSize = maxHeight * numRows;
    int columnSize = maxWidth * numColumns;
    
    //Create an array to how each character's font metrics
    NSMutableArray *characterData = [[NSMutableArray alloc] initWithCapacity:characterSetLength];
    
    //Begin rendering the font to the Graphics context
    UIGraphicsBeginImageContext(CGSizeMake(columnSize, rowSize));
    CGContextSetRGBFillColor(UIGraphicsGetCurrentContext(), 1.0f, 1.0f, 1.0f, 1.0f);
    
    int currentRow = 0;
    int currentColumn = 0;
    for(int index = 0; index < characterSetLength; index++)
    {
      //Get the character for the current index
      NSString *character = [characterSet substringWithRange:NSMakeRange(index, 1)];
      
      //Claculate the character size and location
      CGSize characterSize = [character sizeWithFont:font];
      CGPoint characterLocation = CGPointMake(currentColumn * maxWidth + ((maxWidth - characterSize.width) / 2.0f), currentRow * maxHeight + ((maxHeight - characterSize.height) / 2.0f));
      CGRect characterFrame = CGRectMake(characterLocation.x, characterLocation.y, characterSize.width, characterSize.height);
      
      //Store the size and location data into the array
      [characterData addObject:[NSValue valueWithCGRect:characterFrame]];
      
      //Set the draw location for the character and draw it
      CGPoint drawLocation = CGPointMake(characterLocation.x, rowSize - characterLocation.y - maxHeight);
      [character drawAtPoint:drawLocation withFont:font];
      
      //Column and row management
      currentColumn++;
      if(currentColumn == numColumns)
      {
        currentColumn = 0;
        currentRow++;
        if(currentRow == numRows)
        {
          break;
        }
      }
    }
    
    //Capture the font image from the graphics context into an UIImage
    UIImage *fontImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
        
    //Allocate the texture info struct
    OpenGLTextureInfo* textureInfo = (OpenGLTextureInfo*)malloc(sizeof(OpenGLTextureInfo));
    memset(textureInfo, 0, sizeof(OpenGLTextureInfo));
  
    //Load the font image into the texture info struct
    OpenGLTextureLoader::loadTextureFromImage(fontImage, &textureInfo);
    
    //Create the font texture from texture info struct
    fontInfo->fontTexture = new OpenGLTexture(textureInfo);
    
    //Cycle through and setup each character's font frame
    for(int index = 0; index < characterSetLength; index++)
    {
      //Allocate a new frame info struct and copy the texture info struct into it
      OpenGLTextureInfo* frameInfo = (OpenGLTextureInfo*)malloc(sizeof(OpenGLTextureInfo));
      memset(frameInfo, 0, sizeof(OpenGLTextureInfo));
      memcpy(frameInfo, textureInfo, sizeof(OpenGLTextureInfo));
    
      //Set the frame info's x, y, width, and height data from the character frame data
      CGRect characterFrame = [[characterData objectAtIndex:index] CGRectValue];
      frameInfo->sourceX = characterFrame.origin.x;
      frameInfo->sourceY = characterFrame.origin.y;
      frameInfo->sourceWidth = characterFrame.size.width;
      frameInfo->sourceHeight = characterFrame.size.height;
    
      //Create the OpenGLTexture for this font frame
      fontInfo->fontFrames[index] = new OpenGLTexture(frameInfo);
      
      //Free the frame info struct
      free(frameInfo);
      frameInfo = NULL;
    }
    
    //Free the texture info struct
    free(textureInfo);
    textureInfo = NULL;
    
    //Release the character set
    [characterSet release];
    characterSet = nil;
    
    //Release the character data
    [characterData release];
    characterData = nil;
  }
}