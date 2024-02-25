#include <Arduino.h>
#include <string.h>

const byte sigPin = A1; // Pin kết nối với SIG của CD74HC4067
const byte s0 = 8;
const byte s1 = 9;
const byte s2 = 19;
const byte s3 = 11;

const int maxButton = 12;
// const String keyBoard[] = {"Ctrl_A", "Ctrl_C", "Ctrl_V", "Alt_Shift_F", "Ctrl_Shift_T", "roll_up", "roll_down", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
const String keyBoard[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14"};

// Mảng để lưu thời gian bắt đầu giữ nút
unsigned long startTime[maxButton];
const long holdTime = 1000; // Thời gian giữ nút tối thiểu để bắt đầu "spam" (200ms)

// Mảng để lưu trạng thái hiện tại của nút (đang giữ hay không)
bool buttonHeld[maxButton];

void setup()
{
    Serial.begin(115200);
    pinMode(sigPin, INPUT);
    pinMode(s0, OUTPUT);
    pinMode(s1, OUTPUT);
    pinMode(s2, OUTPUT);
    pinMode(s3, OUTPUT);
    digitalWrite(sigPin, 0);
    for (int i = 0; i <= maxButton; ++i)
    {
        startTime[i] = 0;
        buttonHeld[i] = false;
    }
}

void loop()
{
    for (int i = 0; i <= maxButton; ++i)
    {
        set_Channel(i);
        delay(10); // Thời gian ổn định tín hiệu

        // int buttonState = analogRead(sigPin);
        int buttonState = digitalRead(sigPin);
        // map(buttonState, 0, 1023, 0, 1);

        unsigned long currentTime = millis();

        if (buttonState && !buttonHeld[i])
        {
            // Nút được giữ xuống lần đầu
            unsigned long currentTime = millis();

            startTime[i] = currentTime;
            buttonHeld[i] = true;
            println_Key(i, "ON");
        }
        else if (!buttonState && buttonHeld[i])
        {
            // Nút được thả ra
            buttonHeld[i] = false;
            // println_Key(i, "OFF");
        }
        else if (buttonState && buttonHeld[i] && (currentTime - startTime[i]) > holdTime)
        {
            // Nút được giữ quá 200ms, bắt đầu "spam"
            println_Key(i, "ON");
        }
    }
}

void println_Key(byte channel, const char *state)
{
    Serial.print(F("Channel "));
    Serial.print(channel);
    Serial.print(F(" : ----> "));
    Serial.print(state);
    Serial.print(F(" ----> "));
    Serial.println(keyBoard[channel]);
}

void set_Channel(byte channel)
{
    digitalWrite(s0, bitRead(channel, 0));
    digitalWrite(s1, bitRead(channel, 1));
    digitalWrite(s2, bitRead(channel, 2));
    digitalWrite(s3, bitRead(channel, 3));
}
