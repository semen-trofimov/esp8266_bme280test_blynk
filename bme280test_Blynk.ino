/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution

***************************************************************************
  Это библиотека для датчика влажности, температуры и давления BME280

GOOGL переводчик
   Разработан специально для работы с Adabruit BME280 Breakout
   ----> http://www.adafruit.com/products/2650

   Эти датчики используют I2C или SPI для связи, требуется 2 или 4 контакта
   к интерфейсу. Адрес устройства I2C равен 0x76 или 0x77.

   Adafruit инвестирует время и ресурсы, предоставляя этот открытый исходный код,
   пожалуйста, поддержите оборудование Adafruit andopen-source путем приобретения продуктов
   от Адафрута!

   Написано Limor Fried & Kevin Townsend для Adafruit Industries.
   Лицензия BSD, весь текст, указанный выше, должен быть включен в любое перераспределение
 ***************************************************************************/
/***************************************************************************
 * 
 * Автор Перевода: Обушенков Алексей Андреевич    
 * Группа в ВК https://vk.com/engineer24
 * Канал на YouTube https://www.youtube.com/channel/UCih0Gcl9IEdkR8deDJCiodg
 * Инженерка Engineering room
 * 
 ***************************************************************************
 * Модули в составе проекта:
 *  ESP8266 - 12E WIFI RobotDyn (3.68 $) https://goo.gl/k6TRUz
 *  ESP8266 - 12E WIFI GREAT WALL (3.44 $) https://goo.gl/DcqYMg
 * 
 * Датчик Температуры, Влажности и Давления:
 *  GY-BME280-3.3 (3.78 $)https://goo.gl/T95nQ5
 ***************************************************************************/

//*************************************************************************
// BME 280
#include <Wire.h>
//#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//  Подключается BME 280 по I2C:
//  Плата ESP-12E подключаем SCL - D1(GPIO5) // SDA - D2(GPIO4)
//  Плата Arduino UNO (NANO) SCL - А5 // SDA - А4

 // Если по SPI то вот для Ардуины
//#define BME_SCK 13
//#define BME_MISO 12
//#define BME_MOSI 11
//#define BME_CS 10

// Это стандартное давление для вашей местности в гектопаскаль
// Гуглим стандартное давление для своего города
// и переводим в гектопаскаль
// https://www.unitjuggler.com/перевод-pressure-из-mmHg-в-hPa.html?val=740
#define SEALEVELPRESSURE_HPA (986.5828) //1013.25 (760мм.рт.ст. = 1013.25 гПа (hPa))

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI



//*************************************************************************
// Blynk

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "6b5c3d556cea42c5b7843708a0007830";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "asus";
char pass[] = "t9indigo";

BlynkTimer timer;
unsigned long delayTime = 1000; // Частота вывода в виджет


//*************************************************************************
// Код программы

// Функция для вывода данных в пять виджетов Lable Vaule
void sendSensor()
{
  
// Температура
  Blynk.virtualWrite(V0, bme.readTemperature(), " *С");
  
// Давление в милиметрах ртутного столба
  Blynk.virtualWrite(V1, (bme.readPressure() / 133.3F), " мм.рт.ст.");

// Влажность  
  Blynk.virtualWrite(V2, bme.readHumidity(), " %");

// Высота (Расчетная и не очень точная) это не высотомер  
  Blynk.virtualWrite(V3, bme.readAltitude(SEALEVELPRESSURE_HPA), " м");

}//sendSensor



void setup() {
    Serial.begin(9600);

// Запускаем датчик BME 280
    Serial.println(F("BME280 test"));    
    bool status;
    
// Здесь вводим адрес устройства
// По умолчанию адрес i2c равен 0x77. 
// Если вы добавите перемычку из SDO в V3.3, адрес изменится на 0x77.
// Если вы добавите перемычку из SDO в GND, адрес изменится на 0x76.
    status = bme.begin(0x76);  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    bme.setSampling();
    Serial.println("-- Default Test --");
    Serial.println();

//*******************************************************
// Запускаем Blynk
  Blynk.begin(auth, ssid, pass);

// Настраиваем таймеры  
  
  timer.setInterval(delayTime, sendSensor);

// Настраиваем виджеты
  Blynk.setProperty(V0, "label", "Температура");
  Blynk.setProperty(V1, "label", "Давление");
  Blynk.setProperty(V2, "label", "Влажность");
  Blynk.setProperty(V3, "label", "Высота");
  
}//setup



void loop() { 
    
    Blynk.run();
    timer.run();
}//loop
