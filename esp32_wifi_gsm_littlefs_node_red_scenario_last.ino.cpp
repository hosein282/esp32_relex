# 1 "C:\\Users\\Amin\\AppData\\Local\\Temp\\tmphoi_glxt"
#include <Arduino.h>
# 1 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
#include <LittleFS.h>
#include "base64.hpp"


#define TINY_GSM_MODEM_SIM800 

#define DEVICE_MODEL "G84-TR"

const short VERSION = 2;

#include <WiFi.h>
#include <ESPmDNS.h>
#include <TinyGsmClient.h>
#include <WiFiClientSecure.h>
#include <EmonLib.h>

#define SerialAT Serial2
#define TINY_GSM_DEBUG Serial

const char mci_apn[] = "mcinet";
const char irancell_apn[] = "mtnirancell ";
const char other_apn[] = "internet";
const char gprsUser[] = "";
const char gprsPass[] = "";

TinyGsm modem(SerialAT);


#include "hex.h"
#include <ESPAsyncWebServer.h>

#include <HTTPUpdate.h>
#include "index.h"


#include <ArduinoJson.h>
#include <HardwareSerial.h>
#include <esp_task_wdt.h>


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64


#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#include <Preferences.h>
Preferences EEPROM;


#include <Ticker.h>
#include <ESP32Time.h>

#include <PubSubClient.h>

const char *mqtt_server = "broker.emqx.io";



WiFiClient wifiClient;

TinyGsmClient gsmClient(modem);

Client *activeClient = nullptr;
PubSubClient mqtt;
uint port = 1883;



#include "time.h"
#include <Adafruit_MCP23X17.h>
Adafruit_MCP23X17 mcp;


#include <OneWire.h>
#include <DallasTemperature.h>


const uint8_t oneWireBus = 34;

OneWire oneWire(oneWireBus);


DallasTemperature sensors(&oneWire);


#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();



EnergyMonitor emon;

const int ctPin = 33;
const double assumedVoltage = 220;

unsigned long lastSampleTime = 0;
unsigned long sampleInterval = 15000;



const char *ssid_ap = "VIIO-";
String ssid = "";
String password = "";
String mac = "";
String m_server = "5.238.178.210";
String sub_topic = "action";
String pub_topic = "report";


const char *wifi_ssid = "/ssid.txt";
const char *wifi_pass = "/password.txt";
const char *m_server_k = "/server.txt";
const char *mac_f = "/mac.txt";


const uint8_t totalPhoneNo = 5;
const uint8_t totalPwm = 1;
const uint8_t totalOutputs = 8;
const uint8_t totalInputs = 8;
const uint8_t totalRemotes = 48;
const uint8_t totalTemps = 3;
const uint8_t totalAnalogs = 2;
const uint8_t totalScenarios = 16;

enum MQTT_NET
{
  OFF = 0,
  WIFI = 1,
  GPRS = 2,
};
MQTT_NET mqttNet = OFF;

uint8_t signalQuality = 0;
uint8_t wifiTryCount = 0;



char *TCI_CHARGE = "AT+CUSD=1,\"*140*11#\"";
char *IRANCEL_CHARGE = "AT+CUSD=1,\"*140*121#\"";
char *RIGHTEL_CHARGE = "AT+CUSD=1,\"*141*1#\"";

struct Task
{
  unsigned long executeAt;
  void (*function)();
};

#define MAX_TASKS 10
Task taskQueue[MAX_TASKS];
int taskCount = 0;
#define WINDOW_SIZE 6

struct analog
{
  uint16_t value;
  float temp;
  float avg;
  float buffer[WINDOW_SIZE];
  bool active;
};
struct output
{
  char labelKey[5];
  char timerKey[5];
  uint8_t gpio;
  boolean state;
  String label;
  String timer;
  uint8_t pwm;
  uint8_t type;
  unsigned long now;
  uint16_t time;
  boolean locked;

};

struct Scenario
{
  char key[4];
  String value;
  int8_t input;
  String condition;
  float threshold;
  int8_t outPin;
  int8_t outState;
  uint8_t swType;
  uint8_t notif;
  unsigned long lastNotif;
};
struct Input
{
  char labelKey[4];
  char key[4];
  String label;
  float voltage;
  boolean state;
  uint8_t gpio;
  unsigned long lastTrigger;
  float avg;
  float buffer[WINDOW_SIZE];
};

int scenariosCount = 0;

Scenario scenarios[totalScenarios] = {};

String phoneNo[totalPhoneNo] = {"", "", "", "", ""};

int8_t toggleTimers[8] = {-1, -1, -1, -1, -1, -1, -1, -1};

Input inputs[totalInputs] = {
    {"il1", "in1", "", 0.0, 1, 7, 0, 0.0, {}},
    {"il2", "in2", "", 0.0, 1, 6, 0, 0.0, {}},
    {"il3", "in3", "", 0.0, 1, 5, 0, 0.0, {}},
    {"il4", "in4", "", 0.0, 1, 4, 0, 0.0, {}},
    {"il5", "in5", "", 0.0, 1, 3, 0, 0.0, {}},
    {"il6", "in6", "", 0.0, 1, 2, 0, 0.0, {}},
    {"il7", "in7", "", 0.0, 1, 1, 0, 0.0, {}},
    {"il8", "in8", "", 0.0, 1, 0, 0, 0.0, {}},
};

Input analogInputs[totalAnalogs] = {
    {"al1", "a1", "", 0, 0, 36, 0, 0.0, {}},
    {"al2", "a2", "", 0, 0, 39, 0, 0.0, {}}};

output outputs[totalOutputs] = {
    {"lab1", "sch1", 8, 1, "", "", 0, 0, 0},
    {"lab2", "sch2", 9, 1, "", "", 0, 0, 0},
    {"lab3", "sch3", 10, 1, "", "", 0, 0, 0},
    {"lab4", "sch4", 11, 1, "", "", 0, 0, 0},
    {"lab5", "sch5", 12, 1, "", "", 0, 0, 0},
    {"lab6", "sch6", 13, 1, "", "", 0, 0, 0},
    {"lab7", "sch7", 14, 1, "", "", 0, 0, 0},
    {"lab8", "sch8", 14, 1, "", "", 0, 0, 0},
};

output pwms[totalPwm] = {
    {"pwm1", "pS1", 26, 1, "", "", 0},

};

analog temps[totalTemps] = {};
uint8_t lastTempShown = 0;

analog currentAmp = {0, 0.0, {}, true};

const char *offsetPhone[totalPhoneNo] = {"p1", "p2", "p3", "p4", "p5"};

const char *offsetStates = "states";

uint minCounter = 0;
uint8_t gsmCounter = 0;

String tempLabel = "";
String tempPhone = "";
String tempSch = "";

unsigned int deviceYear = 0;

String op = "";
String buffer;

#define MQTT_MAX_PACKET_SIZE 1024
#define SCH_TASK_TIME 60000
unsigned int aResolution = 4095;
const int freq = 3000;

unsigned int analog_read_threshold = 1500;
float temp_threshold = 1.0;
uint8_t signal_threshold = 2;
uint8_t TEMP_THRESHOLD_BIAS = 3.0;
uint8_t THRESHOLD_BIAS = 204;

unsigned long prevCallTime = 0;
unsigned long prevTaskTime = 0;
unsigned long prevRfTime = 0;

static const unsigned char PROGMEM image_Icon_Wifi_bits[] = {0x1e, 0x00, 0x7f, 0x80, 0xc0, 0xc0, 0x9e, 0x40, 0x3f, 0x00, 0x21, 0x00, 0x0c, 0x00, 0x0c, 0x00};



AsyncWebServer server(80);
AsyncEventSource events("/events");



const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 12600;
const int daylightOffset_sec = 0;

unsigned long Epoch_Time;



Ticker flipper;


ESP32Time rtc(0);



#define SHORT_PRESS_TIME 2000
#define LONG_PRESS_TIME 5000
#define VERY_LONG_TIME 8000
#define SUPER_LONG_TIME 12000
#define MQTT_REPORT_INTERVAL 3000
#define SENSOR_DEBOUNCE_DELAY 2000



#define rxPin 16
#define txPin 17






#define BUTTON_PIN 25
#include <Bounce2.h>
Bounce2::Button button = Bounce2::Button();

#define RESET_GSM 27



#define STATUS_LED 2

String outStates = "";
String inStates = "";
String pwmStates = "";






String smsStatus, senderNumber, receivedDate, msg, date = "";
boolean smsIsReady = false;



boolean DEBUG_MODE = 1;


bool simInserted = false;
bool gsmNetwork = false;
bool hasWifi = false;
bool forceUseGprs = false;
bool gprsConnected = false;
bool mqtt_connected = false;
bool callOnAlert = true;
bool securityMode = false;
bool notifyScenarios = true;
bool alerting = false;
unsigned int remoteCount = 0;

unsigned long now = millis();
unsigned long lastTrigger = 0;







const char *PARAM_INPUT_1 = "output";
const char *PARAM_INPUT_2 = "state";
const char *PARAM_INPUT_3 = "timer";
const char *PARAM_INPUT_4 = "label";
const char *PARAM_INPUT_5 = "pwm";

uint count = 0;

unsigned long pressedTime = 0;
unsigned long releasedTime = 0;
unsigned long previousMillis = 0;
String processor(const String &var);
void callback(char *topic, byte *payload, unsigned int length);
void reconnect();
void initLittleFS();
bool initWiFi();
void setupLCD();
void initDisplay();
void updateDisplay();
void updateSignalDisp();
void updateOperatorDisp();
void updateStatesDSP();
void updateTempDSP();
void loadingDisplay(int progress, String title);
void alertDisplay();
void alertEnableDisplay();
void updatedDisplay();
void smsDisplay();
void clearSmsDisplay();
void reloadDisplay();
void displayRemoteRg(String remote, unsigned long time);
void displayRemoteDel(String remote, unsigned long time);
void i2cScanner();
void setup();
void checkSimNetwork();
void loop();
void getEnergyCons();
void processTasks();
void checkRelayTimes();
void switchRelay(uint8_t index, bool state, uint16_t time, bool isLocked);
void runScenarios();
void notifHexSms(int index, String phone);
void setupVariables();
bool processScenarios(String command, uint8_t index);
void handlePostRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void initWifiAp();
String getMAC();
bool printLocalTime();
void setTimezone(String timezone);
void resetWifi();
String createDateString(const struct tm &timeinfo, int timezoneOffset);
boolean TimePeriodIsOver(unsigned long &periodStartTime, unsigned long TimePeriod);
void publishReport(const char *payload);
void checkSensors();
void readAnalogs();
void checkRfRemote();
void readButton();
void handleShortPress();
void handleRemoteRegister();
void saveRemoteCode(const char *code, uint16_t out);
void handleRemoveRemote();
void removeAllRemotes();
void removeRemoteCode(const char *code);
void compareRemote(String received);
void initSim800Mqtt();
void setupGSM();
void initSms();
void checkMqttStatus();
String SendShortCommand(String command, String response);
String WaitForResponse(String response);
bool checkSim();
void getOperator(bool report);
void getSignalQuality(bool report);
void blinkLed();
void GsmSoftReset();
void GsmReset();
void checkOutputSch(uint8_t input);
String addScenario(String val);
void removeScenario(const char *key);
void readyForSms();
void receiveSms();
void getGsmDateTime();
void parseData(String buff);
void extractUssd(String buff);
void readSerial();
bool isRegistered();
void extractSms(String buff);
void updateDate(String dateTime);
void checkHourTasks();
void checkSmsHistory();
void checkTasks();
void setAverageElement();
void createMovingAverage();
void saveLastRelayStates();
void sendMqttFeedback();
void setPwm(uint8_t index, uint8_t percent);
void clearTimer(uint8_t index);
void doAction(String phoneNumber);
void clearSmsVariables();
String outputIsBusy(uint8_t index);
void Reply(String text, String Phone);
void ReplyHex(String text, String Phone);
void ForwardHex(String text, String Phone);
void callAdmin(int index);
void hangUp();
void answerCall();
void writeToEEPROM(const char *addrOffset, const String &strToWrite);
void writeIntToEEPROM(const char *addrOffset, unsigned int value);
void writeDateTimeEEPROM(const char *addrOffset, const String &strToWrite);
String readFromEEPROM(const char *addrOffset);
int readIntFromEEPROM(const char *addrOffset);
boolean comparePhone(String number);
void flip();
void debugPrint(String text);
String createOutArray();
String createPwmArray();
String createInArray();
String createSettingArray();
String prepareData();
String mqttPeresence();
String createTimersArray();
String createScenariosArray();
String prepareDbData(String event);
String prepareDbLog(String event);
String prepareTimersData();
String prepareSync();
String prepareSMSStats();
void checkUpdate(String firmwareUrl);
void update_progress(int cur, int total);
#line 388 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
String processor(const String &var)
{
  if (var == "BUTTONPLACEHOLDER")
  {
    return prepareData();
  }
  else if (var == "WSYNC")
  {
    return prepareSync();
  }
  return String();
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");


  unsigned char base64Data[length + 1];
  memcpy(base64Data, payload, length);
  base64Data[length] = '\0';


  unsigned char decodedData[256];
  int decodedLen = decode_base64(base64Data, decodedData);

  DynamicJsonDocument doc(256);
  DeserializationError error = deserializeJson(doc, decodedData);

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  int out = doc["out"].as<int>();
  String event = doc["event"].as<String>();
  Serial.println(event);

  if (event == "io")
  {
    int type = doc["type"].as<String>().toInt();
    int time = 3;
    if (doc["time"] != "" && doc["time"] != nullptr)
    {
      time = doc["time"].as<int>();
    }
    bool state = doc["state"].as<bool>();
    String prg = outputIsBusy(out);

    Serial.print("event io : ");
    Serial.println(type);
    if (prg.isEmpty())
    {
      outputs[out].type = type;
      switchRelay(out, !state, time, false);

      doc["mac"] = mac;
      doc["event"] = "feedback";
      doc["oSt"] = outStates;
      String result;
      serializeJson(doc, result);
      Serial.println(result);
      publishReport(result.c_str());


    }
    else
    {
      Serial.println("output is busy");
    }

  }
  else if (event == "pwm")
  {

    uint8_t percent = doc["percent"].as<uint8_t>();

    Serial.print("event pwm : ");
    Serial.println(percent);

    setPwm(out, percent);


  }
  else if (event == "son")
  {
# 499 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
  }
  else if (event == "soff")
  {
# 527 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
  }
  else if (event == "ain")
  {
    Serial.print("event adc : ");

    readAnalogs();

    Serial.println(analogInputs[0].voltage);

    doc["mac"] = mac;
    doc["event"] = "feedback";
    JsonArray array = doc.createNestedArray("ain");

    for (uint8_t i = 0; i < totalAnalogs; i++)
    {
      array.add(analogInputs[i].voltage);
    }

    String result;
    serializeJson(doc, result);
    Serial.println(result);

    publishReport(result.c_str());


  }
  else if (event == "label")
  {

    String label = doc["label"].as<String>();
    uint8_t out = (msg.substring(1, 2).toInt()) - 1;
    if (out != -1)
    {
      String tempLabel = doc["label"].as<String>();

      writeToEEPROM(outputs[out].labelKey, tempLabel);
      outputs[out].label = tempLabel;
      String text = "نام رله " + String(out + 1) + " به " + tempLabel + " تغییر یافت ";
    }
  }
  else if (event == "timer")
  {
    String tempSch = doc["timer"].as<String>();

    String outPrg = outputIsBusy(out);
    if (outPrg != "")
    {
      String text = "رله " + String(out + 1) + " در حالت سناریو قرار دارد ";

      Serial.println(text);
      return;
    }
    writeDateTimeEEPROM(outputs[out].timerKey, tempSch);
    outputs[out].timer = tempSch;
    String result = prepareDbData("feedback");
    publishReport(result.c_str());

    String text;
    if (tempSch.length() >= 2 && tempSch.length() < 6 && tempSch.charAt(0) != 't')
    {
      String t;
      if (tempSch.substring(2, 3) == "-")
      {
        bool minus = true;
        t = tempSch.substring(2, 4) + " درجه ";
      }
      else
      {
        t = tempSch.substring(2, 4) + " درجه ";
      }
      if (tempSch.charAt(0) == 'c')
      {
        text = " عملکرد فن رله " + String(out + 1) + " در دمای پایینتر از" + t + " فعال شد";
      }
      else if (tempSch.charAt(0) == 'h')
      {
        text = " عملکرد هیتر رله " + String(out + 1) + " در دمای بالاتر از" + t + " فعال شد";
      }
    }
    else
    {
      text = "تایمر رله " + String(out + 1) + " فعال شد";
    }
    if (outputs[out].timer.charAt(0) == 't')
    {
      int8_t toggle = outputs[out].timer.substring(1).toInt();
      toggleTimers[out] = toggle * 2;
    }
    else
    {
      toggleTimers[out] = -1;
    }


  }
  else if (event == "scn")
  {

    String tempPrg = doc["value"].as<String>();
    String key = doc["key"].as<String>();
    Serial.println(tempPrg);
    if (tempPrg == "")
    {
      Serial.println("remove scn");
      removeScenario(key.c_str());
    }
    else
    {
      String text = addScenario(tempPrg);
      if (text != "")
      {
        String result = prepareDbData("feedback");
        publishReport(result.c_str());

        debugPrint("Input " + key + " is Set :");
        debugPrint(tempPrg);
      }
    }


  }
  else if (event == "status")
  {
    String result = prepareDbData("feedback");

    publishReport(result.c_str());


  }
  else if (event == "update")
  {
    String url = doc["url"].as<String>();
    int newVer = doc["version"].as<short>();
    Serial.println("Url:");
    Serial.println(url);
    if (newVer > VERSION)
    {
      checkUpdate(url);
    }
    else
    {
      Serial.println("Device is Already Update!");
      doc["mac"] = mac;
      doc["event"] = "feedback";
      doc["update"] = false;
      doc["version"] = VERSION;
      doc.remove("url");
      String result;
      serializeJson(doc, result);
      Serial.println(result);
      publishReport(result.c_str());

      updatedDisplay();
    }

  }
  else if (event == "rfReg")
  {

    handleRemoteRegister();
  }
  else if (event == "rfDel")
  {
    handleRemoveRemote();

  }
  else if (event == "admins")
  {
    String tempPhone = "";
    uint8_t index = 30;
    if (!doc["phone"].isNull())
    {
      tempPhone = doc["phone"].as<String>();
    }
    if (!doc["index"].isNull())
    {
      index = doc["index"].as<uint8_t>();
    }
    Serial.println("phone");
    Serial.println(tempPhone);
    Serial.println(index);
    if ((index <= 5 && index >= 0) && (tempPhone == ""))
    {

      writeToEEPROM(offsetPhone[index], "");
      phoneNo[index] = tempPhone;
    }
    else if ((index >= 0 && index <= 5) && tempPhone.length() == 13)
    {

      phoneNo[index] = tempPhone;
      writeToEEPROM(offsetPhone[index], tempPhone);
    }
    String text;
    for (uint8_t i = 0; i < totalPhoneNo; i++)
    {
      if (!phoneNo[i].isEmpty())
      {
        text += phoneNo[i];
      }
      if (i != (totalPhoneNo - 1))
      {
        text += ",";
      }
    }
    doc.remove("phone");
    doc.remove("index");

    Serial.println(text);
    doc["mac"] = mac;
    doc["event"] = "feedback";
    doc["admins"] = text;
    String result;
    serializeJson(doc, result);
    Serial.println(result);
    publishReport(result.c_str());
  }


}
uint8_t mqtt_count = 0;

void reconnect()
{
  if (mqtt_count > 6)
  {
    gsmNetwork = false;
    hasWifi = false;
    return;
  }
  const char *broker = m_server.c_str();
  if (hasWifi && !gprsConnected && !forceUseGprs)
  {
    activeClient = &wifiClient;
    mqtt.setClient(*activeClient);
    mqtt.setServer(broker, port);
    mqtt.setCallback(callback);
    mqttNet = WIFI;
    Serial.println("mqttNet");
    Serial.println(mqttNet);
  }

  else if (gprsConnected)
  {
    activeClient = &gsmClient;

    mqtt.setClient(*activeClient);
    mqtt.setServer(broker, port);
    mqtt.setCallback(callback);

    mqttNet = GPRS;
    Serial.println("mqttNet");
    Serial.println(mqttNet);
  }
  mqtt.setBufferSize(1024);


  while (!mqtt.connected() && mqtt_count<3)
  {
    mqtt_connected = false;
    Serial.print("Attempting MQTT connection...");

    char clientId[24];
    snprintf(clientId, sizeof(clientId), "ESP32Client-%04X", random(0xffff));

    String myTopic = sub_topic + ">" + mac;


    StaticJsonDocument<64> payload;
    payload["event"] = "state";
    payload["mac"] = mac;
    payload["status"] = "OFFLINE";
    payload["net"] = mqttNet;

    char jsonBuffer[128];
    serializeJson(payload, jsonBuffer, sizeof(jsonBuffer));


    if (mqtt.connect(clientId, "hosein282", "At9127995883", "action_server", 0, false, jsonBuffer))
    {
      mqtt_count = 0;
      Serial.println("connected");
      Serial.println("Sub to");
      Serial.println(myTopic);


      StaticJsonDocument<64> payload;
      payload["event"] = "state";
      payload["mac"] = mac;
      payload["status"] = "ONLINE";
      payload["net"] = mqttNet;

      char jsonBuffer[128];
      serializeJson(payload, jsonBuffer, sizeof(jsonBuffer));



      publishReport(jsonBuffer);


      mqtt.subscribe(myTopic.c_str());
      mqtt_connected = true;
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());

      mqtt_count = mqtt_count + 1;
      delay(1500);
    }
  }
}




void initLittleFS()
{
  if (!LittleFS.begin(true))
  {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  Serial.println("LittleFS mounted successfully");



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


String readFile(fs::FS &fs, const char *path)
{
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory())
  {
    Serial.println("- failed to open file");
    return String();
  }

  String fileContent = file.readString();
  file.close();






  return fileContent;
}


void writeFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
    Serial.println("- file written");
  }
  else
  {
    Serial.println("- write failed");
  }
}

void deleteData(fs::FS &fs, const char *path)
{

  fs.remove(path);
}


bool initWiFi()
{
  wifiTryCount++;
  if (wifiTryCount > 3)
  {
    Serial.println("Wifi try count is over");
    return false;
  }







  if (ssid == "" || password == "")
  {
    Serial.println("Undefined SSID or IP address.");
    resetWifi();
    return false;
  }

  WiFi.mode(WIFI_STA);
# 949 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  unsigned long currentMillis = millis();
  previousMillis = currentMillis;

  while (WiFi.status() != WL_CONNECTED)
  {
    currentMillis = millis();
    if (currentMillis - previousMillis >= 10000)
    {
      Serial.println("Failed to connect.");

      return false;
    }
  }

  if (!MDNS.begin("hubway"))
  {
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.print("Current ESP32 IP: ");
  Serial.println(WiFi.localIP());
  hasWifi = true;
  esp_task_wdt_reset();

  printLocalTime();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html, processor); });
  server.serveStatic("/", LittleFS, "/");

  server.on(
      "/sw", HTTP_POST, [](AsyncWebServerRequest *request)
      { Serial.println("sw"); },
      NULL, handlePostRequest);

  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String inputMessage1;
    String inputMessage2;

    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();

      if (outputs[inputMessage1.toInt()].timer != "") {
        clearTimer(inputMessage1.toInt());
      }
      switchRelay(inputMessage1.toInt(), inputMessage2.toInt(), 0, false);


    } else if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_3)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_3)->value();
      debugPrint(inputMessage2);

      if (inputMessage2.toInt() == 0) {
        clearTimer(inputMessage1.toInt());
      } else {

        uint8_t index = inputMessage1.toInt();
        writeDateTimeEEPROM(outputs[index].timerKey, inputMessage2);
        outputs[index].timer = inputMessage2;
        debugPrint("Relay 1 is Set :");
        debugPrint(inputMessage2);
      }
    } else if (request->hasParam(PARAM_INPUT_2) && request->hasParam(PARAM_INPUT_5)) {
      inputMessage1 = request->getParam(PARAM_INPUT_5)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      debugPrint(inputMessage2);


      uint8_t index = inputMessage1.toInt();
      uint8_t percent = inputMessage2.toInt();
      setPwm(index, percent);
      Serial.println("Pwm");
      Serial.println(index + 1);
      Serial.println(" is Set :");
      Serial.println(percent);

    } else if (request->hasParam("date")) {
      inputMessage1 = request->getParam("date")->value();
      debugPrint(inputMessage1);


      String dateTime = inputMessage1;

      dateTime = "\"" + dateTime + "\"";
      Serial.println(dateTime);

      updateDate(dateTime);
      Serial.println("date updated!");

    } else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK"); });

  server.on(
      "/label", HTTP_POST, [](AsyncWebServerRequest *request)
      {
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
      } });

  events.onConnect([](AsyncEventSourceClient *client)
                   {
                     if (deviceYear < 20)
                     {
                       events.send("", "date", millis());
                     }
                     if (client->lastId())
                     {
                       Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
                     }



                   });

  server.addHandler(&events);

  server.begin();
  return true;
}
bool addTask(void (*taskFunction)(), unsigned long delay)
{
  if (taskCount >= MAX_TASKS)
  {
    return false;
  }

  taskQueue[taskCount].function = taskFunction;
  taskQueue[taskCount].executeAt = millis() + delay;
  taskCount++;
  return true;
}


void setupLCD()
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  display.clearDisplay();
}

void initDisplay()
{

  display.clearDisplay();




  display.setCursor(2, 24);
  display.println("NET");

  display.setCursor(2, 34);
  display.print("TEMP 1");

  display.setCursor(2, 44);
  display.println("ENERGY");


  updateStatesDSP();
  display.display();
}

void updateDisplay()
{


  String m = String(rtc.getMinute());
  String d = String(rtc.getDate());
  if (m.length() == 1)
  {
    m = "0" + m;
  }
  String date = d.substring(d.indexOf(' ') + 1) + " " + String(rtc.getHour(true)) + ":" + m;

  display.setTextSize(1);
  display.setFont(NULL);
  display.setTextColor(WHITE);
  display.fillRect(12, 2, 120, 7, 0);

  display.setCursor(12, 2);
  display.print(date);
  display.setCursor(2, 14);
  if (op == "")
  {
    display.println("No GSM");
  }
  else if (op == "mci")
  {
    display.println("IR-MCI");
  }
  else if (op == "irancell")
  {
    display.println("IRANCELL");
  }
  else if (op == "rightel")
  {
    display.println("RIGHTEL");
  }

  display.fillRect(80, 34, 48, 7, 0);

  display.drawCircle(118, 34, 1, 1);
  String tmp = String(temps[0].value);
  display.setCursor(128 - ((tmp.length() + 2) * 6), 34);

  display.print(tmp + " C");

  display.fillRect(100, 12, 34, 7, 0);

  if (signalQuality <= 0 || signalQuality == 99)
  {
    display.setCursor(108, 14);
    display.println("x");



  }
  else if (signalQuality > 0 && signalQuality <= 7)
  {
    display.fillRect(100, 18, 3, 2, 1);
    display.drawRect(104, 16, 3, 4, 1);
    display.drawRect(108, 14, 3, 6, 1);
    display.drawRect(112, 12, 3, 8, 1);
  }
  else if (signalQuality > 7 && signalQuality <= 14)
  {
    display.fillRect(100, 18, 3, 2, 1);
    display.fillRect(104, 16, 3, 4, 1);
    display.drawRect(108, 14, 3, 6, 1);
    display.drawRect(112, 12, 3, 8, 1);
  }
  else if (signalQuality > 14 && signalQuality <= 21)
  {
    display.fillRect(100, 18, 3, 2, 1);
    display.fillRect(104, 16, 3, 4, 1);
    display.fillRect(108, 14, 3, 6, 1);
    display.drawRect(112, 12, 3, 8, 1);
  }
  else
  {
    display.fillRect(100, 18, 3, 2, 1);
    display.fillRect(104, 16, 3, 4, 1);
    display.fillRect(108, 14, 3, 6, 1);
    display.fillRect(112, 12, 3, 8, 1);
  }
  display.fillRect(74, 12, 10, 7, 0);

  if (WiFi.status() == WL_CONNECTED)
  {

    display.drawBitmap(118, 12, image_Icon_Wifi_bits, 10, 8, 1);

  }
  else
  {

    display.setCursor(118, 14);

    display.println("x");
    display.setTextSize(1);




  }

  display.fillRect(86, 24, 42, 7, 0);

  if (mqtt_connected)
  {
    display.setCursor(104, 24);
    display.println(mqttNet == 1 ? "WIFI" : "GPRS");
  }
  else
  {
    display.setCursor(86, 24);
    display.println("OFFLINE");
  }
  display.fillRect(70, 44, 46, 7, 0);

  String amp = String(currentAmp.buffer[0]);
  display.setCursor(128 - ((amp.length() + 3) * 6), 44);

  display.println(amp);
  display.setCursor(122, 44);
  display.println("W");
# 1299 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
  display.display();
}
void updateSignalDisp()
{
  display.fillRect(100, 12, 16, 7, 0);

  if (signalQuality <= 0 || signalQuality == 99)
  {
    display.setCursor(108, 12);
    display.println("x");



  }
  else if (signalQuality > 0 && signalQuality <= 7)
  {
    display.fillRect(100, 18, 3, 2, 1);
    display.drawRect(104, 16, 3, 4, 1);
    display.drawRect(108, 14, 3, 6, 1);
    display.drawRect(112, 12, 3, 8, 1);
  }
  else if (signalQuality > 7 && signalQuality <= 14)
  {
    display.fillRect(100, 18, 3, 2, 1);
    display.fillRect(104, 16, 3, 4, 1);
    display.drawRect(108, 14, 3, 6, 1);
    display.drawRect(112, 12, 3, 8, 1);
  }
  else if (signalQuality > 14 && signalQuality <= 21)
  {
    display.fillRect(100, 18, 3, 2, 1);
    display.fillRect(104, 16, 3, 4, 1);
    display.fillRect(108, 14, 3, 6, 1);
    display.drawRect(112, 12, 3, 8, 1);
  }
  else
  {
    display.fillRect(100, 18, 3, 2, 1);
    display.fillRect(104, 16, 3, 4, 1);
    display.fillRect(108, 14, 3, 6, 1);
    display.fillRect(112, 12, 3, 8, 1);
  }
}
void updateOperatorDisp()
{
  display.fillRect(2, 14, 48, 7, 0);

  display.setCursor(2, 14);
  if (op == "")
  {
    display.println("No GSM");
  }
  else if (op == "mci")
  {
    display.println("IR-MCI");
  }
  else if (op == "irancell")
  {
    display.println("IRANCELL");
  }
  else if (op == "rightel")
  {
    display.println("RIGHTEL");
  }
}
void updateStatesDSP()
{

  display.fillRect(0, 53, 128, 11, 0);
  uint8_t start = (128 - (totalOutputs * 13)) / 2;
  for (uint8_t i = 0; i < totalOutputs; i++)
  {
    int state = outStates.charAt(i * 2 + 1);

    if (state == '1')
    {
      display.fillRoundRect(start + (i * 13), 53, 11, 11, 1, 1);
      display.setCursor((start + 3) + (i * 13), 55);
      display.setTextColor(BLACK);
      display.print(String(i + 1));
    }
    else
    {
      display.drawRoundRect(start + (i * 13), 53, 11, 11, 1, 1);
      display.setCursor((start + 3) + (i * 13), 55);
      display.setTextColor(WHITE);
      display.print(String(i + 1));
    }
  }
# 1402 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
  display.display();
}
void updateTempDSP()
{
  if (temps[lastTempShown].value == -127.0)
  {
    return;
  }
  if (lastTempShown >= totalTemps)
  {
    lastTempShown = 0;
  }
  display.fillRect(26, 34, 102, 8, 0);
  display.setTextColor(WHITE);
  display.setCursor(2, 34);
  display.print("TEMP " + String(lastTempShown + 1));

  String tmp = String(temps[lastTempShown].value);

  display.drawCircle(118, 34, 1, 1);
  display.setCursor(128 - ((tmp.length() + 2) * 6), 34);
  display.print(tmp + " C");
  display.display();
  lastTempShown++;
}

void loadingDisplay(int progress, String title)
{
  uint8_t p = (progress * 86) / 100;
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setFont(NULL);

  display.setTextSize(2);
  display.setCursor(38, 4);
  display.println("VIIO");
  display.setTextSize(1);
  display.setTextWrap(0);
  display.setCursor(34, 24);
  display.println(title + "..");
  display.drawRect(20, 40, 90, 16, 1);
  display.fillRect(22, 42, p, 12, 1);
  display.display();
}

void alertDisplay()
{
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
void alertEnableDisplay()
{
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
void updatedDisplay()
{
  unsigned long startTime = millis();

  const uint8_t bitmap23[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xfc, 0x7f, 0xff, 0xfe, 0x60, 0x00, 0x06, 0x60, 0x00, 0x06, 0x60, 0x00, 0x06, 0x60, 0x00, 0x06, 0x60, 0x00, 0x06, 0x61, 0x81, 0x86, 0x61, 0xc3, 0x86, 0x60, 0xe7, 0x06, 0x70, 0x7e, 0x0e, 0x70, 0x3c, 0x0e, 0x70, 0x18, 0x0e, 0x38, 0x00, 0x1c, 0x38, 0x00, 0x1c, 0x1c, 0x00, 0x38, 0x0f, 0x00, 0xf0, 0x07, 0xe7, 0xe0, 0x03, 0xff, 0xc0, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  display.clearDisplay();
  while (millis() - startTime < 3000)
  {
    display.println("");
    display.drawBitmap(54, 30, bitmap23, 24, 24, 1);
    display.setCursor(18, 10);
    display.println("Already Updated!");
    display.display();
  }
  initDisplay();
  updateDisplay();
}
void smsDisplay()
{

  static const unsigned char PROGMEM image_ALARM_icon_bits[] = {0x00, 0x00, 0x13, 0xc8, 0x66, 0x66, 0x4c, 0x32, 0x8a, 0x11, 0x98, 0x19, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x3f, 0xfc, 0x60, 0x06, 0x7f, 0xfe, 0x7f, 0xfe, 0x06, 0x60, 0x03, 0xc0, 0x00, 0x00};





  display.drawBitmap(54, 14, image_ALARM_icon_bits, 8, 8, 1);
  display.display();

  addTask(clearSmsDisplay, 1000);
}

void clearSmsDisplay()
{
  display.fillRect(54, 14, 17, 17, 0);
}
void reloadDisplay()
{
  initDisplay();
  updateDisplay();
}

void displayRemoteRg(String remote, unsigned long time)
{

  unsigned long t = (int)(time * 92) / 24000;
  display.clearDisplay();
  display.setCursor(28, 10);
  display.println("ADD REMOTE..");
  display.setCursor(31, 28);
  display.println("set " + remote);
  display.drawRoundRect(20, 44, 92, 16, 3, 1);
  display.fillRoundRect(20, 44, t, 16, 3, 1);
  display.display();
}
void displayRemoteDel(String remote, unsigned long time)
{

  unsigned long t = (int)(time * 92) / 24000;
  display.clearDisplay();
  display.setCursor(28, 10);
  display.println("Remove REMOTE..");
  display.setCursor(31, 28);
  display.println("del " + remote);
  display.drawRoundRect(20, 44, 92, 16, 3, 1);
  display.fillRoundRect(20, 44, t, 16, 3, 1);
  display.display();
}

void i2cScanner()
{
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
      {
        Serial.print("0");
      }
      Serial.println(address, HEX);
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknow error at address 0x");
      if (address < 16)
      {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
  {
    Serial.println("No I2C devices found\n");
  }
  else
  {
    Serial.println("done\n");
  }
  delay(5000);
}



void setup()
{
  EEPROM.begin("esp");





  Serial.begin(9600);
  delay(1000);

  setupLCD();

  delay(1000);
  Serial.print("Firmware Version=>");
  Serial.println(VERSION);

  Serial.print("free Entries: ");
  Serial.println(EEPROM.freeEntries());

  emon.current(ctPin, 30.0);




  Serial2.begin(9600, SERIAL_8N1, rxPin, txPin);
  delay(1000);
  loadingDisplay(0, "Setup");


  pinMode(RESET_GSM, OUTPUT);
  GsmReset();


  delay(3000);
  esp_task_wdt_init(100, true);
  esp_task_wdt_add(NULL);

  initLittleFS();


  loadingDisplay(10, "Init Data");




  sensors.begin();

  sensors.requestTemperatures();

  for (uint8_t i = 0; i < totalTemps; i++)
  {
    temps[i].temp = sensors.getTempCByIndex(i);
    Serial.println(temps[i].temp);
  }
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);


  mySwitch.enableReceive(13);

  button.attach(BUTTON_PIN, INPUT_PULLUP);

  button.interval(5);

  button.setPressedState(LOW);

  Serial.println("SIM800L software serial initialize");

  loadingDisplay(20, "init I/O");



  Serial.println("MCP23xxx Combo Test!");

  if (!mcp.begin_I2C(0x20))
  {
    Serial.println("Error.");
  }
  else
  {
    delay(1000);
    for (uint8_t i = 0; i < totalOutputs; i++)
    {


      mcp.pinMode(outputs[i].gpio, OUTPUT);
      delay(100);

      mcp.digitalWrite(outputs[i].gpio, LOW);

    }
    for (uint8_t i = 0; i < totalInputs; i++)
    {
      mcp.pinMode(inputs[i].gpio, INPUT_PULLUP);
      delay(100);
      mcp.disableInterruptPin(inputs[i].gpio);




    }
  }

  for (uint8_t i = 0; i < totalPwm; i++)
  {
    ledcSetup(i, freq, 8);
    ledcAttachPin(pwms[i].gpio, i);
  }

  loadingDisplay(30, "Load Data");


  digitalWrite(STATUS_LED, LOW);
  pinMode(STATUS_LED, OUTPUT);



  setupVariables();


  loadingDisplay(40, "Set Wifi");

  if (initWiFi())
  {

    reconnect();
  }
  else
  {
    initWifiAp();
  }

  delay(1000);
  if (checkSim())
  {

    loadingDisplay(55, "Setup GSM");

    setupGSM();
# 1740 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
  }
  else
  {
    flipper.attach(1, flip);
    Serial.println("Insert a sim card");
  }
  loadingDisplay(100, "Completed");

  delay(500);
# 1758 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
  if (phoneNo[0].length() == 13)
  {






    debugPrint("Admin Phone Is Registered");
    flipper.detach();

    delay(100);
  }

  initDisplay();

  checkTasks();


}

void checkSimNetwork()
{

  while (!isRegistered())
  {
    gsmCounter = gsmCounter + 1;
    flipper.attach(0.5, flip);
    loadingDisplay(50 + (gsmCounter * 6), "Setup GSM");
    delay(3000);
    if (gsmCounter > 6)
    {
      GsmSoftReset();
      gsmCounter = 0;

      break;
    }
  }
}





unsigned long prevTemp = 0;

void loop()
{

  now = millis();


  while (Serial2.available() > 0)
  {
    parseData(Serial2.readString());
  }



  if (now - prevTaskTime >= SCH_TASK_TIME)
  {
    checkTasks();
    prevTaskTime = now;
  }
  esp_task_wdt_reset();



  getEnergyCons();

  checkSensors();

  readAnalogs();

  readButton();

  checkRfRemote();

  if (now - prevTemp >= 15000)
  {
    sensors.requestTemperatures();
    for (uint8_t i = 0; i < totalTemps; i++)
    {
      temps[i].temp = sensors.getTempCByIndex(i);
    }
    updateTempDSP();
    prevTemp = now;
  }
  runScenarios();

  checkRelayTimes();

  processTasks();



  checkMqttStatus();
}

void getEnergyCons()
{
  if (now - lastSampleTime >= sampleInterval)
  {
    lastSampleTime = now;


    double Irms = emon.calcIrms(1480);
    double power = Irms * assumedVoltage;
    double energyWh = (power * (sampleInterval / 1000.0)) / 3600.0;

    currentAmp.value += energyWh;
    currentAmp.buffer[0] = energyWh;
    Serial.println("energyWh: ");
    Serial.println(currentAmp.value);
  }
}
void processTasks()
{

  for (int i = 0; i < taskCount; i++)
  {

    if (now >= taskQueue[i].executeAt)
    {
      taskQueue[i].function();

      for (int j = i; j < taskCount - 1; j++)
      {
        taskQueue[j] = taskQueue[j + 1];
      }
      taskCount--;
      i--;
    }
  }
}

void checkRelayTimes()
{
  for (uint8_t i = 0; i < totalOutputs; i++)
  {
    if (outputs[i].type == 0 || outputs[i].type == 2 || outputs[i].now == 0)
      continue;

    if (outputs[i].now > 0 && (millis() - outputs[i].now) >= (outputs[i].time * 1000))
    {
      boolean targetState = 0;
      mcp.digitalWrite(outputs[i].gpio, targetState);
      outputs[i].state = targetState;
      outStates[(i * 2) + 1] = targetState ? '1' : '0';
      Serial.println("targetState");
      Serial.println(outputs[i].state);



      for (int j = 0; j < totalScenarios; j++)
      {
        Scenario scenario = scenarios[j];
        if (scenario.value.isEmpty() || scenario.value.charAt(0) != 'o')
          continue;
        uint8_t target = 100;
        if (i == scenario.input)
        {
          target = scenario.outPin;
        }
        else if (i == scenario.outPin)
        {
          target = scenario.input;
        }
        if (target != 100)
        {
          if (scenario.condition == "==")
          {
            mcp.digitalWrite(outputs[target].gpio, targetState);
            outputs[target].state = !targetState;
            outStates[(target * 2) + 1] = targetState ? '1' : '0';
            continue;
          }
          else
          {
            mcp.digitalWrite(outputs[target].gpio, !targetState);
            outputs[target].state = !targetState;
            outStates[(target * 2) + 1] = targetState ? '1' : '0';
            continue;
          }
        }
      }


      sendMqttFeedback();


      outputs[i].now = 0;
    }
  }
}
void switchRelay(uint8_t index, bool state, uint16_t time, bool isLocked)
{

  Serial.println("state");
  Serial.println(state);
  Serial.println(mcp.digitalRead(outputs[index].gpio));

  if (state == mcp.digitalRead(outputs[index].gpio))
  {
    Serial.println("switch return");
    return;
  }

  mcp.digitalWrite(outputs[index].gpio, state);

  outputs[index].state = state;
  outStates[(index * 2) + 1] = state ? '1' : '0';

  Serial.println(outputs[index].state);



  for (int i = 0; i < totalScenarios; i++)
  {
    Scenario scenario = scenarios[i];
    if (scenario.value.isEmpty() || scenario.value.charAt(0) != 'o')
      continue;
    uint8_t target = 100;
    if (index == scenario.input)
    {
      target = scenario.outPin;
    }
    else if (index == scenario.outPin)
    {
      target = scenario.input;
    }
    if (target != 100)
    {
      if (scenario.condition == "==")
      {
        mcp.digitalWrite(outputs[target].gpio, state);
        outputs[target].state = state;
        outStates[(target * 2) + 1] = state ? '1' : '0';
        continue;
      }
      else
      {
        mcp.digitalWrite(outputs[target].gpio, !state);
        outputs[target].state = state;
        outStates[(target * 2) + 1] = state ? '1' : '0';
        continue;
      }
    }
  }


  sendMqttFeedback();


  if (state)
  {
    outputs[index].now = 0;
  }

  outputs[index].locked = isLocked;

  if (isLocked)
    return;


  if (outputs[index].type == 0)
  {

    Serial.println("Unlock");
  }
  else if (outputs[index].type == 2 && state)
  {
    delay(1000);
    mcp.digitalWrite(outputs[index].gpio, !state);
    outputs[index].state = !state;
    outStates[(index * 2) + 1] = !state ? '1' : '0';



    sendMqttFeedback();
  }
  else if (outputs[index].type == 1 && !state)
  {
    outputs[index].now = millis();
    outputs[index].time = time;




  }
  if (time > 0)
  {
    outputs[index].now = millis();
    outputs[index].time = time;
  }
}

void runScenarios()
{
  for (int i = 0; i < totalScenarios; i++)
  {
    Scenario scenario = scenarios[i];
    if (scenario.value.isEmpty())
      continue;
    char it = scenario.value.charAt(0);
    char ot = scenario.value.charAt(1);

    bool oCondition = false;

    bool conditionMet = false;
    float ifStatement;
    float bias;
    int time = 0;
    if (scenario.swType == 1)
    {
      time = 3;
    }

    if (it == 'o')
    {
      continue;
    }
    else if (it == 't')
    {
      ifStatement = temps[scenario.input].value;
      bias = TEMP_THRESHOLD_BIAS;
    }
    else if (it == 'd')
    {
      ifStatement = !mcp.digitalRead(inputs[scenario.input].gpio);


    }
    else if (it == 'a')
    {
      ifStatement = analogInputs[scenario.input].voltage;
      bias = THRESHOLD_BIAS;
    }


    if (ot == 'p')
    {
      oCondition = pwms[scenario.outPin].pwm == scenario.outState;
    }
    else
    {

      oCondition = outputs[scenario.outPin].state == scenario.outState;




    }



    if (it == 'd' && ot == 'r')
    {

      if (scenario.condition == "==")
      {
        if ((int)ifStatement == scenario.outState)
        {
          conditionMet = true;
        }
        else
        {
          conditionMet = false;
        }

      }
      else if (scenario.condition == "!=" && (int)ifStatement != scenario.outState)
      {
        conditionMet = true;
      }

    }
    else if (it == 't')
    {

      if (oCondition)
      {




        if (scenario.condition == ">" && ifStatement < scenario.threshold - bias)
        {

          conditionMet = true;
        }
        else if (scenario.condition == "<" && ifStatement > scenario.threshold + bias)
        {

          conditionMet = true;
        }
        else if (scenario.condition == "==" && (int)ifStatement == (int)scenario.threshold)
        {

          conditionMet = true;
        }

      }
      else
      {
        if (scenario.condition == ">" && ifStatement > scenario.threshold)
        {


          conditionMet = true;
        }
        else if (scenario.condition == "<" && ifStatement < scenario.threshold)
        {


          conditionMet = true;
        }
        else if (scenario.condition == "==" && ifStatement == scenario.threshold)
        {


          conditionMet = true;
        }
      }

    }
    else if (it == 'a')
    {


      if (oCondition)
      {


        if (scenario.condition == ">" && ifStatement < scenario.threshold - bias)
        {

          conditionMet = true;
        }
        else if (scenario.condition == "<" && ifStatement > scenario.threshold + bias)
        {
          conditionMet = true;
        }
        else if (scenario.condition == "==" && (int)ifStatement == (int)scenario.threshold)
        {
          conditionMet = true;
        }

      }
      else
      {
        if (scenario.condition == ">" && ifStatement > scenario.threshold)
        {


          conditionMet = true;
        }
        else if (scenario.condition == "<" && ifStatement < scenario.threshold)
        {


          conditionMet = true;
        }
        else if (scenario.condition == "==" && ifStatement == scenario.threshold)
        {


          conditionMet = true;
        }
      }
    }


    if (conditionMet)
    {
      if (ot == 'p')
      {
        setPwm(scenario.outPin, scenario.outState);

      }
      else
      {
        if (oCondition)
        {
          if (scenario.swType != 0)
          {
            switchRelay(scenario.outPin, !scenario.outState, time, false);

            continue;
          }
          else
          {

            continue;
          }
        }
        else
        {

          switchRelay(scenario.outPin, scenario.outState, time, true);
          Serial.println("conditionMet locked");
          if (scenario.notif == 1 && notifyScenarios)
          {
            notifHexSms(i, phoneNo[0]);
          }
          else if (scenario.notif == 2 && notifyScenarios)
          {

            callAdmin(i);
          }
          continue;

        }
      }
    }
    else
    {

      if (scenario.condition == "==")
      {
        switchRelay(scenario.outPin, !scenario.outState, time, true);

      }
      else
      {
        switchRelay(scenario.outPin, scenario.outState, time, true);

      }
    }
  }
}

void notifHexSms(int index, String phone)
{
  if (phone == "")
    return;
  Scenario scenario = scenarios[index];
  char it = scenario.value.charAt(0);
  char ot = scenario.value.charAt(1);

  String text = "سناریو " + String(index + 1) + "اجراشد " + "\n";
  if (it == 'a')
  {
    text += "سنسور " + String(scenario.input + 1) + " از " + String(scenario.threshold / 40.96);
  }
  else if (it == 'd')
  {
    text += " دیجیتال" + String(scenario.input + 1) + "تحریک شد ";
  }
  else if (it == 't')
  {
    text += "دماسنج " + String(scenario.input + 1) + " از " + String(scenario.threshold);
  }

  if (scenario.condition == ">")
  {
    text += " بیشتر شد";
  }
  else if (scenario.condition == "<")
  {
    text += " کمتر شد";
  }
  if (ot == 'r')
  {
    text += " و رله" + String(scenario.outPin + 1) + " روشن شد";
  }

  if (millis() - scenarios[index].lastNotif > 60000)
  {
    scenarios[index].lastNotif = millis();
    ReplyHex(text, phone);
  }
}


void setupVariables()
{

  outStates = createOutArray();
  inStates = createInArray();
  pwmStates = createPwmArray();





  for (int i = 0; i < totalInputs; i++)
  {
    inputs[i].lastTrigger = 0;
  }
  for (int i = 0; i < totalAnalogs; i++)
  {
    analogInputs[i].lastTrigger = 0;
  }

  Serial.println("WIFI Credentials");


  ssid = readFile(LittleFS, wifi_ssid);
  password = readFile(LittleFS, wifi_pass);
  String server = readFile(LittleFS, m_server_k);
  if (server.isEmpty())
  {
    m_server = "5.238.178.210";
  }
  else
  {
    m_server = server;
  }
  mac = readFile(LittleFS, mac_f);
  Serial.println(ssid);
  Serial.println(password);
  Serial.println(m_server);
  Serial.println(mac);





  Serial.println("List of Registered Phone Numbers");
  for (uint8_t i = 0; i < totalPhoneNo; i++)
  {
    phoneNo[i] = readFromEEPROM(offsetPhone[i]);
    if (phoneNo[i].length() != 13)
    {
      phoneNo[i] = "";
      Serial.println(String(i + 1) + ": empty");
      continue;
    }
    else
    {
      Serial.println(String(i + 1) + ": " + phoneNo[i]);
    }
  }


  Serial.println("List of Schedules");
  for (uint8_t i = 0; i < totalOutputs; i++)
  {
    outputs[i].timer = readFromEEPROM(outputs[i].timerKey);
    if (outputs[i].timer.length() < 2)
    {
      outputs[i].timer = "";
      Serial.println(String(i + 1) + ": empty");
      continue;
    }
    else
    {
      Serial.println(String(i + 1) + ": " + outputs[i].timer);
      if (outputs[i].timer.charAt(0) == 't')
      {
        int8_t toggle = outputs[i].timer.substring(1).toInt();
        toggleTimers[i] = toggle * 2;
      }
    }
  }
# 2470 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
  Serial.println("List of Scenarios");
  for (uint8_t i = 0; i < totalScenarios; i++)
  {
    String key = "s" + String(i + 1);
    scenarios[i].value = readFromEEPROM(key.c_str());
    if (scenarios[i].value.length() > 0)
    {
      processScenarios(scenarios[i].value, i);
      scenariosCount++;
      Serial.println(String(i + 1) + ": " + scenarios[i].value);
    }
    else
    {
      break;
    }
  }

  Serial.println("List of Output Labels");
  for (uint8_t i = 0; i < totalOutputs; i++)
  {
    outputs[i].label = readFromEEPROM(outputs[i].labelKey);
    if (outputs[i].label.length() > 0)
    {
      Serial.println(String(i + 1) + ": " + outputs[i].label);
    }
    else
    {
      outputs[i].label = "";
      Serial.println(String(i + 1) + ": empty");
    }
  }

  Serial.println("List of Input Labels");
  for (uint8_t i = 0; i < totalInputs; i++)
  {
    inputs[i].label = readFromEEPROM(inputs[i].labelKey);
    if (inputs[i].label.length() < 1)
    {
      inputs[i].label = "";
      Serial.println(String(i + 1) + ": empty");
    }
    else
    {
      Serial.println(String(i + 1) + ": " + inputs[i].label);
    }
  }

  Serial.println("List of Remotes");
  const char *filePath = "/remotes.txt";


  String fileContent = readFile(LittleFS, filePath);
  if (fileContent.isEmpty())
  {
    Serial.println("No Remote Registered");
  }
  else
  {
    Serial.println("Remote Registered");
    Serial.println(fileContent);
  }
# 2550 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
}

bool processScenarios(String command, uint8_t index)
{

  int comma1 = command.indexOf(':');
  int comma2 = command.indexOf(':', comma1 + 1);
  int comma3 = command.indexOf(':', comma2 + 1);
  int comma4 = command.indexOf(':', comma3 + 1);
  int comma5 = command.indexOf(':', comma4 + 1);

  String key = "s" + String(index + 1);
  strncpy(scenarios[index].key, key.c_str(), sizeof(scenarios[index].key) - 1);
  scenarios[index].key[sizeof(scenarios[index].key) - 1] = '\0';
  Serial.println(scenarios[index].key);
  scenarios[index].input = command.substring(2, comma1).toInt();
  scenarios[index].condition = command.substring(comma1 + 1, comma2);

  int val = command.substring(comma2 + 1, comma3).toInt();
  if (command.charAt(0) == 'a')
  {
    scenarios[index].threshold = (val * aResolution) / 100;
  }
  else
  {
    scenarios[index].threshold = val;
  }

  scenarios[index].outPin = command.substring(comma3 + 1, comma4).toInt();
  scenarios[index].outState = command.substring(comma4 + 1).toInt();
  scenarios[index].swType = command.substring(comma5 + 1).toInt();
  scenarios[index].notif = command.substring(comma5 + 2).toInt();
  Serial.println("scenarios[index].notif");
  Serial.println(scenarios[index].notif);

  if (scenarios[index].input == -1 || scenarios[index].outPin == -1)
  {
    return false;
  }
  scenarios[index].value = command;
  return true;
}

void handlePostRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  if (index == 0)
  {





    unsigned char base64Data[len + 1];
    memcpy(base64Data, data, len);
    base64Data[len] = '\0';


    unsigned char decodedData[512];
    int decodedLen = decode_base64(base64Data, decodedData);

    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, decodedData);

    if (error)
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    String model = doc["model"].as<String>();

    if (model == "" || model != DEVICE_MODEL)
    {
      request->send(400, "application/json", "{\"error\":\"device model\"}");
      Serial.println("device model is not verified");
      return;
    }


    ssid = doc["ssid"].as<String>();
    password = doc["password"].as<String>();
    m_server = doc["m_server"].as<String>();

    writeFile(LittleFS, wifi_ssid, ssid.c_str());
    writeFile(LittleFS, wifi_pass, password.c_str());

    writeFile(LittleFS, m_server_k, m_server.c_str());
    wifiTryCount = 0;
    StaticJsonDocument<128> payload;
    payload["status"] = "received";
    payload["mac"] = mac;

    char jsonBuffer[128];
    serializeJson(payload, jsonBuffer, sizeof(jsonBuffer));
    Serial.println(jsonBuffer);

    request->send(200, "application/json", jsonBuffer);
    delay(3000);
    ESP.restart();
  }
  else
  {
    request->send(400, "application/json", "{\"error\":\"No data\"}");
  }
}

void initWifiAp()
{
  ssid = "";
  password = "";
  if (mac.isEmpty())
  {
    mac = getMAC();
  }

  WiFi.setTxPower(WIFI_POWER_18_5dBm);
  WiFi.softAP(ssid_ap + mac.substring(0, 2) + mac.substring(9, 11) + mac.substring(15, 17), "2NyTf21=");

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  if (!MDNS.begin("hubway"))
  {
    Serial.println("Error setting up MDNS responder!");
  }


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {


    request->send_P(200, "text/html", index_html, processor); });


  server.on("/ssid", HTTP_GET, [](AsyncWebServerRequest *request)
            {


    StaticJsonDocument<128> payload;
    payload["status"] = "received";
    payload["mac"] = mac;
    JsonArray array = payload.createNestedArray("ssid");

    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i) {
      array.add(WiFi.SSID(i));
    }
    char jsonBuffer[128];
    serializeJson(payload, jsonBuffer, sizeof(jsonBuffer));
    Serial.println(jsonBuffer);

    request->send(200, "application/json", jsonBuffer); });

  server.on(
      "/sw", HTTP_POST, [](AsyncWebServerRequest *request)
      { Serial.println("sw"); },
      NULL, handlePostRequest);


  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String inputMessage1;
    String inputMessage2;

    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();

      if (outputs[inputMessage1.toInt()].timer != "") {
        clearTimer(inputMessage1.toInt());
      }
      switchRelay(inputMessage1.toInt(), inputMessage2.toInt(), 0, false);



    } else if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_3)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_3)->value();
      debugPrint(inputMessage2);

      if (inputMessage2.toInt() == 0) {
        clearTimer(inputMessage1.toInt());
      } else {

        uint8_t index = inputMessage1.toInt();
        writeDateTimeEEPROM(outputs[index].timerKey, inputMessage2);
        outputs[index].timer = inputMessage2;
        debugPrint("Relay 1 is Set :");
        debugPrint(inputMessage2);
      }
    } else if (request->hasParam(PARAM_INPUT_2) && request->hasParam(PARAM_INPUT_5)) {
      inputMessage1 = request->getParam(PARAM_INPUT_5)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      debugPrint(inputMessage2);


      uint8_t index = inputMessage1.toInt();
      uint8_t percent = inputMessage2.toInt();
      setPwm(index, percent);
      Serial.println("Pwm");
      Serial.println(index + 1);
      Serial.println(" is Set :");
      Serial.println(percent);

    } else if (request->hasParam("date")) {
      inputMessage1 = request->getParam("date")->value();
      debugPrint(inputMessage1);


      String dateTime = inputMessage1;

      dateTime = "\"" + dateTime + "\"";
      Serial.println(dateTime);

      updateDate(dateTime);
      Serial.println("date updated!");

    } else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK"); });

  server.on(
      "/label", HTTP_POST, [](AsyncWebServerRequest *request)
      {
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
      } });


  events.onConnect([](AsyncEventSourceClient *client)
                   {
                     if (deviceYear < 20)
                     {
                       events.send("", "date", millis());
                     }
                     if (client->lastId())
                     {
                       Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
                     }



                   });

  server.addHandler(&events);

  server.begin();
}

String getMAC()
{


  String macStr = WiFi.macAddress();
  macStr.toUpperCase();
  Serial.println(macStr);
  writeFile(LittleFS, mac_f, macStr.c_str());

  return macStr;
}

bool printLocalTime()
{


  Serial.print(F("Waiting for NTP time sync: "));
  struct tm timeinfo;


  uint8_t count = 0;
  while (count < 3 && !getLocalTime(&timeinfo))
  {
    count += 1;
    yield();
    delay(500);
    Serial.print(F("."));
  }
  Serial.println(F(""));

  if (timeinfo.tm_year < 2024 || timeinfo.tm_year > 2060)
  {
    Serial.println(F("Failed to obtain time"));
    return false;
  }
  String formattedDate = createDateString(timeinfo, 14);
  updateDate(formattedDate);


  Epoch_Time = rtc.getEpoch();
  return true;
}

void setTimezone(String timezone)
{
  Serial.printf("  Setting Timezone to %s\n", timezone.c_str());
  setenv("TZ", timezone.c_str(), 1);
  tzset();
}

void resetWifi()
{

  ssid = "";
  password = "";
  m_server = "";
  wifiTryCount = 0;
  deleteData(LittleFS, wifi_ssid);
  deleteData(LittleFS, wifi_pass);
  deleteData(LittleFS, m_server_k);

  hasWifi = false;
}

String createDateString(const struct tm &timeinfo, int timezoneOffset)
{

  String dateString = "\"";


  dateString += String(timeinfo.tm_year % 100) + "/";
  dateString += String(timeinfo.tm_mon + 1).length() == 1 ? "0" + String(timeinfo.tm_mon + 1) : String(timeinfo.tm_mon + 1);
  dateString += "/";
  dateString += String(timeinfo.tm_mday).length() == 1 ? "0" + String(timeinfo.tm_mday) : String(timeinfo.tm_mday);
  dateString += ",";


  dateString += String(timeinfo.tm_hour).length() == 1 ? "0" + String(timeinfo.tm_hour) : String(timeinfo.tm_hour);
  dateString += ":";
  dateString += String(timeinfo.tm_min).length() == 1 ? "0" + String(timeinfo.tm_min) : String(timeinfo.tm_min);
  dateString += ":";
  dateString += String(timeinfo.tm_sec).length() == 1 ? "0" + String(timeinfo.tm_sec) : String(timeinfo.tm_sec);


  dateString += (timezoneOffset >= 0 ? "+" : "") + String(timezoneOffset) + "\"";

  return dateString;
}

boolean TimePeriodIsOver(unsigned long &periodStartTime, unsigned long TimePeriod)
{
  unsigned long currentMillis = millis();
  if (currentMillis - periodStartTime >= TimePeriod)
  {
    periodStartTime = currentMillis;
    return true;
  }
  else
    return false;
}
unsigned long prevMqttReport = 0;

void publishReport(const char *payload)
{


  mqtt.publish("action_server", payload);

  events.send(payload, "data", now);


}
void checkSensors()
{
    unsigned long currentMillis = millis();

  for (uint8_t i = 0; i < totalInputs; i++)
  {
    boolean newState = mcp.digitalRead(inputs[i].gpio);

    if (newState != inputs[i].state) {


      if (currentMillis - inputs[i].lastTrigger >= SENSOR_DEBOUNCE_DELAY) {

        inputs[i].lastTrigger = currentMillis;
        inputs[i].state = newState;

        Serial.printf("Sensor%d changed - old state:%d new state:%d\n",
                      i+1, !newState, newState);


        if (mqtt_connected) {
          DynamicJsonDocument doc(64);
          doc["mac"] = mac;
          doc["event"] = "report";
          doc["iSt"] = createInArray();

          String result;
          serializeJson(doc, result);
          Serial.println(result);

          publishReport(result.c_str());
        }
      }
    }
  }
}

void readAnalogs()
{
  bool flag = false;
  for (uint8_t i = 0; i < totalAnalogs; i++)
  {
    int value = analogRead(analogInputs[i].gpio);
    if (abs(value - analogInputs[i].voltage) > analog_read_threshold && now > (analogInputs[i].lastTrigger + 5000))
    {
      Serial.println(abs(value - analogInputs[i].voltage));
      analogInputs[i].lastTrigger = now;
      flag = true;
    }
    analogInputs[i].voltage = value;
  }

  if (flag && mqtt_connected)
  {
    DynamicJsonDocument doc(64);
    doc["mac"] = mac;
    doc["event"] = "report";
    JsonArray array2 = doc.createNestedArray("ain");
    for (uint8_t i = 0; i < totalAnalogs; i++)
    {
      array2.add(analogInputs[i].voltage);
    }
    String result;
    serializeJson(doc, result);
    Serial.println(result);

    publishReport(result.c_str());
  }
}
# 3031 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
void checkRfRemote()
{

  if (mySwitch.available())
  {
    Serial.println("rf available");
    int value = mySwitch.getReceivedValue();

    if (value != 0)
    {

      Serial.print("Received ");
      Serial.print(value);
      Serial.print(" / ");
      Serial.print(mySwitch.getReceivedBitlength());
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println(mySwitch.getReceivedProtocol());

      compareRemote(String(value));
      delay(500);
    }
    else
    {
      Serial.print("Unknown encoding");
    }
    mySwitch.resetAvailable();
  }
}

void readButton()
{
  button.update();

  if (button.pressed())
  {

    checkTasks();

    pressedTime = millis();
  }
  else if (button.released())
  {
    releasedTime = millis();
    unsigned long pressDuration = releasedTime - pressedTime;
    Serial.print("The button duration: ");
    Serial.println(pressDuration);

    if (pressDuration > SUPER_LONG_TIME)
    {
      resetWifi();
    }
    else if (pressDuration > VERY_LONG_TIME)
    {
      handleRemoveRemote();
    }
    else if (pressDuration > LONG_PRESS_TIME)
    {
      handleRemoteRegister();
    }
    else if (pressDuration > SHORT_PRESS_TIME)
    {

      handleShortPress();
    }
  }
}

void handleShortPress()
{
  Serial.println("short press");
  removeAllRemotes();
# 3120 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
}
void handleRemoteRegister()
{
  Serial.println("long press");
  unsigned long start = millis();
  unsigned long lastKey = millis();

  uint8_t relaysCount = 0;
  flipper.attach(0.2, flip);

  while (millis() - start < 24000)
  {
    displayRemoteRg(("Relay " + String(relaysCount + 1)), (millis() - start));

    if (relaysCount >= totalOutputs)
    {
      break;
    }
    while (millis() - lastKey < 4000)
    {
      if (mySwitch.available())
      {
        String value = String(mySwitch.getReceivedValue());
        if (value == 0)
        {
          Serial.print("Unknown encoding");
        }
        else
        {
          Serial.print("Received ");
          Serial.print(value);
          Serial.print(" / ");
          Serial.print(mySwitch.getReceivedBitlength());
          Serial.print("bit ");
          Serial.print("Protocol: ");
          Serial.println(mySwitch.getReceivedProtocol());


          Serial.println(value);

          saveRemoteCode(value.c_str(), relaysCount);
          delay(500);


        }

        mySwitch.resetAvailable();
      }
    }
    relaysCount = relaysCount + 1;
    lastKey = millis();
  }
  Serial.print("Loop Break!");

  flipper.detach();
  initDisplay();
  updateDisplay();
}
void saveRemoteCode(const char *code, uint16_t out)
{
  const char *filePath = "/remotes.txt";


  String fileContent = readFile(LittleFS, filePath);

  if (fileContent.indexOf(code) != -1)
  {
    Serial.println("Code already exists!");
    return;
  }



  fileContent += String(out) + String(code) + "\n";

  writeFile(LittleFS, filePath, fileContent.c_str());

  Serial.print("Saved code: ");
  Serial.println(code);
}
# 3218 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
void handleRemoveRemote()
{
  Serial.println("very long press");
  unsigned long start = millis();

  uint8_t relaysCount = 0;
  flipper.attach(0.2, flip);
  while (millis() < start + 15000)
  {
    displayRemoteDel(("Relay " + String(relaysCount + 1)), (millis() - start));

    if (relaysCount >= totalOutputs)
    {
      break;
    }
    if (mySwitch.available())
    {
      String value = String(mySwitch.getReceivedValue());
      if (value == 0)
      {
        Serial.print("Unknown encoding");
      }
      else
      {
        Serial.print("Received ");
        Serial.print(value);
        Serial.print(" / ");
        Serial.print(mySwitch.getReceivedBitlength());
        Serial.print("bit ");
        Serial.print("Protocol: ");
        Serial.println(mySwitch.getReceivedProtocol());


        removeRemoteCode(value.c_str());
        relaysCount = relaysCount + 1;
        delay(500);

      }

      mySwitch.resetAvailable();
    }
  }
  Serial.print("while loop breaks");

  flipper.detach();
  initDisplay();
  updateDisplay();
}

void removeAllRemotes()
{
  const char *filePath = "/remotes.txt";


  if (LittleFS.exists(filePath))
  {

    if (LittleFS.remove(filePath))
    {
      Serial.println("All remotes removed successfully!");
      remoteCount = 0;
    }
    else
    {
      Serial.println("Failed to remove remotes file!");
    }
  }
  else
  {
    Serial.println("No remotes file found to remove!");
  }


  File file = LittleFS.open(filePath, FILE_WRITE);
  if (file)
  {
    file.close();
    Serial.println("Empty remotes file created.");
  }
  else
  {
    Serial.println("Failed to create an empty remotes file.");
  }
}
void removeRemoteCode(const char *code)
{
  const char *filePath = "/remotes.txt";


  String fileContent = readFile(LittleFS, filePath);

  if (fileContent.isEmpty())
  {
    Serial.println("No remotes registered!");
    return;
  }


  String newContent = "";
  int lineStart = 0;
  bool codeFound = false;

  while (lineStart < fileContent.length())
  {
    int lineEnd = fileContent.indexOf('\n', lineStart);
    if (lineEnd == -1)
    {
      lineEnd = fileContent.length();
    }

    String line = fileContent.substring(lineStart, lineEnd);
    int colonIndex = line.indexOf(':');
    if (colonIndex != -1)
    {
      String storedCode = line.substring(colonIndex + 1);
      if (storedCode != code)
      {
        newContent += line + "\n";
      }
      else
      {
        codeFound = true;
      }
    }

    lineStart = lineEnd + 1;
  }

  if (codeFound)
  {

    writeFile(LittleFS, filePath, newContent.c_str());
    Serial.println("Remote code removed successfully!");
  }
  else
  {
    Serial.println("Remote code not found!");
  }
}
# 3375 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
void compareRemote(String received)
{

  const char *filePath = "/remotes.txt";


  String fileContent = readFile(LittleFS, filePath);

  if (fileContent.isEmpty())
  {
    Serial.println("No remotes registered!");
    return;
  }


  int lineStart = 0;
  while (lineStart < fileContent.length())
  {
    int lineEnd = fileContent.indexOf('\n', lineStart);
    if (lineEnd == -1)
    {
      lineEnd = fileContent.length();
    }

    String line = fileContent.substring(lineStart, lineEnd);

    String out = line.substring(0, 1);
    String code = line.substring(1);

    Serial.println(out);
    Serial.println(code);
    Serial.println(received);
    if (code == received)
    {
      uint8_t index = out.toInt();
      Serial.println("Remote matched!");
      Serial.print("Relay index: ");
      Serial.println(index);


      boolean state = mcp.digitalRead(outputs[index].gpio);
      switchRelay(index, !state, 0, false);

      return;
    }

    lineStart = lineEnd + 1;
  }

  Serial.println("Remote not registered!");
}
# 3502 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
void initSim800Mqtt()
{


  Serial.println(mqtt_connected);
  if (mqtt_connected)
  {
    Serial.println("Already Connected to Mqtt");

    return;
  }

  if (!modem.gprsConnect(op == "irancell" ? irancell_apn : mci_apn))
  {
    Serial.println("Failed to connect to GPRS");
    addTask(initSim800Mqtt, 20000);
    return;
  }
  if (modem.isGprsConnected())
  {
    Serial.println("GPRS is connected");
    gprsConnected = true;
    reconnect();
  }
  else
  {
    Serial.println("GPRS is not connected");
  }

}

void setupGSM()
{
  Serial.print("Waiting for network...");

  esp_task_wdt_reset();
  if (!modem.waitForNetwork(30000L, false))
  {
    Serial.println("gsm network fail");
    gsmNetwork = false;
    addTask(setupGSM, 60000);
  }
  else
  {
    Serial.println(" success");
    if (modem.isNetworkConnected())
    {
      gsmNetwork = true;
      Serial.println("Network connected");
      initSms();
    }
  }
}
void initSms()
{



  getSignalQuality(false);



  readyForSms();

  getGsmDateTime();

  initSim800Mqtt();
}

void checkMqttStatus()
{
  if (mqtt.connected())
  {
    mqtt.loop();
  }
  else if (hasWifi || gprsConnected)
  {
    reconnect();
  }
}

String SendShortCommand(String command, String response)
{
  Serial.print("Sending command: \"");
  Serial.print(command);
  Serial.println("\"");

  Serial2.print(command);
  Serial2.print("\r\n");

  return WaitForResponse(response);
}

String WaitForResponse(String response)
{
  String data;
  unsigned long startTime = millis();
  while (millis() - startTime < 5000)
  {
    if (Serial2.available() > 0)
    {
      data = Serial2.readString();
      Serial.println("read serial:");
      Serial.println(data);
      if (data.indexOf(response) != -1)
      {
        return data;
      }
    }
  }
  Serial.println("Did not receive data.");
  return "";
}

bool checkSim()
{

  String result = SendShortCommand("AT", "");
  delay(1000);
  Serial.println("result:");
  result = SendShortCommand("AT+CPIN?", "");

  Serial.println("result:");
  Serial.println(result);
  if (result.indexOf("OK") != -1)
  {
    simInserted = true;
    return true;
  }
  else
  {
    simInserted = false;
    return false;
  }
# 3653 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
}

void getOperator(bool report)
{
  String result = modem.getOperator();

  String newValue = "";
  if (result.indexOf("43235") != -1)
  {
    newValue = "irancell";
  }
  else if (result.indexOf("TCI") != -1)
  {
    newValue = "mci";
  }
  else if (result.indexOf("43220") != -1 || result.indexOf("43221") != -1)
  {
    newValue = "rightel";
  }
  else
  {
    newValue = "";
  }
  if (report && newValue != op)
  {

    StaticJsonDocument<64> doc;
    doc["mac"] = mac;
    doc["event"] = "report";
    doc["op"] = op;
    doc["sig"] = String(signalQuality);

    String result;
    serializeJson(doc, result);
    Serial.println(result);
    publishReport(result.c_str());


  }
  op = newValue;

  updateOperatorDisp();
  Serial.println("operator : ");
  Serial.println(result);
  Serial.println(op);

  events.send(buffer.c_str(), "gsm", millis());
}

void getSignalQuality(bool report)
{
  uint8_t s;
  s = modem.getSignalQuality();
  if (report && abs(s - signalQuality) > signal_threshold)
  {
    StaticJsonDocument<64> doc;

    doc["mac"] = mac;
    doc["event"] = "report";
    doc["op"] = op;
    doc["sig"] = String(signalQuality);
    String result;
    serializeJson(doc, result);
    Serial.println(result);

    publishReport(result.c_str());
  }
  updateSignalDisp();
  signalQuality = s;

  Serial.println("signal:");
  Serial.println(s);
  if (op.isEmpty())
  {
    getOperator(false);
  }
  events.send(String(s).c_str(), "signal", now);
}
void blinkLed()
{

  delay(100);

}

void GsmSoftReset()
{
  modem.restart();
}

void GsmReset()
{

  Serial.println("gsm reset");

  digitalWrite(RESET_GSM, HIGH);
  delay(700);

  digitalWrite(RESET_GSM, LOW);
  delay(1000);
  digitalWrite(RESET_GSM, HIGH);
  delay(3000);
}

void checkOutputSch(uint8_t input)
{
# 3813 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
}

String addScenario(String val)
{

  int freeIndex = -1;
  for (int i = 0; i < totalScenarios; i++) {
    if (scenarios[i].value.isEmpty()) {
      freeIndex = i;
      break;
    }
  }
  if (freeIndex == -1) {
    Serial.println("max scenarios count reached");
    return "";
  }
  if (processScenarios(val, freeIndex)) {
    String key = "s" + String(freeIndex + 1);
    writeDateTimeEEPROM(key.c_str(), val);
    scenariosCount = 0;

    for (int i = 0; i < totalScenarios; i++) {
      if (!scenarios[i].value.isEmpty()) scenariosCount++;
    }
    String text = scenarios[freeIndex].condition + "خروجی" + String(scenarios[freeIndex].outPin) + String(scenarios[freeIndex].outState);
    return text;
  }
  return "";
}

void removeScenario(const char *key)
{
   Serial.println(key);
  writeToEEPROM(key, "");
  Serial.println("after write");

  for (uint8_t i = 0; i < totalScenarios; i++)
  {
    if (strcmp(key, scenarios[i].key) == 0)
    {

      for (int j = i; j < totalScenarios - 1; j++)
      {
        scenarios[j] = scenarios[j + 1];

        String newKey = "s" + String(j + 1);
        strncpy(scenarios[j].key, newKey.c_str(), sizeof(scenarios[j].key) - 1);
        scenarios[j].key[sizeof(scenarios[j].key) - 1] = '\0';

        writeDateTimeEEPROM(newKey.c_str(), scenarios[j].value);
      }

      scenarios[totalScenarios - 1].value = "";
      String lastKey = "s" + String(totalScenarios);
      writeToEEPROM(lastKey.c_str(), "");
      break;
    }
  }

  scenariosCount = 0;
  for (int i = 0; i < totalScenarios; i++) {
    if (!scenarios[i].value.isEmpty()) scenariosCount++;
  }

  Serial.println("scenario deleted");
  String result = prepareDbData("feedback");
  publishReport(result.c_str());
}

void readyForSms()
{



  SendShortCommand("AT+CLIP=1\r", "");
  delay(500);
  SendShortCommand("AT+CMGF=1", "");



  if (op == "irancell")
  {

    SendShortCommand("AT+CSMP=17,167,0,0", "");

    delay(500);
  }


  SendShortCommand("AT+CMGD=1,4", "");

  delay(1000);


  SendShortCommand("AT+CMGDA= \"DEL ALL\"", "");


  smsIsReady = true;
}

void receiveSms()
{
  uint8_t index = 0;
  String result = "";

  while (index < 3)
  {
    index++;
    result = SendShortCommand("AT+CMGF=1", "CMGF=1");
    if (result.indexOf("OK") != -1)
    {
      delay(500);
      SendShortCommand("AT+CSCS=\"GSM\"", "");


      delay(500);
      SendShortCommand("AT+CSMP=17,167,0,0", "");
      break;
    }
    else
    {
      delay(3000);
    }
  }







}

void getGsmDateTime()
{
  if (deviceYear < 20)
  {
    String result;

    delay(1000);
    if (op == "irancell")
    {

      SendShortCommand("AT+SAPBR=3,1, \"Contype\",\"GPRS\"", "");


      SendShortCommand("AT+SAPBR=3,1, \"APN\",\"CMNET\"", "");



      SendShortCommand("AT+SAPBR=1,1", "");



      SendShortCommand("AT+CNTPCID=1", "");


      SendShortCommand("AT+CNTP=\"3.asia.pool.ntp.org\",14", "");




      SendShortCommand("AT+CNTP", "");

      delay(500);

      result = SendShortCommand("AT+CCLK?", "+CCLK");


      delay(1000);

      SendShortCommand("AT+SAPBR=0,1", "");
    }
    else if (op == "mci")
    {
      Serial2.println("AT+CLTS=1\r");

      delay(500);
      Serial2.println("AT+COPS=0\r");

      delay(500);


      result = SendShortCommand("AT+CCLK?", "+CCLK");
      delay(1000);

    }




    if (result.indexOf("+CCLK") != -1)
    {
      Serial.println("result->");
      Serial.println(result);

      int ind = result.indexOf('"');
      result = result.substring(ind, result.length());
      Serial.print("date buufer=>");
      Serial.println(result);
      updateDate(result);
    }
  }
}







uint8_t smsIndex = 0;

void parseData(String buff)
{
  Serial.println("buffer =>");
  Serial.println(buff);



  unsigned int len, index;


  index = buff.indexOf("\r");
  buff.remove(0, index + 2);
  buff.trim();





  if (buff.indexOf("+CLIP") != -1)
  {
    index = buff.indexOf("\"");
    String phone = buff.substring(index + 1, index + 14);
    Serial.println("+CLIP state");
    Serial.println(phone);
    for (uint8_t i = 0; i < totalPhoneNo; i++)
    {
      if (buff.indexOf(phoneNo[i]) != -1)
      {
        Serial.println("Answered");
        delay(1000);
        answerCall();
        return;
      }
    }
    hangUp();
    return;
  }


  if (buff != "OK")
  {
    index = buff.indexOf(":");
    String cmd = buff.substring(0, index);
    cmd.trim();
    buff.remove(0, index + 2);



    if (buff == "ERROR" && smsIndex > 0)
    {
      Serial.println("In Error");
      Serial.print("last sms index : ");
      Serial.println(smsIndex);
      String temp = "AT+CMGR=" + String(smsIndex) + "\r";
      delay(500);

      Serial2.println(temp);
      return;

    }
# 4122 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
    else if (cmd == "+CPIN")
    {
      Serial.println("buufer==>");
      Serial.println(buff);

      int ind = buff.indexOf(':');
      buff = buffer.substring(ind + 1, buff.length());
      Serial.println("buufer:::");
      Serial.println(buff);
    }

    else if (cmd == "+CMTI")
    {
# 4143 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
      index = buff.indexOf(",");

      String temp = buff.substring(index + 1, buff.length());
      smsIndex = temp.toInt();
      temp = "AT+CMGR=" + temp + "\r";

      Serial2.println(temp);





    }
    else if (cmd == "+CPMS")
    {
      Serial.print("cmd == +CPMS");

      if (buff.indexOf("SM_P") != -1)
      {

        int index = buff.indexOf(",");
        smsIndex = buff.substring(index + 1, index + 2).toInt();

        if (smsIndex > 0)
        {
          for (uint8_t i = 0; i < smsIndex; i++)
          {
            Serial.print("last sms index");
            Serial.println(smsIndex);
            String temp = "AT+CMGR=" + String(smsIndex - i) + "\r";

            Serial2.println(temp);
          }
          smsIndex = 0;
        }
      }
      return;
    }
    else if (cmd == "+CUSD")
    {
      Serial.print("sender =>");
      Serial.println(senderNumber);
      extractUssd(buff);
      return;
    }
    else if (cmd == "+CMGR")
    {
      extractSms(buff);

      if (msg.equals("p") && phoneNo[0].length() == 13)
      {
        for (uint8_t i = 0; i < totalPhoneNo; i++)
        {
          if (phoneNo[i] == senderNumber)
          {
            String text = "1";
            text = prepareSMSStats();
            debugPrint(text);
            prepareData();
            ReplyHex(text, senderNumber);
          }
        }
      }
      else if (msg.equals("p") && phoneNo[0].length() != 13)
      {
        writeToEEPROM(offsetPhone[0], senderNumber);
        phoneNo[0] = senderNumber;







        String sms = "";
        sms = prepareSMSStats();
        debugPrint(sms);
        prepareData();
        ReplyHex(sms, senderNumber);
      }



      if (comparePhone(senderNumber))
      {
        doAction(senderNumber);

      }



      Serial2.println("AT+CMGD=1,4");
      delay(1000);
      if (smsIndex > 0)
      {
        String c = "AT+CMGD=" + String(smsIndex);
        Serial2.println(c);
        delay(1000);
        smsIndex = smsIndex - 1;
      }
    }

  }
}

void extractUssd(String buff)
{
  int startIndex = buff.indexOf('"');
  int endIndex = buff.indexOf('"', startIndex + 1);
  String str = buff.substring(startIndex + 1, endIndex);
  Serial.println("sub str");
  Serial.println(str);

  ForwardHex(str, senderNumber);
}


void readSerial()
{
  unsigned long startTime = millis();
  while (millis() - startTime < 5000)
  {
    if (Serial2.available() > 0)
    {
      Serial.println("read serial:");
      buffer = Serial2.readString();
      Serial.println(buffer);
    }
  }
}

bool isRegistered()
{
  gsmNetwork = modem.isNetworkConnected();
  Serial.println("isRegistered :");
  Serial.println(gsmNetwork);
  return gsmNetwork;
# 4292 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
}






void extractSms(String buff)
{
  unsigned int index;

  index = buff.indexOf(",");
  smsStatus = buff.substring(1, index - 1);
  buff.remove(0, index + 2);
  senderNumber = buff.substring(0, 13);
  buff.remove(0, 19);

  receivedDate = buff.substring(0, 20);
  Serial.println("receive date =>");
  Serial.println(receivedDate);

  if (deviceYear < 20)
  {

    receivedDate = "\"" + receivedDate + "\"";
    updateDate(receivedDate);
  }
  else
  {

  }
  buff.remove(0, buff.indexOf("\r"));
  buff.trim();

  index = buff.indexOf("\n\r");
  buff = buff.substring(0, index);
  buff.trim();
  msg = buff;
  buff = "";
  msg.toLowerCase();


  if (isDigit(msg.charAt(0)) && isDigit(msg.charAt(1)))
  {

    Serial.println("UTF");

    msg = hexToUTF8(msg);
    msg.toLowerCase();

    Serial.println("The converted string is valid UTF-8:");



  }
  else
  {
    Serial.println("The converted string is NOT UTF8");
  }
  smsDisplay();


  String tempcmd = msg.substring(0, 3);

  if (tempcmd.equals("p1=") || tempcmd.equals("p2=") || tempcmd.equals("p3=") || tempcmd.equals("p4=") || tempcmd.equals("p5="))
  {

    tempPhone = msg.substring(3, 16);
    msg = tempcmd;
    debugPrint(msg);
    debugPrint(tempPhone);
  }
  else if (tempcmd.equals("t1=") || tempcmd.equals("t2=") || tempcmd.equals("t3=") || tempcmd.equals("t4="))
  {
    tempSch = msg.substring(3, 21);
    msg = tempcmd;
    debugPrint(msg);
    debugPrint("tempSch");
    debugPrint(tempSch);
  }
  else if (tempcmd.equals("i1=") || tempcmd.equals("i2=") || tempcmd.equals("i3=") || tempcmd.equals("i4="))
  {
    tempSch = msg.substring(3, 21);
    msg = tempcmd;
    debugPrint(msg);
    debugPrint("tempSch");
    debugPrint(tempSch);
  }
  else if (tempcmd.equals("t1x") || tempcmd.equals("t2x") || tempcmd.equals("t3x") || tempcmd.equals("t4x"))
  {
    msg = tempcmd;
    debugPrint(msg);
  }
  else if (tempcmd.indexOf("l") != -1 && tempcmd.indexOf("=") != -1)
  {

    tempLabel = msg.substring(3, 23);
    msg = tempcmd;
    debugPrint(tempLabel);
    debugPrint(msg);
  }
  else if (tempcmd.indexOf("n") != -1 && tempcmd.indexOf("=") != -1)
  {

    tempLabel = msg.substring(3, 23);
    msg = tempcmd;
    debugPrint(tempLabel);
    debugPrint(msg);
  }
  else if (tempcmd.equals("s"))
  {

    debugPrint(msg);
  }

}



void updateDate(String dateTime)
{

  Serial.println("date =>");
  Serial.println(dateTime);

  int year,
      month, day, hour, minute;

  year = (dateTime.substring(1, 3).toInt());
  if (deviceYear == 70)
  {
    return;
  }
  month = (dateTime.substring(4, 6).toInt());
  day = (dateTime.substring(7, 9).toInt());
  hour = (dateTime.substring(10, 12).toInt());
  minute = (dateTime.substring(13, 15).toInt());
  Serial.println("year =>");
  Serial.println(year);

  deviceYear = year;






  rtc.setTime(0, minute, hour, day, month, (2000 + year));

  checkTasks();
  Serial.println("rtc time");
  Serial.println(rtc.getDateTime(true));
}

void checkHourTasks()
{
  Serial.println("Hour Task");
}

void checkSmsHistory()
{
  Serial2.println("AT+CPMS?\r");
  delay(500);
}

void checkTasks()
{



  uint8_t dayOfWeek = rtc.getDayofWeek();
  uint8_t hour = rtc.getHour(true);
  uint8_t minute = rtc.getMinute();
  updateDisplay();



  if (minCounter >= 60)
  {
    minCounter = 0;
    createMovingAverage();
    checkSim();
  }

  if (minCounter > 0 && minCounter % 10 == 0)
  {

    uint8_t gsmCounter = 0;
    setAverageElement();

    if (hasWifi)
    {
      if (WiFi.status() != WL_CONNECTED && wifiTryCount < 2)
      {
        initWiFi();
      }
      else
      {





      }
    }
    else if (ssid != "" && password != "" && wifiTryCount < 2)
    {
      initWiFi();
    }
    if (simInserted)
    {
      if (op == "" || signalQuality == 0 || signalQuality == 99)
      {
        setupGSM();
      }


      checkSmsHistory();
    }
    else
    {




    }
  }

  if (minCounter % 5 == 0)
  {
    if (ssid != "" && password != "" && WiFi.status() != WL_CONNECTED && wifiTryCount < 2)
    {
      initWiFi();
    }
  }
  if (minCounter % 2 == 0)
  {


    if (gsmNetwork)
    {
      getSignalQuality(mqtt_connected);
      if (op == "")
      {
        getOperator(mqtt_connected);
      }
      if (signalQuality == 0 || signalQuality == 99)
      {
        GsmSoftReset();
      }
    }
    else
    {

    }

    if (hasWifi && !mqtt_connected)
    {
      reconnect();
    }
    else if (gprsConnected && !mqtt_connected)
    {
      reconnect();
    }

    if (deviceYear < 20 || deviceYear > 70)
    {

      if (hasWifi)
      {
        printLocalTime();
      }
      else if (gsmNetwork)
      {
        getGsmDateTime();
      }
    }
  }





  sensors.requestTemperatures();
  bool flag = false;
  for (uint8_t i = 0; i < totalTemps; i++)
  {
    float t = sensors.getTempCByIndex(i);
    if (abs(t - temps[i].temp) > temp_threshold)
    {
      flag = true;
    }
    temps[i].temp = t;
  }


  if (flag)
  {
    if (mqtt_connected)
    {
      DynamicJsonDocument doc(64);
      doc["mac"] = mac;
      doc["event"] = "report";
      JsonArray array1 = doc.createNestedArray("temps");
      for (uint8_t i = 0; i < totalTemps; i++)
      {
        temps[i].temp = sensors.getTempCByIndex(i);
        array1.add(temps[i].temp);
      }
      String result;
      serializeJson(doc, result);
      Serial.println(result);
      publishReport(result.c_str());
    }
  }

  minCounter = minCounter + 1;



  for (uint8_t i = 0; i < totalOutputs; i++)
  {





    if (outputs[i].timer.length() == 18)
    {
      if (outputs[i].timer.substring(0, 7).indexOf(String(dayOfWeek)) != -1)
      {

        int startH = outputs[i].timer.substring(8, 10).toInt();
        int startM = outputs[i].timer.substring(11, 13).toInt();
        int endH = outputs[i].timer.substring(13, 15).toInt();
        int endM = outputs[i].timer.substring(16, 18).toInt();

        if (hour >= startH)
        {
          if (hour == startH)
          {
            if (minute > startM)
            {
              if (hour <= endH)
              {
                if (hour == endH)
                {
                  if (minute < endM)
                  {

                    switchRelay(i, LOW, 0, false);


                  }
                  else
                  {


                    switchRelay(i, HIGH, 0, false);



                    continue;
                  }
                }
                if (hour < endH)
                {

                  switchRelay(i, LOW, 0, false);




                  continue;
                }
              }
              else
              {

                switchRelay(i, HIGH, 0, false);



              }
            }
            else
            {

              continue;
            }
          }
          else
          {
            if (hour <= endH)
            {
              if (hour == endH)
              {
                if (minute < endM)
                {

                  switchRelay(i, LOW, 0, false);


                }
                else
                {

                  switchRelay(i, HIGH, 0, false);


                  continue;
                }
              }
              if (hour < endH)
              {

                switchRelay(i, LOW, 0, false);


                continue;
              }
            }
            else
            {

              switchRelay(i, HIGH, 0, false);


            }
          }
        }
      }
    }
    else if (outputs[i].timer.length() == 16)
    {

      if ((outputs[i].timer.substring(0, 7).indexOf(String(dayOfWeek)) != -1))
      {

        int perHour = outputs[i].timer.substring(8, 10).toInt();
        int h = outputs[i].timer.substring(11, 13).toInt();
        int m = outputs[i].timer.substring(14, 16).toInt();
        uint t;
        if (perHour <= 0)
        {

          return;
        }
        if (m <= 0)
        {

          return;
        }
        if (24 % perHour == 0)
        {
          t = 24 / perHour;
        }
        else
        {

          return;
        }
        if (h >= perHour)
        {

          return;
        }

        for (int j = 0; j <= t; j++)
        {
          uint8_t cycleStart = (j + 1) * perHour;
          uint8_t cycleEnd = cycleStart + perHour;
          if (hour >= cycleStart && hour < cycleEnd)
          {

            uint16_t onMTime = (h * 60) + m;
            uint16_t passed = ((hour - cycleStart) * 60) + minute;

            if (passed <= onMTime)
            {

              switchRelay(i, LOW, 0, false);
              Serial.print(i + 1);
              Serial.println(" sch is on ");
            }
            else
            {

              switchRelay(i, HIGH, 0, false);
              Serial.print(i + 1);
              Serial.println(" sch is off ");
            }
            break;
          }
        }
      }
    }
    else if (outputs[i].timer.charAt(0) == 't' && toggleTimers[i] != -1)
    {
      int8_t toggle = outputs[i].timer.substring(1).toInt();
      if (toggleTimers[i] == -1)
      {
        Serial.println("check toggle timer");
        Serial.println("toggle time");
        Serial.println(toggle);
        toggleTimers[i] = toggle * 2;
      }
      else
      {
        if (toggleTimers[i] > toggle)
        {
          toggleTimers[i] = toggleTimers[i] - 1;

          switchRelay(i, LOW, 0, false);
          Serial.println("toggleTimers[i] > toggle");
          Serial.println(toggleTimers[i]);
        }
        else if (toggleTimers[i] > 0 && toggleTimers[i] <= toggle)
        {
          toggleTimers[i] = toggleTimers[i] - 1;

          switchRelay(i, HIGH, 0, false);
          Serial.println("toggleTimers[i] <= toggle");
          Serial.println(toggleTimers[i]);
        }
        else if (toggleTimers[i] == 0)
        {
          toggleTimers[i] = toggle * 2;
          Serial.println("toggleTimers[i] == 0");
        }
      }
    }
  }



}
int avgIndex = 0;

void setAverageElement()
{
  Serial.println("setAverageElement");

  for (uint8_t i = 0; i < totalAnalogs; i++)
  {
    analogInputs[i].buffer[(minCounter / 10) - 1] = analogInputs[i].voltage;
  }

  for (uint8_t i = 0; i < totalTemps; i++)
  {
    temps[i].buffer[(minCounter / 10) - 1] = temps[i].temp;
  }


}

void createMovingAverage()
{
  Serial.println("createMovingAverage");

  for (uint8_t i = 0; i < totalAnalogs; i++)
  {
    float avg = 0;
    for (int j = 0; j < WINDOW_SIZE; j++)
    {
      avg += analogInputs[i].buffer[j];
    }
    avg /= WINDOW_SIZE;

    analogInputs[i].avg = (int)avg;
    Serial.println("analog avg :");
    Serial.println(avg);
  }

  for (uint8_t i = 0; i < totalTemps; i++)
  {
    float avg = 0;
    for (int j = 0; j < WINDOW_SIZE; j++)
    {
      avg += temps[i].buffer[j];
    }
    avg /= WINDOW_SIZE;

    temps[i].avg = (int)avg;
    Serial.println("temp avg :");
    Serial.println(avg);
  }
# 4890 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
  Serial.println("current avg :");
  Serial.println(currentAmp.value);

  currentAmp.value = 0.0;

  if (mqtt_connected)
  {

    String report = prepareDbLog("log");
    publishReport(report.c_str());
  }
}

void saveLastRelayStates()
{







}

void sendMqttFeedback()
{
  if (mqtt_connected)
  {
    StaticJsonDocument<64> doc;

    doc["mac"] = mac;
    doc["event"] = "feedback";
    doc["oSt"] = createOutArray();
    String result;
    serializeJson(doc, result);
    Serial.println(result);
    publishReport(result.c_str());


  }
  delay(200);
  updateStatesDSP();
}
void setPwm(uint8_t index, uint8_t percent)
{

  uint8_t value = (percent * (aResolution / 100));
  if (pwms[index].pwm == percent)
  {
    return;
  }

  ledcWrite(index, value);
  pwms[index].pwm = percent;
  if (mqtt_connected)
  {
    StaticJsonDocument<64> doc;
    doc["mac"] = mac;
    doc["event"] = "feedback";
    pwmStates = createPwmArray();
    doc["pwm"] = pwmStates;
    doc.remove("percent");
    String result;
    serializeJson(doc, result);
    Serial.println(result);

    publishReport(result.c_str());
  }
}



void clearTimer(uint8_t index)
{
  writeDateTimeEEPROM(outputs[index].timerKey, "");
  outputs[index].timer = "";
  Serial.print("cleared timer ");
  Serial.println(index);
}
# 5023 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
void doAction(String phoneNumber)
{





  if (msg.indexOf("r") != -1 && msg.length() > 3)
  {
    uint8_t out = msg.substring(1, 2).toInt() - 1;
    boolean state = 0;
    if (msg.substring(2, 4) == "on")
    {
      state = 1;
    }
    else if (msg.substring(2, 4) == "of")
    {
      state = 0;
    }
    else
    {
      return;
    }

    String outPrg = outputIsBusy(out);
    if (!outPrg.isEmpty())
    {
      clearSmsVariables();
      String text = "رله " + String(out + 1) + " در حالت سناریو قرار دارد ";


      ReplyHex(text, phoneNumber);
      return;
    }

    switchRelay(out, !state, 0, false);



    Serial.print(out + 1);
    Serial.print("is ");
    Serial.println(state == 1 ? "ON" : "OFF");
    String text = "";
    if (outputs[out].label.isEmpty())
    {
      text = "رله " + String(out + 1);
    }
    else
    {
      text = outputs[out].label;
    }

    text = text + ((state) ? " روشن شد " : " خاموش شد ");
    text += "\n";
    for (uint8_t i = 0; i < totalOutputs; i++)
    {
      text += outStates[(i * 2) + 1];
    }

    ReplyHex(text, phoneNumber);
    if (!outputs[out].timer.isEmpty())
    {
      clearTimer(out);
    }
    events.send("refresh", NULL, millis());
  }



  else if (msg.indexOf("l") != -1 && msg.indexOf("=") != -1)
  {
    uint8_t out = (msg.substring(1, 2).toInt()) - 1;
    if (out != -1)
    {
      writeToEEPROM(outputs[out].labelKey, tempLabel);
      outputs[out].label = tempLabel;
      String text = "نام رله " + String(out + 1) + " به " + tempLabel + " تغییر یافت ";
      ReplyHex(text, phoneNumber);
    }
  }


  else if (msg == "labels")
  {
    String text = "نام رله ها:\r\n";
    for (uint8_t i = 0; i < totalOutputs; i++)
    {
      if (!outputs[i].label.isEmpty())
      {
        text = text + String(i + 1) + ": " + (outputs[i].label == "" ? "بی نام" : outputs[i].label) + "\r\n";
      }
    }
    debugPrint(text);
    ReplyHex(text, phoneNumber);
  }

  else if (msg.indexOf("n") != -1 && msg.indexOf("=") != -1)
  {
    uint8_t in = (msg.substring(1, 2).toInt()) - 1;
    if (in != -1)
    {
      writeToEEPROM(inputs[in].labelKey, tempLabel);
      inputs[in].label = tempLabel;
      String text = "نام ورودی " + String(in + 1) + " به " + tempLabel + " تغییر یافت ";
      ReplyHex(text, phoneNumber);
    }
  }



  else if (msg.indexOf("stat=") != -1)
  {
    uint8_t out = msg.substring(msg.indexOf("=")).toInt() - 1;
    String text = "رله" + String(out + 1);

    debugPrint("Relay" + String(out + 1) + " is " + text);
    text = text + (mcp.digitalRead(outputs[out].gpio) == HIGH) ? "روشن است " : "خاموش است ";
    ReplyHex("Relay 1 is " + text, phoneNumber);
  }

  else if (msg == "stat")
  {
    String text = "";

    for (uint8_t i = 0; i < totalOutputs; i++)
    {
      text = text + String(i + 1) + (mcp.digitalRead(outputs[i].gpio) == HIGH) ? "_ON\r\n" : "_OFF\r\n";
    }
    debugPrint(text);
    Reply(text, phoneNumber);
  }



  else if (msg.indexOf("p") != -1 && msg.indexOf("=") != -1)
  {
    uint8_t index = (msg.substring(1, 2).toInt()) - 1;
    if (tempPhone.length() == 13 && tempPhone[0] == '+')
    {
      writeToEEPROM(offsetPhone[index], tempPhone);
    }
    else if (tempPhone.length() == 11 && tempPhone[0] == '0')
    {
      tempPhone = "+98" + tempPhone.substring(1);
      writeToEEPROM(offsetPhone[index], tempPhone);
    }
    phoneNo[index] = tempPhone;
    String text = "شماره مدیر " + String(index + 1) + " با موفقیت ثبت شد";
    Serial.println("Number " + String(index + 1) + " is Registered");

    ReplyHex(text, phoneNumber);
  }

  else if (msg == "list")
  {
    String text = "لیست کاربران";
    for (uint8_t i = 0; i < totalPhoneNo; i++)
    {
      if (!phoneNo[i].isEmpty())
      {

        String phone = "0" + phoneNo[i].substring(3);
        if (i != (totalPhoneNo - 1))
        {
          text = text + "\r\n";
        }
        text = text + String(i + 1) + "." + phone;
      }
    }
    debugPrint("List of Registered Phone Numbers: \r\n" + text);
    ReplyHex(text, phoneNumber);
  }


  else if (msg.indexOf("del=") != -1)
  {
    uint8_t index = (msg.substring(4, 5).toInt()) - 1;
    writeToEEPROM(offsetPhone[index], "");
    phoneNo[index] = "";

    ReplyHex("شماره کاربر " + String(index + 1) + "با موفقیت پاک شد", phoneNumber);
  }


  else if (msg == "del=all")
  {
    writeToEEPROM(offsetPhone[0], "");
    writeToEEPROM(offsetPhone[1], "");
    writeToEEPROM(offsetPhone[2], "");
    writeToEEPROM(offsetPhone[3], "");
    writeToEEPROM(offsetPhone[4], "");
    phoneNo[0] = "";
    phoneNo[1] = "";
    phoneNo[2] = "";
    phoneNo[3] = "";
    phoneNo[4] = "";
    debugPrint("All phone numbers are deleted.");
    ReplyHex("همه شماره ها پاک شدند", phoneNumber);
  }


  else if (msg.indexOf("t") != -1 && msg.indexOf("=") != -1)
  {
    uint8_t index = (msg.substring(1, 2).toInt()) - 1;
    String outPrg = outputIsBusy(index);
    if (!outPrg.isEmpty())
    {
      String text = "رله " + String(index + 1) + " در حالت سناریو قرار دارد ";


      ReplyHex(text, phoneNumber);
      return;
    }
    writeDateTimeEEPROM(outputs[index].timerKey, tempSch);
    outputs[index].timer = tempSch;
    String text;
    if (tempSch.length() >= 2 && tempSch.length() < 6 && tempSch.charAt(0) != 't')
    {
      String t;
      if (tempSch.substring(2, 4) == "-")
      {
        bool minus = true;
        t = tempSch.substring(3, 5) + " درجه ";
      }
      else
      {
      }
      t = tempSch.substring(2, 4) + " درجه ";
      if (tempSch.charAt(0) == 'c')
      {
        text = " عملکرد فن رله " + String(index + 1) + " در دمای پایینتر از" + t + " فعال شد";
      }
      else if (tempSch.charAt(0) == 'h')
      {
        text = " عملکرد هیتر رله " + String(index + 1) + " در دمای بالاتر از" + t + " فعال شد";
      }
    }
    else
    {
      text = "تایمر رله " + String(index + 1) + " فعال شد";
    }
    if (outputs[index].timer.charAt(0) == 't')
    {
      int8_t toggle = outputs[index].timer.substring(1).toInt();
      toggleTimers[index] = toggle * 2;
    }
    else
    {
      toggleTimers[index] = -1;
    }

    ReplyHex(text, phoneNumber);
    debugPrint("Relay " + String(index + 1) + " is Set :");
    debugPrint(tempSch);
  }



  else if (msg.indexOf("s") != -1 && msg.indexOf("=") != -1)
  {
    uint8_t in = (msg.substring(1, 2).toInt()) - 1;
    String text = addScenario(tempSch);
    if (text != "")
    {
      ReplyHex(text, phoneNumber);
    }
    debugPrint("Input " + String(in + 1) + " is Set :");
    debugPrint(tempSch);
  }


  else if (msg.indexOf("s") != -1 && msg.indexOf("x") != -1)
  {
    uint8_t in = (msg.substring(1, 2).toInt()) - 1;
    writeDateTimeEEPROM(scenarios[in].key, "");
    scenarios[in].value = "";
    String text = "برنامه ورودی " + String(in + 1) + " پاک شد ";
    ReplyHex(text, phoneNumber);
    debugPrint("Input " + String(in + 1) + " prg deleted.");
  }
  else if (msg == "set")
  {





    String text =
        "setting : " + String(callOnAlert) + "," + String(securityMode) + "," + String(notifyScenarios) + "," + String(remoteCount);

    ReplyHex(text, phoneNumber);
  }

  else if (msg.indexOf("t") != -1 && msg.indexOf("x") != -1)
  {
    uint8_t out = (msg.substring(1, 2).toInt()) - 1;
    writeDateTimeEEPROM(outputs[out].timerKey, "");
    outputs[out].timer = "";
    toggleTimers[out] = -1;

    String text = " عملکرد رله" + String(out + 1) + " پاک شد ";
    ReplyHex(text, phoneNumber);
    debugPrint("Relay " + String(out + 1) + " Sch deleted.");
    checkTasks();
  }
  else if (msg == "sch")
  {
    String text = "لیست عملکرد رله ها: \r\n";
    for (uint8_t i = 0; i < totalOutputs; i++)
    {
      if (!outputs[i].timer.isEmpty())
      {
        text = text + String(i + 1) + ": " + (outputs[i].timer == "" ? "ساده" : outputs[i].timer) + "\r\n";
      }
    }
    debugPrint("List of Schedules: \r\n" + text);
    ReplyHex(text, phoneNumber);
  }
  else if (msg == "temp")
  {
    String text = "دمای فعلی :";
    text = text + "\r\n";
    sensors.requestTemperatures();

    for (uint8_t i = 0; i < totalTemps; i++)
    {
      temps[i].temp = sensors.getTempCByIndex(i);
      text = text + String(temps[i].temp) + "ºC" + "\r\n";
    }
    ReplyHex(text, phoneNumber);
    debugPrint(msg);
  }


  else if (msg == "son")
  {
# 5373 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
  }
  else if (msg == "soff")
  {
# 5395 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
  }
  else if (msg == "balance")
  {
    Serial.println("Charge Message Recieved!");
    Serial.println(msg);
    if (op == "mci")
    {
      Serial2.println(TCI_CHARGE);
    }
    else if (op == "irancell")
    {
      Serial2.println(IRANCEL_CHARGE);
    }
    else if (op == "rightel")
    {
      Serial2.println(RIGHTEL_CHARGE);
    }
  }

  else if (msg == "non")
  {
    notifyScenarios = true;
    String text = "اعلام روشن شدن دستگاه، فعال شد";
    ReplyHex(text, phoneNumber);
  }
  else if (msg == "noff")
  {
    notifyScenarios = false;
    String text = "اعلام روشن شدن دستگاه، خاموش شد";
    ReplyHex(text, phoneNumber);
  }

  else if (msg == "poff")
  {
# 5444 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
  }
  else if (msg == "wipe")
  {

    Serial.println("restet called");
    EEPROM.clear();

    removeAllRemotes();
    resetWifi();
    String text = "دستگاه ریست و حافظه پاک شد ";
    ReplyHex(text, phoneNumber);
    ESP.restart();
  }

  else if (msg == "dr")
  {
    Serial.print("Remote Saved!");

    removeAllRemotes();
    String text = "همه ریموت ها پاک شدند";
    ReplyHex(text, phoneNumber);
  }

  else if (msg == "calloff")
  {
    callOnAlert = false;
    String text = "تماس تلفنی غیرفعال شد";
    ReplyHex(text, phoneNumber);
  }
  else if (msg == "callon")
  {
    callOnAlert = true;
    String text = "تماس تلفنی فعال شد";
    ReplyHex(text, phoneNumber);
  }

  clearSmsVariables();
}

void clearSmsVariables()
{
  smsStatus = "";
  receivedDate = "";
  msg = "";
  tempPhone = "";
  tempSch = "";
  tempLabel = "";
}

String outputIsBusy(uint8_t index)
{
  for (int i = 0; i < totalScenarios; i++)
  {
    Scenario scenario = scenarios[i];
    if (scenario.value.isEmpty() || scenario.value.charAt(0) != 'o')
      continue;
    uint8_t target = 100;
    if (index == scenario.input)
    {
      target = scenario.outPin;
    }
    else if (index == scenario.outPin)
    {
      target = scenario.input;
    }

    if (target != 100)
    {
      return scenario.value;
    }
  }
  return "";
}





void Reply(String text, String Phone)
{
  SendShortCommand("AT+CMGF=1", "");


  if (op == "irancell")
  {
    SendShortCommand("AT+CSMP=17,167,0,0", "");


  }
  SendShortCommand("AT+CMGS=\"" + Phone + "\"", "");


  SendShortCommand(text, "");

  delay(100);
  Serial2.write(0x1A);


  Serial.println("SMS Sent Successfully.");
  receiveSms();
}

void ReplyHex(String text, String Phone)
{
  char charArray[160];
  text.toCharArray(charArray, 160);
  String hexString = printCodePoints((uint8_t *)charArray);


  SendShortCommand("AT+CMGF=1", "");



  SendShortCommand("AT+CSCS=\"HEX\"", "");


  SendShortCommand("AT+CSMP=17,167,0,8", "");




  SendShortCommand("AT+CMGS=\"" + Phone + "\"", "");


  SendShortCommand(hexString, "");

  delay(100);
  Serial2.write(0x1A);


  Serial.println("SMS Sent Successfully.");
  receiveSms();
}

void ForwardHex(String text, String Phone)
{

  Serial2.print("AT+CMGF=1\r");
  delay(1000);

  Serial2.print("AT+CSCS=\"HEX\"\r");
  delay(1000);


  Serial2.println("AT+CSMP=17,167,0,8");
  delay(1000);

  Serial2.print("AT+CMGS=\"" + Phone + "\"\r");
  delay(1000);
  Serial2.print(text);
  delay(100);
  Serial2.write(0x1A);

  delay(3000);

  Serial.println("SMS Sent Successfully.");
  receiveSms();
}

void callAdmin(int index)
{
  Scenario scenario = scenarios[index];
  if (millis() - scenario.lastNotif > 5 * 60000)
  {
    scenarios[index].lastNotif = millis();
    Serial2.println("ATD+ " + phoneNo[0] + ";");
    addTask(hangUp, 20000);
  }
}

void hangUp()
{
  Serial2.println("ATH");
}

void answerCall()
{
  Serial2.println("ATA");
}





void writeToEEPROM(const char *addrOffset, const String &strToWrite)
{



  EEPROM.putString(addrOffset, strToWrite);



}
void writeIntToEEPROM(const char *addrOffset, unsigned int value)
{

  EEPROM.putUInt(addrOffset, value);
}

void writeDateTimeEEPROM(const char *addrOffset, const String &strToWrite)
{
  EEPROM.putString(addrOffset, strToWrite);
}





String readFromEEPROM(const char *addrOffset)
{
  String value = EEPROM.getString(addrOffset);
  return value;
}
int readIntFromEEPROM(const char *addrOffset)
{
  return EEPROM.getUInt(addrOffset, 0);
}





boolean comparePhone(String number)
{
  boolean flag = 0;

  for (uint8_t i = 0; i < totalPhoneNo; i++)
  {
    phoneNo[i] = readFromEEPROM(offsetPhone[i]);
    if (phoneNo[i].equals(number))
    {
      flag = 1;
      break;
    }
  }


  return flag;
}

void flip()
{



  ++count;

  if (count == 10)
  {
    flipper.attach(0.1, flip);
  }

  else if (count == 60)
  {
    flipper.detach();
  }
}





void debugPrint(String text)
{
  if (DEBUG_MODE == 1)
  {
    Serial.println(text);
  }
}

String createOutArray()
{
  String result = "";
  for (uint8_t i = 0; i < totalOutputs; i++)
  {
    result += String(outputs[i].type);
    result += (mcp.digitalRead(outputs[i].gpio)) ? '1' : '0';



  };
  outStates = result;
  return result;
}

String createPwmArray()
{
  String result = "";
  for (uint8_t i = 0; i < totalPwm; i++)
  {
    result += pwms[i].pwm;
    if (i < (totalPwm - 1))
    {
      result += ",";
    }
  };
  pwmStates = result;
  return result;
}

String createInArray()
{
  String result = "";
  for (uint8_t i = 0; i < totalInputs; i++)
  {
    result += (inputs[i].state) ? '1' : '0';



  };
  inStates = result;

  return result;
}

String createSettingArray()
{
  String result = "";
  result += callOnAlert ? '1' : '0';
  result += securityMode ? '1' : '0';
  result += notifyScenarios ? '1' : '0';
  result += hasWifi ? '1' : '0';
  result += simInserted ? '1' : '0';
  result += gsmNetwork ? '1' : '0';
  result += forceUseGprs ? '1' : '0';
  return result;
}

String prepareData()
{
  StaticJsonDocument<512> doc;

  doc["nt"] = op;
  doc["sig"] = signalQuality;
  doc["date"] = rtc.getEpoch();

  for (uint8_t i = 0; i < totalOutputs; i++)
  {
    JsonObject temp = doc.createNestedObject(String(i));
    temp["name"] = (outputs[i].label.isEmpty()) ? "Relay " + String(i + 1) : outputs[i].label;
    temp["timer"] = (outputs[i].timer.isEmpty()) ? "" : outputs[i].timer;
    temp["state"] = (mcp.digitalRead(outputs[i].gpio)) ? 1 : 0;
  };





  doc["net"] = mqttNet;

  doc["sets"] = createSettingArray();
  doc["progs"] = createScenariosArray();

  doc["iSt"] = createInArray();
  doc["pwm"] = createPwmArray();

  sensors.requestTemperatures();

  JsonArray array1 = doc.createNestedArray("temps");
  for (uint8_t i = 0; i < totalTemps; i++)
  {
    temps[i].temp = sensors.getTempCByIndex(i);
    array1.add(temps[i].temp);
  }
  JsonArray array2 = doc.createNestedArray("ain");

  for (uint8_t i = 0; i < totalAnalogs; i++)
  {
    array2.add(analogInputs[i].voltage);
  }
  String result;
  serializeJson(doc, result);
  Serial.print(result);
  return result;
}

String mqttPeresence()
{
  StaticJsonDocument<64> doc;
  doc["mac"] = mac;

  doc["event"] = "report";
  doc["oSt"] = outStates;
  doc["iSt"] = inStates;
  doc["pwm"] = pwmStates;

  unsigned char unsignedCharArray[164];


  serializeJson(doc, unsignedCharArray, 164);




  unsigned char base64[164];

  unsigned int base64_length = encode_base64(unsignedCharArray, strlen((char *)unsignedCharArray), base64);
  Serial.println(String((const char *)base64));
  return String((const char *)base64);
}

String createTimersArray()
{
  String str;
  for (uint8_t i = 0; i < totalOutputs; i++)
  {
    if (outputs[i].timer.length() > 0)
    {
      str += outputs[i].timer;
    }
    else
    {
      str += "-";
    }
    if (i < (totalOutputs - 1))
    {
      str += ",";
    }
  }
  Serial.println(str);
  return str;
}
String createScenariosArray()
{
  String str = "";
  for (uint8_t i = 0; i < scenariosCount; i++)
  {
    if (scenarios[i].value.length() > 0)
    {
      str += scenarios[i].value;
      str += ":s";
      str += String(i + 1);
    }
    if (i < (scenariosCount - 1))
    {
      str += ",";
    }
  }

  return str;
}


String prepareDbData(String event)
{
  StaticJsonDocument<512> doc;
  doc["op"] = op;
  doc["sig"] = String(signalQuality);
  doc["mac"] = mac;
  doc["event"] = event;

  doc["net"] = mqttNet;
  doc["sets"] = createSettingArray();
  doc["tims"] = createTimersArray();
  doc["progs"] = createScenariosArray();

  doc["oSt"] = createOutArray();
  doc["iSt"] = createInArray();
  doc["pwm"] = createPwmArray();
  doc["curr"] = currentAmp.value;
  sensors.requestTemperatures();

  JsonArray array1 = doc.createNestedArray("temps");
  for (uint8_t i = 0; i < totalTemps; i++)
  {
    temps[i].temp = sensors.getTempCByIndex(i);
    array1.add(temps[i].temp);
  }
  JsonArray array2 = doc.createNestedArray("ain");

  for (uint8_t i = 0; i < totalAnalogs; i++)
  {
    array2.add(analogInputs[i].voltage);
  }

  String result;
  serializeJson(doc, result);
  Serial.println(result);

  return result;
}

String prepareDbLog(String event)
{

  StaticJsonDocument<512> doc;

  doc["op"] = op;
  doc["sig"] = String(signalQuality);

  doc["mac"] = mac;
  doc["event"] = event;
  doc["net"] = mqttNet;

  doc["oSt"] = createOutArray();
  doc["iSt"] = createInArray();
  doc["pwm"] = createPwmArray();
  doc["amp"] = (int)currentAmp.value;

  JsonArray array1 = doc.createNestedArray("temps");
  for (uint8_t i = 0; i < totalTemps; i++)
  {
    array1.add(temps[i].avg);
  }
  JsonArray array2 = doc.createNestedArray("ain");

  for (uint8_t i = 0; i < totalAnalogs; i++)
  {
    array2.add(analogInputs[i].avg);
  }

  String result;
  serializeJson(doc, result);
  Serial.println(result);

  return result;
}

String prepareTimersData()
{
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

String prepareSync()
{
  StaticJsonDocument<256> doc;
  doc["mac"] = "macString";
  String result;
  serializeJson(doc, result);
  return result;
}

String prepareSMSStats()
{
  esp_task_wdt_reset();

  String text = "";
  for (int i = 0; i < totalOutputs; i++)
  {
    String temp;
    if (outputs[i].label.isEmpty())
    {
      temp = temp + "r" + String(i + 1) + "-";
    }
    else
    {
      temp = temp + outputs[i].label + "-";
    }
    if (outputs[i].timer.isEmpty())
    {
      temp = temp + "t" + "-";
    }
    else
    {
      temp = temp + outputs[i].timer + "-";
    }
    if (scenarios[i].value.isEmpty())
    {
      temp = temp + "i" + "-";
    }
    else
    {
      temp = temp + scenarios[i].value + "-";
    }
    if (mcp.digitalRead(outputs[i].gpio))
    {
      temp = temp + 0;
    }
    else
    {
      temp = temp + 1;
    }
    temp = temp + "=";
    if (i != 3)
    {
    }
    text = text + temp;
    Serial.println(temp);
  };
  sensors.requestTemperatures();


  for (uint8_t i = 0; i < totalTemps; i++)
  {
    temps[i].temp = sensors.getTempCByIndex(i);
    text = text + String(temps[i].temp) + "/";
  }
  text = text + String(temps[0].value) + "/";
  text = text + op + "/";
  text = text + String(signalQuality) + "/";
  text = text + String(callOnAlert) + "/" + String(securityMode) + "/" + String(notifyScenarios) + "/" + String(remoteCount);

  return text;
  Serial.println(text);
}

void checkUpdate(String firmwareUrl)
{
  WiFiClient client;




  client.setTimeout(12000);
# 6069 "C:/Users/Amin/Documents/PlatformIO/Projects/250408-163529-esp32doit-devkit-v1 - Copy/src/esp32_wifi_gsm_littlefs_node_red_scenario_last.ino"
  httpUpdate.onProgress(update_progress);

  t_httpUpdate_return ret = httpUpdate.update(client, firmwareUrl, "", [](HTTPClient *client)
                                              {

                                              });


  StaticJsonDocument<256> doc;
  doc["event"] = "feedback";
  doc["mac"] = mac;

  String result;

  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
    doc["update"] = "failed";
    serializeJson(doc, result);
    publishReport(result.c_str());

    Serial.println("HTTP_UPDATE_OK");
    break;
  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("HTTP_UPDATE_NO_UPDATES");
    doc["update"] = "already updated!";
    serializeJson(doc, result);
    publishReport(result.c_str());

    Serial.println("HTTP_UPDATE_OK");
    break;

  case HTTP_UPDATE_OK:
    doc["update"] = "succes";
    serializeJson(doc, result);
    publishReport(result.c_str());

    Serial.println("HTTP_UPDATE_OK");
    break;
  }
}

void update_progress(int cur, int total)
{
  Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
  int p = (cur * 100) / total;
  loadingDisplay(cur, "Updating");
}