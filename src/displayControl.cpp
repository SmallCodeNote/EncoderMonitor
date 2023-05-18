#include <Arduino.h>
#include <M5Unified.h>
#include "displayControl.hpp"

//=====================
//=====================
void displayButton::draw(M5Canvas canvas)
{
    if (enable)
    {
        canvas.drawPng(iconData, ~0u, x, y, width, height, 0, 0, 1, 1, datum_t::middle_center);
    }
}

bool displayButton::contain(m5::touch_detail_t touchDetail)
{
    return contain(touchDetail.x, touchDetail.y);
}

bool displayButton::contain(int _x, int _y)
{
    return enable && _x >= this->x && _x < (this->x + this->width) && _y >= this->y && _y < (this->y + this->height);
}

displayButton::displayButton()
{
    enable = true;
}

displayButton::displayButton(int x, int y, int width, int height, String name, unsigned char *iconData)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->name = name;
    this->iconData = iconData;
    this->enable = true;
}

//=====================
//=====================
void displayButtonLabel::draw(M5Canvas canvas)
{

    canvas.setFont(&fonts::lgfxJapanGothic_24);
    if (!enable)
    {
        canvas.setTextColor(0xffd500);
        canvas.drawRoundRect(x + 3, y + 3, width - 6, height - 6, r, colorCode);
        canvas.drawRoundRect(x + 4, y + 4, width - 8, height - 8, r - 1, colorCode);
        canvas.drawRoundRect(x + 5, y + 5, width - 10, height - 10, r - 2, colorCode);
        canvas.drawCenterString(label, x + width / 2, y + height / 4);
    }
    else
    {
        canvas.setTextColor(TFT_BLACK);
        canvas.fillRoundRect(x + 3, y + 3, width - 6, height - 6, r, colorCode);
        canvas.drawCenterString(label, x + width / 2, y + height / 4);
    }

    //Serial.println("R=" + String(r));
}

bool displayButtonLabel::contain(m5::touch_detail_t touchDetail)
{
    return contain(touchDetail.x, touchDetail.y);
}

bool displayButtonLabel::contain(int _x, int _y)
{
    return _x >= this->x && _x < (this->x + this->width) && _y >= this->y && _y < (this->y + this->height);
}

displayButtonLabel::displayButtonLabel()
{
    colorCode = 0xffd500U;
    enable = true;
}

displayButtonLabel::displayButtonLabel(int x, int y, int width, int height, String name, String label)
{
    if (width < height)
    {
        r = width / 6;
    }
    else
    {
        r = height / 6;
    }

    displayButtonLabel(x, y, width, height, r, name, label);
}

displayButtonLabel::displayButtonLabel(int x, int y, int width, int height, int r, String name, String label)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->r = r;
    this->name = name;
    this->label = label;
    this->enable = true;
    colorCode = 0xffd500;
}

//=====================
//=====================
button_Power::button_Power(int x, int y, int width, int height, int offset, int roundsize, int LineWidth, int colorCode)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->offset = offset;
    this->roundsize = roundsize;
    this->colorCode = colorCode;
    this->LineWidth = LineWidth;
}

button_Power::button_Power()
{
    this->x = 0;
    this->y = 0;
    this->width = 48;
    this->height = 48;
    this->offset = 2;
    this->roundsize = 6;
    this->colorCode = YELLOW;
    this->LineWidth = 3;
}

void button_Power::draw(M5Canvas canvas)
{
    int cx = x + width / 2;
    int cy = y + height / 2;
    int a = width > height ? (int)(height * 0.6) / 2 : (int)(width * 0.6) / 2;

    for (int i = 0; i < LineWidth; i++)
    {
        int L = offset + i;
        int R = roundsize - i;
        if (R < 0)
        {
            R = 0;
        }

        canvas.drawRoundRect(x + L, y + L, width - L * 2, height - L * 2, R, colorCode);
    }

    canvas.fillArc(cx, cy, a, a - LineWidth + 1, -60, 240, colorCode);
    canvas.fillRect(cx - LineWidth / 2, cy - a, LineWidth, a, colorCode);
}

//=====================
//=====================
void displaySlider::draw(M5Canvas canvas)
{
    if (enable)
    {
        canvas.drawRect(x, y, width, height, WHITE);

        float rangebarlength = width - height - height / 2.0;
        float rangebarStart = x + height / 2.0 +  height/ 4.0;

        int CenterX = (int)(rangebarStart + rangebarlength * (value-valueMin) / (float)valueRange);
        int CenterY = y + height / 2;
        Serial.println("displaySlider::draw /CenterX= " + String(CenterX) + "/CenterY= " + String(CenterY)+ "/rangebarlength= " + String(rangebarlength));
        canvas.fillCircle(CenterX, CenterY, height / 2, YELLOW);
        canvas.drawRoundRect(x + height / 2, y + height / 4,
         width - height, height / 2, height / 4, ColorCode);
    //... rangebar start = x + height / 2 + height / 4
    //... rangebar end = x + width - height / 2 - height / 4
    }
}

bool displaySlider::contain(m5::touch_detail_t touchDetail)
{
    return contain(touchDetail.x, touchDetail.y);
}

bool displaySlider::contain(int touch_x, int touch_y)
{
    bool isInside = enable && touch_x >= this->x && touch_x < (this->x + this->width) && touch_y >= this->y && touch_y < (this->y + this->height);

    if (isInside)
    {
        float rangebarlength = width - height - height / 2.0;
        float rangebarStart = x + height / 2.0 +  height/ 4.0;

        value = (int)(valueMin + (valueRange) * (touch_x - rangebarStart) / rangebarlength);
        Serial.println("displaySlider::contain value= " + String(value));
        if (value < valueMin)
        {
            value = valueMin;
        }
        if (value > valueMax)
        {
            value = valueMax;
        }

        Serial.println("displaySlider::contain value= " + String(value)
        + "/valueMin= " + String(valueMin)
        + "/valueRange= "+ String(valueRange)
        + "/touch_x - rangebarStart= " + String(touch_x - rangebarStart)+ " "
        + "/w-h= " + String(width - height));
    }
Serial.println("displaySlider::contain touch_x="+String(touch_x));
    return isInside;
}

int displaySlider::setValueMax(int valueMax)
{
    this->valueMax = valueMax;
    valueRange = valueMax - valueMin;

    return 1;
}

int displaySlider::setValueMin(int valueMin)
{
    this->valueMin = valueMin;
    valueRange = valueMax - valueMin;

    return 1;
}
int displaySlider::setValue(int value)
{

    if (value < valueMin)
    {
        value = valueMin;
    }
    if (value > valueMax)
    {
        value = valueMax;
    }

    this->value = value;

    return 1;
}

displaySlider::displaySlider()
{
    x = 5;
    y = 40;
    width = 200;
    height = 10;
    enable = true;
    ColorCode = 0xffd500;
    valueMin = 0;
    valueMax = 100;
    valueRange = valueMax - valueMin;
    value = valueMin;
}