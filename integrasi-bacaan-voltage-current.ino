/*
Program Sistem Monitoring Arus, Tegangan, Daya dari Solar Panel (Photo Voltaic)
Mata Kuliah Proyek Rekayasa Interdisiplin Kelompok 6
Penanggung Jawab = Muhammad Faiz Rahmadani - 5009201112
*/

#include <Adafruit_ADS1X15.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// #include <DHT.h>
// #include <dht.h>
// #include <BH1750.h>

Adafruit_ADS1115 ads;
LiquidCrystal_I2C lcd(0x27, 20, 4); // Set the LCD address and the number of columns and rows

// global variable for voltage monitoring
float modul;
float vol;
float vol_bat;
float vol_mean = 0;
float power;

// global variable for current monitoring
const float multi = 0.0078125F; // 256/65536
float mv;
const float ampermv = 1.33333333333;
float current;
float curtemp;

void voltmeter()
{
  int i = 0;
  ads.setGain(GAIN_ONE);
  // float read = analogRead(analogPin);
  int16_t adc2 = ads.readADC_SingleEnded(2);
  modul = adc2 * 0.125;
  vol = (modul / (10350.0 / (10350.0 + 216000))) / 1000;
  while (i < 100){
    int16_t adc2 = ads.readADC_SingleEnded(2);
    modul = adc2*0.125;
    vol = (modul / (10350.0/(10350.0+216000)))/1000;
    vol_mean = vol_mean + vol;
    i++;
  }
  vol_mean = vol_mean/100;
  // lcd.clear(); // Clear the LCD screen
  lcd.setCursor(0, 0);
  lcd.print("Tegangan = ");
  lcd.print(vol_mean, 2);
  lcd.print(" volt");
  delay(500);
}

void amperemeter()
{
  current = 0;
  int i = 0;
  ads.setGain(GAIN_SIXTEEN);
  int16_t count;
  count = ads.readADC_Differential_0_1();
  mv = count * multi;
  current = abs(mv * ampermv) + 0.055;
  i++;
  while (i <= 10)
  {
    count = ads.readADC_Differential_0_1();
    mv = count * multi;
    //  current = abs(mv*ampermv)-intcurrent;
    curtemp = abs(mv * ampermv) + 0.055;
    current = current + curtemp;
    i++;
  }
  current = current / 10;
  i = 0;
  // Serial.println(current);
  // lcd.clear(); // Clear the LCD screen
  lcd.setCursor(0, 1);
  lcd.print("Arus = ");
  lcd.print(current, 2);
  lcd.print(" A");
  delay(500);
}

void watt()
{
  float power = vol * current;
  lcd.setCursor(0, 2);
  lcd.print("Daya = ");
  lcd.print(power, 2);
  lcd.print(" Watt");
  delay(500);
}


void setup()
{
  // put your setup code here, to run once
  lcd.init();      // Initialize the LCD
  lcd.backlight(); // Turn on the backlight
  Serial.begin(9600);
  ads.begin();

  // pinMode(analogPin, INPUT); // voltage reading input
  Serial.println("Monitoring System Start....");
  delay(500);
}

void loop()
{
  // put your main code here, to run repeatedly:
  amperemeter();
  voltmeter();
  watt();
  float power_print = vol * current;
  Serial.print(vol);
  Serial.print(",");
  Serial.print(current);
  Serial.print(",");
  Serial.println(power_print);
}
