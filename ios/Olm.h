#import <React/RCTBridgeModule.h>
#import "jsi_adapter.h"

@interface Olm : NSObject <RCTBridgeModule>

@property(nonatomic, assign) BOOL setBridgeOnMainQueue;

@end
