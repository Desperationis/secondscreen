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

        // Ensure fully opaque drawing by disabling graphics exposure
        XSetGraphicsExposures(dpy, gc, False);
    }

    ~Impl() {
        XFreeGC(dpy, gc);
        XDestroyWindow(dpy, win);
        XCloseDisplay(dpy);
    }

    void drawCircle(unsigned long color) {
        color |= 0xff000000;
        XSetForeground(dpy, gc, color);
        XFillArc(dpy, win, gc, (DisplayWidth(dpy, screen)-width)/2, (DisplayHeight(dpy, screen)-height)/2, width, height, 0, 360*64);
        XFlush(dpy);
    }

    void drawCircle(int x, int y, int radius, unsigned long color) {
        color |= 0xff000000;
        XSetForeground(dpy, gc, color);
        XFillArc(dpy, win, gc, x - radius, y - radius, radius * 2, radius * 2, 0, 360 * 64);
        XFlush(dpy);
    }

    void drawLine(int x1, int y1, int x2, int y2, unsigned long color) {
        color |= 0xff000000;
        XSetForeground(dpy, gc, color);
        XDrawLine(dpy, win, gc, x1, y1, x2, y2);
        XFlush(dpy);
    }

    void drawRect(int x, int y, int width, int height, unsigned long color) {
        color |= 0xff000000;
        XSetForeground(dpy, gc, color);
        XDrawRectangle(dpy, win, gc, x, y, width, height);
        XFlush(dpy);
    }

    void drawLines(XPoint* points, int count, unsigned long color) {
        color |= 0xff000000;
        XSetForeground(dpy, gc, color);
        XDrawLines(dpy, win, gc, points, count, CoordModeOrigin);
        XFlush(dpy);
    }

    void drawRects(XRectangle* rects, int count, unsigned long color) {
        color |= 0xff000000;
        XSetForeground(dpy, gc, color);
        XDrawRectangles(dpy, win, gc, rects, count);
        XFlush(dpy);
    }

    void drawPolygons(XPoint** polygons, int* counts, int numPolygons, unsigned long color) {
        color |= 0xff000000;
        XSetForeground(dpy, gc, color);
        for (int i = 0; i < numPolygons; ++i) {
            XDrawLines(dpy, win, gc, polygons[i], counts[i], CoordModeOrigin);
        }
        XFlush(dpy);
    }
};

X11Overlay::X11Overlay(int width, int height) : pImpl(new Impl(width, height)) {}
X11Overlay::~X11Overlay() { delete pImpl; }
void X11Overlay::drawCircle(unsigned long color) { pImpl->drawCircle(color); }
void X11Overlay::drawCircle(int x, int y, int radius, unsigned long color) { pImpl->drawCircle(x, y, radius, color); }
void X11Overlay::drawLine(int x1, int y1, int x2, int y2, unsigned long color) { pImpl->drawLine(x1, y1, x2, y2, color); }
void X11Overlay::drawRect(int x, int y, int width, int height, unsigned long color) { pImpl->drawRect(x, y, width, height, color); }
void X11Overlay::drawLines(XPoint* points, int count, unsigned long color) { pImpl->drawLines(points, count, color); }
void X11Overlay::drawRects(XRectangle* rects, int count, unsigned long color) { pImpl->drawRects(rects, count, color); }
void X11Overlay::drawPolygons(XPoint** polygons, int* counts, int numPolygons, unsigned long color) { pImpl->drawPolygons(polygons, counts, numPolygons, color); }
