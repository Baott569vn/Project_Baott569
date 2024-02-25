#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Arduino.h>
#include <string.h>
#include "customCharacters.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define ENCODER_CLK 6
#define ENCODER_DT 7
#define ATTINY85_I2C_ADDRESS 0x04

#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const byte X1 = 8; // Pin kết nối với SIG của CD74HC4067
const byte X2 = 9; // Pin kết nối với SIG của CD74HC4067
const byte A = 2;
const byte B = 3;
const byte C = 4;

byte maxButton = 16;

// Mảng để lưu thời gian bắt đầu giữ nút
unsigned long startTime[16];
const long holdTime = 400; // Thời gian giữ nút tối thiểu để bắt đầu "spam"

// Mảng để lưu trạng thái hiện tại của nút (đang giữ hay không)
bool buttonHeld[16];

void setup()
{
    Serial.begin(115200);
    Wire.begin(); // Khởi động bus I2C như master
    pinMode(X1, INPUT);
    pinMode(X2, INPUT);
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(ENCODER_CLK, INPUT);
    pinMode(ENCODER_DT, INPUT);

    for (byte i{0}; i < maxButton; ++i)
    {
        startTime[i] = 0;
        buttonHeld[i] = false;
    }

    // check oled on
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }
    delay(500);
    display.clearDisplay();
    // Draw bitmap on the screen
    display.drawBitmap(0, 0, pic_Bitmap, 128, 64, 1);
    display.display();
    delay(1000);
    display.clearDisplay();
    display.display();
    // for text oled
    display.setTextColor(WHITE);
}

int lastClk = HIGH;

void loop()
{
    set_Channel(X1, 0);
    set_Channel(X2, 8);
}

void set_key(byte current_IC, byte i)
{
    static int munber_Held = 1;
    int buttonState = digitalRead(current_IC);
    unsigned long currentTime = millis();

    // encoder
    int newClk = digitalRead(ENCODER_CLK);
    if (newClk != lastClk)
    {
        // Có sự thay đổi ở chân CLK
        lastClk = newClk;
        int dtValue = digitalRead(ENCODER_DT);
        if (newClk == LOW && dtValue == HIGH)
        {
            println_Key(15, "ON", &munber_Held);
        }
        if (newClk == LOW && dtValue == LOW)
        {
            println_Key(16, "ON", &munber_Held);
        }
    }

    // button
    if (current_IC == 9 && (i - 8) == 6)
    {
        if (buttonState == 0)
        {
            println_Key(14, "ON", &munber_Held);
        }
        else
        {
            display.clearDisplay();
            display.display();
        }
        return;
    }

    if (buttonState && !buttonHeld[i])
    {
        // Nút được giữ xuống lần đầu
        unsigned long currentTime = millis();
        startTime[i] = currentTime;
        buttonHeld[i] = true;
        println_Key(i, "ON", &munber_Held);
    }
    else if (!buttonState && buttonHeld[i])
    {
        // Nút được thả ra
        buttonHeld[i] = false;
        // println_Key(i, "OFF");
        munber_Held = 1;
    }
    else if (buttonState && buttonHeld[i] && (currentTime - startTime[i]) > holdTime)
    {
        // Nút được giữ quá, bắt đầu "spam"
        munber_Held++;
        println_Key(i, "ON", &munber_Held);
    }
}

void println_Key(byte channel, const char *state, int *P_munber_Held)
{
    // Serial.print(F("Channel "));
    // Serial.print(channel);
    // Serial.print(F(" : ----> "));
    // Serial.print(state);
    // Serial.print(F(" ----> "));
    // Serial.println(keyBoard[channel]);
    send_ToAttiny(channel);
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(20, 0);
    display.println(keyBoard[channel]);
    display.setTextSize(1);
    display.setCursor(64, 20);
    display.println((*(P_munber_Held)));
    display.display();
}

void set_Channel(byte channel, byte ic_2)
{
    byte currentMaxbutton = channel == 8 ? 8 : 9;

    for (byte i{0}; i < maxButton - currentMaxbutton; ++i)
    {
        digitalWrite(A, bitRead(i, 0));
        digitalWrite(B, bitRead(i, 1));
        digitalWrite(C, bitRead(i, 2));
        delay(5); // Thời gian ổn định tín hiệu
        set_key(channel, i + ic_2);
    }
}

void send_ToAttiny(byte channel) {
  Wire.beginTransmission(ATTINY85_I2C_ADDRESS);
  Wire.write(channel); // Gửi num
  Wire.endTransmission();
}