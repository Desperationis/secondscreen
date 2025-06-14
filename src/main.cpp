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
    
    const char* font =
    "-adobe-helvetica-bold-r-normal--14-140-75-75-p-80-iso8859-1";
    /*
    Field Index | Example       | Description
    -------------|---------------|-------------------------------------------------------
         [1]     *               | Wildcard to match any foundry
         [2]     adobe           | Foundry (font vendor), e.g., adobe, b&h
         [3]     helvetica       | Font family name
         [4]     bold            | Weight: normal, bold, medium, etc.
         [5]     r               | Slant: r (roman), i (italic), o (oblique)
         [6]     normal          | Set width: normal, condensed, expanded
         [7]     *               | Add style name (optional additional style)
         [8]     14              | Pixel size (height in pixels)
         [9]     140             | Point size * 10
        [10]     75              | Horizontal resolution (dpi)
        [11]     75              | Vertical resolution (dpi)
        [12]     p               | Spacing: p (proportional), m (monospaced), c (charcell)
        [13]     80              | Average width (in 1/10ths pixel)
        [14]     iso8859         | Character set registry
        [15]     1               | Encoding (often language/country specific)
    */

    overlay.drawText(100, 300, "Monospaced", 0xff0000, font); // Wildcard font
                                                              //
    overlay.pollInput();

    // Keep running
    while (true) {
        sleep(1);
    }

    return 0;
}
