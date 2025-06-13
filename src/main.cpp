#include "X11Overlay.h"
#include <unistd.h>

int main() {
    X11Overlay overlay(300, 300);
    //overlay.drawCircle(0xff0000); // Red in hex RGB

    // Keep running until killed
    while (true) {
        sleep(1);
    }

    return 0;
}
