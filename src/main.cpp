#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main() {
    Display *dpy = XOpenDisplay(NULL);
    if (!dpy) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    int screen = DefaultScreen(dpy);
    Window root = RootWindow(dpy, screen);
    int width = DisplayWidth(dpy, screen);
    int height = DisplayHeight(dpy, screen);

    // Window size and position (centered)
    int circle_diameter = 300;
    int x = (width - circle_diameter) / 2;
    int y = (height - circle_diameter) / 2;

    XSetWindowAttributes attrs;
    attrs.override_redirect = True;
    attrs.background_pixel = 0;
    attrs.border_pixel = 0;

    Window win = XCreateWindow(dpy, root, x, y, circle_diameter, circle_diameter, 0,
                               CopyFromParent, InputOutput, CopyFromParent,
                               CWOverrideRedirect | CWBackPixel | CWBorderPixel, &attrs);

    // Create shape mask
    Pixmap mask = XCreatePixmap(dpy, win, circle_diameter, circle_diameter, 1);
    GC gc = XCreateGC(dpy, mask, 0, NULL);
    XSetForeground(dpy, gc, 0);
    XFillRectangle(dpy, mask, gc, 0, 0, circle_diameter, circle_diameter);
    XSetForeground(dpy, gc, 1);
    XFillArc(dpy, mask, gc, 0, 0, circle_diameter, circle_diameter, 0, 360 * 64);
    XShapeCombineMask(dpy, win, ShapeBounding, 0, 0, mask, ShapeSet);
    XShapeCombineMask(dpy, win, ShapeInput, 0, 0, None, ShapeSet); // Input pass-through

    // Graphics context for red fill
    GC red_gc = XCreateGC(dpy, win, 0, NULL);
    XSetForeground(dpy, red_gc, 0xff0000); // Red color

    XMapWindow(dpy, win);

    // Draw the red circle
    XFillArc(dpy, win, red_gc, 0, 0, circle_diameter, circle_diameter, 0, 360 * 64);
    XFlush(dpy);

    // Run until killed
    while (1) {
        sleep(1);
    }

    return 0;
}
