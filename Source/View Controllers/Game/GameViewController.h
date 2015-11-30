//
//  GameViewController.h
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

@class CADisplayLink;
@class EAGLContext;
@class OpenGLView;

@interface GameViewController : UIViewController
{
  CADisplayLink *m_DisplayLink;
  EAGLContext *m_OpenGLContext;
  OpenGLView *m_OpenGLView;
}

- (void)invalidateRunLoop;

@property (nonatomic, retain) IBOutlet OpenGLView *openGLView;

@end

