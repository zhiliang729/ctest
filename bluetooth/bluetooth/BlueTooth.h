//
//  BlueTooth.h
//  bluetooth
//
//  Created by zhangliang on 14-5-13.
//  Copyright (c) 2014年 com.yourcompany.orgnanization. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Device : NSObject
{
    NSString * bluetoothName;
    NSString * uuid;
    NSString * major;
    NSString * minor;
}

@end

@interface BlueTooth : NSObject
{
    NSMutableArray * nearDeviceArr;
    Device * connectedDevice;
}

@property (nonatomic, retain) NSMutableArray * nearDeviceArr;
@property (nonatomic, retain) Device * connectedDevice;

//取得单例
+ (BlueTooth *)sharedBlueTooth;

- (void)basicInfoInit;/*先调用此方法进行数据初始化*/

- (void)startBlueToothSearch;

- (void)stopBlueToothSearch;

+ (NSString *)version;
@end
