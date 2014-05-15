//
//  BlueTooth.h
//  bluetooth
//
//  Created by zhangliang on 14-5-13.
//  Copyright (c) 2014年 com.yourcompany.orgnanization. All rights reserved.
//

#import <Foundation/Foundation.h>

@class CBPeripheral;
@class CBService;
@class CBCharacteristic;
@protocol  BlueToothDelegate;

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
    id<BlueToothDelegate> delegate;
}

@property (nonatomic, retain) NSMutableArray * nearDeviceArr;
@property (nonatomic, retain) Device * connectedDevice;

//取得单例
+ (BlueTooth *)sharedBlueTooth;

- (void)basicInfoInit;/*先调用此方法进行数据初始化*/

- (void)startBlueToothSearch;/*开始搜索*/

- (void)stopBlueToothSearch;/*结束搜索*/

- (void)connectToPeripheral:(CBPeripheral *)peripheral;/*连接设备*/

- (void)disconnectPeripheral:(CBPeripheral *)peripheral;/*断开连接*/

- (void)searchServiceOfPeripheral:(CBPeripheral *)peripheral;/*查询蓝牙服务*/

- (void)searchCharacteristicsOfPeripheral:(CBPeripheral *)peripheral ofService:(CBService *)service;/*查询蓝牙特征值*/

- (void)writeData:(NSData *)data ForCharacteristic:(CBCharacteristic *)characteristic ToPeripheral:(CBPeripheral *)peripheral;/*给蓝牙发送数据*/


+ (NSString *)version;
@end


@protocol BlueToothDelegate <NSObject>

- (void)peripheral:(CBPeripheral *)peripheral didReceivedDataForCharacteristic:(CBCharacteristic *)characteristic;

@end

