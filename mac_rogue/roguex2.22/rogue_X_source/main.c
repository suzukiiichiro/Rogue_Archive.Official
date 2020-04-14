#include <Carbon/Carbon.h>
#include "macHeaders.h"
extern char* _maccur_pgm_name;
extern Boolean ok_go_on;
extern char svFname[BUFSIZ];
WindowRef guideWin,itemWin;
extern pascal OSStatus doCommand(EventHandlerCallRef myhandler, EventRef event, void* userData);
int main(int argc, char* argv[])
{
    IBNibRef 		nibRef;
    extern WindowRef 		the_windo;

    OSStatus		err;
	EventTypeSpec   appspec[]={{kEventClassCommand,kEventProcessCommand},
								{kEventClassApplication, kEventAppQuit},
								{ kEventClassKeyboard, kEventRawKeyDown },
								{ kEventClassKeyboard, kEventRawKeyRepeat },

							};
	*svFname = 0;
	ok_go_on = false;

    // Create a Nib reference passing the name of the nib file (without the .nib extension)
    // CreateNibReference only searches into the application bundle.
    err = CreateNibReference(CFSTR("main"), &nibRef);
    require_noerr( err, CantGetNibRef );
    
    // Once the nib reference is created, set the menu bar. "MainMenu" is the name of the menu bar
    // object. This name is set in InterfaceBuilder when the nib is created.
    err = SetMenuBarFromNib(nibRef, CFSTR("MenuBar"));
    require_noerr( err, CantSetMenuBar );
    
    // Then create a window. "MainWindow" is the name of the window object. This name is set in 
    // InterfaceBuilder when the nib is created.
    err = CreateWindowFromNib(nibRef, CFSTR("MainWindow"), &the_windo);
    require_noerr( err, CantCreateWindow );
	
    err = CreateWindowFromNib(nibRef, CFSTR("Items"), &itemWin);
    require_noerr( err, CantCreateWindow );
	SetDrawerPreferredEdge(itemWin, kWindowEdgeRight);
	

    // We don't need the nib reference anymore.
    DisposeNibReference(nibRef);
    
    // The window was created hidden so show it.
    //ShowWindow( the_windo );
	SetDrawerParent(itemWin, the_windo);
#ifdef JAPAN
	_maccur_pgm_name = "ローグ・クローン2 日本語版";
#else
	_maccur_pgm_name = "Rogue clone II 1.3a";
#endif
	maccur_init();
	
	err = InstallApplicationEventHandler(NewEventHandlerUPP(doCommand), 1,appspec,NULL,NULL);
	//ShowWindow(the_windo);
	
	
    // Call the event loop
	InstallItemListEvents();

    RunApplicationEventLoop();
	//mymain(0);
	//return 0;

CantCreateWindow:
CantSetMenuBar:
CantGetNibRef:
	return err;
}

