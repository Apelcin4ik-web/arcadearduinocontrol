#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 
#include <FastBot.h> 
#include <EEPROM.h>

/* Установите здесь свои SSID и пароль */
const char* ssid = "wi-fi";       // SSID
const char* pass = "123456789";  // пароль

#define BOT_TOKEN "6073939960:AAE9k8enGsG0PxML7GJpmWVZ1JcB4A05I3Q" // Здесь введите Ваш Bot Token, сгенерированный Botfather
#define CHAT_ID "218474719,-918671620" // Здесь введите идентификатор пользователя Telegram (UserID)

int games = 0;

const int butShot = 14;  //№ пина для подключения реле счетчика

uint32_t tmr1;         // переменная таймера
int Gflag = 0;

// Инициализация Telegram BOT
FastBot bot(BOT_TOKEN);

void newMsg(FB_msg& msg) {
 // выводим всю информацию о сообщении
 Serial.println(msg.toString());
 if (msg.text == "/led_on") {
  digitalWrite(12, HIGH);
 bot.sendMessage("Автомат включен",CHAT_ID); // Светодиод включен
 }

 if (msg.text == "/led_on@ApelcinBot") {
  digitalWrite(12, HIGH);
 bot.sendMessage("Автомат включен",CHAT_ID); // Светодиод включен
 }

 if (msg.text == "/resetgames") {
      bot.sendMessage("Стираю стетчик игр...",CHAT_ID);
      EEPROM.put(0, 0);
      EEPROM.commit();
      bot.sendMessage("Счетчик сброшен!",CHAT_ID);
      EEPROM.get(0, games);
      String welcome = "Игр сыгранно ";
      welcome += games;
      welcome += "\n";
      bot.sendMessage(welcome, CHAT_ID);
    }

 if (msg.text == "/resetavtomat") {
      bot.sendMessage("Перезагружаю автомат...",CHAT_ID);
      digitalWrite(12, LOW);
      delay(30000);
      digitalWrite(12, HIGH);
      bot.sendMessage("Автомат перезагружен!",CHAT_ID);
    }

 if (msg.text == "/startgame") {
      bot.sendMessage("Включаю игру",CHAT_ID);
      digitalWrite(13, LOW);
      delay(500);
      digitalWrite(13, HIGH);
      bot.sendMessage("Игра началась!",CHAT_ID);
    }
  if (msg.text == "/startgame@ApelcinBot") {
      bot.sendMessage("Включаю игру",CHAT_ID);
      digitalWrite(13, LOW);
      delay(500);
      digitalWrite(13, HIGH);
      bot.sendMessage("Игра началась!",CHAT_ID);
    }
 if (msg.text == "/state") {
      EEPROM.get(0, games);
     String welcome = "Привет братка, Apelcin4ik \n";
      welcome += "Игр сыгранно ";
      welcome += games;
      welcome += "\n";
      bot.sendMessage(welcome, CHAT_ID);
    }
   if (msg.text == "/state@ApelcinBot") {
      EEPROM.get(0, games);
     String welcome = "Привет братка, Apelcin4ik \n";
      welcome += "Игр сыгранно ";
      welcome += games;
      welcome += "\n";
      bot.sendMessage(welcome, CHAT_ID);
    }
    
 if (msg.text == "/start") {
      String welcome = "Привет, Apelcin4ik \n";
      welcome += "Используй эти команды для управления.\n\n";
      welcome += "/led_on Автомат включен \n";
      welcome += "/led_off Автомат выключен \n";
      welcome += "/resetavtomat перезагрузка автомата \n";
      welcome += "/state просмотр статистики \n";
      welcome += "/resetgames сброс счетчика игр \n";
      bot.sendMessage(welcome, CHAT_ID);
    }
 if (msg.text == "/led_off") {
  digitalWrite(12, LOW);
 bot.sendMessage("Автомат выключен",CHAT_ID); // Светодиод выключен
 }

 if (msg.text == "/today") {
  bot.sendMessage("Команда полученна идет подсчет...",CHAT_ID); // Светодиод выключен
  today_data ();
 }

 if (msg.text == "/led_off@ApelcinBot") {
  digitalWrite(12, LOW);
 bot.sendMessage("Автомат выключен",CHAT_ID); // Светодиод выключен
 }
 
}

void setup() {
Serial.begin(115200);
EEPROM.begin(100);
connectWiFi();

  Serial.println("ESP IS ON!");
  Serial.println("Игр в памяти");
  EEPROM.get(0, games);
  Serial.println(games);
  bot.setChatID(CHAT_ID); // передайте "" (пустую строку) чтобы отключить проверку
  bot.attach(newMsg);
  bot.sendMessage("ESP запущен и готов к работе!"); 

pinMode(butShot, INPUT_PULLUP); 
digitalWrite(butShot, HIGH);
pinMode(LED_BUILTIN, OUTPUT);
digitalWrite(LED_BUILTIN, LOW);
pinMode(12, OUTPUT);
digitalWrite(12, HIGH);
pinMode(13, OUTPUT);
digitalWrite(13, HIGH);
pinMode(3, INPUT_PULLUP); 
digitalWrite(3, HIGH);


}

void loop() { 
  
 if (digitalRead(butShot) == LOW) {
    EEPROM.get(0, games);
    int sum = games + 1;
    EEPROM.put(0, sum);
    EEPROM.commit();
    EEPROM.get(0, games);
    Serial.println(games);
    send_data ();
    tmr1 = millis();
    Gflag = 1;
    Serial.println("+");
     }

bot.tick(); 

if (Gflag == 1){
if (millis() - tmr1 >= 3000) {  // ищем разницу
    tmr1 = millis();                   // сброс таймера
    if (digitalRead(3) == LOW){
      Gflag = 0;
        bot.sendMessage("игра включена",CHAT_ID); // Светодиод выключен
        Serial.println("-");
      }else{
        Gflag = 0;
        bot.sendMessage("Ебууучая монетница, жди звоночка бро!",CHAT_ID); // Светодиод выключен
        Serial.println("-");
      }
  }
}



}


void send_data ()
{
  WiFiClient client;
  if (!client.connect("188.53.138.27", 90)) \
  {
    Serial.println("connection failed");
    return;
  }
  String host = "188.53.138.27:90";
  String url = "/igra/post-data.php?";
  url += "game=";
  url += "1";
  url += "&pam=";
  url += games;

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  client.stop();
}

void today_data ()
{
  WiFiClient client;
  if (!client.connect("188.53.138.27", 90)) \
  {
    Serial.println("connection failed");
    return;
  }
  String host = "188.53.138.27:90";
  String url = "/igra/query-data.php";

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  client.stop();
}

void connectWiFi() {
 WiFi.begin(ssid, pass);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 if (millis() > 15000) ESP.restart();
 }
 Serial.println("Подключен");
}
