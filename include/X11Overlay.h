#ifndef X11OVERLAY_HPP
#define X11OVERLAY_HPP

class X11Overlay {
public:
    X11Overlay(int width, int height);
    ~X11Overlay();

    void drawCircle(unsigned long color);

private:
    struct Impl;
    Impl* pImpl;
};

#endif

