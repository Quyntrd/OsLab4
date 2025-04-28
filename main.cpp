#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <cstring>

void DrawScene(Display* display, Window window, GC gc) {
    // Небо
    XSetForeground(display, gc, 0x87CEEB); // RGB(135,206,235)
    XFillRectangle(display, window, gc, 0, 0, 800, 300);

    // Трава
    XSetForeground(display, gc, 0x228B22); // RGB(34,139,34)
    XFillRectangle(display, window, gc, 0, 300, 800, 300);

    // Дом
    XSetForeground(display, gc, 0x8B4513); // RGB(139,69,19)
    XFillRectangle(display, window, gc, 300, 300, 200, 150);

    // Крыша (треугольник)
    XPoint roof[3] = { {280, 300}, {520, 300}, {400, 220} };
    XSetForeground(display, gc, 0x696969); // RGB(105,105,105)
    XFillPolygon(display, window, gc, roof, 3, Convex, CoordModeOrigin);

    // Окно
    XSetForeground(display, gc, 0xADD8E6); // RGB(173,216,230)
    XFillRectangle(display, window, gc, 330, 330, 40, 40);

    // Дверь
    XSetForeground(display, gc, 0x000000); // Black
    XFillRectangle(display, window, gc, 420, 360, 40, 90);

    // Солнце
    XSetForeground(display, gc, 0xFFFF00); // Yellow
    XFillArc(display, window, gc, 80, 50, 70, 70, 0, 360*64);

    // Дерево (ствол)
    XSetForeground(display, gc, 0x654321); // RGB(101,67,33)
    XFillRectangle(display, window, gc, 600, 360, 20, 90);

    // Дерево (крона)
    XSetForeground(display, gc, 0x008000); // RGB(0,128,0)
    XFillArc(display, window, gc, 570, 290, 80, 80, 0, 360*64);

    // Подпись
    XSetForeground(display, gc, 0x000000); // Black
    XFontStruct* font = XLoadQueryFont(display, "fixed");
    if (font) {
        XSetFont(display, gc, font->fid);
    }
    XDrawString(display, window, gc, 10, 20, "Домик в саду", strlen("Домик в саду"));
}

int main() {
    Display* display = XOpenDisplay(nullptr);
    if (!display) {
        return 1;
    }

    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);

    Window window = XCreateSimpleWindow(display, root, 100, 100, 800, 600, 1,
                                        BlackPixel(display, screen), WhitePixel(display, screen));

    XStoreName(display, window, "Домик в саду");

    XSelectInput(display, window, ExposureMask | KeyPressMask | StructureNotifyMask);
    XMapWindow(display, window);

    GC gc = XCreateGC(display, window, 0, nullptr);

    bool running = true;
    while (running) {
        XEvent event;
        XNextEvent(display, &event);

        switch (event.type) {
        case Expose:
            DrawScene(display, window, gc);
            break;
        case KeyPress:
            running = false;
            break;
        case DestroyNotify:
            running = false;
            break;
        }
    }

    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return 0;
}
