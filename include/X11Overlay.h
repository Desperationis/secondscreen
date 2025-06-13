#ifndef X11OVERLAY_HPP
#define X11OVERLAY_HPP

#include <X11/Xlib.h>
#include <X11/Xutil.h>

class X11Overlay {
public:
    X11Overlay(int width, int height);
    ~X11Overlay();

    // Basic circle draw (centered on screen)
    void drawCircle(unsigned long color);

    // Draw a circle at (x, y) with radius and color
    void drawCircle(int x, int y, int radius, unsigned long color);

    // Draw a single line
    void drawLine(int x1, int y1, int x2, int y2, unsigned long color);

    // Draw a single rectangle
    void drawRect(int x, int y, int width, int height, unsigned long color);

    // Draw multiple connected lines
    void drawLines(XPoint* points, int count, unsigned long color);

    // Draw multiple rectangles
    void drawRects(XRectangle* rects, int count, unsigned long color);

    // Draw multiple polygons
    void drawPolygons(XPoint** polygons, int* counts, int numPolygons, unsigned long color);
private:
    Display* dpy;
    int screen;
    Window root;
    Window win;
    int width, height;
    GC gc;

};

#endif
