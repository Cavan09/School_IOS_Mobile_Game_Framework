//
//  OpenGLView.cpp
//  GameDevFramework
//
//  Created by Brad Flood on 11-10-16.
//  Copyright 2011 Algonquin College. All rights reserved.
//

#import "OpenGLView.h"
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>


@implementation OpenGLView

@synthesize enableDepthBuffer = m_EnableDepthBuffer;

+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

- (id)initWithCoder:(NSCoder*)coder
{
    if ((self = [super initWithCoder:coder]))
	{
        CAEAGLLayer* openGLLayer = (CAEAGLLayer *)[self layer];
        
        //Get the device's content scale factor.
        CGFloat contentScale = 1.0f;
        if([UIScreen instancesRespondToSelector:@selector(scale)])
        {
            contentScale = [[UIScreen mainScreen] scale];
        }
        
        //Set the layer's content scale.
        [openGLLayer setContentsScale:contentScale];
        
        //Set the layer's opacoty and drawable properties.
        [openGLLayer setOpaque:YES];
        [openGLLayer setDrawableProperties:[NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil]];
        
        //Set the view's scale factor.
		[self setContentScaleFactor:contentScale];
        
        //Set the depth buffer to be off, by default.
        m_EnableDepthBuffer = NO;
    }
    return self;
}

- (void)dealloc
{
    [self destroyFramebuffer];
    [m_OpenGLContext release];
    [super dealloc];
}

- (void)setEnableDepthBuffer:(BOOL)aEnableDepthBuffer
{
    m_EnableDepthBuffer = aEnableDepthBuffer;
    
    [self destroyFramebuffer];
	[self createFramebuffer];
    [self setupViewport];
}

- (void)setOpenGLContext:(EAGLContext *)aContext;
{
    [aContext retain];
    [m_OpenGLContext release];
    m_OpenGLContext = aContext;
    [self setNeedsLayout];
}

- (void)layoutSubviews
{
    //If our view is resized, we'll be asked to layout subviews. This is the perfect opportunity
    //to also update the framebuffer so that it is the same size as our display area.
	[EAGLContext setCurrentContext:m_OpenGLContext];
	[self destroyFramebuffer];
	[self createFramebuffer];
    [self setupViewport];
}

- (BOOL)createFramebuffer
{
    //Set the Open GL context.
    [EAGLContext setCurrentContext:m_OpenGLContext];
    
    if(m_OpenGLContext != nil)
    {
        //Generate IDs for a framebuffer object and a color renderbuffer.
        glGenFramebuffersOES(1, &m_FrameBuffer.viewFramebuffer);
        glGenRenderbuffersOES(1, &m_FrameBuffer.viewRenderbuffer);
        
        //Bind the IDs to the render and color buffers.
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_FrameBuffer.viewFramebuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_FrameBuffer.viewRenderbuffer);
        
        //This call associates the storage for the current render buffer with the EAGLDrawable (our CAEAGLLayer)
        //allowing us to draw into a buffer that will later be rendered to screen wherever the layer is
        //(which corresponds with our view).
        [m_OpenGLContext renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)[self layer]];
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, m_FrameBuffer.viewRenderbuffer);
        
        //Get the backbuffer width and height.
        glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &m_FrameBuffer.bufferWidth);
        glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &m_FrameBuffer.bufferHeight);
        
        NSLog(@"[OPENGL] Frame buffer created (%i, %i)", m_FrameBuffer.bufferWidth, m_FrameBuffer.bufferHeight);
        
        //If we need a depth buffer, create and attach one via another renderbuffer.
        if([self enableDepthBuffer] == YES)
        {
            glGenRenderbuffersOES(1, &m_FrameBuffer.depthRenderbuffer);
            glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_FrameBuffer.depthRenderbuffer);
            glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, m_FrameBuffer.bufferWidth, m_FrameBuffer.bufferHeight);
            glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, m_FrameBuffer.depthRenderbuffer);
        }
        
        //Check for errors, if any are found; return NO.
        if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
        {
            return NO;
        }
        
        //We have successfully created the framebuffer, return YES.
        return YES;
    }
    
    return NO;
}

- (void)destroyFramebuffer
{
    //Clean up any buffers we have allocated.
    glDeleteFramebuffersOES(1, &m_FrameBuffer.viewFramebuffer);
    m_FrameBuffer.viewFramebuffer = 0;
    glDeleteRenderbuffersOES(1, &m_FrameBuffer.viewRenderbuffer);
    m_FrameBuffer.viewRenderbuffer = 0;
    
    if(m_FrameBuffer.depthRenderbuffer)
    {
        glDeleteRenderbuffersOES(1, &m_FrameBuffer.depthRenderbuffer);
        m_FrameBuffer.depthRenderbuffer = 0;
    }
}

- (void)setupViewport
{
    GLfloat left = 0.0f;
    GLfloat right = m_FrameBuffer.bufferWidth;
    GLfloat bottom = 0.0f;
    GLfloat top = m_FrameBuffer.bufferHeight;
    GLfloat near = -1.0f;
    GLfloat far = 1.0f;
	
	//Setup our camera view
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(left, right, bottom, top, near, far);
	glMatrixMode(GL_MODELVIEW);
    
    //Setup the viewport.
    glViewport(0, 0, m_FrameBuffer.bufferWidth, m_FrameBuffer.bufferHeight);
}

- (void)beginDraw
{
	//Make sure that you are drawing to the current context
	[EAGLContext setCurrentContext:m_OpenGLContext];
    
    //Bind the view buffer.
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_FrameBuffer.viewFramebuffer);
    
    //Make sure we are in model matrix mode and set a clean transform.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

- (void)endDraw
{
    //If the depth render is created, clear the depth buffer and enable it.
    if(m_FrameBuffer.depthRenderbuffer)
    {
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
    }
    
    //Bind the renderbuffer.
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_FrameBuffer.viewRenderbuffer);
    
    //Display the the renderbuffer.
    if(m_OpenGLContext != nil)
    {
        [m_OpenGLContext presentRenderbuffer:GL_RENDERBUFFER_OES];	
    }
}

@end