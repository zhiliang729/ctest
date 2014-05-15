//
//  ZLTwinklingLight.m
//  BluetoothDemo
//
//  Created by zhangliang on 14-5-13.
//  Copyright (c) 2014年 com.yourcompany.orgnanization. All rights reserved.
//

#import "ZLTwinklingLight.h"

@interface ZLTwinklingLight ()

@end

@implementation ZLTwinklingLight
@synthesize twinkSpeed;
@synthesize shapeType;
@synthesize color;
@synthesize isContinue;

- (id)initWithFrame:(CGRect)frame
{
    return [self initWithFrame:frame twinkSpeed:1 color:[UIColor greenColor] shape:ShapeSquare isContinue:YES];
}


- (id)initWithFrame:(CGRect)frame twinkSpeed:(CGFloat)speed color:(UIColor *)_color shape:(LightShape)type isContinue:(BOOL)conti
{
    self = [super initWithFrame:frame];
    if (self) {
        self.color = _color;
        self.twinkSpeed = speed;
        self.shapeType = type;
        self.backgroundColor = _color;
        if (type == ShapeSquare) {
            
        }else{
            self.layer.cornerRadius = self.frame.size.height/2;
        }
        self.isContinue = conti;
        if (self.isContinue) {
            [self twink];
        }else{
            
        }
    }
    return self;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

- (void)startTwink
{
    self.alpha = 1.0;
    if (isContinue) {/*已开始了*/

    }else{
        self.isContinue = YES;
        [self twink];
    }
}

- (void)twink
{
    [UIView beginAnimations:@"" context:nil];
	[UIView setAnimationCurve:UIViewAnimationCurveLinear];
	[UIView setAnimationDuration:self.twinkSpeed];
	[UIView setAnimationDelegate:self];
	[UIView setAnimationDidStopSelector:@selector(twinkStoped)];
    
    if (self.alpha == 0.0) {
        self.alpha = 1.0;
    }else{
        self.alpha = 0.0;
    }
    
	[UIView commitAnimations];
}

- (void)twinkStoped
{
    if (isContinue) {
        [self twink];
    }else{
        
    }
}


- (void)stopTwink
{
    self.alpha = 1.0;
    self.isContinue = NO;
}

- (void)dealloc
{
    [color release];
    [super dealloc];
}

@end
