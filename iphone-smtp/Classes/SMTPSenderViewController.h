#import <UIKit/UIKit.h>

#import <CFNetwork/CFNetwork.h>
#import "SKPSMTPMessage.h"
#import "NSData+Base64Additions.h"

#define FROM_EMAIL_PREF_KEY @"kFromEmailPreferenceKey"
#define TO_EMAIL_PREF_KEY @"kToEmailPreferenceKey"
#define RELAY_HOST_PREF_KEY @"kRelayHostPreferenceKey"
#define USE_SSL_BOOL_PREF_KEY @"kUseSSLBoolPreferenceKey"
#define USE_AUTH_BOOL_PREF_KEY @"kUseAuthBoolPreferenceKey"
#define AUTH_USERNAME_PREF_KEY @"kAuthUsernamePreferenceKey"
#define AUTH_PASSWORD_PREF_KEY @"kAuthPasswordPreferenceKey"
#define MESSAGE_SUBJECT_PREF_KEY @"kMessageSubjectPreferenceKey"
#define MESSAGE_BODY_PREF_KEY @"kMessageBodyPreferenceKey"
#define MESSAGE_SIG_PREF_KEY @"kMessageSigPreferenceKey"
#define SEND_IMAGE_BOOL_PREF_KEY @"kSendImageBoolPreferenceKey"
#define SEND_VCARD_BOOL_PREF_KEY @"kSendVcardBoolPreferenceKey"



@interface SMTPSenderViewController : UIViewController <SKPSMTPMessageDelegate>
{
    IBOutlet UITextField *fromEmail;
    IBOutlet UITextField *toEmail;
    IBOutlet UITextField *relayHost;
    IBOutlet UISwitch *SSLSwitch;
    
    IBOutlet UISwitch *useAuthSwitch;
    IBOutlet UITextField *login;
    IBOutlet UITextField *password;

    IBOutlet UITextField *subject;
    IBOutlet UITextField *messageBody;
    IBOutlet UITextField *sig;
    IBOutlet UISwitch *sendImageSwitch;
    IBOutlet UISwitch *sendVCFSwitch;
    
    IBOutlet UIActivityIndicatorView *Spinner;
    IBOutlet UIProgressView *ProgressBar;
    SKPSMTPState HighestState;
    
    NSDictionary *prefKeyDictionary;
}

@property (nonatomic, retain) UITextField *fromEmail;
@property (nonatomic, retain) UITextField *toEmail;
@property (nonatomic, retain) UITextField *relayHost;
@property (nonatomic, retain) UISwitch *SSLSwitch;

@property (nonatomic, retain) UISwitch *useAuthSwitch;
@property (nonatomic, retain) UITextField *login;
@property (nonatomic, retain) UITextField *password;

@property (nonatomic, retain) UITextField *subject;
@property (nonatomic, retain) UITextField *messageBody;
@property (nonatomic, retain) UITextField *sig;
@property (nonatomic, retain) UISwitch *sendImageSwitch;
@property (nonatomic, retain) UISwitch *sendVCFSwitch;

@property (nonatomic, retain) UIActivityIndicatorView *Spinner;
@property (nonatomic, retain) UIProgressView *ProgressBar;
@property (nonatomic, assign) SKPSMTPState HighestState;

@property (nonatomic, retain) NSDictionary *prefKeyDictionary;



- (IBAction)sendMail:(id)sender;
- (IBAction)switchSSL:(UISwitch *)sender;
- (IBAction)switchSendImage:(UISwitch *)sender;
- (IBAction)switchSendVCF:(UISwitch *)sender;
- (IBAction)switchUseAuth:(UISwitch *)sender;
@end
