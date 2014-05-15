//
//  ZLTwinklingLight.h
//  BluetoothDemo
//
//  Created by zhangliang on 14-5-13.
//  Copyright (c) 2014年 com.yourcompany.orgnanization. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef enum _LightShape{
    ShapeRound = 0,
    ShapeSquare = 1
}LightShape;

@interface ZLTwinklingLight : UIView
{
    CGFloat twinkSpeed;/*闪烁时间*/
    UIColor * color;/*颜色*/
    int shapeType;/*形状*/
}

@property (nonatomic, assign) CGFloat twinkSpeed;
@property (nonatomic, retain) UIColor * color;/*颜色*/
@property (nonatomic, assign) int shapeType;
@property (nonatomic, assign) BOOL isContinue;

- (id)initWithFrame:(CGRect)frame twinkSpeed:(CGFloat)speed color:(UIColor *)color shape:(LightShape)type isContinue:(BOOL)conti;

- (void)startTwink;/*开始闪烁*/

- (void)stopTwink;/*结束闪烁*/

@end
