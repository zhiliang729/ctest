//
//  ZLMainViewController.m
//  BluetoothDemo
//
//  Created by zhangliang on 14-5-13.
//  Copyright (c) 2014年 com.yourcompany.orgnanization. All rights reserved.
//

#import "ZLMainViewController.h"
#import "ZLTwinklingLight.h"

@interface ZLMainViewController ()
@property (nonatomic, retain) ZLTwinklingLight * lightRed;
@property (nonatomic, retain) ZLTwinklingLight * lightGreen;
@end

#define SPEED_LOW 2.0
#define SPEED_HIGH 0.2


@implementation ZLMainViewController
@synthesize mScrollView;
@synthesize lightGreen;
@synthesize lightRed;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    self.navigationController.navigationBarHidden = YES;
    self.mScrollView.contentSize = CGSizeMake(320, 480+200);
    
    self.lightRed = [[[ZLTwinklingLight alloc] initWithFrame:CGRectMake(50, 50, 50, 50) twinkSpeed:SPEED_HIGH color:[UIColor redColor] shape:ShapeRound isContinue:YES]autorelease];
    [self.mScrollView addSubview:lightRed];
    
    [lightRed startTwink];
    
    
    self.lightGreen = [[[ZLTwinklingLight alloc] initWithFrame:CGRectMake(150, 50, 50, 50) twinkSpeed:SPEED_HIGH color:[UIColor greenColor] shape:ShapeRound isContinue:YES] autorelease];
    [self.mScrollView addSubview:lightGreen];
    [lightGreen startTwink];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc {
    [lightRed release];
    [lightGreen release];
    [mScrollView release];
    [_startRecvButton release];
    [_startSendButton release];
    [super dealloc];
}
- (IBAction)startSend:(UIButton *)sender {
    self.lightRed.twinkSpeed = SPEED_LOW;
}

- (IBAction)startRecv:(UIButton *)sender {
    self.lightGreen.twinkSpeed = SPEED_LOW;
}

- (IBAction)stopSend:(UIButton *)sender {
    self.lightRed.twinkSpeed = SPEED_HIGH;
}

- (IBAction)stopRecv:(UIButton *)sender {
    self.lightGreen.twinkSpeed = SPEED_HIGH;
}

- (IBAction)startRed:(UIButton *)sender {
    [self stopSend:nil];
    if (self.lightRed.isContinue) {
    }else{
        [self.lightRed startTwink];
    }
}

- (IBAction)startGreen:(UIButton *)sender {
    [self stopRecv:nil];
    if (self.lightGreen.isContinue) {
    }else{
        [self.lightGreen startTwink];
    }
}

- (IBAction)stopRed:(id)sender {
    [self.lightRed stopTwink];
}

- (IBAction)stopGreen:(UIButton *)sender {
    [self.lightGreen stopTwink];
}
@end
