// X11Overlay.cpp
#include "X11Overlay.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/shape.h>
#include <X11/Xatom.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

struct X11Overlay::Impl {
    Display* dpy;
    int screen;
    Window root;
    Window win;
    int width, height;
    GC gc;
    Pixmap pixmap;

    Impl(int w, int h) : width(w), height(h) {
        dpy = XOpenDisplay(NULL);
        if (!dpy) {
            fprintf(stderr, "Cannot open display\n");
            exit(1);
        }

        screen = DefaultScreen(dpy);
        root = RootWindow(dpy, screen);

        XVisualInfo vinfo;
        if (!XMatchVisualInfo(dpy, screen, 32, TrueColor, &vinfo)) {
            fprintf(stderr, "No matching visual\n");
            exit(1);
        }

        XSetWindowAttributes attrs;
        attrs.colormap = XCreateColormap(dpy, root, vinfo.visual, AllocNone);
        attrs.override_redirect = True;
        attrs.background_pixel = 0;
        attrs.border_pixel = 0;

        win = XCreateWindow(dpy, root, 0, 0, DisplayWidth(dpy, screen), DisplayHeight(dpy, screen), 0,
                            vinfo.depth, InputOutput, vinfo.visual,
                            CWColormap | CWOverrideRedirect | CWBackPixel | CWBorderPixel,
                            &attrs);

        // Input pass-through
        XShapeCombineRectangles(dpy, win, ShapeInput, 0, 0, NULL, 0, ShapeSet, 0);

        XMapWindow(dpy, win);
        XFlush(dpy);

        gc = XCreateGC(dpy, win, 0, NULL);
    }

    ~Impl() {
        XFreeGC(dpy, gc);
        XDestroyWindow(dpy, win);
        XCloseDisplay(dpy);
    }

    void drawCircle(unsigned long color) {
        XSetForeground(dpy, gc, color);
        XFillArc(dpy, win, gc, (DisplayWidth(dpy, screen)-width)/2, (DisplayHeight(dpy, screen)-height)/2, width, height, 0, 360*64);
        XFlush(dpy);
    }
};

X11Overlay::X11Overlay(int width, int height) : pImpl(new Impl(width, height)) {}
X11Overlay::~X11Overlay() { delete pImpl; }
void X11Overlay::drawCircle(unsigned long color) { pImpl->drawCircle(color); }
