#include <Arduino.h>
#include <M5Unified.h>

#ifndef __displayControl
#define __displayControl

class displayButton
{
public:
    int x;
    int y;
    int width;
    int height;
    String name;
    bool enable;

    unsigned char *iconData;

    void draw(M5Canvas canvas);
    bool contain(m5::touch_detail_t touchDetail);
    bool contain(int x, int y);

    displayButton(int x, int y, int width, int height, String name, unsigned char *iconData);
    displayButton();
};

class displayButtonLabel
{
public:
    int x;
    int y;
    int width;
    int height;
    int r;
    String name;
    bool enable;

    String label;

    u_int32_t colorCode = 0xffd500U;

    void draw(M5Canvas canvas);
    bool contain(m5::touch_detail_t touchDetail);
    bool contain(int x, int y);

    displayButtonLabel(int x, int y, int width, int height, int r, String name, String label);
    displayButtonLabel(int x, int y, int width, int height, String name, String label);
    displayButtonLabel();
};

class button_Power
{
public:
    int x;
    int y;
    int width;
    int height;
    int offset = 2;
    int roundsize = 6;
    int colorCode = 0xffd500;
    int LineWidth = 3;

    button_Power(int x, int y, int width, int height, int offset, int roundsize, int LineWidth, int colorCode);
    button_Power();
    void draw(M5Canvas canvas);
};

class displaySlider
{
public:
    int value;

    int x;
    int y;
    int width;
    int height;
    String name;
    bool enable;

    int valueMax;
    int valueMin;
    int valueRange;

    int32_t ColorCode;

    void draw(M5Canvas canvas);
    bool contain(m5::touch_detail_t touchDetail);
    bool contain(int x, int y);

    displaySlider(int x, int y, int width, int height, String name, int32_t ColorCode);
    displaySlider();
    int setValueMax(int valueMax);
    int setValueMin(int valueMin);
    int setValue(int value);
};

#endif // __displayControl