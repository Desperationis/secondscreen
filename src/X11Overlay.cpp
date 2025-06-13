#include "X11Overlay.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>

struct X11Overlay::Impl {
    Display* dpy;
    int screen;
    Window root;
    Window win;
    int width, height;
    GC gc;

    Impl(int w, int h) : width(w), height(h) {
        dpy = XOpenDisplay(NULL);
        if (!dpy) {
            fprintf(stderr, "Cannot open display\n");
            exit(1);
        }

        screen = DefaultScreen(dpy);
        root = RootWindow(dpy, screen);

        int x = (DisplayWidth(dpy, screen) - width) / 2;
        int y = (DisplayHeight(dpy, screen) - height) / 2;

        XSetWindowAttributes attrs;
        attrs.override_redirect = True;
        attrs.background_pixel = 0;
        attrs.border_pixel = 0;

        win = XCreateWindow(dpy, root, x, y, width, height, 0,
                            CopyFromParent, InputOutput, CopyFromParent,
                            CWOverrideRedirect | CWBackPixel | CWBorderPixel, &attrs);

        // Transparent shape mask
        Pixmap mask = XCreatePixmap(dpy, win, width, height, 1);
        GC mask_gc = XCreateGC(dpy, mask, 0, NULL);
        XSetForeground(dpy, mask_gc, 0);
        XFillRectangle(dpy, mask, mask_gc, 0, 0, width, height);
        XSetForeground(dpy, mask_gc, 1);
        XFillArc(dpy, mask, mask_gc, 0, 0, width, height, 0, 360 * 64);
        XShapeCombineMask(dpy, win, ShapeBounding, 0, 0, mask, ShapeSet);
        XShapeCombineMask(dpy, win, ShapeInput, 0, 0, None, ShapeSet);
        XFreePixmap(dpy, mask);
        XFreeGC(dpy, mask_gc);

        // Graphics context for drawing
        gc = XCreateGC(dpy, win, 0, NULL);
        XMapWindow(dpy, win);
        XFlush(dpy);
    }

    ~Impl() {
        XDestroyWindow(dpy, win);
        XFreeGC(dpy, gc);
        XCloseDisplay(dpy);
    }

    void drawCircle(unsigned long color) {
        XSetForeground(dpy, gc, color);
        XFillArc(dpy, win, gc, 0, 0, width, height, 0, 360 * 64);
        XFlush(dpy);
    }
};

// Public API
X11Overlay::X11Overlay(int width, int height) {
    pImpl = new Impl(width, height);
}

X11Overlay::~X11Overlay() {
    delete pImpl;
}

void X11Overlay::drawCircle(unsigned long color) {
    pImpl->drawCircle(color);
}
