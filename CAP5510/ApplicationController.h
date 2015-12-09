//
//  ApplicationController.h
//  CAP5510
//
//  Created by Abhishek Banthia on 11/14/15.
//  Copyright © 2015 Abhishek Banthia. All rights reserved.
//

#import <Foundation/Foundation.h>
@import AppKit;

@interface ApplicationController : NSObject

- (IBAction)calculate:(id)sender;
- (IBAction)openFile:(id)sender;
- (IBAction)saveFile:(id)sender;
- (IBAction)printContent:(id)sender;

@property (weak) IBOutlet NSTextField *inputPath;
@property (weak) IBOutlet NSTextField *hammingDistance;
@property (weak) IBOutlet NSTextField *motifLength;
@property (weak) IBOutlet NSTextField *savePath;
@property (weak) IBOutlet NSProgressIndicator *activityInProgress;
@property (strong, nonatomic) IBOutlet NSTextView *outputTextView;
@property (strong, nonatomic)  NSMutableString *outputString;
@property (weak, nonatomic) IBOutlet NSWindow *mainWindow;
@property (strong, nonatomic) NSString *calculateButtonTitle;
@end
