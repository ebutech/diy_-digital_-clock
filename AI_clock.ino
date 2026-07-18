/*
====================================================
TM1637 + DS3231 Digital Clock
12-Hour Format (AM/PM)
TM1637 Library + RTClib
====================================================
*/

#include <Wire.h>
#include <RTClib.h>
#include <TM1637.h>

//====================================================
// TM1637 Pins
//====================================================
#define CLK 2
#define DIO 3

TM1637 tm(CLK, DIO);
RTC_DS3231 rtc;

//====================================================
// Variables
//====================================================
bool colon = false;

void setup()
{
    Serial.begin(9600);

    //====================================================
    // Initialize TM1637 Display
    // Brightness range: 0 (Dim) - 7 (Bright)
    //====================================================
    tm.init();
    tm.set(2);

    //====================================================
    // Initialize RTC
    //====================================================
    if (!rtc.begin())
    {
        Serial.println("Couldn't find DS3231 RTC");
        while (1);
    }

    //====================================================
    // *************** SET THE DATE & TIME HERE ***************
    //
    // Format:
    //
    // rtc.adjust(DateTime(Year, Month, Day,
    //                     Hour, Minute, Second));
    //
    // IMPORTANT:
    // • Hour MUST be in 24-hour format.
    //
    // Examples:
    //
    // 8:30 AM
    // rtc.adjust(DateTime(2026, 7, 18, 8, 30, 0));
    //
    // 12:00 PM (Noon)
    // rtc.adjust(DateTime(2026, 7, 18, 12, 0, 0));
    //
    // 5:01 PM
    // rtc.adjust(DateTime(2026, 7, 18, 17, 1, 0));
    //
    // 11:45 PM
    // rtc.adjust(DateTime(2026, 7, 18, 23, 45, 0));
    //
    // Upload this sketch ONCE to set the RTC.
    // After the RTC has been set, comment out this line
    // so the clock keeps running normally.
    //====================================================

    rtc.adjust(DateTime(2026, 7, 18, 2, 36, 0));

    // After setting the RTC once, change it to:
    // // rtc.adjust(DateTime(2026, 7, 18, 17, 1, 0));

    Serial.println("RTC Time Set Successfully");
}

void loop()
{
    //====================================================
    // Read current date and time from RTC
    //====================================================
    DateTime now = rtc.now();

    int hour24 = now.hour();
    int minute = now.minute();
    int second = now.second();

    bool isPM = false;
    int hour12;

    //====================================================
    // Convert 24-hour time to 12-hour time
    //====================================================
    if (hour24 == 0)
    {
        hour12 = 12;
        isPM = false;
    }
    else if (hour24 < 12)
    {
        hour12 = hour24;
        isPM = false;
    }
    else if (hour24 == 12)
    {
        hour12 = 12;
        isPM = true;
    }
    else
    {
        hour12 = hour24 - 12;
        isPM = true;
    }

    //====================================================
    // Display HH:MM on TM1637
    //====================================================
    tm.display(0, hour12 / 10);
    tm.display(1, hour12 % 10);
    tm.display(2, minute / 10);
    tm.display(3, minute % 10);

    //====================================================
    // Blink the colon every second
    //====================================================
    colon = !colon;
    tm.point(colon);

    //====================================================
    // Print Date & Time to Serial Monitor
    //====================================================
    Serial.print(now.year());
    Serial.print('/');

    if (now.month() < 10)
        Serial.print('0');
    Serial.print(now.month());

    Serial.print('/');

    if (now.day() < 10)
        Serial.print('0');
    Serial.print(now.day());

    Serial.print("   ");

    if (hour12 < 10)
        Serial.print('0');
    Serial.print(hour12);

    Serial.print(':');

    if (minute < 10)
        Serial.print('0');
    Serial.print(minute);

    Serial.print(':');

    if (second < 10)
        Serial.print('0');
    Serial.print(second);

    Serial.print(" ");

    if (isPM)
        Serial.println("PM");
    else
        Serial.println("AM");

    //====================================================
    // Update every second
    //====================================================
    delay(1000);
}