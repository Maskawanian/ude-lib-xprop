// Copyright (c) 2009 Dan Saul, and others.
// Covered by the MIT Licence, see LICENCE for details.

#include "xprop.h"

/* prototypes */
static void __pv_hack_set_properties(Display *display, Window window);

/* dlopened xlib so we can find the symbols in the real xlib to call them */
static void *lib_xlib = NULL;

/* the function that actually sets the properties on toplevel window */
static void
__pv_hack_set_properties(Display *display, Window window)
{
	char buf[4096];
	pid_t pid = getpid();
	
	// Where this is loaded force a pid on the window.
	Atom atom_pid = XInternAtom(display, "_NET_WM_PID", False);
	XChangeProperty(display, window, atom_pid, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&pid, 1);
	
	// _NET_WM_PID requires WM_CLIENT_MACHINE to be set as well.
	Atom atom_hostname = XInternAtom(display, "WM_CLIENT_MACHINE", False);
	gethostname (buf, 4096);
	buf[4096 - 1] = '\0';
	XChangeProperty(display, window, atom_hostname, XA_STRING, 8, PropModeReplace, (unsigned char *)buf, strlen(buf));
}

/* XCreateWindow intercept hack */
Window
XCreateWindow(
              Display *display,
              Window parent,
              int x, int y,
              unsigned int width, unsigned int height,
              unsigned int border_width,
              int depth,
              unsigned int class,
              Visual *visual,
              unsigned long valuemask,
              XSetWindowAttributes *attributes
              )
{
   static Window (*func)
      (
       Display *display,
       Window parent,
       int x, int y,
       unsigned int width, unsigned int height,
       unsigned int border_width,
       int depth,
       unsigned int class,
       Visual *visual,
       unsigned long valuemask,
       XSetWindowAttributes *attributes
       ) = NULL;
   int i;

   /* find the real Xlib and the real X function */
   if (!lib_xlib) lib_xlib = dlopen("libX11.so", RTLD_GLOBAL | RTLD_LAZY);
   if (!func) func = dlsym (lib_xlib, "XCreateWindow");

   /* multihead screen handling loop */
   for (i = 0; i < ScreenCount(display); i++)
     {
        /* if the window is created as a toplevel window */
        if (parent == RootWindow(display, i))
          {
             Window window;

             /* create it */
             window = (*func) (display, parent, x, y, width, height,
                                border_width, depth, class, visual, valuemask,
                                attributes);
             /* set properties */
             __pv_hack_set_properties(display, window);
             /* return it */
             return window;
          }
     }
   /* normal child window - create as usual */
   return (*func) (display, parent, x, y, width, height, border_width, depth,
                   class, visual, valuemask, attributes);
}

/* XCreateSimpleWindow intercept hack */
Window
XCreateSimpleWindow(
                    Display *display,
                    Window parent,
                    int x, int y,
                    unsigned int width, unsigned int height,
                    unsigned int border_width,
                    unsigned long border,
                    unsigned long background
                    )
{
   static Window (*func)
      (
       Display *display,
       Window parent,
       int x, int y,
       unsigned int width, unsigned int height,
       unsigned int border_width,
       unsigned long border,
       unsigned long background
       ) = NULL;
   int i;

   /* find the real Xlib and the real X function */
   if (!lib_xlib) lib_xlib = dlopen("libX11.so", RTLD_GLOBAL | RTLD_LAZY);
   if (!func) func = dlsym (lib_xlib, "XCreateSimpleWindow");

   /* multihead screen handling loop */
   for (i = 0; i < ScreenCount(display); i++)
     {
        /* if the window is created as a toplevel window */
        if (parent == RootWindow(display, i))
          {
             Window window;

             /* create it */
             window = (*func) (display, parent, x, y, width, height,
                                border_width, border, background);
             /* set properties */
             __pv_hack_set_properties(display, window);
             /* return it */
             return window;
          }
     }
   /* normal child window - create as usual */
   return (*func) (display, parent, x, y, width, height,
                   border_width, border, background);
}

/* XReparentWindow intercept hack */
int
XReparentWindow(
                Display *display,
                Window window,
                Window parent,
                int x, int y
                )
{
   static int (*func)
      (
       Display *display,
       Window window,
       Window parent,
       int x, int y
       ) = NULL;
   int i;

   /* find the real Xlib and the real X function */
   if (!lib_xlib) lib_xlib = dlopen("libX11.so", RTLD_GLOBAL | RTLD_LAZY);
   if (!func) func = dlsym (lib_xlib, "XReparentWindow");

   /* multihead screen handling loop */
   for (i = 0; i < ScreenCount(display); i++)
     {
        /* if the window is created as a toplevel window */
        if (parent == RootWindow(display, i))
          {
             /* set properties */
             __pv_hack_set_properties(display, window);
             /* reparent it */
             return (*func) (display, window, parent, x, y);
          }
     }
   /* normal child window reparenting - reparent as usual */
   return (*func) (display, window, parent, x, y);
}

