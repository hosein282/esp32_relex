#include <LittleFS.h>
#include "base64.hpp"
#include <stdint.h>
#include <iostream>

#define DEVICE_MODEL "G84-TR"

const short VERSION = 2;

#include <WiFi.h>
// #include <ESPmDNS.h>

#include "hex.h"
#include <ESPAsyncWebServer.h>
#include <HTTPUpdate.h>
#include "index.h"


//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
#include <ArduinoJson.h>
#include <HardwareSerial.h>
#include <esp_task_wdt.h>


// #include <SoftwareSerial.h>
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// #include <EEPROM.h>
#include <Preferences.h>
Preferences EEPROM;
Preferences REMOTES;

#include <Ticker.h>  //Ticker Library
#include <ESP32Time.h>
// #include <ESPSupabaseRealtime.h>
// #include <ESPSupabase.h>
#include <PubSubClient.h>

const char *mqtt_server = "broker.emqx.io";

// #include <ArduinoOTA.h>

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);


#include "time.h"
//MMMMMMMMMMMMMMMMMMMMMMMMMMM Temprature ds18b20
#include <OneWire.h>
#include <DallasTemperature.h>
// GPIO where the DS18B20 is connected to
const uint8_t oneWireBus = 23;
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

//MMMMMMMMMMMMMMMMMMMMMMMMMMM 433MHZ receiver
#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();

//MMMMMMMMMMMMMMMMMMMMMMMMMMM AES

//MMMMMMMMMMMMMMMMMMMMMMMMMMM BLE
const char *ssid_ap = "RELEX-";
String ssid = "";
String password = "";
String mac = "";
String m_server = "";
String sub_topic = "action";
String pub_topic = "report";

// File paths to save input values permanently
const char *wifi_ssid = "/ssid.txt";
const char *wifi_pass = "/password.txt";
const char *m_server_k = "/server.txt";
const char *mac_f = "/mac.txt";

//sender phone number with country code
const uint8_t totalPhoneNo = 5;
const uint8_t totalPwm = 2;
const uint8_t totalOutputs = 6;
const uint8_t totalInputs = 2;
const uint8_t totalRemotes = 24;
const uint8_t totalTemps = 3;
const uint8_t totalAnalogs = 2;

uint8_t signalQuality = 0;


float temps[3] = { 0.5, 0.0, 0.0 };



// float temp0;
// float temp1;
// float temp2;

char *TCI_CHARGE = "AT+CUSD=1,\"*140*11#\"";
char *IRANCEL_CHARGE = "AT+CUSD=1,\"*140*121#\"";
char *RIGHTEL_CHARGE = "AT+CUSD=1,\"*140*121#\"";

struct schedule {
  char key[4];
  String value;
};





struct output {
  char labelKey[5];
  char timerKey[5];
  uint8_t gpio;
  boolean state;
  String label;
  String timer;
  uint8_t pwm;
};

struct Scenario {
  char key[4];
  String value;
  int8_t input;
  String condition;  // نوع شرط: ">", "<", "=="
  float threshold;   // مقدار آستانه
  int8_t outPin;     // پین خروجی
  int8_t outState;   // حالت خروجی: HIGH یا LOW
};
const int totalScenarios = 4;  // حداکثر تعداد سناریوها
int scenariosCount = 0;        //  تعداد سناریوها

Scenario scenarios[totalScenarios] = {
  // { "s1",
  //   "",
  //   -1,
  //   "",
  //   0.0,
  //   -1,
  //   0 },
  // { "s2",
  //   "",
  //   -1,
  //   "",
  //   0.0,
  //   -1,
  //   0 },
  // { "s3",
  //   "",
  //   -1,
  //   "",
  //   0.0,
  //   -1,
  //   0 },

  // { "s4",
  //   "",
  //   -1,
  //   "",
  //   0.0,
  //   -1,
  //   0 }
};


String phoneNo[totalPhoneNo] = { "", "", "", "", "" };

int8_t toggleTimers[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
// String schTime[4] = { "", "", "", "" };

// String inputs[4] = { "", "", "", "" };

//in 18,39,34,35,19,21,22,-23
//  out 14,12,13,-15,2,33,26,*27
/// bahman 36,39,34,35,    //18,19,21,22

struct Input {
  char labelKey[4];
  char key[4];
  String label;
  float voltage;
  uint8_t state;
  uint8_t gpio;
};


Input inputs[2] = {
  { "il3", "in3", "", 0.0, 0, 34 },
  { "il4", "in4", "", 0.0, 0, 35 }
};

Input analogInputs[2] = {
  { "al1", "a1", "", 0.0, 0, 36 },
  { "al2", "a2", "", 0.0, 0, 39 }

};

// String labels[4] = { "", "", "", "" };

// int relays[4] = { 26, 25, 32, 33 };
/// bahman 32,33,25,26,27,14,4,13

output outputs[totalOutputs] = {
  {
    "lab1",
    "sch1",
    26,
    1,
    "",
    "",
  },
  {
    "lab2",
    "sch2",
    15,
    1,
    "",
    "",
  },
  {
    "lab3",
    "sch3",
    32,
    1,
    "",
    "",
  },
  {
    "lab4",
    "sch4",
    27,
    1,
    "",
    "",
  },
  {
    "lab5",
    "sch5",
    14,
    1,
    "",
    "",
  },
  {
    "lab6",
    "sch6",
    19,
    1,
    "",
    "",
  },
  // {
  //   "lab7",
  //   "sch7",
  //   32,
  //   1,
  //   "",
  //   "",
  // },
  // {
  //   "lab8",
  //   "sch8",
  //   33,
  //   1,
  //   "",
  //   "",
  // }
};


const int freq = 3000;  //3610

output pwms[totalPwm] = {
  { "pwm1",
    "pS1",
    13,
    1,
    "",
    "",
    0 },
  { "pwm2",
    "pS2",
    18,
    1,
    "",
    "",
    0 }
};




const char *offsetPhone[totalPhoneNo] = { "p1", "p2", "p3", "p4", "p5" };  // 13


const char *offsetDateTime = "date";
const char *offsetStates = "states";  // 4

uint minCounter = 0;
String tempLabel = "";
String tempPhone = "";
String tempSch = "";

unsigned int deviceYear = 0;

String op = "";
String buffer;

#define MQTT_MAX_PACKET_SIZE 1024
#define SCH_TASK_TIME 60000
unsigned int aResolution = 4095;
unsigned int analog_read_threshold = 250;
float temp_threshold = 1.5;
unsigned int signal_threshold = 2;
unsigned long prevTaskTime = 0;
unsigned long prevRfTime = 0;

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncEventSource events("/events");


// a string to hold NTP server to request epoch time
// const char *ntpServer = "europe.pool.ntp.org";
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 12600;
const int daylightOffset_sec = 0;
// Variable to hold current epoch timestamp
unsigned long Epoch_Time;

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

Ticker flipper;

//ESP32Time rtc;bu
ESP32Time rtc(0);  // offset in seconds GMT+1

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
// #include "Cipher.h"

// #include <CryptoAES_CBC.h>
// #include <AES.h>
//key[16] cotain 16 byte key(128 bit) for encryption
// char *key = "#dsa123do%Fsd!~a";

// const byte aes_key[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
// const byte aes_iv[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

// AES128 aes128;

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

#define SHORT_PRESS_TIME 2000      // 30000 milliseconds
#define LONG_PRESS_TIME 5000       // 30000 milliseconds
#define VERY_LONG_TIME 8000        // 30000 milliseconds
#define SUPER_LONG_TIME 12000      // 30000 milliseconds
#define MQTT_REPORT_INTERVAL 3000  // 30000 milliseconds

//GSM Module RX pin to Arduino 3
//GSM Module TX pin to Arduino 2
#define rxPin 16  //1
#define txPin 17  //2

// #define SENSOR_1 18
// #define SENSOR_2 19
// #define SENSOR_3 21
// #define SENSOR_4 22

#define BUTTON_PIN 33
#include <Bounce2.h>
Bounce2::Button button = Bounce2::Button();

#define RESET_GSM 25

// #define BUILTIN_LED 32

#define STATUS_LED 2

String outStates = "";
String inStates = "";
String pwmStates = "";


// boolean STATE_RELAY_1 = 0;
// boolean STATE_RELAY_2 = 0;
// boolean STATE_RELAY_3 = 0;
// boolean STATE_RELAY_4 = 0;
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
String smsStatus, senderNumber, receivedDate, msg, date = "";
boolean gsmIsReady = false;

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

boolean DEBUG_MODE = 1;

/// setting variables
bool hasGSM = false;
bool hasWifi = false;
bool mqtt_connected = false;
bool callOnAlert = true;
bool securityMode = false;
bool notifySwitchOn = true;
bool alerting = false;
unsigned int remoteCount = 0;


unsigned long now = millis();
unsigned long lastTrigger = 0;
// unsigned long pressStartTime1 = 0;
// unsigned long pressStartTime2 = 0;
// unsigned long pressStartTime3 = 0;
// unsigned long pressStartTime4 = 0;

unsigned long analogTrigger[totalAnalogs];
unsigned long inputTrigger[totalInputs];
boolean inputFlag[totalInputs];



// boolean input1Flag = 0;
// boolean input2Flag = 0;
// boolean input3Flag = 0;
// boolean input4Flag = 0;


// boolean remoteFlag = 0;


const char *PARAM_INPUT_1 = "output";
const char *PARAM_INPUT_2 = "state";
const char *PARAM_INPUT_3 = "timer";
const char *PARAM_INPUT_4 = "label";
const char *PARAM_INPUT_5 = "label";

uint count = 0;
int currentState;      // the current reading from the input pin
int lastState = HIGH;  // the previous state from the input pin
unsigned long pressedTime = 0;
unsigned long releasedTime = 0;
unsigned long previousMillis = 0;


//=======================================================================
// Replaces placeholder with DHT values
String processor(const String &var) {
  if (var == "BUTTONPLACEHOLDER") {
    return prepareData();
  } else if (var == "WSYNC") {
    return prepareSync();
  }
  return String();
}
String outputState(int output) {
  if (digitalRead(output)) {
    return "";
  } else {
    return "checked";
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  // Convert uint8_t* to char*
  unsigned char base64Data[length + 1];
  memcpy(base64Data, payload, length);
  base64Data[length] = '\0';


  // // Decode base64 data
  unsigned char decodedData[256];
  int decodedLen = decode_base64(base64Data, decodedData);
  // Serial.println(messageBuffer[length]);

  DynamicJsonDocument doc(256);
  DeserializationError error = deserializeJson(doc, decodedData);

  // DeserializationError error = deserializeJson(doc, messageBuffer);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  // Access the JSON values
  int out = doc["out"].as<int>();            // Use as<const char*> for conversion;
  String event = doc["event"].as<String>();  // Use as<const char*> for conversion;
  Serial.println(event);
  if (event == "io") {

    bool state = doc["state"].as<bool>();  // Use as<const char*> for conversion;
    String prg = outputIsBusy(out);

    Serial.print("event io : ");
    Serial.println(prg);
    if (prg.isEmpty()) {
      switchRelay(out, !state);

      // Serial.println("switch relay");
      doc["mac"] = mac;
      doc["event"] = "feedback";
      doc["oSt"] = outStates;
      String result;
      serializeJson(doc, result);
      Serial.println(result);

      mqtt.publish("action_server", result.c_str());
      // updateStatesDSP();

    } else {
      Serial.println("output is busy");
    }
  } else if (event == "pwm") {

    uint8_t percent = doc["percent"].as<uint8_t>();  // Use as<const char*> for conversion;

    Serial.print("event pwm : ");
    Serial.println(percent);

    setPwm(out, percent);

    // Serial.println("switch relay");

    // updateStatesDSP();


  } else if (event == "son") {
    // bool flag = 0;
    // for (uint8_t i = 0; i < totalInputs; i++) {
    //   if (inputs[i].value.charAt(0) == 's') {
    //     uint8_t out = inputs[i].out - 1;
    //     flag = 1;
    //   }
    // }
    // if (flag) {
    //   securityMode = true;
    //   digitalWrite(STATUS_LED, HIGH);
    //   alertEnableDisplay();
    //   doc["mac"] = mac;
    //   doc["event"] = "feedback";
    //   doc["sets"] = createSettingArray();
    //   String result;
    //   serializeJson(doc, result);
    //   mqtt.publish("action_server", result.c_str());

    //   Serial.println(result);
    // }
  } else if (event == "soff") {
    // bool flag = 0;
    // for (uint8_t i = 0; totalInputs < 4; i++) {
    //   if (inputs[i].value.charAt(0) == 's') {
    //     uint8_t out = inputs[i].out - 1;

    //     if (digitalRead(outputs[out].gpio) == LOW) {
    //       flag = 1;
    //       // digitalWrite(outputs[out].gpio, HIGH);
    //       switchRelay(outputs[out].gpio, HIGH);
    //     }
    //   }
    // }
    // if (flag) {
    //   securityMode = false;
    //   digitalWrite(STATUS_LED, LOW);
    //   doc["mac"] = mac;
    //   doc["event"] = "feedback";
    //   doc["sets"] = createSettingArray();
    //   String result;
    //   serializeJson(doc, result);
    //   mqtt.publish("action_server", result.c_str());

    //   Serial.println(result);
    // }
  } else if (event == "ain") {
    Serial.print("event adc : ");

    readAnalogs();

    Serial.println(analogInputs[0].voltage);
    // Serial.println("switch relay");
    doc["mac"] = mac;
    doc["event"] = "feedback";
    JsonArray array = doc.createNestedArray("ain");

    for (uint8_t i = 0; i < totalAnalogs; i++) {
      array.add(analogInputs[i].voltage);
    }

    String result;
    serializeJson(doc, result);
    Serial.println(result);

    mqtt.publish("action_server", result.c_str());

  } else if (event == "label") {

    String label = doc["label"].as<String>();  // Use as<const char*> for conversion;
    uint8_t out = (msg.substring(1, 2).toInt()) - 1;
    if (out != -1) {
      String tempLabel = doc["label"].as<String>();  // Use as<const char*> for conversion;


      writeToEEPROM(outputs[out].labelKey, tempLabel);
      outputs[out].label = tempLabel;
      String text = "نام رله " + String(out + 1) + " به " + tempLabel + " تغییر یافت ";
    }
  } else if (event == "timer") {
    String tempSch = doc["timer"].as<String>();  // Use as<const char*> for conversion;

    String outPrg = outputIsBusy(out);
    if (outPrg != "") {
      String text = "رله " + String(out + 1) + " قفل و در حالت ";
      text = text + ((outPrg.charAt(0) == 's') ? "دزدگیر قرار دارد" : "پمپ قرار دارد");
      Serial.println(text);
      return;
    }
    writeDateTimeEEPROM(outputs[out].timerKey, tempSch);
    outputs[out].timer = tempSch;
    String result = prepareDbData("feedback");
    mqtt.publish("action_server", result.c_str());
    String text;
    if (tempSch.length() >= 2 && tempSch.length() < 6 && tempSch.charAt(0) != 't') {
      String t;
      if (tempSch.substring(2, 3) == "-") {
        bool minus = true;
        t = tempSch.substring(2, 4) + " درجه ";
      } else {
        t = tempSch.substring(2, 4) + " درجه ";
      }
      if (tempSch.charAt(0) == 'c') {
        text = " عملکرد فن رله " + String(out + 1) + " در دمای پایینتر از" + t + " فعال شد";
      } else if (tempSch.charAt(0) == 'h') {
        text = " عملکرد هیتر رله " + String(out + 1) + " در دمای بالاتر از" + t + " فعال شد";
      }
    } else {
      text = "تایمر رله " + String(out + 1) + " فعال شد";
    }
    if (outputs[out].timer.charAt(0) == 't') {
      int8_t toggle = outputs[out].timer.substring(1).toInt();
      toggleTimers[out] = toggle * 2;
    } else {
      toggleTimers[out] = -1;
    }
    // checkTasks();
  } else if (event == "scn") {

    String tempPrg = doc["value"].as<String>();  // Use as<const char*> for conversion;
    String key = doc["key"].as<String>();
    Serial.println(tempPrg);
    if (tempPrg == "") {
      Serial.println("remove scn");
      removeScenario(key.c_str());
    } else {
      String text = addScenario(tempPrg);
      if (text != "") {
        String result = prepareDbData("feedback");
        mqtt.publish("action_server", result.c_str());
        debugPrint("Input " + key + " is Set :");
        debugPrint(tempPrg);
      }
    }


  } else if (event == "status") {
    String result = prepareDbData("feedback");

    mqtt.publish("action_server", result.c_str());
  } else if (event == "update") {
    String url = doc["url"].as<String>();
    int newVer = doc["version"].as<short>();
    Serial.println("Url:");
    Serial.println(url);
    if (newVer > VERSION) {
      checkUpdate(url);
    } else {
      Serial.println("Device is Already Update!");
      doc["mac"] = mac;
      doc["event"] = "feedback";
      doc["update"] = false;
      doc["version"] = VERSION;
      doc.remove("url");
      String result;
      serializeJson(doc, result);
      Serial.println(result);
      mqtt.publish("action_server", result.c_str());
      updatedDisplay();
    }
  }


  // Switch on the LED if an 1 was received as first character
}

void reconnect() {
  mqtt.setServer(m_server.c_str(), 1883);
  mqtt.setCallback(callback);
  mqtt.setBufferSize(648);
  uint8_t count = 0;
  // Loop until we're reconnected
  while (!mqtt.connected() && count < 1) {
    mqtt_connected = false;
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    String myTopic = sub_topic + ">" + mac;

    // String m_password = "123456";
    clientId += String(random(0xffff), HEX);
    StaticJsonDocument<128> payload;
    payload["event"] = "state";
    payload["mac"] = mac;
    payload["status"] = "OFFLINE";

    String JSON;
    serializeJson(payload, JSON);

    // Attempt to connect with last Will Message
    if (mqtt.connect(clientId.c_str(), "hosein282", "At9127995883", "action_server", 0, true, JSON.c_str())) {
      Serial.println("connected");
      Serial.println("Sub to");
      Serial.println(myTopic);
      // Once connected, publish an announcement...
      String sts = prepareDbData("report");

      // String peresence = mqttPeresence();
      mqtt.publish("action_server", sts.c_str());
      // ... and resubscribe
      mqtt.subscribe(myTopic.c_str());
      mqtt_connected = true;
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      // Wait 5 seconds before retrying
      count = count + 1;
    }
  }
}

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

// Initialize LittleFS
void initLittleFS() {
  if (!LittleFS.begin(true)) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  Serial.println("LittleFS mounted successfully");

  // Get all information of your LITTLEFS

  unsigned int totalBytes = LittleFS.totalBytes();
  unsigned int usedBytes = LittleFS.usedBytes();

  Serial.println("File sistem info.");

  Serial.print("Total space:      ");
  Serial.print(totalBytes);
  Serial.println("byte");

  Serial.print("Total space used: ");
  Serial.print(usedBytes);
  Serial.println("byte");
}

// Read File from LittleFS
String readFile(fs::FS &fs, const char *path) {
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file");
    return String();
  }

  String fileContent;
  while (file.available()) {
    fileContent = file.readStringUntil('\n');
    break;
  }
  return fileContent;
}

// Write file to LittleFS
void writeFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
}



void deleteData(fs::FS &fs, const char *path) {
  //Remove the file
  fs.remove(path);
}


// Initialize WiFi
bool initWiFi() {
  // IPAddress localIP(192, 168, 1, 48);

  // Set your Gateway IP address
  // IPAddress localGateway(192, 168, 1, 1);
  //IPAddress localGateway(192, 168, 1, 1); //hardcoded
  // IPAddress subnet(255, 255, 255, 0);
  if (ssid == "" || password == "") {
    Serial.println("Undefined SSID or IP address.");
    resetWifi();
    return false;
  }

  WiFi.mode(WIFI_STA);
  // localIP.fromString(ip.c_str());
  // localGateway.fromString(gateway.c_str());

  // if (!WiFi.config(localIP, localGateway, subnet)) {
  //   Serial.println("STA Failed to configure");
  //   return false;
  // }
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  unsigned long currentMillis = millis();
  previousMillis = currentMillis;

  while (WiFi.status() != WL_CONNECTED) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= 10000) {
      Serial.println("Failed to connect.");
      // resetWifi();
      return false;
    }
  }
  // if (!MDNS.begin("gsm")) {  // Set the hostname to "esp32.local"
  //   Serial.println("Error setting up MDNS responder!");
  //   while (1) {
  //     delay(1000);
  //   }
  // }
  Serial.print("Current ESP32 IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Gateway (router) IP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Primary DNS: ");
  Serial.println(WiFi.dnsIP(0));
  Serial.print("Secondary DNS: ");
  Serial.println(WiFi.dnsIP(1));
  Serial.println(WiFi.localIP());
  hasWifi = true;
  printLocalTime();


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });
  server.serveStatic("/", LittleFS, "/");

  server.on(
    "/sw", HTTP_POST, [](AsyncWebServerRequest *request) {
      Serial.println("sw");
    },
    NULL, handlePostRequest);
  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();

      if (outputs[inputMessage1.toInt()].timer != "") {
        clearTimer(inputMessage1.toInt());
      }
      digitalWrite(outputs[inputMessage1.toInt()].gpio, inputMessage2.toInt());

    } else if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_3)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_3)->value();
      debugPrint(inputMessage2);

      if (inputMessage2.toInt() == 0) {
        clearTimer(inputMessage1.toInt());
      } else {
        //set new timer
        uint8_t index = inputMessage1.toInt();
        writeDateTimeEEPROM(outputs[index].timerKey, inputMessage2);
        outputs[index].timer = inputMessage2;
        debugPrint("Relay 1 is Set :");
        debugPrint(inputMessage2);
      }
    } else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  server.on(
    "/label", HTTP_POST, [](AsyncWebServerRequest *request) {
      Serial.println("body:");
      uint8_t paramsNr = request->params();
      Serial.println(paramsNr);
      String key;
      String val;
      for (uint8_t i = 0; i < paramsNr; i++) {
        AsyncWebParameter *p = request->getParam(i);
        Serial.print("Param name: ");
        key = p->name();
        Serial.println(key);
        Serial.print("Param value: ");
        val = p->value();
        Serial.println(val);
        Serial.println("------");
      }

      uint8_t index = val.substring(0, 1).toInt();
      String labelTmp = val.substring(2);

      if (index >= 0 && index < 5 && !labelTmp.isEmpty()) {
        writeToEEPROM(outputs[index].labelKey, labelTmp);
        outputs[index].label = labelTmp;
        AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "OK");
        response->addHeader("Connection", "close");
        request->send(response);
      } else {
        AsyncWebServerResponse *response = request->beginResponse(406, "text/plain", "ERR");
        response->addHeader("Connection", "close");
        request->send(response);
      }
    });
  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient *client) {
    if (client->lastId()) {
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    // client->send("hello!", NULL, millis(), 10000);
  });

  server.addHandler(&events);

  server.begin();
  return true;
}

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

void setupLCD() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setFont(NULL);
  display.setCursor(30, 2);
  display.print("INITIALIZE ...");
  display.display();
}

void updateDisplay() {
  String m = String(rtc.getMinute());
  String d = String(rtc.getDate());
  if (m.length() == 1) {
    m = "0" + m;
  }
  String date = d.substring(d.indexOf(' ') + 1) + " " + String(rtc.getHour(true)) + ":" + m;

  display.clearDisplay();
  display.setTextSize(1);
  display.setFont(NULL);
  display.setTextColor(WHITE);

  display.setCursor(0, 2);
  display.print(date);
  display.setCursor(2, 14);
  if (op == "") {
    display.println("GSM");
  } else if (op == "mci") {
    display.println("IR-MCI");
  } else if (op == "irancell") {
    display.println("IRANCELL");
  }

  if (signalQuality <= 0 || signalQuality == 99) {
    display.setCursor(122, 14);
    display.println("x");
    // display.drawRect(114, 16, 3, 4, 1);
    // display.drawRect(118, 14, 3, 6, 1);
    // display.drawRect(122, 12, 3, 8, 1);
  } else if (signalQuality > 0 && signalQuality <= 7) {
    display.fillRect(110, 18, 3, 2, 1);
    display.drawRect(114, 16, 3, 4, 1);
    display.drawRect(118, 14, 3, 6, 1);
    display.drawRect(122, 12, 3, 8, 1);
  } else if (signalQuality > 7 && signalQuality <= 14) {
    display.fillRect(110, 18, 3, 2, 1);
    display.fillRect(114, 16, 3, 4, 1);
    display.drawRect(118, 14, 3, 6, 1);
    display.drawRect(122, 12, 3, 8, 1);
  } else if (signalQuality > 14 && signalQuality <= 21) {
    display.fillRect(110, 18, 3, 2, 1);
    display.fillRect(114, 16, 3, 4, 1);
    display.fillRect(118, 14, 3, 6, 1);
    display.drawRect(122, 12, 3, 8, 1);

  } else {
    display.fillRect(110, 18, 3, 2, 1);
    display.fillRect(114, 16, 3, 4, 1);
    display.fillRect(118, 14, 3, 6, 1);
    display.fillRect(122, 12, 3, 8, 1);
  }

  display.setCursor(2, 24);
  display.print("TEMP");
  String tmp = String(temps[0]);
  display.setCursor(128 - ((tmp.length() + 2) * 6), 24);
  display.print(tmp + " C");
  // if (ssid != "") {

  //   display.setCursor(128 - (ssid.length() * 6), 24);
  //   display.print(ssid);
  // } else {
  //   display.setCursor(86, 24);
  //   display.print("NOT SET");
  // }

  display.setCursor(2, 34);
  display.println("WIFI");
  if (hasWifi) {
    display.setCursor(74, 34);
    display.println("CONNECTED");

  } else {
    display.setCursor(110, 34);
    display.println("OFF");
  }

  display.setCursor(2, 44);
  display.println("STATUS");
  if (mqtt_connected) {
    display.setCursor(92, 44);
    display.println("ONLINE");

  } else {
    display.setCursor(86, 44);
    display.println("OFFLINE");
  }
  display.setCursor(2, 54);
  display.println("OUTPUTS");
  if (outStates != "") {
    // outStates.replace(",", "");
    display.setCursor(128 - (outStates.length() * 6), 54);
    display.println(outStates);
  }
  // display.setCursor(2, 54);
  // display.println("INPUTS");
  // if (inStates != "") {
  //   display.setCursor(128 - (inStates.length() * 6), 54);
  //   display.println(inStates);
  // }
  display.display();
}
void updateStatesDSP() {
  // display.fillRect(0, 44, 128, 8, 0);
  display.fillRect(0, 54, 128, 8, 0);
  display.setCursor(2, 54);
  display.println("OUTPUTS");
  if (outStates != "") {
    display.setCursor(128 - (outStates.length() * 6), 54);
    display.println(outStates);
  }

  // display.setCursor(2, 54);
  // display.println("INPUTS");
  // if (inStates != "") {
  //   display.setCursor(128 - (inStates.length() * 6), 54);
  //   display.println(inStates);
  // }
  display.display();
}
void updateTempDSP() {
  // display.fillRect(0, 44, 128, 8, 0);
  display.fillRect(0, 24, 128, 8, 0);

  display.setCursor(2, 24);
  display.print("TEMP");
  String tmp = String(temps[0]);
  display.setCursor(128 - ((tmp.length() + 2) * 6), 24);
  display.print(tmp + " C");

  display.display();
}

void loadingDisplay(int progress, String title) {
  uint8_t p = (progress * 86) / 100;
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setFont(NULL);
  display.setCursor(0, 10);
  display.setTextWrap(0);
  display.setCursor(34, 10);
  display.println(title + "...");
  display.drawRect(20, 30, 90, 20, 1);
  display.fillRect(22, 32, p, 16, 1);
  display.display();
}

void alertDisplay() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setFont(NULL);
  display.setCursor(42, 29);
  display.setTextWrap(0);
  display.println("ALERT !!!");
  display.setCursor(28, 42);
  display.println("Security Mode");
  display.display();
}
void alertEnableDisplay() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setFont(NULL);
  display.setCursor(12, 42);
  display.println("Security Mode");
  display.setCursor(42, 29);
  display.setTextWrap(0);
  display.println("ENABLED");

  display.display();
}
void updatedDisplay() {
  unsigned long startTime = millis();

  const uint8_t bitmap23[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xfc, 0x7f, 0xff, 0xfe, 0x60, 0x00, 0x06, 0x60, 0x00, 0x06, 0x60, 0x00, 0x06, 0x60, 0x00, 0x06, 0x60, 0x00, 0x06, 0x61, 0x81, 0x86, 0x61, 0xc3, 0x86, 0x60, 0xe7, 0x06, 0x70, 0x7e, 0x0e, 0x70, 0x3c, 0x0e, 0x70, 0x18, 0x0e, 0x38, 0x00, 0x1c, 0x38, 0x00, 0x1c, 0x1c, 0x00, 0x38, 0x0f, 0x00, 0xf0, 0x07, 0xe7, 0xe0, 0x03, 0xff, 0xc0, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

  display.clearDisplay();
  while (millis() - startTime < 3000) {
    display.println("");
    display.drawBitmap(54, 30, bitmap23, 24, 24, 1);
    display.setCursor(18, 10);
    display.println("Already Updated!");
    display.display();
  }
}
void smsDisplay(String number) {
  unsigned long startTime = millis();

  number.remove(0, 1);
  String lastFour = number.substring(number.length() - 4);
  String formatted = "0" + number.substring(2, 6) + "***" + lastFour;

  display.clearDisplay();
  while (millis() - startTime < 3000) {
    display.setCursor(28, 10);
    display.println("SMS Recieved!");
    display.setCursor(32, 28);
    display.println(formatted);
    display.drawRoundRect(20, 4, 92, 20, 3, 1);
    display.display();
  }
}

/*******************************************************************************
 * setup function
 ******************************************************************************/
void setup() {
  EEPROM.begin("esp");
  REMOTES.begin("remotes");
  setupLCD();

  // aes128.setKey(aes_key, 16);  // Setting Key for AES


  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  Serial.begin(9600);
  delay(1000);
  Serial.print("Firmware Version=>");
  Serial.println(VERSION);
  loadingDisplay(0, "Setup");

  Serial.print("free Entries: ");
  Serial.println(EEPROM.freeEntries());
  Serial.println(REMOTES.freeEntries());
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

  Serial2.begin(9600, SERIAL_8N1, rxPin, txPin);

  //Reset Pin
  pinMode(RESET_GSM, OUTPUT);

  GsmReset();


  delay(2000);
  esp_task_wdt_init(100, true);  // timeout = 5 ثانیه، ریست سیستم در صورت تایم‌اوت
  esp_task_wdt_add(NULL);

  initLittleFS();
  loadingDisplay(10, "Init Data");

  //first parameter is name of access point, second is the password
  // wifiManager.autoConnect("GSM-Controller", "123456");
  // Start the DS18B20 sensor
  sensors.begin();


  sensors.requestTemperatures();

  for (uint8_t i = 0; i < totalTemps; i++) {
    temps[i] = sensors.getTempCByIndex(i);
  }
  // temp0 = sensors.getTempCByIndex(0);
  // temp1 = sensors.getTempCByIndex(1);
  // temp2 = sensors.getTempCByIndex(2);


  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);



  // Receiver on interrupt 0 => that is pin #2
  mySwitch.enableReceive(4);

  button.attach(BUTTON_PIN, INPUT_PULLUP);  // USE EXTERNAL PULL-UP

  button.interval(5);
  // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
  button.setPressedState(LOW);

  Serial.println("SIM800L software serial initialize");
  loadingDisplay(20, "init I/O");

  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  for (uint8_t i = 0; i < totalOutputs; i++) {
    digitalWrite(outputs[i].gpio, outputs[i].state);
    // switchRelay(i, outputs[i].state);
    pinMode(outputs[i].gpio, OUTPUT);  //Relay 1
  }
  for (uint8_t i = 0; i < totalInputs; i++) {
    pinMode(inputs[i].gpio, INPUT_PULLUP);  //Relay 1
  }

  for (uint8_t i = 0; i < totalPwm; i++) {
    ledcSetup(i, freq, 8);
    ledcAttachPin(pwms[i].gpio, i);
  }

  loadingDisplay(30, "Load Data");

  // pinMode(BUILTIN_LED, OUTPUT);  //Relay 4
  digitalWrite(STATUS_LED, LOW);
  pinMode(STATUS_LED, OUTPUT);  //Relay 4
  // digitalWrite(BUILTIN_LED, LOW);

  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  setupVariables();

  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  uint8_t gsmCounter = 0;

  loadingDisplay(40, "Set Wifi");


  if (initWiFi()) {
    reconnect();
  } else {
    initWifiAp();
  }

  delay(1000);
  if (checkSim()) {
    while (!isRegistered()) {
      gsmCounter = gsmCounter + 1;
      flipper.attach(0.5, flip);
      loadingDisplay(50 + (gsmCounter * 4), "Setup GSM");
      delay(3000);
      if (gsmCounter > 6) {
        GsmReset();
        gsmCounter = 0;
        // flipper.detach();
        break;
      }
    }
  } else {
    flipper.attach(1, flip);
    Serial.println("Insert a sim card");
  }

  if (hasGSM) {
    setupGSM();
  }
  loadingDisplay(80, "Setup");

  //Initialize Ticker every 0.5s
  // gsmTicker.attach(3600, getGsmDateTime);  //Use attach_ms if you need time in ms

  if (phoneNo[0].length() == 13) {
    // minTicker.attach(60, checkTasks);  //Use attach_ms if you need time in ms

    // if (notifySwitchOn) {
    //   String txt = "دستگاه روشن شد و آماده به کار است";
    //   // ReplyHex(txt, phoneNo[0]);
    // }
    debugPrint("Admin Phone Is Registered");
    flipper.detach();

    delay(100);
  }
  loadingDisplay(100, "Completed");

  delay(200);

  updateDisplay();
  checkTasks();
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
}

/*******************************************************************************
 * Loop Function
 ******************************************************************************/
void loop() {
  esp_task_wdt_reset();

  now = millis();

  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  while (Serial2.available() > 0) {
    parseData(Serial2.readString());
  }
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

  if (now - prevTaskTime >= SCH_TASK_TIME) {
    checkTasks();
    prevTaskTime = now;
  }

  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  checkSensors();

  readAnalogs();


  readButton();

  checkRfRemote();

  mqtt_connected = mqtt.loop();
}



void runScenarios() {
  for (int i = 0; i < totalScenarios; i++) {
    Scenario scenario = scenarios[i];
    if (scenario.value.isEmpty()) return;
    char it = scenario.value.charAt(0);  //in type
    char ot = scenario.value.charAt(1);  // out type
    Serial.println("run scn");
    Serial.print("it=");
    Serial.println(it);
    Serial.print("ot=");
    Serial.println(ot);

    bool conditionMet = false;
    float ifStatement;

    if (it == 't') {
      ifStatement = temps[scenario.input];
    } else if (it == 'd') {
      ifStatement = inputs[scenario.input].state;
    } else if (it == 'a') {
      ifStatement = analogInputs[scenario.input].voltage;
    }
    Serial.println("statement");
    Serial.print(ifStatement);



    // بررسی شرط
    if (scenario.condition == ">" && ifStatement > scenario.threshold) {
      Serial.println("ifStatement > scenario.threshold");

      conditionMet = true;
    } else if (scenario.condition == "<" && ifStatement < scenario.threshold) {
      Serial.println("ifStatement < scenario.threshold");

      conditionMet = true;
    } else if (scenario.condition == "==" && ifStatement == scenario.threshold) {
      Serial.println("ifStatement == scenario.threshold");

      conditionMet = true;
    } else if (scenario.condition == "=D" && ifStatement == 1) {
      Serial.println("ifStatement == 1");

      conditionMet = true;
    } else if (scenario.condition == "!D" && ifStatement == 0) {
      Serial.println(" ifStatement == 0");

      conditionMet = true;
    }


    // اجرای عملکرد اگر شرط برقرار شد
    if (conditionMet) {
      if (ot == 'p') {
        setPwm(scenario.outPin, scenario.outState);
        Serial.println("setPwm(scenario.outPin");
      } else {
        switchRelay(scenario.outPin, scenario.outState);
        Serial.println("switchRelay(scenario.outPin");
      }
    }
  }
}

//main loop ends


void setupVariables() {

  outStates = createOutArray();
  inStates = createInArray();
  pwmStates = createPwmArray();

  for (int i = 0; i < totalInputs; i++) {
    inputFlag[i] = 0;  // Set all elements to false
  }
  for (int i = 0; i < totalInputs; i++) {
    inputTrigger[i] = 0;  // Set all elements to 0
  }
  for (int i = 0; i < totalAnalogs; i++) {
    analogTrigger[i] = 0;  // Set all elements to 0
  }

  Serial.println("WIFI Credentials");

  // Load values saved in LittleFS
  ssid = readFile(LittleFS, wifi_ssid);
  password = readFile(LittleFS, wifi_pass);
  m_server = readFile(LittleFS, m_server_k);
  mac = readFile(LittleFS, mac_f);
  Serial.println(ssid);
  Serial.println(password);
  Serial.println(m_server);
  Serial.println(mac);

  // }

  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMF

  Serial.println("List of Registered Phone Numbers");
  for (uint8_t i = 0; i < totalPhoneNo; i++) {
    phoneNo[i] = readFromEEPROM(offsetPhone[i]);
    if (phoneNo[i].length() != 13) {
      phoneNo[i] = "";
      Serial.println(String(i + 1) + ": empty");
    } else {
      Serial.println(String(i + 1) + ": " + phoneNo[i]);
    }
  }
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

  Serial.println("List of Schedules");
  for (uint8_t i = 0; i < totalOutputs; i++) {
    outputs[i].timer = readFromEEPROM(outputs[i].timerKey);
    if (outputs[i].timer.length() < 2) {
      outputs[i].timer = "";
      Serial.println(String(i + 1) + ": empty");
    } else {
      Serial.println(String(i + 1) + ": " + outputs[i].timer);
      if (outputs[i].timer.charAt(0) == 't') {
        int8_t toggle = outputs[i].timer.substring(1).toInt();
        toggleTimers[i] = toggle * 2;
      }
    }
  }
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  // Serial.println("List of inputs");
  // for (uint8_t i = 0; i < totalInputs; i++) {
  //   inputs[i].value = readFromEEPROM(inputs[i].key);
  //   if (inputs[i].value.length() < 1) {
  //     inputs[i].value = "";
  //     Serial.println(String(i + 1) + ": empty");
  //   } else {
  //     inputs[i].out = (inputs[i].value.substring(1).toInt()) - 1;

  //     Serial.println(String(i + 1) + ": " + inputs[i].value);
  //   }
  // }
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  Serial.println("List of Scenarios");
  for (uint8_t i = 0; i < totalScenarios; i++) {
    String key = "s" + String(i + 1);
    scenarios[i].value = readFromEEPROM(key.c_str());
    if (scenarios[i].value.length() > 0) {
      processScenarios(scenarios[i].value, i);
      scenariosCount++;
      Serial.println(String(i + 1) + ": " + scenarios[i].value);
    }
    Serial.println("scenariosCount ");
    Serial.println(scenariosCount);
  }
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  Serial.println("List of Output Labels");
  for (uint8_t i = 0; i < totalOutputs; i++) {
    outputs[i].label = readFromEEPROM(outputs[i].labelKey);
    if (outputs[i].label.length() > 0) {
      outputs[i].label = "";
      Serial.println(String(i + 1) + ": empty");
    } else {
      Serial.println(String(i + 1) + ": " + outputs[i].label);
    }
  }
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  Serial.println("List of Input Labels");
  for (uint8_t i = 0; i < totalInputs; i++) {
    inputs[i].label = readFromEEPROM(inputs[i].labelKey);
    if (inputs[i].label.length() < 1) {
      inputs[i].label = "";
      Serial.println(String(i + 1) + ": empty");
    } else {
      Serial.println(String(i + 1) + ": " + inputs[i].label);
    }
  }
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  Serial.println("List of Remotes");

  // Load and print stored remote codes
  for (int i = 0; i < totalRemotes; i++) {  // Example: limit to 10 codes
    String codeKey = "r" + String(i);
    if (REMOTES.isKey(codeKey.c_str())) {
      remoteCount = remoteCount + 1;
      Serial.print("Stored code: ");
      Serial.println(REMOTES.getString(codeKey.c_str()));
    }
  }
  if (remoteCount == 0) {
    Serial.println("empty");
  } else {
    Serial.println(remoteCount);
    // for (uint8_t i = 0; i < remoteCount; i++) {
    //   char key[4] = "";
    //   sprintf(key, "rm%d", i + 1);
    //   Serial.println(String(i + 1) + ": " + readFromEEPROM(key));
    // }
  }
}

bool processScenarios(String command, uint8_t index) {

  int comma1 = command.indexOf(':');
  int comma2 = command.indexOf(':', comma1 + 1);
  int comma3 = command.indexOf(':', comma2 + 1);
  int comma4 = command.indexOf(':', comma3 + 1);
  // int comma5 = command.indexOf(':', comma4 + 1);

  String key = "s" + String(index + 1);
  strncpy(scenarios[index].key, key.c_str(), sizeof(scenarios[index].key) - 1);
  scenarios[index].key[sizeof(scenarios[index].key) - 1] = '\0';  // Ensure null termination
  Serial.println(scenarios[index].key);
  scenarios[index].input = command.substring(2, comma1).toInt();
  scenarios[index].condition = command.substring(comma1 + 1, comma2);

  int val = command.substring(comma2 + 1, comma3).toInt();
  if (command.charAt(0) == 'a') {
    scenarios[index].threshold = (val * aResolution) / 100;
  } else {
    scenarios[index].threshold = val;
  }
  scenarios[index].outPin = command.substring(comma3 + 1, comma4).toInt();
  scenarios[index].outState = command.substring(comma4 + 1).toInt();

  if (scenarios[index].input == -1 || scenarios[index].outPin == -1) {
    return false;
  }
  scenarios[index].value = command;
  return true;
}

void handlePostRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  if (index == 0) {
    // The first part of the request, we can process the data
    // Serial.write(data, len);  // optional - print raw data to Serial
    // Parse the JSON

    // Convert uint8_t* to char*
    unsigned char base64Data[len + 1];
    memcpy(base64Data, data, len);
    base64Data[len] = '\0';

    // Decode base64 data
    unsigned char decodedData[512];
    int decodedLen = decode_base64(base64Data, decodedData);


    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, decodedData);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    String model = doc["model"].as<String>();

    if (model == "" || model != DEVICE_MODEL) {
      request->send(400, "application/json", "{\"error\":\"device model\"}");
      Serial.println("device model is not verified");
      return;
    }

    // Access the JSON values
    ssid = doc["ssid"].as<String>();          // Use as<const char*> for conversion;
    password = doc["password"].as<String>();  // Use as<const char*> for conversion;
    m_server = doc["m_server"].as<String>();  // Use as<const char*> for conversion;


    writeFile(LittleFS, wifi_ssid, ssid.c_str());
    writeFile(LittleFS, wifi_pass, password.c_str());
    // writeFile(LittleFS, ankey, anon_key.c_str());
    writeFile(LittleFS, m_server_k, m_server.c_str());

    StaticJsonDocument<128> payload;
    payload["status"] = "received";
    payload["mac"] = mac;

    String JSON;
    serializeJson(payload, JSON);
    Serial.println(JSON);
    // "{\"status\":\"received\"}"
    request->send(200, "application/json", JSON);
    delay(3000);
    ESP.restart();
  } else {
    request->send(400, "application/json", "{\"error\":\"No data\"}");
  }
}


void initWifiAp() {
  if (mac.isEmpty()) {
    mac = getMAC();
  }
  // setupBLE();
  WiFi.softAP(ssid_ap + mac.substring(0, 2) + mac.substring(15, 17), "2NyTf21=");

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);


  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    // response->addHeader("Access-Control-Allow-Origin", "*");
    request->send_P(200, "text/html", index_html, processor);
  });


  // Route for root / web page
  server.on("/ssid", HTTP_GET, [](AsyncWebServerRequest *request) {
    // response->addHeader("Access-Control-Allow-Origin", "*");
    // String payload = prepareSync();
    StaticJsonDocument<128> payload;
    payload["status"] = "received";
    payload["mac"] = mac;
    JsonArray array = payload.createNestedArray("ssid");

    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i) {
      array.add(WiFi.SSID(i));
    }
    String JSON;
    serializeJson(payload, JSON);
    Serial.println(JSON);
    // "{\"status\":\"received\"}"
    request->send(200, "application/json", JSON);
  });

  server.on(
    "/sw", HTTP_POST, [](AsyncWebServerRequest *request) {
      Serial.println("sw");
    },
    NULL, handlePostRequest);

  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();

      if (outputs[inputMessage1.toInt()].timer != "") {
        clearTimer(inputMessage1.toInt());
      }
      digitalWrite(outputs[inputMessage1.toInt()].gpio, inputMessage2.toInt());

    } else if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_3)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_3)->value();
      debugPrint(inputMessage2);

      if (inputMessage2.toInt() == 0) {
        clearTimer(inputMessage1.toInt());
      } else {
        //set new timer
        uint8_t index = inputMessage1.toInt();
        writeDateTimeEEPROM(outputs[index].timerKey, inputMessage2);
        outputs[index].timer = inputMessage2;
        debugPrint("Relay 1 is Set :");
        debugPrint(inputMessage2);
      }
    } else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  server.on(
    "/label", HTTP_POST, [](AsyncWebServerRequest *request) {
      Serial.println("body:");
      uint8_t paramsNr = request->params();
      Serial.println(paramsNr);
      String key;
      String val;
      for (uint8_t i = 0; i < paramsNr; i++) {
        AsyncWebParameter *p = request->getParam(i);
        Serial.print("Param name: ");
        key = p->name();
        Serial.println(key);
        Serial.print("Param value: ");
        val = p->value();
        Serial.println(val);
        Serial.println("------");
      }

      uint8_t index = val.substring(0, 1).toInt();
      String labelTmp = val.substring(2);

      if (index >= 0 && index < 5 && !labelTmp.isEmpty()) {
        writeToEEPROM(outputs[index].labelKey, labelTmp);
        outputs[index].label = labelTmp;
        AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "OK");
        response->addHeader("Connection", "close");
        request->send(response);
      } else {
        AsyncWebServerResponse *response = request->beginResponse(406, "text/plain", "ERR");
        response->addHeader("Connection", "close");
        request->send(response);
      }
    });




  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient *client) {
    if (client->lastId()) {
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    // client->send("hello!", NULL, millis(), 10000);
  });

  server.addHandler(&events);

  server.begin();
}

String getMAC() {
  // Get the MAC address
  // uint8_t mac[6];
  // WiFi.softAPmacAddress(mac);
  // String macStr = String();

  // Create a string from the MAC address
  // for (int i = 0; i < 6; i++) {
  //   if (macStr.length() > 0) {
  //     macStr += ":";  // Add separator for readability
  //   }
  //   macStr += String(mac[i], HEX);  // Convert each byte to hex
  // }

  // Convert to upper case for standard MAC format
  String macStr = WiFi.macAddress();
  macStr.toUpperCase();
  Serial.println(macStr);
  writeFile(LittleFS, mac_f, macStr.c_str());

  return macStr;
}

bool printLocalTime() {
  //Sunday, December 01 2024 11:36:05

  Serial.print(F("Waiting for NTP time sync: "));
  struct tm timeinfo;

  // time_t now = time(nullptr);
  uint8_t count = 0;
  while (count < 3 && !getLocalTime(&timeinfo)) {
    count += 1;
    yield();
    delay(500);
    Serial.print(F("."));
  }
  Serial.println(F(""));
  String formattedDate = createDateString(timeinfo, 14);
  updateDate(formattedDate);
  if (timeinfo.tm_year < 2024) {
    return false;
  }
  // time(&now);
  Epoch_Time = rtc.getEpoch();
  return true;
}

void setTimezone(String timezone) {
  Serial.printf("  Setting Timezone to %s\n", timezone.c_str());
  setenv("TZ", timezone.c_str(), 1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}

void resetWifi() {
  ssid = "";
  password = "";
  m_server = "";

  deleteData(LittleFS, wifi_ssid);
  deleteData(LittleFS, wifi_pass);
  deleteData(LittleFS, m_server_k);

  hasWifi = false;
}

String createDateString(const struct tm &timeinfo, int timezoneOffset) {
  // String to store the formatted date
  String dateString = "\"";

  // Append day, month, year
  dateString += String(timeinfo.tm_year % 100) + "/";                                                                         // Year since 1900
  dateString += String(timeinfo.tm_mon + 1).length() == 1 ? "0" + String(timeinfo.tm_mon + 1) : String(timeinfo.tm_mon + 1);  // Months are 0-based
  dateString += "/";                                                                                                          // Months are 0-based
  dateString += String(timeinfo.tm_mday).length() == 1 ? "0" + String(timeinfo.tm_mday) : String(timeinfo.tm_mday);
  dateString += ",";


  // Append hour, minute, second with proper formatting
  dateString += String(timeinfo.tm_hour).length() == 1 ? "0" + String(timeinfo.tm_hour) : String(timeinfo.tm_hour);
  dateString += ":";
  dateString += String(timeinfo.tm_min).length() == 1 ? "0" + String(timeinfo.tm_min) : String(timeinfo.tm_min);
  dateString += ":";
  dateString += String(timeinfo.tm_sec).length() == 1 ? "0" + String(timeinfo.tm_sec) : String(timeinfo.tm_sec);

  // Append timezone offset
  dateString += (timezoneOffset >= 0 ? "+" : "") + String(timezoneOffset) + "\"";

  return dateString;
}

boolean TimePeriodIsOver(unsigned long &periodStartTime, unsigned long TimePeriod) {
  unsigned long currentMillis = millis();
  if (currentMillis - periodStartTime >= TimePeriod) {
    periodStartTime = currentMillis;  // set new expireTime
    return true;                      // more time than TimePeriod) has elapsed since last time if-condition was true
  } else return false;                // not expired
}
unsigned long prevMqttReport = 0;

void publishReport(const char *payload) {
  if (millis() - prevMqttReport > MQTT_REPORT_INTERVAL) {
    mqtt.publish("action_server", payload);
    prevMqttReport = millis();
  }
}
void checkSensors() {
  // int s1 = digitalRead(inputs[0].gpio);
  // int s2 = digitalRead(inputs[1].gpio);
  // int s3 = digitalRead(inputs[2].gpio);
  // int s4 = digitalRead(inputs[3].gpio);

  for (uint8_t i = 0; i < totalInputs; i++) {
    // if (inputs[i].value != "") {
    if (digitalRead(inputs[i].gpio) != inputs[i].state && now > (inputTrigger[i] + 1000)) {
      inputTrigger[i] = now;
      inputs[i].state = 1;
      // updateInputDb(i);
      Serial.println("Sensor" + String(i + 1) + " debounce");
      if (mqtt_connected) {
        DynamicJsonDocument doc(64);
        doc["mac"] = mac;
        doc["event"] = "report";
        doc["iSt"] = createInArray();
        String result;
        serializeJson(doc, result);
        Serial.println(result);
        publishReport(result.c_str());
        // mqtt.publish("action_server", );
      }

      // checkOutputSch(i);
    } else if (digitalRead(inputs[i].gpio) != inputs[i].state) {
      inputs[i].state = 0;
      // if (hasWifi) {
      // DynamicJsonDocument doc(64);
      // doc["mac"] = mac;
      // doc["event"] = "feedback";
      // doc["iSt"] = createInArray();
      // String result;
      // serializeJson(doc, result);
      // Serial.println(result);

      // mqtt.publish("action_server", result.c_str());
      // }
      inputTrigger[i] = now;
    }
  }
}


void readAnalogs() {
  bool flag = false;
  for (uint8_t i = 0; i < totalAnalogs; i++) {
    int value = analogRead(analogInputs[i].gpio);
    if (abs(value - analogInputs[i].voltage) > analog_read_threshold && now > (analogTrigger[i] + 5000)) {
      analogTrigger[i] = now;
      flag = true;
    }
    analogInputs[i].voltage = value;
  }
  if (flag) {
    if (mqtt_connected) {
      DynamicJsonDocument doc(64);
      doc["mac"] = mac;
      doc["event"] = "report";
      JsonArray array2 = doc.createNestedArray("ain");
      for (uint8_t i = 0; i < totalAnalogs; i++) {
        array2.add(analogInputs[i].voltage);
      }
      String result;
      serializeJson(doc, result);
      Serial.println(result);
      mqtt.publish("action_server", result.c_str());
    }
  }
}
// void checkPump() {
//   for (uint8_t i = 0; i < totalInputs; i++) {
//     if (inputs[i].value.charAt(0) == 'p') {
//       uint8_t out = inputs[i].out;
//       if (digitalRead(inputs[i].gpio) == HIGH) {
//         // digitalWrite(outputs[out].gpio, LOW);
//         switchRelay(out, LOW);

//       } else {
//         // digitalWrite(outputs[out].gpio, HIGH);
//         switchRelay(out, HIGH);
//       }
//     }
//   }
// }



String lasetRvalue = "";
void checkRfRemote() {

  if (mySwitch.available()) {
    Serial.println("rf available");
    Serial.println(lasetRvalue);
    int value = mySwitch.getReceivedValue();

    if (value != 0) {

      Serial.print("Received ");
      Serial.print(value);
      Serial.print(" / ");
      Serial.print(mySwitch.getReceivedBitlength());
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println(mySwitch.getReceivedProtocol());

      compareRemote(String(value));
      delay(500);

    } else {
      Serial.print("Unknown encoding");
    }
    mySwitch.resetAvailable();
  }
}



void readButton() {
  button.update();
  // read the state of the switch/button:
  if (button.pressed()) {

    checkTasks();

    pressedTime = millis();
  } else if (button.released()) {
    releasedTime = millis();
    unsigned long pressDuration = releasedTime - pressedTime;
    Serial.print("The button duration: ");
    Serial.println(pressDuration);

    if (pressDuration > SUPER_LONG_TIME) {
      resetWifi();
    } else if (pressDuration > VERY_LONG_TIME) {
      handleRemoveRemote();
    } else if (pressDuration > LONG_PRESS_TIME) {
      handleRemoteRegister();
    } else if (pressDuration > SHORT_PRESS_TIME) {

      handleShortPress();
    }
  }
  // save the the last state
  lastState = currentState;
}


void handleShortPress() {
  Serial.println("short press");
  // if (alerting) {
  //   for (uint8_t i = 0; i < totalInputs; i++) {
  //     if (inputs[i].value.charAt(0) == 's') {
  //       uint8_t out = inputs[i].out;

  //       if (digitalRead(outputs[out].gpio) == LOW) {
  //         switchRelay(out, HIGH);

  //         continue;
  //       }
  //     }
  //   }
  //   alerting = false;

  //   String text = "آژیر دزدگیر خاموش شد ";
  //   ReplyHex(text, phoneNo[0]);
  // }
}
void handleRemoteRegister() {
  Serial.println("long press");
  unsigned long start = millis();

  uint8_t relaysCount = 0;
  flipper.attach(0.2, flip);
  while (millis() < start + 15000) {
    if (relaysCount >= totalOutputs) {
      break;
    }
    if (mySwitch.available()) {
      String value = String(mySwitch.getReceivedValue());
      if (value == 0) {
        Serial.print("Unknown encoding");
      } else {
        Serial.print("Received ");
        Serial.print(value);
        Serial.print(" / ");
        Serial.print(mySwitch.getReceivedBitlength());
        Serial.print("bit ");
        Serial.print("Protocol: ");
        Serial.println(mySwitch.getReceivedProtocol());
        // save remote
        value = String(relaysCount) + value;
        Serial.println(value);

        saveRemoteCode(value.c_str(), relaysCount);
        relaysCount = relaysCount + 1;
        //
      }

      mySwitch.resetAvailable();
    }
  }
  Serial.print("Loop Break!");

  flipper.detach();
}
void saveRemoteCode(const char *code, uint16_t out) {
  for (int i = 0; i < totalRemotes; i++) {  // Example: limit to 10 codes
    String codeKey = "r" + String(i);
    if (REMOTES.getString(codeKey.c_str()).substring(1).indexOf(code + 1) != -1) {
      Serial.print("Code already exists!");
      break;
    }
    if (!REMOTES.isKey(codeKey.c_str())) {
      REMOTES.putString(codeKey.c_str(), code);
      remoteCount = remoteCount + 1;
      Serial.print("Saved code: ");
      Serial.println(code);
      delay(800);
      break;
    }
  }
}

void handleRemoveRemote() {
  Serial.println("very long press");
  unsigned long start = millis();

  uint8_t relaysCount = 0;
  flipper.attach(0.2, flip);
  while (millis() < start + 15000) {
    if (relaysCount >= totalOutputs) {
      break;
    }
    if (mySwitch.available()) {
      String value = String(mySwitch.getReceivedValue());
      if (value == 0) {
        Serial.print("Unknown encoding");
      } else {
        Serial.print("Received ");
        Serial.print(value);
        Serial.print(" / ");
        Serial.print(mySwitch.getReceivedBitlength());
        Serial.print("bit ");
        Serial.print("Protocol: ");
        Serial.println(mySwitch.getReceivedProtocol());
        // save remote

        removeRemoteCode(value.c_str());
        relaysCount = relaysCount + 1;
        //
      }

      mySwitch.resetAvailable();
    }
  }
  Serial.print("while loop breaks");

  flipper.detach();
}

void removeRemoteCode(const char *code) {

  // Check if the code exists in preferences
  for (int i = 0; i < totalRemotes; i++) {  // Example: limit to 10 codes
    String codeKey = "r" + String(i);

    if (REMOTES.getString(codeKey.c_str()).substring(1) == code) {
      REMOTES.remove(codeKey.c_str());
      Serial.print("Remote Deleted!");
      remoteCount = remoteCount - 1;
      delay(800);
      return;
    }
  }
  Serial.print("Not exist!");
  return;
}

void compareRemote(String received) {
  if (remoteCount == 0) {
    Serial.println("remote not registered!");
    return;
  }
  lasetRvalue = received;

  // for (int i = 0; i < remoteCount; i++) {
  // Check if the code exists in preferences
  for (int i = 0; i < totalRemotes; i++) {  // Example: limit to 10 codes
    String codeKey = "r" + String(i);
    String value = REMOTES.getString(codeKey.c_str());
    if (value.substring(1) == received) {
      uint8_t index = value.substring(0, 1).toInt();
      if (index == 0 && alerting) {
        // check for silent security
        // for (uint8_t i = 0; i < totalInputs; i++) {
        //   if (!inputs[i].value.isEmpty()) {
        //     switchRelay(i, HIGH);

        //     alerting = false;
        //     digitalWrite(STATUS_LED, LOW);
        //     securityMode = false;
        //     continue;
        //   }
        // }
        String text = "آژیر دزدگیر خاموش شد ";
        ReplyHex(text, phoneNo[0]);
        return;
      }

      Serial.println("value of key");
      Serial.println(index);
      if (lasetRvalue == received && now >= prevRfTime + 500 && now < prevRfTime + 1000) {
        Serial.print("RF Long Pressed! ");
        prevRfTime = millis();
        // if (index == 0 && !alerting && !securityMode) {
        //   for (uint8_t i = 0; i < totalInputs; i++) {
        //     if (inputs[i].value.charAt(0) == 's') {
        //       digitalWrite(STATUS_LED, HIGH);
        //       securityMode = true;
        //       return;
        //     }
        //   }
        // } else if (index == 1 && !alerting && securityMode) {
        //   digitalWrite(STATUS_LED, LOW);
        //   securityMode = false;
        // }
        return;
      }

      prevRfTime = millis();
      // check for silent security
      // for (uint8_t i = 0; i < totalInputs; i++) {
      //   if (inputs[i].out == index) {
      //     Serial.println("prg is active!");

      //     return;
      //   }
      // }
      if (index <= totalOutputs && outputs[index].timer.isEmpty()) {
        boolean state = digitalRead(outputs[index].gpio);
        // digitalWrite(outputs[index].gpio, !state);
        switchRelay(index, !state);

        Serial.println("Code is Correct!");
      } else {
        Serial.println("timer is active!");
      }

      // remoteFlag = 1;
      return;
    }
  }
}


void setupGSM() {
  delay(3000);


  getSignalQuality();

  getOperator();

  readyForSms();

  getGsmDateTime();
}

String SendShortCommand(String command) {
  Serial.print("Sending command: \"");
  Serial.print(command);
  Serial.println("\"");

  Serial2.print(command);
  Serial2.print("\r\n");

  return WaitForResponse();
}

String WaitForResponse() {
  String data;
  unsigned long startTime = millis();
  while (millis() - startTime < 5000) {
    if (Serial2.available() > 0) {
      Serial.println("read serial:");
      data = Serial2.readString();
      Serial.println(data);
      return data;
    }
  }
  Serial.println("Did not receive data.");
  return "";
}

bool checkSim() {
  String result = SendShortCommand("AT+CPIN?");
  if (result.indexOf("+CPIN") != -1 && result.indexOf("READY") != -1) {
    hasGSM = true;
    return true;
  } else {
    hasGSM = false;
    return false;
  }
}

void getOperator() {
  String result = SendShortCommand("AT+COPS?");
  String newValue = "";
  if (result.indexOf("43235") != -1) {
    newValue = "irancell";
  } else if (result.indexOf("TCI") != -1) {
    newValue = "mci";
  } else {
    newValue = "-";
  }
  if (mqtt_connected && newValue != op) {

    StaticJsonDocument<64> doc;
    doc["mac"] = mac;
    doc["event"] = "report";
    doc["op"] = op;
    doc["sig"] = String(signalQuality);

    String result;
    serializeJson(doc, result);
    Serial.println(result);
    mqtt.publish("action_server", result.c_str());
  }
  op = newValue;

  updateDisplay();
  Serial.println("operator : ");
  Serial.println(result);
  Serial.println(op);

  events.send(buffer.c_str(), "gsm", millis());
}

void getSignalQuality() {
  // Serial2.println("AT+CSQ\r\n");
  // readSerial();
  // delay(1000);
  String result = SendShortCommand("AT+CSQ");

  uint8_t s;
  if (!result.isEmpty()) {
    s = (result.substring(result.indexOf("Q:") + 3, result.indexOf("Q:") + 5)).toInt();
  } else {
    s = 99;
  }
  if (mqtt_connected && abs(s - signalQuality) > signal_threshold) {
    StaticJsonDocument<64> doc;

    doc["mac"] = mac;
    doc["event"] = "report";
    doc["op"] = op;
    doc["sig"] = String(signalQuality);
    String result;
    serializeJson(doc, result);
    Serial.println(result);
    mqtt.publish("action_server", result.c_str());
  }

  signalQuality = s;
  Serial.println("signal:");
  Serial.println(s);
  events.send(String(s).c_str(), "signal", now);
}
void blinkLed() {
  // digitalWrite(BUILTIN_LED, HIGH);
  delay(100);
  // digitalWrite(BUILTIN_LED, LOW);
}
void GsmReset() {
  Serial.println("gsm reset");
  digitalWrite(RESET_GSM, LOW);
  delay(600);
  digitalWrite(RESET_GSM, HIGH);
}


void checkOutputSch(uint8_t input) {
  // if (inputs[input].value != "") {
  inputFlag[input] = 1;

  // pump mode
  // if (inputs[input].value.charAt(0) == 'p') {
  //   uint8_t out = inputs[input].out;

  //   if (digitalRead(inputs[input].gpio) == HIGH) {
  //     digitalWrite(outputs[out].gpio, LOW);
  //   } else {
  //     digitalWrite(outputs[out].gpio, HIGH);
  //   }
  //   // setRealy(relays[out], 0);
  //   Serial.println("pump");
  //   // Serial.print(out);
  // }
  // security mode
  // if (inputs[input].value.charAt(0) == 's' && securityMode) {
  //   alerting = true;
  //   uint8_t out = inputs[input].out;
  //   Serial.println("security mode->alerting");
  //   // digitalWrite(outputs[out].gpio, LOW);
  //   switchRelay(out, LOW);
  //   alertDisplay();

  //   // setRealy(relays[out], 0);
  //   String text = " هشدار! سنسور";
  //   if (inputs[input].label.isEmpty()) {
  //     text = text + (String(input + 1)) + " تحریک شده است";
  //   } else {
  //     text = text + (inputs[input].label) + " تحریک شده است";
  //   }
  //   if (mqtt_connected) {
  //     String report = prepareDbData("feedback");
  //     mqtt.publish("action_server", report.c_str());
  //   }
  //   if (phoneNo[0] != "") {
  //     ReplyHex(text, phoneNo[0]);
  //     if (callOnAlert) {
  //       callAdmin();
  //     }
  //   }
  //   Serial.print("security alerted");
  // }
  // temp mode
  // if (inputs[input].value.charAt(0) != "t") {
  //   int out = inputs[input].substring(1, 3).toInt() - 1;
  //   // setRealy(relays[out], 0);
  //   digitalWrite(relays[out], LOW);

  //   debugPrint("temp mode :");
  //   Serial.print(out);
  // }
  // }
}

String addScenario(String val) {
  if (scenariosCount >= totalScenarios) {
    Serial.println("max scenarios count reached");
    return "";
  }
  if (processScenarios(val, scenariosCount)) {
    String key = "s" + String(scenariosCount + 1);
    writeDateTimeEEPROM(key.c_str(), val);
    scenariosCount++;

    String text = "";
    text = scenarios[scenariosCount].condition + "خروجی" + String(scenarios[scenariosCount].outPin) + String(scenarios[scenariosCount].outState);
    return text;
  }
}

void removeScenario(const char *key) {
  Serial.println(key);
  writeToEEPROM(key, "");
  Serial.println("afret write");


  for (uint8_t i = 0; i < scenariosCount; i++) {
    Serial.println("scenarios[i].key");
    Serial.println(scenarios[i].key);
    if (strcmp(key, scenarios[i].key) == 0) {
      // Shift elements to the left
      Serial.println("Shift elements ");

      for (int j = i; j < scenariosCount - 1; j++) {
        scenarios[j] = scenarios[j + 1];
      }
      scenariosCount--;
      Serial.println("scenario deleted");
      String result = prepareDbData("feedback");
      mqtt.publish("action_server", result.c_str());
    }
  }
}

void readyForSms() {
  // delay(1000);
  // Serial2.println("AT+CMGF=1\r");  //SMS text mode
  SendShortCommand("AT+CMGF=1");

  delay(500);

  if (op == "irancell") {
    // Serial2.println("AT+CSMP=17,167,0,0");
    SendShortCommand("AT+CSMP=17,167,0,0");

    delay(500);
  }
  //delete all sms
  // Serial2.println("AT+CMGD=1,4");
  SendShortCommand("AT+CMGD=1,4");

  delay(1000);

  // Serial2.println("AT+CMGDA= \"DEL ALL\"");
  SendShortCommand("AT+CMGDA= \"DEL ALL\"");

  delay(1000);
  gsmIsReady = true;
}

void receiveSms() {
  delay(2000);
  Serial2.println("AT+CMGF=1\r");  //SMS text mode
  delay(500);
  Serial2.print("AT+CSCS=\"GSM\"\r");
  delay(500);
  Serial2.println("AT+CSMP=17,167,0,0");
  delay(1000);
}

void getGsmDateTime() {
  if (deviceYear < 20) {
    String result;
    // digitalWrite(BUILTIN_LED, HIGH);
    delay(1000);
    if (op == "irancell") {
      Serial2.println("AT+SAPBR=3,1, \"Contype\",\"GPRS\"\r\n");
      delay(1000);
      Serial2.println("AT+SAPBR=3,1, \"APN\",\"CMNET\"\r\n");
      delay(1000);
      // readSerial();
      Serial2.println("AT+SAPBR=1,1\r\n");
      delay(1000);
      // readSerial();
      Serial2.println("AT+CNTPCID=1\r\n");
      delay(3000);
      Serial2.println("AT+CNTP=\"3.asia.pool.ntp.org\",14\r\n");
      // readSerial();

      delay(1000);
      Serial2.println("AT+CNTP\r\n");
      // readSerial();
      delay(500);
      // Serial2.println("AT+CCLK?\r\n");
      result = SendShortCommand("AT+CCLK?");

      // readSerial();
      delay(1000);
      Serial2.println("AT+SAPBR=0,1\r\n");
    } else if (op == "mci") {
      Serial2.println("AT+CLTS=1\r");
      // readSerial();
      delay(500);
      Serial2.println("AT+COPS=0\r");
      // readSerial();
      delay(500);
      // readSerial();
      // Serial2.println("AT+CCLK?\r");
      result = SendShortCommand("AT+CCLK?");
      delay(1000);
      // readSerial();
    }

    //+CCLK: "24/07/06,19:07:09+32"
    //+CCLK: "24/07/06,19:09:38+14"

    if (result.indexOf("+CCLK") != -1) {
      int ind = result.indexOf('"');
      result = result.substring(ind, result.length());
      Serial.print("date buufer=>");
      Serial.println(result);
      updateDate(result);
    }
  }
}
/*******************************************************************************
 * parseData function:
 * this function parse the incomming command such as CMTI or CMGR etc.
 * if the sms is received. then this function read that sms and then pass 
 * that sms to "extractSms" function. Then "extractSms" function divide the
 * sms into parts. such as sender_phone, sms_body, received_date etc.
 ******************************************************************************/
uint8_t smsIndex = 0;

void parseData(String buff) {
  Serial.println("buffer =>");
  Serial.println(buff);

  // buffer = buff;

  unsigned int len, index;
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Remove sent "AT Command" from the response string.
  index = buff.indexOf("\r");
  buff.remove(0, index + 2);
  buff.trim();

  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

  if (buff != "OK") {
    index = buff.indexOf(":");
    String cmd = buff.substring(0, index);
    cmd.trim();
    buff.remove(0, index + 2);
    // Serial.println("cmd==>");
    // Serial.print(cmd);

    if (buff == "ERROR" && smsIndex > 0) {
      Serial.println("In Error");
      Serial.print("last sms index : ");
      Serial.println(smsIndex);
      String temp = "AT+CMGR=" + String(smsIndex) + "\r";
      delay(500);
      //get the message stored at memory location "temp"
      Serial2.println(temp);
      return;
      // signal quality
    }
    //  else if (cmd == "+CSQ") {
    //   uint8_t s;
    //   if (!buff.isEmpty()) {
    //     s = buff.substring(0, 2).toInt();
    //   } else {
    //     s = 99;
    //   }
    //   sig = String(s);
    //   Serial.println("signal:");
    //   Serial.println(s);
    //   events.send(String(s).c_str(), "signal", now);
    //   return;
    // }
    // else if (cmd == "+COPS") {

    //   if (buff.indexOf("43235") != -1) {
    //     op = "irancell";
    //   } else if (buff.indexOf("TCI") != -1) {
    //     op = "mci";
    //   }
    //   Serial.println("operator : ");
    //   Serial.println(buff);
    //   Serial.println(op);

    // }
    // else if (cmd == "+CCLK") {
    //   //+CCLK: "24/07/06,19:07:09+32"
    //   //+CCLK: "24/07/06,19:09:38+14"

    //   int ind = buff.indexOf('"');
    //   buff = buffer.substring(ind, buff.length());
    //   Serial.println("buufer:::");
    //   Serial.println(buff);
    //   updateDate(buff);

    // }
    else if (cmd == "+CPIN") {
      Serial.println("buufer==>");
      Serial.println(buff);
      //+CPIN: READY
      int ind = buff.indexOf(':');
      buff = buffer.substring(ind + 1, buff.length());
      Serial.println("buufer:::");
      Serial.println(buff);

    }

    else if (cmd == "+CMTI") {

      //hex => "REC UNREAD","2B393839313237393935383833","","24/09/07,19:47:46+18"
      // 52346F6E

      //"REC UNREAD","+989127995883","","24/09/07,19:56:18+18"
      // R4on

      //get newly arrived memory location and store it in temp
      index = buff.indexOf(",");

      String temp = buff.substring(index + 1, buff.length());
      smsIndex = temp.toInt();
      temp = "AT+CMGR=" + temp + "\r";
      //get the message stored at memory location "temp"
      Serial2.println(temp);
      // readSerial();
      // delay(1000);
      // if (buffer != "OK") {
      //   Serial2.println(temp);
      // }
    } else if (cmd == "+CPMS") {
      Serial.print("cmd == +CPMS");

      if (buff.indexOf("SM_P") != -1) {

        int index = buff.indexOf(",");
        smsIndex = buff.substring(index + 1, index + 2).toInt();

        if (smsIndex > 0) {
          for (uint8_t i; i < smsIndex; i++) {
            Serial.print("last sms index");
            Serial.println(smsIndex);
            String temp = "AT+CMGR=" + String(smsIndex - i) + "\r";
            //get the message stored at memory location "temp"
            Serial2.println(temp);
          }
          smsIndex = 0;
        }
      }
      return;


    } else if (cmd == "+CUSD") {
      Serial.print("sender =>");
      Serial.println(senderNumber);
      extractUssd(buff);
      return;

    } else if (cmd == "+CMGR") {
      extractSms(buff);
      //----------------------------------------------------------------------------
      if (msg.equals("p") && phoneNo[0].length() == 13) {
        for (uint8_t i = 0; i < totalPhoneNo; i++) {
          if (phoneNo[i] == senderNumber) {
            String text = "1";
            text = prepareSMSStats();
            debugPrint(text);
            prepareData();
            ReplyHex(text, senderNumber);
          }
        }
      } else if (msg.equals("p") && phoneNo[0].length() != 13) {
        writeToEEPROM(offsetPhone[0], senderNumber);
        phoneNo[0] = senderNumber;
        // String text = "Number is Registered: ";
        // text = text + senderNumber;
        // debugPrint(text);
        // String sms = "شماره شما بعنوان مدیر ثبت شد\n";
        // sms = sms + "مدل دستگاه :";
        // sms = sms + DEVICE_MODEL;
        // ReplyHex(sms, senderNumber);
        String sms = "";
        sms = prepareSMSStats();
        debugPrint(sms);
        prepareData();
        ReplyHex(sms, senderNumber);
      }
      //----------------------------------------------------------------------------


      //----------------------------------------------------------------------------
      if (comparePhone(senderNumber)) {
        doAction(senderNumber);
        //delete all sms
      }

      // receiveSms();

      Serial2.println("AT+CMGD=1,4");
      delay(1000);
      if (smsIndex > 0) {
        String c = "AT+CMGD=" + String(smsIndex);
        Serial2.println(c);
        delay(1000);
        smsIndex = smsIndex - 1;
      }
    }
    //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  }
}

void extractUssd(String buff) {
  int startIndex = buff.indexOf('"');                     // Find the first quote
  int endIndex = buff.indexOf('"', startIndex + 1);       // Find the next quote after the first
  String str = buff.substring(startIndex + 1, endIndex);  // Extract between the quotes
  Serial.println("sub str");
  Serial.println(str);

  ForwardHex(str, senderNumber);
}

// READ FROM SERIAL
void readSerial() {
  unsigned long startTime = millis();
  while (millis() - startTime < 5000) {
    if (Serial2.available() > 0) {
      Serial.println("read serial:");
      buffer = Serial2.readString();
      Serial.println(buffer);
    }
  }
}

bool isRegistered() {
  String result = SendShortCommand("AT+CREG?");
  // (result.indexOf(F("+CREG: 0,2"))) != -1 ||
  if ((result.indexOf(F("+CREG: 0,1"))) != -1 || (result.indexOf(F("+CREG: 0,5"))) != -1 || (result.indexOf(F("+CREG: 1,1"))) != -1 || (result.indexOf(F("+CREG: 1,5"))) != -1) {
    Serial.println("isRegistered =true");
    return true;
  } else {
    Serial.println("isRegistered =false");
    return false;
  }
}

/*******************************************************************************
 * extractSms function:
 * This function divide the sms into parts. such as sender_phone, sms_body, 
 * received_date etc.
 ******************************************************************************/
void extractSms(String buff) {
  unsigned int index;

  index = buff.indexOf(",");
  smsStatus = buff.substring(1, index - 1);
  buff.remove(0, index + 2);
  senderNumber = buff.substring(0, 13);
  buff.remove(0, 19);

  receivedDate = buff.substring(0, 20);
  Serial.println("receive date =>");
  Serial.println(receivedDate);
  // updateDate(receivedDate);
  if (deviceYear < 20) {
    //receivedDate 24/09/01,17:40:29+18
    receivedDate = "\"" + receivedDate + "\"";
    updateDate(receivedDate);
  } else {
    // digitalWrite(BUILTIN_LED, HIGH);
  }
  buff.remove(0, buff.indexOf("\r"));
  buff.trim();

  index = buff.indexOf("\n\r");
  buff = buff.substring(0, index);
  buff.trim();
  msg = buff;
  buff = "";
  msg.toLowerCase();


  // Check if the resulting UTF-8 string is valid
  if (isDigit(msg.charAt(0)) && isDigit(msg.charAt(1))) {

    Serial.println("UTF");
    String utf8String = hexToUTF8(msg);
    msg = utf8String;
    msg.toLowerCase();

    Serial.println("The converted string is valid UTF-8:");
    Serial.println(utf8String);

    // if (isValidUTF8((const uint8_t *)utf8String.c_str(), utf8String.length())) {
  } else {
    Serial.println("The converted string is NOT UTF8");
  }
  smsDisplay(senderNumber);

  //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
  String tempcmd = msg.substring(0, 3);


  if (tempcmd.equals("p1=") || tempcmd.equals("p2=") || tempcmd.equals("p3=") || tempcmd.equals("p4=") || tempcmd.equals("p5=")) {

    tempPhone = msg.substring(3, 16);
    msg = tempcmd;
    debugPrint(msg);
    debugPrint(tempPhone);
  } else if (tempcmd.equals("t1=") || tempcmd.equals("t2=") || tempcmd.equals("t3=") || tempcmd.equals("t4=")) {
    tempSch = msg.substring(3, 21);
    msg = tempcmd;
    debugPrint(msg);
    debugPrint("tempSch");
    debugPrint(tempSch);
  } else if (tempcmd.equals("i1=") || tempcmd.equals("i2=") || tempcmd.equals("i3=") || tempcmd.equals("i4=")) {
    tempSch = msg.substring(3, 21);
    msg = tempcmd;
    debugPrint(msg);
    debugPrint("tempSch");
    debugPrint(tempSch);
  } else if (tempcmd.equals("t1x") || tempcmd.equals("t2x") || tempcmd.equals("t3x") || tempcmd.equals("t4x")) {
    msg = tempcmd;
    debugPrint(msg);
  } else if (tempcmd.indexOf("l") != -1 && tempcmd.indexOf("=") != -1) {
    // tempcmd.equals("al=") || tempcmd.equals("bl=") || tempcmd.equals("cl=") || tempcmd.equals("dl=")) {
    tempLabel = msg.substring(3, 23);
    msg = tempcmd;
    debugPrint(tempLabel);
    debugPrint(msg);
  } else if (tempcmd.indexOf("n") != -1 && tempcmd.indexOf("=") != -1) {
    // tempcmd.equals("al=") || tempcmd.equals("bl=") || tempcmd.equals("cl=") || tempcmd.equals("dl=")) {
    tempLabel = msg.substring(3, 23);
    msg = tempcmd;
    debugPrint(tempLabel);
    debugPrint(msg);
  } else if (tempcmd.equals("s")) {
    // msg = tempcmd;
    debugPrint(msg);
  }
  //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
}

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//Calls when receive sms ad extract time and set rtc timer
void updateDate(String dateTime) {
  // if (
  Serial.println("date =>");
  Serial.println(dateTime);

  int year,
    month, day, hour, minute;

  year = (dateTime.substring(1, 3).toInt());
  month = (dateTime.substring(4, 6).toInt());
  day = (dateTime.substring(7, 9).toInt());
  hour = (dateTime.substring(10, 12).toInt());
  minute = (dateTime.substring(13, 15).toInt());
  Serial.println("year =>");
  Serial.println(year);
  deviceYear = year;
  if (deviceYear > 20) {
    // digitalWrite(BUILTIN_LED, HIGH);
  } else {
    // String text = "خطا در دریافت تاریخ و زمان رخ داده، لطفا برای اصلاح تاریخ یک پیام ارسال نمایید";
    // ReplyHex(text,phoneNo[0]);
  }

  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

  rtc.setTime(0, minute, hour, day, month, (2000 + year));  // 17th Jan 2021 15:24:30

  checkTasks();
  Serial.println("rtc time");
  Serial.println(rtc.getDateTime(true));
}

void checkHourTasks() {
  Serial.println("Hour Task");
}

void checkSmsHistory() {
  Serial2.println("AT+CPMS?\r");
  delay(500);
}



void checkTasks() {
  // Serial.println("refreshed time =>");
  // Serial.println(rtc.getDateTime(true));
  runScenarios();


  uint8_t dayOfWeek = rtc.getDayofWeek();
  uint8_t hour = rtc.getHour(true);
  uint8_t minute = rtc.getMinute();
  updateDisplay();

  //count minutes
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  // one hour
  if (minCounter > 60) {
    minCounter = 0;
  }

  if (minCounter > 0 && minCounter % 10 == 0) {

    uint8_t gsmCounter = 0;

    if (hasWifi) {
      if (WiFi.status() != WL_CONNECTED) {
        initWiFi();
      }
    } else if (ssid != "" && password != "") {
      initWiFi();
    }
    if (hasGSM && op == "") {
      GsmReset();
      while (!isRegistered()) {
        GsmReset();
        flipper.attach(0.5, flip);
        gsmCounter = gsmCounter + 1;
        if (gsmCounter > 3) {
          break;
          // gsmCounter = 0;
        }
      }
    } else {
      checkSim();
    }
  }
  if (minCounter % 5 == 0) {
    if (ssid != "" && password != "" && WiFi.status() != WL_CONNECTED) {
      initWiFi();
    }
    // if (hasWifi && mqtt_connected) {
    //   String report = prepareDbData("log");
    //   mqtt.publish("action_server", report.c_str());
    // }
    if (hasGSM) {
      checkSmsHistory();
    }
  }
  if (minCounter % 2 == 0) {
    /// if has borker registered
    if (hasWifi && m_server != "") {

      //  else {
      //   String peresence = mqttPeresence();
      //   mqtt.publish("action_server", peresence.c_str());
      // }
    }
    if (hasGSM) {
      getSignalQuality();
      if (op == "") {
        getOperator();
      }
    } else {
      checkSim();
    }
    if (deviceYear < 20) {
      if (hasWifi) {
        printLocalTime();

      } else if (hasGSM) {
        getGsmDateTime();
      }
    }
  }
  // if (mqtt_connected) {
  //   String result = prepareDbData("report");
  //   mqtt.publish("action_server", result.c_str());
  // }
  if (!mqtt_connected) {
    reconnect();
  }


  sensors.requestTemperatures();
  bool flag = false;
  for (uint8_t i = 0; i < totalTemps; i++) {
    float t = sensors.getTempCByIndex(i);
    if (abs(t - temps[i]) > temp_threshold) {
      flag = true;
    }
    temps[i] = t;
  }

  /// send mqtt if threshold temp trigged
  if (flag) {
    updateTempDSP();
    if (mqtt_connected) {
      DynamicJsonDocument doc(64);
      doc["mac"] = mac;
      doc["event"] = "report";
      JsonArray array1 = doc.createNestedArray("temps");
      for (uint8_t i = 0; i < totalTemps; i++) {
        temps[i] = sensors.getTempCByIndex(i);
        array1.add(temps[i]);
      }
      String result;
      serializeJson(doc, result);
      Serial.println(result);
      mqtt.publish("action_server", result.c_str());
    }
  }


  minCounter = minCounter + 1;

  //check relays schedules
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  for (uint8_t i = 0; i < totalOutputs; i++) {
    // format : 0123456/06:3013:00
    // if (outputs[i].timer.charAt(0) != 't') {
    //   toggleTimers[i] = -1;
    // }

    if (outputs[i].timer.length() == 18) {
      if (outputs[i].timer.substring(0, 7).indexOf(String(dayOfWeek)) != -1) {
        // begin time
        int startH = outputs[i].timer.substring(8, 10).toInt();
        int startM = outputs[i].timer.substring(11, 13).toInt();
        int endH = outputs[i].timer.substring(13, 15).toInt();
        int endM = outputs[i].timer.substring(16, 18).toInt();


        if (hour >= startH) {
          if (hour == startH) {
            if (minute > startM) {
              if (hour <= endH) {
                if (hour == endH) {
                  if (minute < endM) {
                    // digitalWrite(outputs[i].gpio, LOW);
                    switchRelay(i, LOW);
                    // Serial.print(i + 1);
                    // Serial.println("is on ");
                  } else {
                    // digitalWrite(outputs[i].gpio, HIGH);

                    switchRelay(i, HIGH);

                    // Serial.print(i + 1);
                    // Serial.println("is off ");
                    continue;
                  }
                }
                if (hour < endH) {
                  // digitalWrite(outputs[i].gpio, LOW);
                  switchRelay(i, LOW);

                  // Serial.print(i + 1);
                  // Serial.println("is on ");

                  continue;
                }
              } else {
                // digitalWrite(outputs[i].gpio, HIGH);
                switchRelay(i, HIGH);

                // Serial.print(i + 1);
                // Serial.println("is off ");
              }
            } else {
              // Serial.println("return");
              continue;
            }

          } else {
            if (hour <= endH) {
              if (hour == endH) {
                if (minute < endM) {
                  // digitalWrite(outputs[i].gpio, LOW);
                  switchRelay(i, LOW);
                  // Serial.print(i + 1);
                  // Serial.println("is on ");
                } else {
                  // digitalWrite(outputs[i].gpio, HIGH);
                  switchRelay(i, HIGH);
                  // Serial.print(i + 1);
                  // Serial.println("is off ");
                  continue;
                }
              }
              if (hour < endH) {
                // digitalWrite(outputs[i].gpio, LOW);
                switchRelay(i, LOW);
                // Serial.print(i + 1);
                // Serial.println("is on ");
                continue;
              }
            } else {
              // digitalWrite(outputs[i].gpio, HIGH);
              switchRelay(i, HIGH);
              // Serial.print(i + 1);
              // Serial.println("is off ");
            }
          }
        }
      }
    } else if (outputs[i].timer.length() == 16) {
      // format : 0123456/06/03:00
      if ((outputs[i].timer.substring(0, 7).indexOf(String(dayOfWeek)) != -1)) {
        // begin time
        int perHour = outputs[i].timer.substring(8, 10).toInt();
        int h = outputs[i].timer.substring(11, 13).toInt();
        int m = outputs[i].timer.substring(14, 16).toInt();
        uint t;
        if (perHour <= 0) {
          // Serial.print("perHour must at least 1 ");
          return;
        }
        if (m <= 0) {
          // Serial.print("m must grater than 0 ");
          return;
        }
        if (24 % perHour == 0) {
          t = 24 / perHour;
        } else {
          // Serial.print("perHour not divide by 24");
          return;
        }
        if (h >= perHour) {
          // Serial.print("on hours bigger than perHour");
          return;
        }


        for (int j = 0; j <= t; j++) {
          uint8_t cycleStart = (j + 1) * perHour;   // j = 6 => 12
          uint8_t cycleEnd = cycleStart + perHour;  // 12 +2 =>14
          if (hour >= cycleStart && hour < cycleEnd) {
            //current hour is in this cylcle
            uint16_t onMTime = (h * 60) + m;
            uint16_t passed = ((hour - cycleStart) * 60) + minute;

            if (passed <= onMTime) {
              // digitalWrite(outputs[i].gpio, LOW);
              switchRelay(i, LOW);
              Serial.print(i + 1);
              Serial.println(" sch is on ");
            } else {
              // digitalWrite(outputs[i].gpio, HIGH);
              switchRelay(i, HIGH);
              Serial.print(i + 1);
              Serial.println(" sch is off ");
            }
            break;
          }
        }
      }

    }

    // else if (outputs[i].timer.charAt(0) != 't' && outputs[i].timer.length() >= 2 && outputs[i].timer.length() < 6) {
    //   int temp = outputs[i].timer.substring(2).toInt();
    //   Serial.println(temp);

    //   char type = outputs[i].timer.charAt(0);
    //   // temp index
    //   char tIndex = outputs[i].timer.charAt(1);
    //   uint8_t ind = 0;
    //   if (tIndex == 'c') {
    //     ind = 2;
    //   } else if (tIndex == 'b') {
    //     ind = 1;
    //   }
    //   // Serial.println("temp:");
    //   // Serial.println(temp);
    //   // Serial.println("type:");
    //   // Serial.println(type);
    //   sensors.requestTemperatures();
    //   float temperatureC = sensors.getTempCByIndex(ind);
    //   Serial.print(temp0);
    //   Serial.println("ºC");
    //   if (type == 'c') {
    //     if (temperatureC > temp) {
    //       //off
    //       // digitalWrite(outputs[i].gpio, LOW);
    //       switchRelay(i, LOW);

    //     } else if (temperatureC < (temp - 3)) {
    //       //on
    //       // digitalWrite(outputs[i].gpio, HIGH);
    //       switchRelay(i, HIGH);
    //     }
    //   } else if (type == 'h') {
    //     if (temperatureC < temp) {
    //       //off
    //       // digitalWrite(outputs[i].gpio, LOW);
    //       switchRelay(i, LOW);

    //     } else if (temperatureC > (temp + 3)) {
    //       //on
    //       // digitalWrite(outputs[i].gpio, HIGH);
    //       switchRelay(i, HIGH);
    //     }
    //   }
    // }
    else if (outputs[i].timer.charAt(0) == 't' && toggleTimers[i] != -1) {
      int8_t toggle = outputs[i].timer.substring(1).toInt();
      if (toggleTimers[i] == -1) {
        Serial.println("check toggle timer");
        Serial.println("toggle time");
        Serial.println(toggle);
        toggleTimers[i] = toggle * 2;
      } else {
        if (toggleTimers[i] > toggle) {
          toggleTimers[i] = toggleTimers[i] - 1;
          // digitalWrite(outputs[i].gpio, LOW);
          switchRelay(i, LOW);
          Serial.println("toggleTimers[i] > toggle");
          Serial.println(toggleTimers[i]);


        } else if (toggleTimers[i] > 0 && toggleTimers[i] <= toggle) {
          toggleTimers[i] = toggleTimers[i] - 1;
          // digitalWrite(outputs[i].gpio, HIGH);
          switchRelay(i, HIGH);
          Serial.println("toggleTimers[i] <= toggle");
          Serial.println(toggleTimers[i]);
        } else if (toggleTimers[i] == 0) {
          toggleTimers[i] = toggle * 2;
          Serial.println("toggleTimers[i] == 0");
        }
      }
    }
  }


  // MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  // getSignalQuality();
}


void saveLastRelayStates() {
  // String rStates = STATE_RELAY_1 ? "1" : "0";
  // rStates += STATE_RELAY_2 ? "1" : "0";
  // rStates += STATE_RELAY_3 ? "1" : "0";
  // rStates += STATE_RELAY_4 ? "1" : "0";
  // writeDateTimeEEPROM(offsetStates, rStates);
  // Serial.println("rStates saved");
  // Serial.println(rStates);
}
void switchRelay(uint8_t index, bool state) {
  if (state == digitalRead(outputs[index].gpio)) {
    return;
  }
  digitalWrite(outputs[index].gpio, state);
  outStates[index] = state ? '0' : '1';
  StaticJsonDocument<64> doc;

  doc["mac"] = mac;
  doc["event"] = "feedback";
  doc["oSt"] = createOutArray();
  String result;
  serializeJson(doc, result);
  Serial.println(result);
  mqtt.publish("action_server", result.c_str());
  // Serial.println(outStates);
  updateStatesDSP();
}

void setPwm(uint8_t index, uint8_t percent) {

  uint8_t value = (percent * (aResolution / 100));
  if (pwms[index].pwm == percent) {
    return;
  }

  ledcWrite(index, value);
  pwms[index].pwm = percent;
  if (mqtt_connected) {
    StaticJsonDocument<64> doc;
    doc["mac"] = mac;
    doc["event"] = "feedback";
    pwmStates = createPwmArray();
    doc["pwm"] = pwmStates;
    doc.remove("percent");
    String result;
    serializeJson(doc, result);
    Serial.println(result);

    mqtt.publish("action_server", result.c_str());
  }
}
/*******************************************************************************
 * Performs action according to the received sms
 ******************************************************************************/
void clearTimer(uint8_t index) {
  writeDateTimeEEPROM(outputs[index].timerKey, "");
  outputs[index].timer = "";
  Serial.print("cleared timer ");
  Serial.println(index);
}

void doAction(String phoneNumber) {

  // Switchs
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

  // r1on r1off
  if (msg.indexOf("r") != -1 && msg.length() > 3) {
    uint8_t out = msg.substring(1, 2).toInt() - 1;
    boolean state = 0;
    if (msg.substring(2, 4) == "on") {
      state = 1;
    } else if (msg.substring(2, 4) == "of") {
      state = 0;
    } else {
      return;
    }

    String outPrg = outputIsBusy(out);
    if (!outPrg.isEmpty()) {
      clearSmsVariables();
      String text = "رله " + String(out + 1) + " قفل و در حالت ";
      text = text + ((outPrg.charAt(0) == 's') ? "دزدگیر قرار دارد" : "پمپ قرار دارد");
      ReplyHex(text, phoneNumber);
      return;
    }
    // digitalWrite(outputs[out].gpio, !state);
    switchRelay(out, !state);

    // STATE_RELAY_1 = state;

    Serial.print(out + 1);
    Serial.print("is ");
    Serial.println(state == 1 ? "ON" : "OFF");
    String text = "";
    if (outputs[out].label.isEmpty()) {
      text = "رله " + String(out + 1);
    } else {
      text = outputs[out].label;
    }

    text = text + ((state) ? " روشن شد " : " خاموش شد ");
    text += "\n" + createOutArray();
    ReplyHex(text, phoneNumber);
    if (!outputs[out].timer.isEmpty()) {
      clearTimer(out);
    }
    events.send("refresh", NULL, millis());
  }

  // 1l=pump
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  else if (msg.indexOf("l") != -1 && msg.indexOf("=") != -1) {
    uint8_t out = (msg.substring(1, 2).toInt()) - 1;
    if (out != -1) {
      writeToEEPROM(outputs[out].labelKey, tempLabel);
      outputs[out].label = tempLabel;
      String text = "نام رله " + String(out + 1) + " به " + tempLabel + " تغییر یافت ";
      ReplyHex(text, phoneNumber);
    }
  }
  //Labels

  else if (msg == "labels") {
    String text = "نام رله ها:\r\n";
    for (uint8_t i = 0; i < totalOutputs; i++) {
      if (!outputs[i].label.isEmpty()) {
        text = text + String(i + 1) + ": " + (outputs[i].label == "" ? "بی نام" : outputs[i].label) + "\r\n";
      }
    }
    debugPrint(text);
    ReplyHex(text, phoneNumber);
  }
  //Input Labels
  else if (msg.indexOf("n") != -1 && msg.indexOf("=") != -1) {
    uint8_t in = (msg.substring(1, 2).toInt()) - 1;
    if (in != -1) {
      writeToEEPROM(inputs[in].labelKey, tempLabel);
      inputs[in].label = tempLabel;
      String text = "نام ورودی " + String(in + 1) + " به " + tempLabel + " تغییر یافت ";
      ReplyHex(text, phoneNumber);
    }
  }

  //Stats
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  else if (msg.indexOf("stat=") != -1) {
    uint8_t out = msg.substring(msg.indexOf("=")).toInt() - 1;
    String text = "رله" + String(out + 1);

    debugPrint("Relay" + String(out + 1) + " is " + text);
    text = text + (digitalRead(outputs[out].gpio) == LOW) ? "روشن است " : "خاموش است ";
    ReplyHex("Relay 1 is " + text, phoneNumber);
  }

  else if (msg == "stat") {
    String text = "";

    for (uint8_t i = 0; i < totalOutputs; i++) {
      text = text + String(i + 1) + (digitalRead(outputs[i].gpio) == LOW) ? "_ON\r\n" : "_OFF\r\n";
    }
    debugPrint(text);
    Reply(text, phoneNumber);
  }
  // Phones
  //p1=09127995883
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  else if (msg.indexOf("p") != -1 && msg.indexOf("=") != -1) {
    uint8_t index = (msg.substring(1, 2).toInt()) - 1;
    if (tempPhone.length() == 13 && tempPhone[0] == '+') {
      writeToEEPROM(offsetPhone[index], tempPhone);
    } else if (tempPhone.length() == 11 && tempPhone[0] == '0') {
      tempPhone = "+98" + tempPhone.substring(1);
      writeToEEPROM(offsetPhone[index], tempPhone);
    }
    phoneNo[index] = tempPhone;
    String text = "شماره مدیر " + String(index + 1) + " با موفقیت ثبت شد";
    Serial.println("Number " + String(index + 1) + " is Registered");
    // debugPrint(text);
    ReplyHex(text, phoneNumber);
  }
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  else if (msg == "list") {
    String text = "لیست کاربران";
    for (uint8_t i = 0; i < totalPhoneNo; i++) {
      if (!phoneNo[i].isEmpty()) {
        //+989127995883
        String phone = "0" + phoneNo[i].substring(3);
        if (i != (totalPhoneNo - 1)) {
          text = text + "\r\n";
        }
        text = text + String(i + 1) + "." + phone;
      }
    }
    debugPrint("List of Registered Phone Numbers: \r\n" + text);
    ReplyHex(text, phoneNumber);
  }
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

  else if (msg.indexOf("del=") != -1) {
    uint8_t index = (msg.substring(4, 5).toInt()) - 1;
    writeToEEPROM(offsetPhone[index], "");
    phoneNo[index] = "";

    ReplyHex("شماره کاربر " + String(index + 1) + "با موفقیت پاک شد", phoneNumber);
  }

  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  else if (msg == "del=all") {
    writeToEEPROM(offsetPhone[0], "");
    writeToEEPROM(offsetPhone[1], "");
    writeToEEPROM(offsetPhone[2], "");
    writeToEEPROM(offsetPhone[3], "");
    writeToEEPROM(offsetPhone[4], "");
    writeDateTimeEEPROM(offsetDateTime, "");
    phoneNo[0] = "";
    phoneNo[1] = "";
    phoneNo[2] = "";
    phoneNo[3] = "";
    phoneNo[4] = "";
    debugPrint("All phone numbers are deleted.");
    ReplyHex("همه شماره ها پاک شدند", phoneNumber);
  }
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  ///set timers
  else if (msg.indexOf("t") != -1 && msg.indexOf("=") != -1) {
    uint8_t index = (msg.substring(1, 2).toInt()) - 1;
    String outPrg = outputIsBusy(index);
    if (!outPrg.isEmpty()) {
      String text = "رله " + String(index + 1) + " قفل و در حالت ";
      text = text + ((outPrg.charAt(0) == 's') ? "دزدگیر قرار دارد" : "پمپ قرار دارد");
      ReplyHex(text, phoneNumber);
      return;
    }
    writeDateTimeEEPROM(outputs[index].timerKey, tempSch);
    outputs[index].timer = tempSch;
    String text;
    if (tempSch.length() >= 2 && tempSch.length() < 6 && tempSch.charAt(0) != 't') {
      String t;
      if (tempSch.substring(2, 4) == "-") {
        bool minus = true;
        t = tempSch.substring(3, 5) + " درجه ";
      } else {
      }
      t = tempSch.substring(2, 4) + " درجه ";
      if (tempSch.charAt(0) == 'c') {
        text = " عملکرد فن رله " + String(index + 1) + " در دمای پایینتر از" + t + " فعال شد";
      } else if (tempSch.charAt(0) == 'h') {
        text = " عملکرد هیتر رله " + String(index + 1) + " در دمای بالاتر از" + t + " فعال شد";
      }
    } else {
      text = "تایمر رله " + String(index + 1) + " فعال شد";
    }
    if (outputs[index].timer.charAt(0) == 't') {
      int8_t toggle = outputs[index].timer.substring(1).toInt();
      toggleTimers[index] = toggle * 2;
    } else {
      toggleTimers[index] = -1;
    }

    ReplyHex(text, phoneNumber);
    debugPrint("Relay " + String(index + 1) + " is Set :");
    debugPrint(tempSch);
  }
  //set input program
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

  else if (msg.indexOf("s") != -1 && msg.indexOf("=") != -1) {
    uint8_t in = (msg.substring(1, 2).toInt()) - 1;
    String text = addScenario(tempSch);
    if (text != "") {
      ReplyHex(text, phoneNumber);
    }
    debugPrint("Input " + String(in + 1) + " is Set :");
    debugPrint(tempSch);
  }
  //clear input program
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  else if (msg.indexOf("s") != -1 && msg.indexOf("x") != -1) {
    uint8_t in = (msg.substring(1, 2).toInt()) - 1;
    writeDateTimeEEPROM(scenarios[in].key, "");
    scenarios[in].value = "";
    String text = "برنامه ورودی " + String(in + 1) + " پاک شد ";
    ReplyHex(text, phoneNumber);
    debugPrint("Input " + String(in + 1) + " prg deleted.");
  } else if (msg == "set") {
    /// setting variables
    //  callOnAlert = true;
    //  securityMode = false;
    //  notifySwitchOn = true;
    //  int remoteCount = 0;
    String text =
      "setting : " + String(callOnAlert) + "," + String(securityMode) + "," + String(notifySwitchOn) + "," + String(remoteCount);

    ReplyHex(text, phoneNumber);
  }
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  else if (msg.indexOf("t") != -1 && msg.indexOf("x") != -1) {
    uint8_t out = (msg.substring(1, 2).toInt()) - 1;
    writeDateTimeEEPROM(outputs[out].timerKey, "");
    outputs[out].timer = "";
    toggleTimers[out] = -1;

    String text = " عملکرد رله" + String(out + 1) + " پاک شد ";
    ReplyHex(text, phoneNumber);
    debugPrint("Relay " + String(out + 1) + " Sch deleted.");
    checkTasks();
  } else if (msg == "sch") {
    String text = "لیست عملکرد رله ها: \r\n";
    for (uint8_t i = 0; i < totalOutputs; i++) {
      if (!outputs[i].timer.isEmpty()) {
        text = text + String(i + 1) + ": " + (outputs[i].timer == "" ? "ساده" : outputs[i].timer) + "\r\n";
      }
    }
    debugPrint("List of Schedules: \r\n" + text);
    ReplyHex(text, phoneNumber);
  } else if (msg == "temp") {
    String text = "دمای فعلی :";
    text = text + "\r\n";
    sensors.requestTemperatures();

    for (uint8_t i = 0; i < totalTemps; i++) {
      temps[i] = sensors.getTempCByIndex(i);
      text = text + String(temps[i]) + "ºC" + "\r\n";
    }
    // temp0 = sensors.getTempCByIndex(0);
    // temp1 = sensors.getTempCByIndex(1);
    // temp2 = sensors.getTempCByIndex(2);
    // text = text + String(temp0) + "ºC" + "\r\n";
    // text = text + String(temp1) + "ºC" + "\r\n";
    // text = text + String(temp2) + "ºC" + "\r\n";
    ReplyHex(text, phoneNumber);
    debugPrint(msg);
  }
  // SECURITY & PUMP OFF
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  else if (msg == "son") {
    // bool flag = 0;
    // for (uint8_t i = 0; i < totalInputs; i++) {
    //   if (inputs[i].value.charAt(0) == 's') {
    //     uint8_t out = inputs[i].out - 1;
    //     flag = 1;
    //   }
    // }
    // if (flag) {
    //   securityMode = true;
    //   digitalWrite(STATUS_LED, HIGH);
    //   String text = "سیستم امنیتی فعال شد";
    //   alertEnableDisplay();
    //   ReplyHex(text, phoneNumber);
    // }
  } else if (msg == "soff") {
    // bool flag = 0;
    // for (uint8_t i = 0; totalInputs < 4; i++) {
    //   if (inputs[i].value.charAt(0) == 's') {
    //     uint8_t out = inputs[i].out - 1;

    //     if (digitalRead(outputs[out].gpio) == LOW) {
    //       flag = 1;
    //       // digitalWrite(outputs[out].gpio, HIGH);
    //       switchRelay(out, HIGH);
    //     }
    //   }
    // }
    // if (flag) {
    //   securityMode = false;
    //   digitalWrite(STATUS_LED, LOW);
    //   String text = "هشدارهای سیستم امنیتی خاموش شدند";
    //   ReplyHex(text, phoneNumber);
    // }


  }  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  else if (msg == "balance") {
    Serial.println("Charge Message Recieved!");
    Serial.println(msg);
    if (op == "mci") {
      Serial2.println(TCI_CHARGE);
    } else if (op == "irancell") {
      Serial2.println(IRANCEL_CHARGE);
    }
  }

  else if (msg == "non") {
    notifySwitchOn = true;
    String text = "اعلام روشن شدن دستگاه، فعال شد";
    ReplyHex(text, phoneNumber);
  } else if (msg == "noff") {
    notifySwitchOn = false;
    String text = "اعلام روشن شدن دستگاه، خاموش شد";
    ReplyHex(text, phoneNumber);
  }

  else if (msg == "poff") {
    // bool flag = 0;
    // for (uint8_t i = 0; i < totalInputs; i++) {
    //   if (inputs[i].value.charAt(0) == 'p') {
    //     uint8_t out = inputs[i].out - 1;
    //     flag = 1;
    //     if (digitalRead(outputs[out].gpio) == LOW) {
    //       // digitalWrite(outputs[out].gpio, HIGH);
    //       switchRelay(out, HIGH);
    //     }
    //   }
    // }
    // if (flag) {
    //   String text = "پمپ خاموش شد";
    //   ReplyHex(text, phoneNumber);
    // }

  } else if (msg == "wipe") {
    // write a 0 to all 512 bytes of the EEPROM
    Serial.println("restet called");
    EEPROM.clear();
    REMOTES.clear();

    String text = "دستگاه ریست و حافظه پاک شد ";
    ReplyHex(text, phoneNumber);
    ESP.restart();
  }

  else if (msg == "dr") {
    Serial.print("Remote Saved!");
    REMOTES.clear();
    remoteCount = 0;
    String text = "همه ریموت ها پاک شدند";
    ReplyHex(text, phoneNumber);
  }

  else if (msg == "calloff") {
    callOnAlert = false;
    String text = "تماس تلفنی غیرفعال شد";
    ReplyHex(text, phoneNumber);
  } else if (msg == "callon") {
    callOnAlert = true;
    String text = "تماس تلفنی فعال شد";
    ReplyHex(text, phoneNumber);
  }
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  clearSmsVariables();
}

void clearSmsVariables() {
  smsStatus = "";
  receivedDate = "";
  msg = "";
  tempPhone = "";
  tempSch = "";
  tempLabel = "";
}


String outputIsBusy(uint8_t index) {
  // for (uint8_t i = 0; i < totalOutputs; i++) {
  //   if (inputs[i].out == index) {
  //     return inputs[i].value;
  //   }
  // }
  // if (outputs[index].timer != "") {
  //   return outputs[index].timer;
  // }
  return "";
}



/*******************************************************************************
 * Reply function
 * Send an sms
 ******************************************************************************/
void Reply(String text, String Phone) {
  Serial2.print("AT+CMGF=1\r");
  delay(1000);
  if (op == "irancell") {
    Serial2.println("AT+CSMP=17,167,0,0");
    delay(1000);
  }
  Serial2.print("AT+CMGS=\"" + Phone + "\"\r");
  delay(1000);
  Serial2.print(text);
  delay(100);
  Serial2.write(0x1A);
  //ascii code for ctrl-26 //sim800.println((char)26); //ascii code for ctrl-26
  delay(2000);
  Serial.println("SMS Sent Successfully.");
  receiveSms();
}

void ReplyHex(String text, String Phone) {
  char charArray[160];  // Adjust size as needed
  text.toCharArray(charArray, 160);
  String hexString = printCodePoints((uint8_t *)charArray);

  Serial2.print("AT+CMGF=1\r");
  delay(1000);

  Serial2.print("AT+CSCS=\"HEX\"\r");
  delay(1000);

  // if (op == "irancell") {
  Serial2.println("AT+CSMP=17,167,0,8");
  delay(1000);
  // }
  Serial2.print("AT+CMGS=\"" + Phone + "\"\r");
  delay(1000);
  Serial2.print(hexString);
  delay(100);
  Serial2.write(0x1A);
  //ascii code for ctrl-26 //sim800.println((char)26); //ascii code for ctrl-26
  delay(3000);

  Serial.println("SMS Sent Successfully.");
  receiveSms();
}
void ForwardHex(String text, String Phone) {

  Serial2.print("AT+CMGF=1\r");
  delay(1000);

  Serial2.print("AT+CSCS=\"HEX\"\r");
  delay(1000);

  // if (op == "irancell") {
  Serial2.println("AT+CSMP=17,167,0,8");
  delay(1000);
  // }
  Serial2.print("AT+CMGS=\"" + Phone + "\"\r");
  delay(1000);
  Serial2.print(text);
  delay(100);
  Serial2.write(0x1A);
  //ascii code for ctrl-26 //sim800.println((char)26); //ascii code for ctrl-26
  delay(3000);

  Serial.println("SMS Sent Successfully.");
  receiveSms();
}
void callAdmin() {
  Serial2.println("ATD+ " + phoneNo[0] + ";");
  delay(20000);
  Serial2.println("ATH");
}




/*******************************************************************************
 * writeToEEPROM function:
 * Store registered phone numbers in EEPROM
 ******************************************************************************/
void writeToEEPROM(const char *addrOffset, const String &strToWrite) {

  // byte len = 13;  //strToWrite.length();
  //EEPROM.write(addrOffset, len);
  EEPROM.putString(addrOffset, strToWrite);

  // for (int i = 0; i < len; i++) {
  // }
}
void writeIntToEEPROM(const char *addrOffset, unsigned int value) {

  EEPROM.putUInt(addrOffset, value);
}

void writeDateTimeEEPROM(const char *addrOffset, const String &strToWrite) {
  EEPROM.putString(addrOffset, strToWrite);
}


void countRemoteToEEPROM(uint16_t *count) {
  // EEPROM.putUInt("rmts", count);
}



/*******************************************************************************
 * readFromEEPROM function:
 * Store phone numbers in EEPROM
 ******************************************************************************/
String readFromEEPROM(const char *addrOffset) {
  String value = EEPROM.getString(addrOffset);
  return value;
}
int readIntFromEEPROM(const char *addrOffset) {
  return EEPROM.getUInt(addrOffset, 0);
}



/*******************************************************************************
 * comparePhone function:
 * compare phone numbers stored in EEPROM
 ******************************************************************************/
boolean comparePhone(String number) {
  boolean flag = 0;
  //--------------------------------------------------
  for (uint8_t i = 0; i < totalPhoneNo; i++) {
    phoneNo[i] = readFromEEPROM(offsetPhone[i]);
    if (phoneNo[i].equals(number)) {
      flag = 1;
      break;
    }
  }

  //--------------------------------------------------
  return flag;
}


void flip() {
  // uint8_t state = digitalRead(BUILTIN_LED);  // get the current state of GPIO1 pin
  // digitalWrite(BUILTIN_LED, !state);         // set pin to the opposite state

  ++count;
  // when the counter reaches a certain value, start blinking like crazy
  if (count == 10) {
    flipper.attach(0.1, flip);
  }
  // when the counter reaches yet another value, stop blinking
  else if (count == 60) {
    flipper.detach();
  }
}



/*******************************************************************************
 * debugPrint function:
 * compare phone numbers stored in EEPROM
 ******************************************************************************/
void debugPrint(String text) {
  if (DEBUG_MODE == 1) {
    Serial.println(text);
  }
}

String createOutArray() {
  String result = "";
  for (uint8_t i = 0; i < totalOutputs; i++) {
    result += (digitalRead(outputs[i].gpio)) ? '0' : '1';
    // if (i < (totalOutputs - 1)) {
    //   result += ",";
    // }
  };
  outStates = result;
  return result;
}

String createPwmArray() {
  String result = "";
  for (uint8_t i = 0; i < totalPwm; i++) {
    result += pwms[i].pwm;
    if (i < (totalPwm - 1)) {
      result += ",";
    }
  };
  pwmStates = result;
  return result;
}


String createInArray() {
  String result = "";
  for (uint8_t i = 0; i < totalInputs; i++) {
    result += (digitalRead(inputs[i].gpio)) ? '1' : '0';
    // if (i < (totalOutputs - 1)) {
    // result += ",";
    // }
  };
  inStates = result;

  return result;
}


String createSettingArray() {
  String result = "";
  result += callOnAlert ? '1' : '0';
  result += securityMode ? '1' : '0';
  result += notifySwitchOn ? '1' : '0';
  result += hasWifi ? '1' : '0';
  result += hasGSM ? '1' : '0';
  return result;
}



String prepareData() {
  StaticJsonDocument<512> doc;
  sensors.requestTemperatures();
  for (uint8_t i = 0; i < totalTemps; i++) {
    temps[i] = sensors.getTempCByIndex(i);
  }
  // temp0 = sensors.getTempCByIndex(0);
  // temp1 = sensors.getTempCByIndex(1);
  // temp2 = sensors.getTempCByIndex(2);

  doc["network"] = op;
  JsonArray array = doc.createNestedArray("ain");
  for (uint8_t i = 0; i < totalTemps; i++) {
    temps[i] = sensors.getTempCByIndex(i);
    array.add(temps[i]);
  }
  // doc["temp0"] = temp0;
  // doc["temp1"] = temp0;
  // doc["temp2"] = temp0;
  for (uint8_t i = 0; i < totalOutputs; i++) {
    JsonObject temp = doc.createNestedObject(String(i));
    temp["name"] = (outputs[i].label.isEmpty()) ? "Relay " + String(i + 1) : outputs[i].label;
    temp["timer"] = (outputs[i].timer.isEmpty()) ? "" : outputs[i].timer;
    temp["state"] = (digitalRead(outputs[i].gpio)) ? 0 : 1;
  };
  // for (uint8_t i = 0; i < totalInputs; i++) {
  //   JsonObject tmp = doc.createNestedObject("input" + String(i));
  //   tmp["value"] = (inputs[i].value.isEmpty()) ? "" : String(inputs[i].state);
  // };
  String result;
  serializeJson(doc, result);
  Serial.print(result);
  return result;
}

String prepareDbInputs(bool status) {
  StaticJsonDocument<512> doc;

  doc["status"] = status;
  String result;
  serializeJson(doc, result);
  return result;
}

String mqttPeresence() {
  StaticJsonDocument<64> doc;
  doc["mac"] = mac;
  // doc["connected"] = rtc.getEpoch();
  doc["event"] = "report";
  doc["oSt"] = outStates;
  doc["iSt"] = inStates;
  doc["pwm"] = pwmStates;

  unsigned char unsignedCharArray[164];  // +1 for the null terminator

  // String result;
  serializeJson(doc, unsignedCharArray, 164);
  // Serial.println("ciphered:" + vigenereCipher(result, cipher_key, true));  //true to encode
  // Serial.println(result);

  // Encode base64 data
  unsigned char base64[164];
  // encode_base64() places a null terminator automatically, because the output is a string
  unsigned int base64_length = encode_base64(unsignedCharArray, strlen((char *)unsignedCharArray), base64);
  Serial.println(String((const char *)base64));
  return String((const char *)base64);
}


String createTimersArray() {
  String str;
  for (uint8_t i = 0; i < totalOutputs; i++) {
    if (outputs[i].timer.length() > 0) {
      str += outputs[i].timer;
    } else {
      str += "-";
    }
    if (i < (totalOutputs - 1)) {
      str += ",";
    }
  }
  Serial.println(str);
  return str;
}
String createScenariosArray() {
  String str = "";
  for (uint8_t i = 0; i < scenariosCount; i++) {
    if (scenarios[i].value.length() > 0) {
      str += scenarios[i].value;
      str += ":s";
      str += String(i + 1);
    }
    if (i < (scenariosCount - 1)) {
      str += ",";
    }
  }

  return str;
}


String prepareDbData(String event) {

  StaticJsonDocument<512> doc;


  doc["op"] = op;
  doc["sig"] = String(signalQuality);

  doc["mac"] = mac;
  doc["event"] = event;
  doc["status"] = "ONLINE";
  // doc["conn"] = rtc.getEpoch();
  doc["sets"] = createSettingArray();
  doc["tims"] = createTimersArray();
  doc["progs"] = createScenariosArray();


  doc["oSt"] = createOutArray();
  doc["iSt"] = createInArray();
  doc["pwm"] = createPwmArray();

  sensors.requestTemperatures();


  JsonArray array1 = doc.createNestedArray("temps");
  for (uint8_t i = 0; i < totalTemps; i++) {
    temps[i] = sensors.getTempCByIndex(i);
    array1.add(temps[i]);
  }
  JsonArray array2 = doc.createNestedArray("ain");

  for (uint8_t i = 0; i < totalAnalogs; i++) {
    array2.add(analogInputs[i].voltage);
  }

  String result;
  serializeJson(doc, result);
  Serial.println(result);

  return result;
}

String prepareTimersData() {
  StaticJsonDocument<256> doc;
  sensors.requestTemperatures();


  doc["mac"] = mac;
  doc["event"] = "report";
  doc["timers"] = createTimersArray();
  doc["progs"] = createScenariosArray();

  String result;
  serializeJson(doc, result);
  Serial.println(result);

  return result;
}

String prepareSync() {
  StaticJsonDocument<256> doc;
  doc["mac"] = "macString";
  String result;
  serializeJson(doc, result);
  return result;
}

String prepareSMSStats() {
  String text = "";
  for (int i = 0; i < totalOutputs; i++) {
    String temp;
    if (outputs[i].label.isEmpty()) {
      temp = temp + "r" + String(i + 1) + "-";
    } else {
      temp = temp + outputs[i].label + "-";
    }
    if (outputs[i].timer.isEmpty()) {
      temp = temp + "t" + "-";
    } else {
      temp = temp + outputs[i].timer + "-";
    }
    if (scenarios[i].value.isEmpty()) {
      temp = temp + "i" + "-";
    } else {
      temp = temp + scenarios[i].value + "-";
    }
    if (digitalRead(outputs[i].gpio)) {
      temp = temp + 0;
    } else {
      temp = temp + 1;
    }
    temp = temp + "=";
    if (i != 3) {
    }
    text = text + temp;
    Serial.println(temp);
  };
  sensors.requestTemperatures();

  // temp0 = sensors.getTempCByIndex(0);
  for (uint8_t i = 0; i < totalTemps; i++) {
    temps[i] = sensors.getTempCByIndex(i);
  }
  text = text + String(temps[0]) + "/";
  text = text + op + "/";
  text = text + String(signalQuality) + "/";
  text = text + String(callOnAlert) + "/" + String(securityMode) + "/" + String(notifySwitchOn) + "/" + String(remoteCount);

  return text;
  Serial.println(text);
}

void startOTA() {
  // ArduinoOTA.begin(WiFi.localIP(), "Arduino", "password", InternalStorage);
}

void checkUpdate(String firmwareUrl) {
  WiFiClient client;
  // client.setCACert(rootCACertificate);
  // client.setReuse(false);  // add this

  // Reading data over SSL may be slow, use an adequate timeout
  client.setTimeout(12000);  // timeout argument is defined in milliseconds for setTimeout

  // The line below is optional. It can be used to blink the LED on the board during flashing
  // The LED will be on during download of one buffer of data from the network. The LED will
  // be off during writing that buffer to flash
  // On a good connection the LED should flash regularly. On a bad connection the LED will be
  // on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
  // value is used to put the LED on. If the LED is on with HIGH, that value should be passed
  // httpUpdate.setLedPin(LED_BUILTIN, HIGH);
  httpUpdate.onProgress(update_progress);

  t_httpUpdate_return ret = httpUpdate.update(client, firmwareUrl, "", [](HTTPClient *client) {
    // client->setAuthorization("test", "password");
  });
  // Or:
  //t_httpUpdate_return ret = httpUpdate.update(client, "server", 443, "/file.bin");
  StaticJsonDocument<256> doc;
  doc["event"] = "feedback";
  doc["mac"] = mac;

  String result;

  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
      doc["update"] = "failed";
      serializeJson(doc, result);
      mqtt.publish("action_server", result.c_str());
      Serial.println("HTTP_UPDATE_OK");
      break;
    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("HTTP_UPDATE_NO_UPDATES");
      doc["update"] = "already updated!";
      serializeJson(doc, result);
      mqtt.publish("action_server", result.c_str());
      Serial.println("HTTP_UPDATE_OK");
      break;

    case HTTP_UPDATE_OK:
      doc["update"] = "succes";
      serializeJson(doc, result);
      mqtt.publish("action_server", result.c_str());
      Serial.println("HTTP_UPDATE_OK");
      break;
  }
}

void update_progress(int cur, int total) {
  Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
  int p = (cur * 100) / total;
  loadingDisplay(cur, "Updating");
}
void handleSketchDownload(String server) {
  // const char *SERVER = server.c_str();        // Set your correct hostname
  // const unsigned short SERVER_PORT = 443;     // Commonly 80 (HTTP) | 443 (HTTPS)
  // const char *PATH = "/update-v%d.bin";       // Set the URI to the .bin firmware
  // const unsigned long CHECK_INTERVAL = 6000;  // Time interval between update checks (ms)

  // // Time interval check
  // static unsigned long previousMillis;
  // unsigned long currentMillis = millis();
  // if (currentMillis - previousMillis < CHECK_INTERVAL)
  //   return;
  // previousMillis = currentMillis;

  // // HttpClient client(wifiClient, SERVER, SERVER_PORT);  // HTTP

  // HttpClient client(client, SERVER, SERVER_PORT);  // HTTPS

  // char buff[32];
  // snprintf(buff, sizeof(buff), PATH, VERSION + 1);

  // Serial.print("Check for update file ");
  // Serial.println(buff);

  // // Make the GET request
  // client.get(buff);

  // int statusCode = client.responseStatusCode();
  // Serial.print("Update status code: ");
  // Serial.println(statusCode);
  // if (statusCode != 200) {
  //   client.stop();
  //   return;
  // }

  // long length = client.contentLength();
  // if (length == HttpClient::kNoContentLengthHeader) {
  //   client.stop();
  //   Serial.println("Server didn't provide Content-length header. Can't continue with update.");
  //   return;
  // }
  // Serial.print("Server returned update file of size ");
  // Serial.print(length);
  // Serial.println(" bytes");


  // InternalStorage.close();
  // client.stop();
}