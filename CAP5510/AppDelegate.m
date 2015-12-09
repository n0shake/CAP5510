//
//  AppDelegate.m
//  CAP5510
//
//  Created by Abhishek Banthia on 11/14/15.
//  Copyright © 2015 Abhishek Banthia. All rights reserved.
//

#import "AppDelegate.h"

@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    [self setDefaultHeight];
}

- (void) setDefaultHeight
{
    NSRect frame = self.window.frame;
    
    if(frame.size.height > self.window.minSize.height)
        frame.size.height = self.window.minSize.height;
    
    [self.window setFrame:frame display:YES animate:NO];

}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

@end
