#include "X11Overlay.h"
#include <unistd.h>  // for sleep
#include <X11/Xlib.h>

int main() {
    X11Overlay overlay(300, 300);

    // 1. Draw a centered circle
    overlay.drawCircle(0xff0000); // Red

    // 2. Draw a circle at (200, 200) with radius 50
    overlay.drawCircle(200, 200, 50, 0x00ff00); // Green

    // 3. Draw a single line from (100,100) to (300,100)
    overlay.drawLine(100, 100, 300, 100, 0x0000ff); // Blue

    // 4. Draw a single rectangle at (400, 200) with size 100x50
    overlay.drawRect(400, 200, 100, 50, 0xffff00); // Yellow

    // 5. Draw multiple connected lines
    XPoint linePoints[] = { {500, 500}, {520, 520}, {540, 500}, {560, 520} };
    overlay.drawLines(linePoints, 4, 0xff00ff); // Magenta

    // 6. Draw multiple rectangles
    XRectangle rects[] = {
        {600, 100, 30, 30},
        {640, 100, 30, 30},
        {680, 100, 30, 30}
    };
    overlay.drawRects(rects, 3, 0x00ffff); // Cyan

    // 7. Draw multiple polygons (here: 2 triangles)
    XPoint poly1[] = { {100, 600}, {150, 650}, {50, 650}, {100, 600} };
    XPoint poly2[] = { {300, 600}, {350, 650}, {250, 650}, {300, 600} };
    XPoint* polys[] = { poly1, poly2 };
    int polyCounts[] = { 4, 4 };
    overlay.drawPolygons(polys, polyCounts, 2, 0xffffff); // White

    // Keep running
    while (true) {
        sleep(1);
    }

    return 0;
}
