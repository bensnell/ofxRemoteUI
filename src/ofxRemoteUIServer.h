//
//  ofxRemoteUI.h
//  emptyExample
//
//  Created by Oriol Ferrer Mesià on 09/01/13.
//
//

#ifndef _ofxRemoteUIServer__
#define _ofxRemoteUIServer__

// you will need to add this to your "Header Search Path" for ofxOsc to compile
// ../../../addons/ofxOsc/libs ../../../addons/ofxOsc/libs/oscpack ../../../addons/ofxOsc/libs/oscpack/src ../../../addons/ofxOsc/libs/oscpack/src/ip ../../../addons/ofxOsc/libs/oscpack/src/ip/posix ../../../addons/ofxOsc/libs/oscpack/src/ip/win32 ../../../addons/ofxOsc/libs/oscpack/src/osc ../../../addons/ofxOsc/src
#include "ofxOsc.h"
#include "ofxRemoteUI.h"
#include "ofxXmlSettings.h"
#include <map>
#include <set>
#include <vector>
#include "ofxRemoteUISimpleNotifications.h"

// ################################################################################################
// ## EASY ACCES MACROS ## use these instead of direct calls
// ################################################################################################

//use this macro to share floats, ints, bools
#define OFX_REMOTEUI_SERVER_SHARE_PARAM(val,...)									( ofxRemoteUIServer::instance()->shareParam( #val, &val, ##__VA_ARGS__ ) )

//use this macro to share enums + enumList
#define OFX_REMOTEUI_SERVER_SHARE_ENUM_PARAM(val,enumMin,enumMax,menuList,...)		( ofxRemoteUIServer::instance()->shareParam( #val, (int*)&val,enumMin, enumMax,menuList, ##__VA_ARGS__ ) )

//use this macro to share ofColors
#define OFX_REMOTEUI_SERVER_SHARE_COLOR_PARAM(color,...)							( ofxRemoteUIServer::instance()->shareParam( #color, (unsigned char*)&color.v[0], ##__VA_ARGS__ ) )

/*set a specific color for the upcoming params */
#define OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR(c)								( ofxRemoteUIServer::instance()->setParamColor( c ) )

/*set a new group for the upcoming params*/
#define OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_GROUP(g)								( ofxRemoteUIServer::instance()->setParamGroup( g ) )

/*set a new 'random' color upcoming params*/
#define OFX_REMOTEUI_SERVER_SET_NEW_COLOR()											( ofxRemoteUIServer::instance()->setNewParamColor(1) )

/*set a new color with a custom level of difference from the prev color*/
#define OFX_REMOTEUI_SERVER_SET_NEW_COLOR_N(num)									( ofxRemoteUIServer::instance()->setNewParamColor(num) )

/*setup the server, set a specific port if you must. otherwise a random one will be chosen 
 the first time, and it will be reused for successive launches */
#define OFX_REMOTEUI_SERVER_SETUP(port, ...)										( ofxRemoteUIServer::instance()->setup(port, ##__VA_ARGS__) )

/*update the server. no need to call this from OF*/
#define OFX_REMOTEUI_SERVER_UPDATE(deltaTime)										( ofxRemoteUIServer::instance()->update(deltaTime) )

/*draw the server msgs. no need to call this from OF
 only call this if you disabled automatic notifications and still 
 want to see them in a custom location*/
#define OFX_REMOTEUI_SERVER_DRAW(x,y)												( ofxRemoteUIServer::instance()->draw(x,y) )

/*close the server. no need to call this from OF*/
#define OFX_REMOTEUI_SERVER_CLOSE()													( ofxRemoteUIServer::instance()->close() )

/*save current param status to default xml.
  No need to call this on app quit in OF, happensautomatically */
#define	OFX_REMOTEUI_SERVER_SAVE_TO_XML()											( ofxRemoteUIServer::instance()->saveToXML(OFXREMOTEUI_SETTINGS_FILENAME) )

/*loads last saved default xml params into your variables */
#define	OFX_REMOTEUI_SERVER_LOAD_FROM_XML()											( ofxRemoteUIServer::instance()->loadFromXML(OFXREMOTEUI_SETTINGS_FILENAME) )

/*set if saves to XML automatically on app exit. Default is YES in OF*/
#define	OFX_REMOTEUI_SERVER_SET_SAVES_ON_EXIT(save)									( ofxRemoteUIServer::instance()->setSaveToXMLOnExit(save) )

/*in OF, auto draws on screen imprtant events and param updates. defaults to YES in OF*/
#define	OFX_REMOTEUI_SERVER_SET_DRAWS_NOTIF(draw)									( ofxRemoteUIServer::instance()->setDrawsNotificationsAutomaticallly(draw) )

/*sends all params to client, same as pressing sync on client
 updates client UI to match current param values. use this if you modify 
 params internally and want those changes reflected in the UI*/
#define OFX_REMOTEUI_SERVER_PUSH_TO_CLIENT()										( ofxRemoteUIServer::instance()->pushParamsToClient() )

/*get a pointer to the server*/
#define OFX_REMOTEUI_SERVER_GET_INSTANCE()											( ofxRemoteUIServer::instance() )

#ifdef OF_AVAILABLE
/*run the server on a back thread. Useful for apps with very low framerate.
 default is disabled in OF; only works in OF! */
#define OFX_REMOTEUI_SERVER_START_THREADED()										( ofxRemoteUIServer::instance()->startInBackgroundThread() )

#endif



class ofxRemoteUIServer: public ofxRemoteUI
#ifdef OF_AVAILABLE
, ofThread
#endif
{

public:

	static ofxRemoteUIServer* instance();

	void setup(int port = -1, float updateInterval = 0.1/*sec*/);

#ifdef OF_AVAILABLE
	void startInBackgroundThread();
	/*	Calling this will run the server in a background thread.
		all param changes will run in a separate thread, this might cause issues with your app
		as parameters can be changed at any time! so be aware, especially with strings. You might get crashes.
		This can be useful in situation where your main thread is blocked for seconds, or your app runs
		at a very low framerate. In those situations, the server doesnt get updated often enough,
		and you might get disconnected. Using a background thread means you can still control your params
		as the main thread is blocked, but it also means the changes may happen at any time. Also, the
		callback method will be called from a background thread, so keep it in mind. (no GL calls in there!)
	 */
#endif

	//You shouldnt need to call any of these directly. Use the Macros supplied above instead.
	void update(float dt);
	void draw(int x = 20, int y = 20); //draws important notifications on screen
	void close();
	vector<string> loadFromXML(string fileName); //returns list of param names in current setup but not set in XML
	void saveToXML(string fileName);

	void shareParam(string paramName, float* param, float min, float max, ofColor bgColor = ofColor(0,0,0,0) );
	void shareParam(string paramName, bool* param, ofColor bgColor = ofColor(0,0,0,0), int nothing = 0 ); //"nothing" args are just to match other methods
	void shareParam(string paramName, int* param, int min, int max, ofColor bgColor = ofColor(0,0,0,0) );
	void shareParam(string paramName, string* param, ofColor bgColor = ofColor(0,0,0,0), int nothing = 0 ); //"nothing" args are just to match other methods
	void shareParam(string paramName, int* param, int min, int max, vector<string> names, ofColor c = ofColor(0,0,0,0)); //enum!
	void shareParam(string paramName, unsigned char* param, ofColor bgColor = ofColor(0,0,0,0), int nothing = 0 );	//ofColor

	void setParamGroup(string g);		//set for all the upcoming params
	void setParamColor( ofColor c );

	void setNewParamColor(int num); //randomly sets a new param color for all upcoming params
	void unsetParamColor();  //back to un-colored params (shows alternating rows on OSX client)

	//get notified when a client changes something remotelly
	void setCallback( void (*callb)(RemoteUIServerCallBackArg) );
	//if you want to get notified when a param changes, implement a callback method like this:
	//
	//	void serverCallback(RemoteUIServerCallBackArg arg){
	//		switch (arg.action) {
	//			case CLIENT_CONNECTED: break;
	//			case CLIENT_DISCONNECTED: break;
	//			case CLIENT_UPDATED_PARAM:
	//				arg.param.print();
	//				break;
	//			...
	//			default:break;
	//		}
	//	}

	void setSaveToXMLOnExit(bool save);
	void setDrawsNotificationsAutomaticallly(bool draw);

	void pushParamsToClient(); //pushes all param values to client, updating its UI



private:

	ofxRemoteUIServer(); // use ofxRemoteUIServer::instance() instead! Use the MACROS defined above!
	~ofxRemoteUIServer();

	void			restoreAllParamsToInitialXML();
	void			restoreAllParamsToDefaultValues();
	void			connect(string address, int port);
	void			setColorForParam(RemoteUIParam &p, ofColor c);
	vector<string>	getAvailablePresets();
	void			deletePreset(string name);
	void			updateServer(float dt);
	void			(*callBack)(RemoteUIServerCallBackArg);
	void			threadedFunction();

	vector<ofColor> colorTables;
	int				colorTableIndex;
	bool			colorSet; //if user called setParamColor()
	ofColor			paramColor;
	string			upcomingGroup;
	ofxOscSender	broadcastSender;
	float			broadcastTime;
	int				broadcastCount;
	bool			portIsSet;

	string			computerName;
	string			binaryName;
	string			computerIP;

	bool			doBroadcast; //controls if the server advertises itself
	bool			drawNotifications;

	bool			loadedFromXML; //we start with loadedFromXML=false; once loadXML is called, this becomes true
	bool			saveToXmlOnExit;

	bool			threadedUpdate;
	bool			showValuesOnScreen; //displays all params on screen
	bool			updatedThisFrame;

#ifdef OF_AVAILABLE
	ofxRemoteUISimpleNotifications onScreenNotifications;
	void			_appExited(ofEventArgs &e);
	void			_draw(ofEventArgs &d);
	void			_update(ofEventArgs &d);
	void			_keyPressed(ofKeyEventArgs &e);

	int				selectedItem;
#endif

	static ofxRemoteUIServer* singleton;
};

#endif
