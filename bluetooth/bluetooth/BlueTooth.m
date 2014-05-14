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
    NSDictionary * dic = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:NO], CBCentralManagerScanOptionAllowDuplicatesKey, nil];
}

- (void)stopBlueToothSearch
{
    
}

+ (NSString *)version
{
    return VERSION;
}

#pragma mark - CBCentralManagerDelegate  CBCentralManager对象的代理必须遵守CBCentralManagerDelegate协议。唯一的必须实现的方法表明CentralManager的能力，可选方法允许发现和连接外设。
- (void)centralManagerDidUpdateState:(CBCentralManager *)central/*当CentralManager的状态变化，此方法被调用，只有在状态为CBCentralManagerStatePoweredOn时发出，此状态意味着扫描已经停止或者任何连接的外围设备已经断开连接。如果状态值为CBCentralManagerStatePoweredOff，则Manager中所有的CBPeripheral对象都无效了，必须重新搜索或发现*/
{
    
}
#pragma mark - CBPeripheralDelegate

#pragma mark - CBPeripheralManagerDelegate



- (void)dealloc
{
    [connectedDevice release];
    [nearDeviceArr release];
    [super dealloc];
}

@end



/*NSString *  CBCentralManagerOptionShowPowerAlertKey    //如果CentralManager实例化了，展示一个alert，提示需要的系统version*/

/*!
 *  @const  CBCentralManagerOptionRestoreIdentifierKey
 *
 *  @discussion A NSString containing a unique identifier (UID) for the <code>CBCentralManager</code> that is being instantiated. This UID is used
 *				by the system to identify a specific <code>CBCentralManager</code> instance for restoration and, therefore, must remain the same for
 *				subsequent application executions in order for the manager to be restored.
 *
 *  @see		initWithDelegate:queue:options:
 *  @seealso	centralManager:willRestoreState:
 *
 */
/*NSString * const CBCentralManagerOptionRestoreIdentifierKey  //如果CentralManager实例化了

/*!
 *  @const CBCentralManagerScanOptionAllowDuplicatesKey
 *
 *  @discussion A NSNumber (Boolean) indicating that the scan should run without duplicate filtering. By default, multiple discoveries of the
 *              same peripheral are coalesced into a single discovery event. Specifying this option will cause a discovery event to be generated
 *				every time the peripheral is seen, which may be many times per second. This can be useful in specific situations, such as making
 *				a connection based on a peripheral's RSSI, but may have an adverse affect on battery-life and application performance.
 *
 *  @see        scanForPeripheralsWithServices:options:
 *
 */
CB_EXTERN NSString * const CBCentralManagerScanOptionAllowDuplicatesKey;

/*!
 *  @const CBCentralManagerScanOptionSolicitedServiceUUIDsKey
 *
 *  @discussion An NSArray of <code>CBUUID</code> objects respresenting service UUIDs. Causes the scan to also look for peripherals soliciting
 *				any of the services contained in the list.
 *
 *  @see        scanForPeripheralsWithServices:options:
 *
 */
CB_EXTERN NSString * const CBCentralManagerScanOptionSolicitedServiceUUIDsKey NS_AVAILABLE(NA, 7_0);

/*!
 *  @const CBConnectPeripheralOptionNotifyOnConnectionKey
 *
 *  @discussion A NSNumber (Boolean) indicating that the system should display a connection alert for a given peripheral, if the application is suspended
 *              when a successful connection is made.
 *              This is useful for applications that have not specified the <code>bluetooth-central</code> background mode and cannot display their
 *              own alert. If more than one application has requested notification for a given peripheral, the one that was most recently in the foreground
 *              will receive the alert.
 *
 *  @see        connectPeripheral:
 *
 */
CB_EXTERN NSString * const CBConnectPeripheralOptionNotifyOnConnectionKey NS_AVAILABLE(NA, 6_0);

/*!
 *  @const CBConnectPeripheralOptionNotifyOnDisconnectionKey
 *
 *  @discussion A NSNumber (Boolean) indicating that the system should display a disconnection alert for a given peripheral, if the application
 *              is suspended at the time of the disconnection.
 *              This is useful for applications that have not specified the <code>bluetooth-central</code> background mode and cannot display their
 *              own alert. If more than one application has requested notification for a given peripheral, the one that was most recently in the foreground
 *              will receive the alert.
 *
 *  @see        connectPeripheral:
 *
 */
CB_EXTERN NSString * const CBConnectPeripheralOptionNotifyOnDisconnectionKey;

/*!
 *  @const CBConnectPeripheralOptionNotifyOnNotificationKey
 *
 *  @discussion A NSNumber (Boolean) indicating that the system should display an alert for all notifications received from a given peripheral, if
 *              the application is suspended at the time.
 *              This is useful for applications that have not specified the <code>bluetooth-central</code> background mode and cannot display their
 *              own alert. If more than one application has requested notification for a given peripheral, the one that was most recently in the foreground
 *              will receive the alert.
 *
 *  @see        connectPeripheral:
 *
 */
CB_EXTERN NSString * const CBConnectPeripheralOptionNotifyOnNotificationKey NS_AVAILABLE(NA, 6_0);

/*!
 *  @const  CBCentralManagerRestoredStatePeripheralsKey
 *
 *  @discussion An NSArray of <code>CBPeripheral</code> objects containing all peripherals that were connected or pending connection at the time the
 *				application was terminated by the system. When possible, all known information for each peripheral will be restored, including any discovered
 *				services, characteristics and descriptors, as well as characteristic notification states.
 *
 *  @see		centralManager:willRestoreState:
 *	@seealso	connectPeripheral:options:
 *
 */
CB_EXTERN NSString * const CBCentralManagerRestoredStatePeripheralsKey NS_AVAILABLE(NA, 7_0);

/*!
 *  @const  CBCentralManagerRestoredStateScanServicesKey
 *
 *  @discussion An NSArray of <code>CBUUID</code> objects containing the service(s) being scanned for at the time the application was terminated by the system.
 *
 *  @see		centralManager:willRestoreState:
 *	@seealso	scanForPeripheralsWithServices:options:
 *
 */
CB_EXTERN NSString * const CBCentralManagerRestoredStateScanServicesKey NS_AVAILABLE(NA, 7_0);

/*!
 *  @const  CBCentralManagerRestoredStateScanOptionsKey
 *
 *  @discussion A NSDictionary containing the scan options at the time the application was terminated by the system.
 *
 *  @see		centralManager:willRestoreState:
 *	@seealso	scanForPeripheralsWithServices:options:
 *
 */
CB_EXTERN NSString * const CBCentralManagerRestoredStateScanOptionsKey NS_AVAILABLE(NA, 7_0);

