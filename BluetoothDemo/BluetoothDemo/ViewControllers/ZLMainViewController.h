//
//  ZLMainViewController.h
//  BluetoothDemo
//
//  Created by zhangliang on 14-5-13.
//  Copyright (c) 2014年 com.yourcompany.orgnanization. All rights reserved.
//

#import <UIKit/UIKit.h>
@interface ZLMainViewController : UIViewController<UIScrollViewDelegate>
@property (retain, nonatomic) IBOutlet UIScrollView *mScrollView;
@property (retain, nonatomic) IBOutlet UIButton *startSendButton;
@property (retain, nonatomic) IBOutlet UIButton *startRecvButton;


- (IBAction)startSend:(UIButton *)sender;
- (IBAction)startRecv:(UIButton *)sender;

- (IBAction)stopSend:(UIButton *)sender;
- (IBAction)stopRecv:(UIButton *)sende;

- (IBAction)startRed:(UIButton *)sender;
- (IBAction)startGreen:(UIButton *)sender;

- (IBAction)stopRed:(id)sender;
- (IBAction)stopGreen:(UIButton *)sender;
@end
