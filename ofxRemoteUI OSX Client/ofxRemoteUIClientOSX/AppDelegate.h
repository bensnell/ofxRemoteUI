//
//  AppDelegate.h.h
//  ofxRemoteUIClientOSX
//
//  Created by Oriol Ferrer Mesia on 8/28/11.
//  Copyright 2011 uri.cat. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "ofxRemoteUIClient.h"
#import "Item.h"
#import "MyScrollView.h"

#define REFRESH_RATE			1.0f/15.0f
#define STATUS_REFRESH_RATE		0.333f
#define ROW_HEIGHT				34.0f
#define ROW_WIDTH				350.0f

@interface AppDelegate : NSObject <NSApplicationDelegate>{

	IBOutlet NSWindow *window;
	IBOutlet NSButton *updateFromServerButton;
	IBOutlet NSButton *updateContinuouslyCheckbox;
	IBOutlet NSButton *connectButton;
	IBOutlet NSSearchField *addressField; //NSSearchField
	IBOutlet NSTextField *portField;
	IBOutlet NSImageView *statusImage;
	IBOutlet NSProgressIndicator *progress;
	IBOutlet NSTextField *lagField;
	IBOutlet NSView *listContainer;
	IBOutlet MyScrollView * scroll;

	bool updateContinuosly;

	map<string, Item*> widgets;
	vector<string> orderedKeys; // used to keep the order in which the items were added

	ofxRemoteUIClient * client;
	NSTimer * timer;
	NSTimer * statusTimer;
	NSPoint lastLayout;

	BOOL waitingForResults;
}


-(IBAction)pressedSync:(id)sender;
-(IBAction)pressedContinuously:(id)sender;
-(IBAction)pressedConnect:(id)sender;
-(void) connect;

-(void)setup;
-(void)update;

-(BOOL)fullParamsUpdate;
-(void)partialParamsUpdate;

-(void)userChangedParam:(RemoteUIParam)p paramName:(string)name; //this is a delegate method, items will call this on widgetChange

@end