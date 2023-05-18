#include <Arduino.h>
#include <M5Unified.h>
#include <EEPROM.h>
#include "displayControl.hpp"
#include "forms.hpp"

/// @brief signal pin assign A-Phase
const int Pin_signalA = 32;

/// @brief signal pin assign B-Phase
const int Pin_signalB = 33;

/// @brief Encorder Count
int Enc_Count = 0;

int Enc_CountLast = 0;

/// @brief Encorder Profile Struct
struct DATA_SET
{
  /// @brief Encorder resolustion
  int Enc_PPR;

  /// @brief WheelDiameter[mm]
  int Enc_LPR;

  /// @brief TargetLength[mm]
  int Enc_TargetLength;
};
/// @brief Encorder Profile
DATA_SET data;

// icon data
extern unsigned char icon_Setting[2143];
extern unsigned char icon_Power[1624];
extern unsigned char icon_M5LOGO[13611];
extern unsigned char icon_Save[1624];

extern unsigned char btn_OK[1859];
extern unsigned char btn_CANCEL[2815];
extern unsigned char btn_Right[1407];
extern unsigned char btn_Left[1417];

extern unsigned char btn_RESET[1698];
extern unsigned char icon_QR[1661];

form *FormView;
form_Top Form_Top;
form_ShutdownMessage Form_ShutdownMessage;
form_SaveMessage Form_SaveMessage;
form_Config Form_Config;
form_QR Form_QR;

/// @brief Main Display
M5GFX Display_Main;
M5Canvas Display_Main_Canvas(&Display_Main);

bool A;
bool B;

/// @brief Encorder phase History
byte Enc_Log;

/// @brief Encorder phase History String
String Enc_Log_String = "";

String EncLogToString()
{
  switch (Enc_Log)
  {
  case B00000001:
    Enc_Log_String = "0001";
    break;
  case B00000111:
    Enc_Log_String = "0111";
    break;
  case B00001110:
    Enc_Log_String = "1110";
    break;
  case B00001000:
    Enc_Log_String = "1000";
    break;
  case B00000010:
    Enc_Log_String = "0010";
    break;
  case B00001011:
    Enc_Log_String = "1011";
    break;
  case B00001101:
    Enc_Log_String = "1101";
    break;
  case B00000100:
    Enc_Log_String = "0100";
    break;
  }
  return Enc_Log_String;
}

void taskGetEncoder(void *pvParameters)
{
  while (1)
  {
    A = digitalRead(Pin_signalA);
    B = digitalRead(Pin_signalB);

    Enc_Log = (Enc_Log << 2) & B00001111;
    Enc_Log = Enc_Log + (A << 1) + B;

    Enc_Log_String = EncLogToString();

    switch (Enc_Log)
    {
    case B00000001:
    case B00000111:
    case B00001110:
    case B00001000:
      if (Enc_Count < INT_MAX)
      {
        ++Enc_Count;
        // Serial.printf("\n[%d%d] %d",A,B,Enc_Count);
      }
      break;
    case B00000010:
    case B00001011:
    case B00001101:
    case B00000100:
      if (Enc_Count > INT_MIN)
      {
        --Enc_Count;
        // Serial.printf("\n[%d%d] %d",A,B,Enc_Count);
      }
    }
    //    taskYIELD();
  }
}

void InitializeComponent()
{
  M5.Lcd.printf("Start MainMonitor Initialize\r\n"); // LCDに表示

  // Main Display initialize
  M5.setPrimaryDisplayType(m5gfx::board_M5StackCore2);

  // M5GFX initialize
  Display_Main = M5.Display;
  int w = Display_Main.width();
  int h = Display_Main.height();

  // Create sprite for MainDisplay
  Display_Main_Canvas.createSprite(w, h);
  Display_Main_Canvas.setTextColor(0xffd500);
  Display_Main_Canvas.setFont(&fonts::lgfxJapanGothic_12);
  Display_Main_Canvas.setTextColor(0xffd500);
}

void setup()
{

  pinMode(Pin_signalA, INPUT_PULLUP);
  pinMode(Pin_signalB, INPUT_PULLUP);

  EEPROM.begin(50); // 50byte
  EEPROM.get<DATA_SET>(0, data);
  if (data.Enc_PPR <= 0)
  {
    data.Enc_PPR = 96;
    data.Enc_TargetLength = 6000;
    data.Enc_LPR = 201;
  }

  auto cfg = M5.config();
  cfg.serial_baudrate = 19200;
  M5.begin(cfg);

  M5.Lcd.setCursor(0, 0);

  Serial.println("\nInitializeComponent");
  M5.Lcd.setBrightness(50);
  InitializeComponent();

  xTaskCreatePinnedToCore(taskGetEncoder, "Task0", 4096, NULL, 1, NULL, 0);

  // stop watch dog timer : core0
  disableCore0WDT();

  Form_Top = form_Top(Display_Main_Canvas, 0);
  Form_ShutdownMessage = form_ShutdownMessage(Display_Main_Canvas, 0);
  Form_SaveMessage = form_SaveMessage(Display_Main_Canvas, 0);
  Form_Config = form_Config(Display_Main_Canvas, 0);
  Form_QR = form_QR(Display_Main_Canvas, 0);

  FormView = &Form_Top;

  FormView->formEnable = true;
  FormView->draw(0, "");

  M5.Lcd.setCursor(0, 0);

  Serial.println("\n" + getStringSplit("ABC\tDE\tF\t", '\t', 2));
  Serial.println("\n" + getStringSplit("0\t1111\t22\t3333\t44\t55555", '\t', 3));
}

static m5::touch_state_t prev_state;
static constexpr const char *state_name[16] =
    {"none", "touch", "touch_end", "touch_begin", "___", "hold", "hold_end", "hold_begin", "___", "flick", "flick_end", "flick_begin", "___", "drag", "drag_end", "drag_begin"};

int prev_x = 0;
int prev_y = 0;
int prev_v = 0;

void loop()
{
  M5.update();

  auto count = M5.Touch.getCount();
  if (count)
  {
    static m5::touch_state_t prev_state;
    auto t = M5.Touch.getDetail();
    if (prev_state != t.state)
    {
      prev_state = t.state;

      if (prev_x != t.x || prev_y != t.y)
      {
        prev_x = t.x;
        prev_y = t.y;

        int touchIndex = FormView->touchCheck(t);

        //================
        // Form_Top
        //================
        if (touchIndex && FormView == &Form_Top)
        {
          Serial.println("\nForm_Top tIndex = " + String(touchIndex));
          switch (touchIndex)
          {
          case 1:
            FormView = &Form_ShutdownMessage;
            FormView->draw(0, "");
            break;
          case 2:
            FormView = &Form_Config;

            Form_Config.Enc_LPR_MIN = data.Enc_LPR / 2;
            Form_Config.Enc_LPR_MAX = data.Enc_LPR * 2;

            Form_Config.Enc_PPR_MIN = data.Enc_LPR / 4;
            Form_Config.Enc_PPR_MAX = data.Enc_LPR * 4;

            Form_Config.Enc_TargetLength_MIN = (int)(data.Enc_TargetLength / 2.0);
            Form_Config.Enc_TargetLength_MAX = (int)(data.Enc_TargetLength * 2.0);

            FormView->draw(0, String(data.Enc_PPR) + "\t" + String(data.Enc_TargetLength) + "\t" + String(data.Enc_LPR));
            break;
          case 3:
            Enc_Count = 0;
            FormView->draw(0, "");
            break;
          case 4:
            FormView = &Form_QR;
            FormView->draw(0, String(data.Enc_LPR * Enc_Count / data.Enc_PPR / 1000.0));
            break;

          default:
            FormView = &Form_Top;
            FormView->draw(0, "");
          }

          touchIndex = 0;
        }
        else if (touchIndex && FormView == &Form_QR)
        {
          switch (touchIndex)
          {
          case 1:
            FormView = &Form_Top;
            Enc_CountLast = 0;
            FormView->draw(0, "");
            break;
          default:
            FormView = &Form_Top;
            FormView->draw(0, "");
          }
          touchIndex = 0;
        }
        else if (touchIndex && FormView == &Form_ShutdownMessage)
        {
          Serial.println("\nForm_ShutdownMessage tIndex = " + String(touchIndex));
          switch (touchIndex)
          {
          case 1:
            M5.Power.powerOff();
            break;
          case 2:
            FormView = &Form_Top;
            FormView->draw(0, "");
            break;
          default:
            FormView = &Form_Top;
            FormView->draw(0, "");
          }

          touchIndex = 0;
        }
        else if (touchIndex && FormView == &Form_SaveMessage)
        {
          switch (touchIndex)
          {
          case 1:
            data.Enc_LPR = Form_Config.Enc_LPR;
            data.Enc_PPR = Form_Config.Enc_PPR;
            data.Enc_TargetLength = Form_Config.Enc_TargetLength;

            EEPROM.put<DATA_SET>(0, data);
            EEPROM.commit();
          case 2:
            FormView = &Form_Top;
            FormView->draw(0, "");
            break;
          default:
            FormView = &Form_Top;
            FormView->draw(0, "");
          }
          touchIndex = 0;
        }
        else if (touchIndex && FormView == &Form_Config)
        {
          switch (touchIndex)
          {
          case 1:
            FormView = &Form_SaveMessage;
            FormView->draw(0, "");
            break;
          case 2:
            FormView = &Form_Top;
            FormView->draw(0, "");
            break;
          case 11:
            Form_Config.BTN_PPR.enable = true;
            Form_Config.BTN_TargetLength.enable = false;
            Form_Config.BTN_LPR.enable = false;
            // Form_Config.BTN_viewFactor.enable = false;
            FormView->draw(0, String(data.Enc_PPR) + "\t" + String(data.Enc_TargetLength) + "\t" + String(data.Enc_LPR));
            Serial.println("11");
            break;
          case 12:
            Form_Config.BTN_PPR.enable = false;
            Form_Config.BTN_TargetLength.enable = true;
            Form_Config.BTN_LPR.enable = false;
            // Form_Config.BTN_viewFactor.enable = false;
            FormView->draw(0, String(data.Enc_PPR) + "\t" + String(data.Enc_TargetLength) + "\t" + String(data.Enc_LPR));
            Serial.println("12");
            break;
          case 13:
            Form_Config.BTN_PPR.enable = false;
            Form_Config.BTN_TargetLength.enable = false;
            Form_Config.BTN_LPR.enable = true;
            // Form_Config.BTN_viewFactor.enable = false;
            FormView->draw(0, String(data.Enc_PPR) + "\t" + String(data.Enc_TargetLength) + "\t" + String(data.Enc_LPR));
            Serial.println("13");

            break;
          case 14:
            Form_Config.BTN_PPR.enable = false;
            Form_Config.BTN_TargetLength.enable = false;
            Form_Config.BTN_LPR.enable = false;
            // Form_Config.BTN_viewFactor.enable = true;
            FormView->draw(0, String(data.Enc_PPR) + "\t" + String(data.Enc_TargetLength) + "\t" + String(data.Enc_LPR));
            Serial.println("14");

            break;
          case 15:
            Serial.println("15");
            data.Enc_LPR = Form_Config.Enc_LPR;
            data.Enc_PPR = Form_Config.Enc_PPR;
            data.Enc_TargetLength = Form_Config.Enc_TargetLength;
            FormView->draw(0, String(data.Enc_PPR) + "\t" + String(data.Enc_TargetLength) + "\t" + String(data.Enc_LPR));
            break;
          case 21:
            Form_Config.setModeValue((Form_Config.getModeValue()).toInt() + 1);
            FormView->draw(0, "");
            break;
          case 22:
            Form_Config.setModeValue((Form_Config.getModeValue()).toInt() - 1);
            FormView->draw(0, "");
            break;

          default:
            FormView->draw(0, String(data.Enc_PPR) + "\t" + String(data.Enc_TargetLength) + "\t" + String(data.Enc_LPR));
            Serial.println("default");
          }

          touchIndex = 0;
        }
      }
    }
  }

  if(Enc_CountLast != Enc_Count)
  {
    Enc_CountLast = Enc_Count;
    float countValue = data.Enc_LPR * Enc_Count / data.Enc_PPR;
    int RemainTime = (data.Enc_TargetLength - countValue) / data.Enc_LPR;

    String Message = "";
    if (data.Enc_TargetLength > 0)
    {
      Message = "Last " + String(RemainTime);
    }

    //if (FormView == &Form_Top && prev_v != countValue)
    if (FormView == &Form_Top)
    {
      //prev_v = countValue;
      // FormView->draw(countValue / 1000.0, String(A) + "," + String(B) + " " + EncLogToString());
      FormView->draw(countValue / 1000.0, Message);
    }
  }

  delay(50);
}