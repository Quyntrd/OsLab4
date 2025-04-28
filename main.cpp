#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <ctime>

// Функция отрисовки сцены
void DrawScene(Display* display, Window window, GC gc) {
    // Получение размеров окна
    XWindowAttributes attrs;
    XGetWindowAttributes(display, window, &attrs);
    int width = attrs.width;
    int height = attrs.height;

    // 1. Космос (чёрный фон)
    XSetForeground(display, gc, BlackPixel(display, DefaultScreen(display)));
    XFillRectangle(display, window, gc, 0, 0, width, height);

    // 2. Звёзды (белые точки)
    XSetForeground(display, gc, 0xFFFFFF); // Белый цвет
    for (int i = 0; i < 100; i++) {
        int x = rand() % width;
        int y = rand() % height;
        XDrawPoint(display, window, gc, x, y);
    }

    // 3. Ракета

    // Корпус ракеты (серебристый прямоугольник)
    int rx = width/2 - 40;
    int ry = height/2;
    int rw = 80;
    int rh = 200;
    XSetForeground(display, gc, 0xC0C0C0); // Серебристый
    XFillRectangle(display, window, gc, rx, ry, rw, rh);

    // Носовой конус ракеты (красный треугольник)
    XPoint nose[3] = {
        {rx, ry},
        {rx + rw, ry},
        {rx + rw/2, ry - 80}
    };
    XSetForeground(display, gc, 0xFF0000); // Красный
    XFillPolygon(display, window, gc, nose, 3, Convex, CoordModeOrigin);

    // Левый плавник
    XPoint finL[3] = {
        {rx, ry + rh/2},
        {rx - 30, ry + rh + 20},
        {rx, ry + rh}
    };
    // Правый плавник
    XPoint finR[3] = {
        {rx + rw, ry + rh/2},
        {rx + rw + 30, ry + rh + 20},
        {rx + rw, ry + rh}
    };
    XSetForeground(display, gc, 0xFF4500); // Оранжево-красный
    XFillPolygon(display, window, gc, finL, 3, Convex, CoordModeOrigin);
    XFillPolygon(display, window, gc, finR, 3, Convex, CoordModeOrigin);

    // Окна ракеты (три голубых круга)
    XSetForeground(display, gc, 0x87CEEB); // Светло-голубой
    for (int i = 0; i < 3; i++) {
        int cx = rx + rw/2;
        int cy = ry + 40 + i*50;
        XFillArc(display, window, gc, cx-15, cy-15, 30, 30, 0, 360*64);
    }

    // 4. Подпись
    XSetForeground(display, gc, 0xFFFFFF); // Белый
    XFontStruct* font = XLoadQueryFont(display, "fixed");
    if (font) {
        XSetFont(display, gc, font->fid);
    }
    XDrawString(display, window, gc, 10, 30, "Ракета в космосе", strlen("Ракета в космосе"));
}

int main() {
    srand(time(nullptr)); // Инициализация генератора случайных чисел

    Display* display = XOpenDisplay(nullptr);
    if (!display) {
        return 1;
    }

    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);

    Window window = XCreateSimpleWindow(display, root, 100, 100, 800, 600, 1,
                                        BlackPixel(display, screen), WhitePixel(display, screen));

    XStoreName(display, window, "Ракета в космосе");

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
