#include "xfunc.h"

void sendKey(unsigned long k)
{
    Display* pDisplay = XOpenDisplay( NULL );

    XTestFakeKeyEvent( pDisplay, XKeysymToKeycode( pDisplay, k ), true, CurrentTime );
    XTestFakeKeyEvent( pDisplay, XKeysymToKeycode( pDisplay, k ), false, CurrentTime );

    //if( pDisplay == NULL ) return 1;

    XCloseDisplay(pDisplay);
}

/*
  event.display = display;
  event.window = destination of the event;
  event.root = the root window;
  event.subwindow = None;
  event.time = CurrentTime;
  event.x = 1;
  event.y = 1;
  event.x_root = 1;
  event.y_root = 1;
  event.same_screen = TRUE;

  event.type = KeyPress;  or  event.type = KeyRelease;
  event.keycode = keycode;
  event.state = modifiers;

  XSendEvent(event.display, event.window, TRUE, KeyPressMask, (XEvent *)event);
  */

