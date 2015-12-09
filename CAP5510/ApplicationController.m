//
//  ApplicationController.m
//  CAP5510
//
//  Created by Abhishek Banthia on 11/14/15.
//  Copyright © 2015 Abhishek Banthia. All rights reserved.
//

#import "ApplicationController.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "InputDefs.h"
#include "DNACommonFuncs.h"
#include "BitFuncs.h"
#include "SortFuncs.h"
#include "UtilityFuncs.h"
#include "PMSPrune.h"

@implementation ApplicationController

-(void)awakeFromNib
{
    self.outputString = [[NSMutableString alloc] init];
    self.calculateButtonTitle = @"Calculate";
    self.activityInProgress.hidden = YES;
}

//global input parameters
int motifLen = 11;
int hammingDist = 3;
CInputStringSet inputStrs;
CCompactMotif foundMotifs[CONST_MAX_NUM_FOUND_MOTIFS_ALLOWED];
int numFoundMotifs = 0;

int VerifyMotifs( int moLen, int hmDist, const CInputStringSet * inptStrs,
                 const CCompactMotif * foMotifs, int nFoMotifs) {
    int i;
    int nCorrectMotifs = 0;
    char aMotif[CONST_MAX_MOTIF_STRING_LENGTH + 4];
    for (i = 0; i < nFoMotifs; i++) {
        DecodeDNAString((char *)&foMotifs[i], moLen/4 + 1, aMotif);
        //fprintf(stdout, "\nTest: ");
        //PrintDNAString(aMotif, moLen, 1, 0, stdout);
        if (IsMotifInputStrSet(aMotif, moLen, hmDist, inptStrs, 0, inptStrs->m_num - 1)) {
            //if (IsMotifInputStrSetWithBindingSite(aMotif, moLen, hmDist, inptStrs, 0, inptStrs->m_num - 1)) {
            nCorrectMotifs++;
        } else {
            
        }
    }
    return nCorrectMotifs;
}


void PrintMotitsToFile(const char * fName, int moLen, CCompactMotif * foMotifs, int nMotifs) {
    int i;
    char aMotif[CONST_MAX_MOTIF_STRING_LENGTH];
    FILE * f;
    f = fopen(fName, "w+");
    if (f == NULL) {
        fprintf(stdout, "\nUnable to create output file %s", fName);
        return;
    }
    //fprintf(stdout, "#Motifs=%d\n", nMotifs);
    for (i = 0; i < nMotifs; i++) {
        DecodeDNAString(foMotifs + i, moLen/4 + 1, aMotif);
        PrintDNAString(aMotif, moLen, 1, 0, f);
        fprintf(f, "\n");
    }
    

    //fprintf(stdout, "#Motifs=%d", nMotifs);
    fclose(f);	//fclose closes the stream
}

void action(const char *fileName, int hammingDistance, int motifLength, id param) {
    
    dispatch_sync(dispatch_get_main_queue(), ^{
         [param updateProgressIndicator];
    });

    
    int t1, t2;
    int i, j;
    char iFileName[500];
    char oFileName[500];
    
    ComputeDNAByteTable();
    ComputeDNAByteTableInverse();
    
    motifLen = 13;
    hammingDist = 4;
    sprintf(iFileName, "TestingInput\\input_l%d_d%d_0.txt", motifLen, hammingDist);
    sprintf(oFileName, "output_l%d_d%d_0.txt", motifLen, hammingDist);
    
    //	if (argc < 5) {
    //		fprintf(stdout, "\nWrong parameters.\nCommand line: \nPMSPrune.exe [input file] [output file] [motif length] [d]");
    //		return;
    //	} else {
    //		strcpy(iFileName, "/Users/abhishekbanthia/Documents/XCode projects/CAP5510/CAP5510/input_example.txt");
    //		strcpy(oFileName, "/Users/abhishekbanthia/Documents/XCode projects/CAP5510/CAP5510/Abhishek.txt");
    //		motifLen = atoi(argv[3]);
    //		hammingDist = atoi(argv[4]);
    //	}
    
    strcpy(iFileName, fileName);
    strcpy(oFileName, "/Users/abhishekbanthia/Documents/XCode projects/CAP5510/CAP5510/SampleAbhishek.txt");
    motifLen = motifLength; //should be dynamically asking for motif length. View original source for reference
    hammingDist = hammingDistance; // should be dynamically asking for hamming distance. View original code for reference
    
    printf("File name:%s", iFileName);
    
    ReadInputFile(iFileName, &inputStrs); //passes file name and gets a pointer to sorted input string
    
    fprintf(stdout, "\n\nInput Strings: %d strings l=%d d=%d", inputStrs.m_num, motifLen, hammingDist);
    for (i = 0; i < inputStrs.m_num; i++) {
        fprintf(stdout, "\n\n%d\n", i + 1);
        PrintInputString(&inputStrs.m_str[i], 0, stdout);
    }
    
    t1 = time(NULL);
    numFoundMotifs = PMSPrune(motifLen, hammingDist, &inputStrs, foundMotifs, CONST_MAX_NUM_FOUND_MOTIFS_ALLOWED);
    t2 = time(NULL);
    fprintf(stdout, "\nTime=%d seconds", t2 - t1);
    
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0ul);
    dispatch_async(queue, ^{
        PrintMotitsToFile("/Users/abhishekbanthia/Documents/XCode projects/CAP5510/CAP5510/SampleAbhishek.txt", motifLen, foundMotifs, numFoundMotifs);
        
        dispatch_sync(dispatch_get_main_queue(), ^{
            [param stopProgressIndicator];
        });
    });
    
   
    
    return;
}


- (void)updateProgressIndicator
{
    self.activityInProgress.hidden = NO;
    [self.activityInProgress startAnimation:nil];
    
    for (NSView *view in self.mainWindow.contentView.subviews) {
        if (view != self.activityInProgress) {
            view.alphaValue = 0.2;
            if ([view isKindOfClass:[NSTextField class]]) {
                NSTextField *textfield = (NSTextField *)view;
                textfield.enabled = NO;
            }
        }
    }
}

- (void)stopProgressIndicator
{
    self.activityInProgress.hidden = YES;
    [self.activityInProgress stopAnimation:nil];
    
    for (NSView *view in self.mainWindow.contentView.subviews) {
        if (view != self.activityInProgress) {
            view.alphaValue = 1;
            if ([view isKindOfClass:[NSTextField class]]) {
                NSTextField *textfield = (NSTextField *)view;
                textfield.enabled = YES;
            }

        }
    }
    
    self.calculateButtonTitle = @"Print";
}

-(void)printContent:(id)sender
{
    NSSize screenSize = [[self.mainWindow screen] frame].size;
    NSRect frame = self.mainWindow.frame;
    
    if(frame.size.height > self.mainWindow.minSize.height)
        frame.size.height = self.mainWindow.minSize.height;
    else
        frame.size.height = 461;
    
    [self.mainWindow setFrame:frame display:YES animate:YES];

    NSError *error;
    
    NSString *stringFromFileAtURL = [[NSString alloc]
                                     
                                     initWithContentsOfURL:[NSURL URLWithString:@"file:///Users/abhishekbanthia/Documents/XCode%20projects/CAP5510/CAP5510/SampleAbhishek.txt"]
                                     
                                     encoding:NSUTF8StringEncoding
                                     
                                     error:&error];
    
    if (stringFromFileAtURL == nil) {
        //Error
        NSLog(@"File is empty");
        self.outputTextView.string = @"Output File is Empty";
    }
    else
    {
        self.outputTextView.string = stringFromFileAtURL;
    }
}

-(void)calculate:(id)sender
{
    NSButton *buttonTitle = (NSButton *)sender;
    
    if ([buttonTitle.title isEqualToString:@"Print"])
    {
        [self printContent:sender];
        return;
    }
    
//    action("/Users/abhishekbanthia/Documents/XCode projects/CAP5510/CAP5510/input_example.txt");
    
    if (self.hammingDistance.stringValue.length && self.motifLength.stringValue.length && self.inputPath.stringValue.length)
    {
       
        dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0ul);
        dispatch_async(queue, ^{
             action([self.inputPath.stringValue UTF8String], self.hammingDistance.intValue, self.motifLength.intValue, self);
        });

    }
    else
    {
        NSLog(@"Please enter the motif length/Hamming Distance");
    }
}

-(void)openFile:(id)sender
{
    NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    
    openDlg.allowedFileTypes = @[@"txt", @"TXT"];
    
    // Enable the selection of files in the dialog.
    [openDlg setCanChooseFiles:YES];
    
    // Multiple files not allowed
    [openDlg setAllowsMultipleSelection:NO];
    
    // Can't select a directory
    [openDlg setCanChooseDirectories:NO];
    
    if ( [openDlg runModal] == NSModalResponseOK )
    {
        
        for(NSURL *urlString in openDlg.URLs)
        {
            NSString *absoluteString = [urlString absoluteString];
            NSString *formattedString = [[absoluteString substringFromIndex:7] stringByRemovingPercentEncoding];
            //            NSLog(@"Absolute String:%@", formattedString);
//
            self.inputPath.stringValue = formattedString;
        }
    }
}

-(void)saveFile:(id)sender
{
    // create the save panel
    NSSavePanel *panel = [NSSavePanel savePanel];
    
    panel.allowedFileTypes = @[@"txt", @"TXT"];
    
    // set a new file name
    [panel setNameFieldStringValue:@"NewFile.txt"];
    
    // display the panel
    [panel beginWithCompletionHandler:^(NSInteger result) {
        
        if (result == NSFileHandlingPanelOKButton) {
            
            // create a file manager and grab the save panel's returned URL
            NSFileManager *manager = [NSFileManager defaultManager];
            NSURL *saveURL = [panel URL];
            
            self.savePath.stringValue = [saveURL.absoluteString stringByRemovingPercentEncoding];
            
            // then copy a previous file to the new location
//            [manager copyItemAtURL:self.myURL toURL:saveURL error:nil];
        }
    }];
    
}

@end
