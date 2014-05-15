//
//  BlueTooth.m
//  bluetooth
//
//  Created by zhangliang on 14-5-13.
//  Copyright (c) 2014年 com.yourcompany.orgnanization. All rights reserved.
//

#import "BlueTooth.h"
#import <CoreBluetooth/CoreBluetooth.h>

#define VERSION @"1.0"

@interface BlueTooth ()<CBCentralManagerDelegate, CBPeripheralDelegate, CBPeripheralManagerDelegate>/*主manager 次manager， 次delegate*/
@property (nonatomic, retain) CBCentralManager * manager;
@property (nonatomic, assign) BOOL isinited;
@end

static BlueTooth *sharedBlueTooth = nil;

@implementation BlueTooth
@synthesize nearDeviceArr;
@synthesize connectedDevice;
@synthesize manager;
@synthesize isinited;

+ (BlueTooth *)sharedBlueTooth {
	@synchronized(self) {
		if (sharedBlueTooth == nil) {
			sharedBlueTooth = [[super allocWithZone:NULL] init];
		}
	}
	return sharedBlueTooth;
}

+ (id)allocWithZone:(NSZone *)zone {
    @synchronized(self)
    {
        if(sharedBlueTooth==nil)
        {
            sharedBlueTooth=[super allocWithZone:zone];
        }
        return sharedBlueTooth ;
    }
    
    return nil;
}

- (id)copyWithZone:(NSZone *)zone
{
    return self;
}

- (id)retain
{
    return self;
}

- (NSUInteger)retainCount
{
    return NSUIntegerMax;  //denotes an object that cannot be released
}

- (oneway void)release
{
    //do nothing
}

- (id)autorelease
{
    return self;
}

#pragma mark - method

- (void)basicInfoInit/*先调用此方法进行数据初始化*/
{
    if (isinited) {
        
    }else{
        self.manager = [[[CBCentralManager alloc] initWithDelegate:self queue:nil] autorelease];
        self.manager.delegate = self;
        
        self.nearDeviceArr = [[[NSMutableArray alloc] initWithCapacity:0] autorelease];
        
        self.isinited = YES;
    }
}

- (void)startBlueToothSearch
{
    NSDictionary * dic = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:NO], CBCentralManagerScanOptionAllowDuplicatesKey, nil];/*指定不用过滤重复的设备*/
    [self.manager scanForPeripheralsWithServices:nil options:dic];
}

- (void)stopBlueToothSearch
{
    [self.manager stopScan];
}


- (void)connectToPeripheral:(CBPeripheral *)peripheral
{
    [self.manager connectPeripheral:peripheral options:[NSDictionary dictionaryWithObject:[NSNumber numberWithBool:YES] forKey:CBConnectPeripheralOptionNotifyOnDisconnectionKey]];/*指定当连接断开时，给予提示，但是不一定能收到提示，如果多个应用都对同一个设备注册了此消息，则谁先进入前台，谁得到消息*/
}

- (void)disconnectPeripheral:(CBPeripheral *)peripheral
{
    
}

- (void)searchServiceOfPeripheral:(CBPeripheral *)peripheral
{
    [peripheral discoverServices:nil];
}

- (void)searchCharacteristicsOfPeripheral:(CBPeripheral *)peripheral ofService:(CBService *)service
{
    [peripheral discoverCharacteristics:nil forService:service];
}

- (void)writeData:(NSData *)data ForCharacteristic:(CBCharacteristic *)characteristic ToPeripheral:(CBPeripheral *)peripheral/*给蓝牙发送数据， 会触发代理didWriteValueForCharacteristic:error:*/
{
    [peripheral writeValue:data forCharacteristic:characteristic type:CBCharacteristicWriteWithResponse];
}



// retrievePeripheralsWithIdentifiers 使用例子

-(void)retrieve:(id)Sender//检索
{
    
//    [self.tvLog setText:@""];
    
    NSMutableArray * identifiers = [NSMutableArray array];
    
    for (CBPeripheral * peripheral in self.nearDeviceArr) {
        
        [identifiers addObject:peripheral.identifier];
        
    }
    
    
    
//    self.retrievePeripherals = [self.manager retrievePeripheralsWithIdentifiers:identifiers];
    
//    for (CBPeripheral* peripheral in self.retrievePeripherals) {
//        //处理
//        
//    }
    
//    [self.tableViewPeripheral reloadData];
//    处理
    
}

//retrieveConnectedPeripheralsWithServices 使用例子


-(IBAction) Retrieve:(id)Sender

{
    
    NSMutableArray * services = [NSMutableArray array];
    
//    for (CBPeripheral * peripheral in self.peripheralArray) {
//        
//        if (peripheral.isConnected) {
//            
//            for (CBService *service in peripheral.services) {
//                
//                [services addObject:service.UUID];
//                
//            }
//            
//        }
//        
//    }
//    
//    [self addLog:@"[self.cbCentralMgr retrieveConnectedPeripheralsWithServices:peripheral.services]"];
//    
//    self.retrievePeripherals = [self.cbCentralMgr retrieveConnectedPeripheralsWithServices:services];
//    
//    for (CBPeripheral* peripheral in self.retrievePeripherals) {
//        
//        [self addLog:[NSString stringWithFormat: @"%@ name:%@",peripheral,peripheral.name]];
//        
//    }
//    
//    [self.tableViewPeripheral reloadData];
    
}

+ (NSString *)version
{
    return VERSION;
}

#pragma mark - CBCentralManagerDelegate  CBCentralManager对象的代理必须遵守CBCentralManagerDelegate协议。唯一的必须实现的方法表明CentralManager的能力，可选方法允许发现和连接外设。
- (void)centralManagerDidUpdateState:(CBCentralManager *)central/*当CentralManager的状态变化，此方法被调用，只有在状态为CBCentralManagerStatePoweredOn时发出，此状态意味着扫描已经停止或者任何连接的外围设备已经断开连接。如果状态值为CBCentralManagerStatePoweredOff，则Manager中所有的CBPeripheral对象都无效了，必须重新搜索或发现*/
{
    
}

/*如果周围蓝牙有多个，则这个方法会调用多次*/
- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary *)advertisementData RSSI:(NSNumber *)RSSI
{
    
}

//一个中心设备可以同时连接多个周围的蓝牙设备
//当连接上某个蓝牙之后，CBCentralManager会通知代理处理
- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral
{
    
    
    /*因为在后面我们要从外设蓝牙那边再获取一些信息，并与之通讯，这些过程会有一些事件可能要处理，所以要给这个外设设置代理，比如：
     
     peripheral.delegate = self;*/
}

#pragma mark - CBPeripheralDelegate
/*!
 *  @method peripheral:didDiscoverServices:
 *
 *  @param peripheral	The peripheral providing this information.
 *	@param error		If an error occurred, the cause of the failure.
 *
 *  @discussion			This method returns the result of a discoverServices: call. If the service(s) were read successfully, they can be retrieved via peripheral's  services property.
 *
 */
- (void)peripheral:(CBPeripheral *)peripheral didDiscoverServices:(NSError *)error//返回的蓝牙服务通知通过代理实现
{
    for (CBService* service in peripheral.services){
        
    }
}

/*!
 *  @param peripheral	The peripheral providing this information.
 *  @param service		The CBService object containing the characteristic(s).
 *	@param error		If an error occurred, the cause of the failure.
 *
 *  @discussion			This method returns the result of a  discoverCharacteristics:forService:  call. If the characteristic(s) were read successfully, they can be retrieved via service's characteristics property.
 */
- (void)peripheral:(CBPeripheral *)peripheral didDiscoverCharacteristicsForService:(CBService *)service error:(NSError *)error//返回的蓝牙特征值通知通过代理实现
{
    for (CBCharacteristic * characteristic in service.characteristics) {
        
    }
}

/*!
 *  @param peripheral		The peripheral providing this information.
 *  @param characteristic	A <code>CBCharacteristic</code> object.
 *	@param error			If an error occurred, the cause of the failure.
 *
 *  @discussion				This method returns the result of a  writeValue:forCharacteristic:type: call, when the CBCharacteristicWriteWithResponse type is used.
 */
- (void)peripheral:(CBPeripheral *)peripheral didWriteValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error//写数据后处理
{
    
}


//处理蓝牙发过来的数据  数据接收
- (void)peripheral:(CBPeripheral *)peripheral didUpdateValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error
{
    
}

#pragma mark - CBPeripheralManagerDelegate
- (void)peripheralManagerDidUpdateState:(CBPeripheralManager *)peripheral
{
    
}


- (void)dealloc
{
    [connectedDevice release];
    [nearDeviceArr release];
    [super dealloc];
}

@end



/*NSString *  CBCentralManagerOptionShowPowerAlertKey    //如果CentralManager实例化了，展示一个alert，提示需要的系统version*/


/*NSString * const CBCentralManagerOptionRestoreIdentifierKey  //CBCentralManager实例化后，保存CBCentralManager实例的一个唯一标识，此标志是系统用来标识一个特定的Manager实例，用来维护，因此，为了manager能被维护，必须在随后应用程序的执行中保持一致。

/*CB_EXTERN NSString * const CBCentralManagerScanOptionAllowDuplicatesKey; //指明扫描时不用进行重复过滤    通常情况下，同一设备多次发现会被合并为一个发现事件中，指定此选项，则在每次该设备可见的时候都生成事件，有可能一秒钟有好几次。这在一些特定的情况下是有用的，比如，通过一个设备的rssi来连接的时候，但是也有不利的一面：影响电池寿命、影响应用的性能   */


/*CB_EXTERN NSString * const CBCentralManagerScanOptionSolicitedServiceUUIDsKey NS_AVAILABLE(NA, 7_0);       //请求服务的UUID键值   致使扫描也会查找服务列表中包含的设备*/


/*CB_EXTERN NSString * const CBConnectPeripheralOptionNotifyOnConnectionKey NS_AVAILABLE(NA, 6_0);        //A NSNumber (Boolean) indicating that the system should display a connection alert for a given peripheral, if the application is suspended, when a successful connection is made.This is useful for applications that have not specified the bluetooth-central background mode and cannot display their own alert. If more than one application has requested notification for a given peripheral, the one that was most recently in the foreground will receive the alert.*/

/*
CB_EXTERN NSString * const CBConnectPeripheralOptionNotifyOnDisconnectionKey;       //A NSNumber (Boolean) indicating that the system should display a disconnection alert for a given peripheral, if the application is suspended at the time of the disconnection. This is useful for applications that have not specified the <code>bluetooth-central</code> background mode and cannot display their own alert. If more than one application has requested notification for a given peripheral, the one that was most recently in the foreground will receive the alert. */

/*
 CB_EXTERN NSString * const CBConnectPeripheralOptionNotifyOnNotificationKey NS_AVAILABLE(NA, 6_0);         //A NSNumber (Boolean) indicating that the system should display an alert for all notifications received from a given peripheral, if the application is suspended at the time.   This is useful for applications that have not specified the bluetooth-central background mode and cannot display their  own alert. If more than one application has requested notification for a given peripheral, the one that was most recently in the foreground   will receive the alert.
*/

/*
 CB_EXTERN NSString * const CBCentralManagerRestoredStatePeripheralsKey NS_AVAILABLE(NA, 7_0);              //An NSArray of CBPeripheral objects containing all peripherals that were connected or pending connection at the time the	application was terminated by the system. When possible, all known information for each peripheral will be restored, including any discovered services, characteristics and descriptors, as well as characteristic notification states.*/

/*
CB_EXTERN NSString * const CBCentralManagerRestoredStateScanServicesKey NS_AVAILABLE(NA, 7_0);            //An NSArray of CBUUID objects containing the service(s) being scanned for at the time the application was terminated by the system. */

/*
CB_EXTERN NSString * const CBCentralManagerRestoredStateScanOptionsKey NS_AVAILABLE(NA, 7_0);           //A NSDictionary containing the scan options at the time the application was terminated by the system.*/

