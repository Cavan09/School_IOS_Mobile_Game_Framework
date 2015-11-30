//
//  OpenGLView.h
//  GameDevFramework
//
//  Created by Brad Flood on 11-10-16.
//  Copyright 2011 Algonquin College. All rights reserved.
//

#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "OpenGLBuffer.h"


@class EAGLContext;

@interface OpenGLView : UIView
{
    EAGLContext* m_OpenGLContext;
	OpenGLBuffer m_FrameBuffer;
    BOOL m_EnableDepthBuffer;
}

@property (nonatomic, assign) BOOL enableDepthBuffer;

- (void)setOpenGLContext:(EAGLContext *)context;
- (BOOL)createFramebuffer;
- (void)destroyFramebuffer;
- (void)setupViewport;
- (void)beginDraw;
- (void)endDraw;

@end
