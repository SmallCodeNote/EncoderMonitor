#include <Arduino.h>
#include <M5ModuleDisplay.h>
#include <M5Unified.h>
#include "displayControl.hpp"

#ifndef __InitializeComponent
#define __InitializeComponent

extern unsigned char icon_Setting[2143];
extern unsigned char icon_Power[1624];
extern unsigned char icon_Right[1407];
extern unsigned char icon_Left[1417];
extern unsigned char icon_Save[1624];

extern unsigned char btn_OK[1859];
extern unsigned char btn_CANCEL[2815];

extern unsigned char btn_RESET[1698];
extern unsigned char icon_QR[1661];

String getStringSplit(String str, char separator, int index)
{
    int len = str.length();
    int startPosition = 0;

    int separatorCount = 0;
    for (; startPosition < len; startPosition++)
    {
        if (separatorCount == index)
            break;

        if (str[startPosition] == separator)
            separatorCount++;
    }

    int endPosition = str.indexOf(separator, startPosition);

    if (!endPosition)
        endPosition = len - 1;

    return str.substring(startPosition, endPosition);
}

class form
{
public:
    int formWidth;
    int formHeight;

    bool formEnable;
    String formName;
    String valueString;
    M5Canvas Display_Main_Canvas;

    form()
    {
        formName = "BaseClass";
        formEnable = true;
    }

    form(M5Canvas formCanvas, int value)
    {
        formName = "BaseClass";
        formEnable = true;

        Display_Main_Canvas = formCanvas;
        formWidth = Display_Main_Canvas.width();
        formHeight = Display_Main_Canvas.height();
    }

    virtual void draw(float value, String text)
    {
    }

    virtual int touchCheck(m5::touch_detail_t t)
    {
        return 0;
    }
};

// =====================
// form_Top
// =====================
class form_Top : public form
{
private:
    displayButton BTN_Power;
    displayButton BTN_Config;
    displayButton BTN_RESET;
    displayButton BTN_QR;

public:
    form_Top()
    {
        formName = "Top";
        formEnable = true;
    }

    /// @brief Initialize Canvas / Button
    form_Top(M5Canvas formCanvas, int value)
    {
        formName = "Top";
        formEnable = true;

        Display_Main_Canvas = formCanvas;
        formWidth = Display_Main_Canvas.width();
        formHeight = Display_Main_Canvas.height();

        // Config ICON Button ===================
        BTN_Config = displayButton();
        BTN_Config.x = 174;
        BTN_Config.y = 187;
        BTN_Config.width = 52;
        BTN_Config.height = 52;
        BTN_Config.iconData = icon_Setting;

        // Power ICON Button ===================
        BTN_Power = displayButton();
        BTN_Power.x = 249;
        BTN_Power.y = 187;
        BTN_Power.width = 52;
        BTN_Power.height = 52;
        BTN_Power.iconData = icon_Power;
        // ================================

        // RESET ICON Button ===================
        BTN_RESET = displayButton();
        BTN_RESET.x = 249;
        BTN_RESET.y = 0;
        BTN_RESET.width = 52;
        BTN_RESET.height = 52;
        BTN_RESET.iconData = btn_RESET;
        // ================================

        // QR ICON Button ===================
        BTN_QR = displayButton();
        BTN_QR.x = 174;
        BTN_QR.y = 0;
        BTN_QR.width = 52;
        BTN_QR.height = 52;
        BTN_QR.iconData = icon_QR;
        // ================================
    }

    void draw(float value, String text) override
    {
        if (formEnable)
        {
            Display_Main_Canvas.fillScreen(BLACK);
            BTN_Config.draw(Display_Main_Canvas);
            BTN_Power.draw(Display_Main_Canvas);
            BTN_RESET.draw(Display_Main_Canvas);
            BTN_QR.draw(Display_Main_Canvas);

            Display_Main_Canvas.setCursor(30, 80);
            if (value < 0)
            {
                Display_Main_Canvas.setFont(&fonts::Font8);
                Display_Main_Canvas.printf("%.3f", -value);
            }
            else
            {
                Display_Main_Canvas.setFont(&fonts::Font8);
                Display_Main_Canvas.printf("%.3f", value);
            }

            Display_Main_Canvas.setFont(&fonts::lgfxJapanGothic_24);
            Display_Main_Canvas.setTextColor(0xffd500);
            Display_Main_Canvas.drawString("[" + text + "]", 2, 240 - 24 - 2);

            Display_Main_Canvas.pushSprite(0, 0);
        }
    }

    int touchCheck(m5::touch_detail_t t) override
    {
        if (formEnable)
        {
            if (BTN_Power.contain(t))
            {
                return 1;
            }
            else if (BTN_Config.contain(t))
            {
                return 2;
            }
            else if (BTN_RESET.contain(t))
            {
                return 3;
            }
            else if (BTN_QR.contain(t))
            {
                return 4;
            }
        }
        return 0;
    }
};

// =====================
// form_ShutdownMessage
// =====================

class form_ShutdownMessage : public form
{
private:
    displayButton BTN_OK;
    displayButton BTN_CANCEL;

public:
    form_ShutdownMessage()
    {
        formName = "ShutdownMessage";
        formEnable = true;
    }
    /// @brief Initialize Canvas / Button
    form_ShutdownMessage(M5Canvas formCanvas, int value)
    {
        formName = "ShutdownMessage";
        formEnable = true;

        Display_Main_Canvas = formCanvas;
        formWidth = Display_Main_Canvas.width();
        formHeight = Display_Main_Canvas.height();

        BTN_OK = displayButton();
        // OK ICON Button ===================
        BTN_OK.x = 0;
        BTN_OK.y = 187;
        BTN_OK.width = 160;
        BTN_OK.height = 52;
        BTN_OK.iconData = btn_OK;
        BTN_OK.enable = true;

        BTN_CANCEL = displayButton();
        // CANCEL ICON Button ===================
        BTN_CANCEL.x = 160;
        BTN_CANCEL.y = 187;
        BTN_CANCEL.width = 160;
        BTN_CANCEL.height = 52;
        BTN_CANCEL.iconData = btn_CANCEL;
        BTN_CANCEL.enable = true;
        // ================================
    }

    void draw(float value, String text)
    {
        if (formEnable)
        {
            Display_Main_Canvas.fillScreen(BLACK);
            BTN_OK.draw(Display_Main_Canvas);
            BTN_CANCEL.draw(Display_Main_Canvas);

            Display_Main_Canvas.setFont(&fonts::lgfxJapanGothic_40);
            Display_Main_Canvas.setTextColor(0xffd500);
            Display_Main_Canvas.drawCenterString("Shutdown ?", formWidth / 2, 40);
            Display_Main_Canvas.pushSprite(0, 0);
        }
    }
    int touchCheck(m5::touch_detail_t t)
    {
        if (BTN_OK.contain(t))
        {
            return 1;
        }
        else if (BTN_CANCEL.contain(t))
        {
            return 2;
        }
        return 0;
    }
};

// =====================
// form_SaveMessage
// =====================
class form_SaveMessage : public form
{
private:
    displayButton BTN_OK;
    displayButton BTN_CANCEL;

public:
    form_SaveMessage()
    {
        formName = "ShutdownMessage";
        formEnable = true;
    }

    /// @brief Initialize Canvas / Button
    form_SaveMessage(M5Canvas formCanvas, int value)
    {
        formName = "SaveMessage";
        formEnable = true;

        Display_Main_Canvas = formCanvas;
        formWidth = Display_Main_Canvas.width();
        formHeight = Display_Main_Canvas.height();

        BTN_OK = displayButton();
        // OK ICON Button ===================
        BTN_OK.x = 0;
        BTN_OK.y = 187;
        BTN_OK.width = 160;
        BTN_OK.height = 52;
        BTN_OK.iconData = btn_OK;
        BTN_OK.enable = true;

        BTN_CANCEL = displayButton();
        // CANCEL ICON Button ===================
        BTN_CANCEL.x = 160;
        BTN_CANCEL.y = 187;
        BTN_CANCEL.width = 160;
        BTN_CANCEL.height = 52;
        BTN_CANCEL.iconData = btn_CANCEL;
        BTN_CANCEL.enable = true;
        // ================================
    }

    void draw(float value, String text)
    {
        if (formEnable)
        {
            Display_Main_Canvas.fillScreen(BLACK);
            BTN_OK.draw(Display_Main_Canvas);
            BTN_CANCEL.draw(Display_Main_Canvas);

            Display_Main_Canvas.setFont(&fonts::lgfxJapanGothic_40);
            Display_Main_Canvas.setTextColor(0xffd500);
            Display_Main_Canvas.drawCenterString("Save ?", formWidth / 2, 40);
            Display_Main_Canvas.pushSprite(0, 0);
        }
    }
    int touchCheck(m5::touch_detail_t t)
    {
        if (BTN_OK.contain(t))
        {
            return 1;
        }
        else if (BTN_CANCEL.contain(t))
        {
            return 2;
        }
        return 0;
    }
};

// =====================
// form_QR
// =====================
class form_QR : public form
{
private:
    displayButton BTN_OK;
    //    displayButton BTN_CANCEL;

public:
    form_QR()
    {
        formName = "QR";
        formEnable = true;
    }

    /// @brief Initialize Canvas / Button
    form_QR(M5Canvas formCanvas, int value)
    {
        formName = "QR";
        formEnable = true;

        Display_Main_Canvas = formCanvas;
        formWidth = Display_Main_Canvas.width();
        formHeight = Display_Main_Canvas.height();

        BTN_OK = displayButton();
        // OK ICON Button ===================
        BTN_OK.x = 160;
        BTN_OK.y = 187;
        BTN_OK.width = 160;
        BTN_OK.height = 52;
        BTN_OK.iconData = btn_OK;
        BTN_OK.enable = true;
        //=====================
    }

    void draw(float value, String text)
    {
        if (formEnable)
        {
            Display_Main_Canvas.fillScreen(BLACK);
            BTN_OK.draw(Display_Main_Canvas);
            Display_Main_Canvas.fillRect(0,0,320,180,TFT_WHITE);
            Display_Main_Canvas.qrcode(text, 50, 20, 140, 4);
            Display_Main_Canvas.pushSprite(0, 0);
        }
    }
    int touchCheck(m5::touch_detail_t t)
    {
        if (BTN_OK.contain(t))
        {
            return 1;
        }
        return 0;
    }
};

// =====================
// form_Config
// =====================
class form_Config : public form
{
private:
public:
    displayButton BTN_OK;
    displayButton BTN_CANCEL;

    int Enc_PPR;
    int Enc_TargetLength;
    int Enc_LPR;

    int Enc_PPR_MAX;
    int Enc_TargetLength_MAX;
    int Enc_LPR_MAX;

    int Enc_PPR_MIN;
    int Enc_TargetLength_MIN;
    int Enc_LPR_MIN;

    displayButtonLabel BTN_PPR;
    displayButtonLabel BTN_TargetLength;
    displayButtonLabel BTN_LPR;

    displaySlider SLD_value;

    displayButton BTN_UP;
    displayButton BTN_DOWN;

    form_Config()
    {
        formName = "ShutdownMessage";
        formEnable = true;
    }

    /// @brief Initialize Canvas / Button
    form_Config(M5Canvas formCanvas, int value)
    {
        formName = "Config";
        formEnable = true;

        Display_Main_Canvas = formCanvas;
        formWidth = Display_Main_Canvas.width();
        formHeight = Display_Main_Canvas.height();

        // OK ICON Button ===================
        BTN_OK = displayButton();
        BTN_OK.x = 0;
        BTN_OK.y = 187;
        BTN_OK.width = 160;
        BTN_OK.height = 52;
        BTN_OK.iconData = btn_OK;
        BTN_OK.enable = true;

        // CANCEL ICON Button ===================
        BTN_CANCEL = displayButton();
        BTN_CANCEL.x = 160;
        BTN_CANCEL.y = 187;
        BTN_CANCEL.width = 160;
        BTN_CANCEL.height = 52;
        BTN_CANCEL.iconData = btn_CANCEL;
        BTN_CANCEL.enable = true;
        // ================================

        // PPRBTN_PPR ICON Button ===================
        BTN_PPR = displayButtonLabel();
        BTN_PPR.x = 0;
        BTN_PPR.y = 0;
        BTN_PPR.r = 12;
        BTN_PPR.width = 320 / 3;
        BTN_PPR.height = 48;
        BTN_PPR.label = "PPR";
        BTN_PPR.enable = false;
        // ================================
        // PPRBTN_TargetLength ICON Button ===================
        BTN_TargetLength = displayButtonLabel();
        BTN_TargetLength.x = 320 / 3;
        BTN_TargetLength.y = 0;
        BTN_TargetLength.r = 12;
        BTN_TargetLength.width = 320 / 3;
        BTN_TargetLength.height = 48;
        BTN_TargetLength.label = "TRGT";
        BTN_TargetLength.enable = true;
        // ================================
        // PPRBTN_LPR ICON Button ===================
        BTN_LPR = displayButtonLabel();
        BTN_LPR.x = 320 / 3 * 2;
        BTN_LPR.y = 0;
        BTN_LPR.r = 12;
        BTN_LPR.width = 320 / 3;
        BTN_LPR.height = 48;
        BTN_LPR.label = "LPR";
        BTN_LPR.enable = false;
        // ================================

        // SLD_value displaySlider===================
        SLD_value = displaySlider();
        SLD_value.x = 10;
        SLD_value.y = 120;
        SLD_value.width = 320 / 4 * 3;
        SLD_value.height = 48;
        SLD_value.enable = true;

        // OK ICON Button ===================
        BTN_UP = displayButton();
        BTN_UP.x = 320 - 52;
        BTN_UP.y = 52;
        BTN_UP.width = 52;
        BTN_UP.height = 52;
        BTN_UP.iconData = icon_Right;
        BTN_UP.enable = true;

        // CANCEL ICON Button ===================
        BTN_DOWN = displayButton();
        BTN_DOWN.x = 320 - 52;
        BTN_DOWN.y = 104;
        BTN_DOWN.width = 52;
        BTN_DOWN.height = 52;
        BTN_DOWN.iconData = icon_Left;
        BTN_DOWN.enable = true;
        // ================================
    }

    String getModeTitle()
    {
        if (BTN_PPR.enable)
        {
            return "Pulses per Revolution";
        }
        else if (BTN_TargetLength.enable)
        {
            return "TargetLength";
        }
        else if (BTN_LPR.enable)
        {
            return "Length per Revolution";
        }
    }

    String getModeValue()
    {
        if (BTN_PPR.enable)
        {
            return String(Enc_PPR);
        }
        else if (BTN_TargetLength.enable)
        {
            return String(Enc_TargetLength);
        }
        else if (BTN_LPR.enable)
        {
            return String(Enc_LPR);
        }
    }

    void setModeValue(int value)
    {
        // Serial.println("setModeValue " + String(value));
        if (BTN_PPR.enable)
        {
            Enc_PPR = value;
        }
        else if (BTN_TargetLength.enable)
        {
            Enc_TargetLength = value;
        }
        else if (BTN_LPR.enable)
        {
            Enc_LPR = value;
        }
    }

    void ValueRefreshFromText(String text)
    {

        String str_Enc_PPR = getStringSplit(text, '\t', 0);
        String str_Enc_TargetLength = getStringSplit(text, '\t', 1);
        String str_Enc_LPR = getStringSplit(text, '\t', 2);

        Serial.println(str_Enc_PPR);

        Enc_PPR = str_Enc_PPR.toInt();
        Enc_TargetLength = str_Enc_TargetLength.toInt();
        Enc_LPR = str_Enc_LPR.toInt();
    }

    /// @brief
    /// @param value
    /// @param text Format: Enc_PPR'\t'Enc_TargetLength'\t'Enc_LPR'\t'viewFactor
    void draw(float value, String text)
    {
        if (text.length() > 3)
        {
            ValueRefreshFromText(text);
        }

        if (formEnable)
        {
            Display_Main_Canvas.fillScreen(BLACK);
            BTN_OK.draw(Display_Main_Canvas);
            BTN_CANCEL.draw(Display_Main_Canvas);
            BTN_PPR.draw(Display_Main_Canvas);
            BTN_TargetLength.draw(Display_Main_Canvas);
            BTN_LPR.draw(Display_Main_Canvas);

            BTN_UP.draw(Display_Main_Canvas);
            BTN_DOWN.draw(Display_Main_Canvas);

            // BTN_viewFactor.draw(Display_Main_Canvas);

            if (BTN_PPR.enable)
            {
                SLD_value.setValueMin(Enc_PPR_MIN);
                SLD_value.setValueMax(Enc_PPR_MAX);
                SLD_value.setValue(Enc_PPR);
            }
            else if (BTN_TargetLength.enable)
            {
                SLD_value.setValueMin(Enc_TargetLength_MIN);
                SLD_value.setValueMax(Enc_TargetLength_MAX);
                SLD_value.setValue(Enc_TargetLength);
            }
            else if (BTN_LPR.enable)
            {
                SLD_value.setValueMin(Enc_LPR_MIN);
                SLD_value.setValueMax(Enc_LPR_MAX);
                SLD_value.setValue(Enc_LPR);
            }

            Serial.println("form_Config::draw value= " + String(SLD_value.value) + "/max= " + String(SLD_value.valueMax) + "/min= " + String(SLD_value.valueMin));
            SLD_value.draw(Display_Main_Canvas);

            Display_Main_Canvas.setFont(&fonts::lgfxJapanGothic_24);
            Display_Main_Canvas.setTextColor(0xffd500);

            int textLineIndex = 0;
            int textLineXoffset = 10;
            int textLineStartHeight = 50;
            int textLineHeight = 24;

            Display_Main_Canvas.drawString(getModeTitle(), textLineXoffset, textLineHeight * textLineIndex + textLineStartHeight);
            textLineIndex++;
            Display_Main_Canvas.drawString(getModeValue(), textLineXoffset, textLineHeight * textLineIndex + textLineStartHeight);
            textLineIndex++;

            Display_Main_Canvas.pushSprite(0, 0);
        }
    }
    int touchCheck(m5::touch_detail_t t) override
    {
        if (BTN_OK.contain(t))
        {
            return 1;
        }
        else if (BTN_CANCEL.contain(t))
        {
            return 2;
        }
        else if (BTN_PPR.contain(t))
        {
            return 11;
        }
        else if (BTN_TargetLength.contain(t))
        {
            return 12;
        }
        else if (BTN_LPR.contain(t))
        {
            return 13;
        }
        else if (BTN_UP.contain(t))
        {
            return 21;
        }
        else if (BTN_DOWN.contain(t))
        {
            return 22;
        }
        else if (SLD_value.contain(t))
        {
            setModeValue(SLD_value.value);
            return 15;
        }
        return 0;
    }
};

#endif