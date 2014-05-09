//
//  SKPSMTPMessage.h
//
//  Created by Ian Baird on 10/28/08.
//
//  Copyright (c) 2008 Skorpiostech, Inc. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person
//  obtaining a copy of this software and associated documentation
//  files (the "Software"), to deal in the Software without
//  restriction, including without limitation the rights to use,
//  copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following
//  conditions:
//
//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//  OTHER DEALINGS IN THE SOFTWARE.
//

#import <UIKit/UIKit.h>
#import <CFNetwork/CFNetwork.h>

//#define DEBUGGING_LOGS
//#undef DEBUGGING_LOGS

#ifdef DEBUGGING_LOGS
#define DEBUGLOG NSLog
#else
#define DEBUGLOG DummyLog
#endif
void DummyLog(NSString * format, ...);

enum 
{
    kSKPSMTPIdle = 0,
    kSKPSMTPConnecting = 1,
    kSKPSMTPWaitingEHLOReply = 2,
    kSKPSMTPWaitingTLSReply = 3,
    kSKPSMTPWaitingLOGINUsernameReply = 4,
    kSKPSMTPWaitingLOGINPasswordReply = 5,
    kSKPSMTPWaitingAuthSuccess = 6,
    kSKPSMTPWaitingFromReply = 7,
    kSKPSMTPWaitingToReply = 8,
    kSKPSMTPWaitingForEnterMail = 9,
    kSKPSMTPWaitingSendSuccess = 10,
    kSKPSMTPWaitingQuitReply = 11,
    kSKPSMTPMessageSent = 12
};
typedef NSUInteger SKPSMTPState;
    
// Message part keys
extern NSString *kSKPSMTPPartContentDispositionKey;
extern NSString *kSKPSMTPPartContentTypeKey;
extern NSString *kSKPSMTPPartContentTypeCharsetKey;  //6T

extern NSString *kSKPSMTPPartMessageKey;
extern NSString *kSKPSMTPPartContentTransferEncodingKey;

// Error message codes
#define kSKPSMPTErrorConnectionTimeout -5
#define kSKPSMTPErrorConnectionFailed -3
#define kSKPSMTPErrorConnectionInterrupted -4
#define kSKPSMTPErrorUnsupportedLogin -2
#define kSKPSMTPErrorTLSFail -1
#define kSKPSMTPErrorInvalidUserPass 535
#define kSKPSMTPErrorInvalidMessage 550
#define kSKPSMTPErrorNoRelay 530

@class SKPSMTPMessage;

@protocol SKPSMTPMessageDelegate
@required

- (void)messageSent:(SKPSMTPMessage *)message;
- (void)messageFailed:(SKPSMTPMessage *)message error:(NSError *)error;
@optional
- (void)messageState:(SKPSMTPState)messageState;

@end

@interface SKPSMTPMessage : NSObject <NSCopying>
{
    NSString *login;
    NSString *pass;
    NSString *relayHost;
    NSArray *relayPorts;
    
    NSString *subject;
    NSString *fromEmail;
    NSString *toEmail;
    NSString *replyToEmail;
	NSString *ccEmail;
	NSString *bccEmail;
    NSArray *parts;
    
    NSOutputStream *outputStream;
    NSInputStream *inputStream;
    
    BOOL requiresAuth;
    BOOL wantsSecure;
    BOOL validateSSLChain;
    
    SKPSMTPState sendState;
    BOOL isSecure;
    NSMutableString *inputString;
    
    // Auth support flags
    BOOL serverAuthCRAMMD5;
    BOOL serverAuthPLAIN;
    BOOL serverAuthLOGIN;
    BOOL serverAuthDIGESTMD5;
    
    // Content support flags
    BOOL server8bitMessages;
    
    NSObject <SKPSMTPMessageDelegate> *delegate;
    
    NSTimeInterval connectTimeout;
    
    NSTimer *connectTimer;
    NSTimer *watchdogTimer;
}

@property(nonatomic, retain) NSString *login;
@property(nonatomic, retain) NSString *pass;
@property(nonatomic, retain) NSString *relayHost;

@property(nonatomic, retain) NSArray *relayPorts;
@property(nonatomic, assign) BOOL requiresAuth;
@property(nonatomic, assign) BOOL wantsSecure;
@property(nonatomic, assign) BOOL validateSSLChain;

@property(nonatomic, retain) NSString *subject;
@property(nonatomic, retain) NSString *fromEmail;
@property(nonatomic, retain) NSString *toEmail;
@property(nonatomic, retain) NSString *replyToEmail;
@property(nonatomic, retain) NSString *ccEmail;
@property(nonatomic, retain) NSString *bccEmail;
@property(nonatomic, retain) NSArray *parts;

@property(nonatomic, assign) NSTimeInterval connectTimeout;

@property(nonatomic, assign) NSObject <SKPSMTPMessageDelegate> *delegate;

- (BOOL)send;

@end