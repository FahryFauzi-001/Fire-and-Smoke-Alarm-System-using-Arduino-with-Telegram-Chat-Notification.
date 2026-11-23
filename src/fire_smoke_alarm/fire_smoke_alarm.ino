/*=========================================================================
=============================== Header Files =============================
===========================================================================-*/
#include "WiFiS3.h"
#include "Arduino_LED_Matrix.h"


/*=========================================================================
=============================== Macros ====================================
===========================================================================-*/

/*------------------------ Pin Definitions -----------------------------------*/
#define Smoke_sensor 2
#define Flame_sensor 6
#define Green_led 3
#define Red_led 5
#define Buzzer 4




/*===============================================================================
=============================== Object Instantiation =============================
===========================================================================-*/

ArduinoLEDMatrix matrix;




/*=========================================================================
===============================Global variables=============================
===========================================================================-*/

/* --------------------- WiFi Credentials and WiFi status ----------------*/
const char *ssid = "Velzion";
const char *password = "velzion777";
int status = WL_IDLE_STATUS;


/* ---------------------- Telegram Bot Details ---------------------- */
String botToken = "8838232931:AAEc5vO7n1PRB_NTO7-TNVOZ379Ww1NQvYU";
String chatID = "8579854373";


/* ---------------------- Network Led icons ---------------------- */
const uint32_t network_connect_icon[] = { 0x308311, 0xba1b4db0, 0xdb6db6db };
const uint32_t network_disconnect_icon[] = { 0x3003a1, 0xb41badb0, 0xdb6db6db };


/* ---------------------- Network connection animation ---------------------- */
const uint32_t network_connection_anime[][4] = {
  { 0x0, 0x0, 0x0, 600 },
  { 0x0, 0x0, 0x600600, 600 },
  { 0x0, 0xc00, 0xc06c06c0, 600 },
  { 0x1, 0x80180d80, 0xd86d86d8, 600 },
  { 0x300301, 0xb01b0db0, 0xdb6db6db, 660}
};


/* ---------------------- Sensor Status Variables ---------------------- */
bool flame_status = false;
bool smoke_status = true;





/*================================================================================
=============================== User Defined Functions =============================
===========================================================================-*/

/* --------------------------- WiFi connect Function --------------------------- */
void wifi_connect(){

  if (WiFi.status() == WL_NO_MODULE) {

    Serial.println("Communication with WiFi module failed!");

    matrix.loadFrame(network_disconnect_icon);

    while (true);
  }

  Serial.print("Connecting to WiFi...");

  matrix.loadSequence(network_connection_anime);

  matrix.play(true);

  delay(6000);

  while (WiFi.begin(ssid, password) != WL_CONNECTED) {

    Serial.print(".");

    delay(1000);
  }

  matrix.loadFrame(network_connect_icon);

  Serial.println("\nConnected to WiFi!");

  Serial.print("IP Address: ");

  Serial.println(WiFi.localIP());
}


/* ------------------------ WiFi reconnect Function ----------------------------- */
void wifi_reconnect(){

  Serial.println("Wifi Reconnecting........");

  matrix.loadFrame(network_disconnect_icon);

  delay(6000);

  wifi_connect();
}


/* ------------------------ Fire & Smoke detect Function ------------------------- */
bool fire_smoke_detect(){

  flame_status = digitalRead(Flame_sensor);

  smoke_status = digitalRead(Smoke_sensor);

  if(!flame_status || !smoke_status){

      return true;
  }
  else{

     return false;
  }

}


/* ------------------------ Telegram Alert Function ----------------------------- */
void trigger_SMS(){

  if (WiFi.status() == WL_CONNECTED) {

    WiFiSSLClient client;

    String message = "FIRE ALERT\n";
    message += "Smoke or Flame detected!\n";
    message += "Evacuate immediately!";

    message.replace(" ", "%20");
    message.replace("\n", "%0A");

    String url = "/bot" + botToken +
                 "/sendMessage?chat_id=" + chatID +
                 "&text=" + message;

    Serial.println("Connecting to Telegram...");

    if (client.connect("api.telegram.org", 443)) {

      client.println("GET " + url + " HTTP/1.1");
      client.println("Host: api.telegram.org");
      client.println("Connection: close");
      client.println();

      Serial.println("Telegram alert sent!");

      while (client.connected()) {

        String line = client.readStringUntil('\n');

        if (line == "\r") {

          break;
        }
      }

      while (client.available()) {

        String line = client.readStringUntil('\n');

        Serial.println(line);
      }

      client.stop();

    }
    else {

      Serial.println("Connection to Telegram failed!");
    }

  }
  else {

    Serial.println("WiFi not connected!");
  }
}



/* -------------------------- Alarm Trigger Function ----------------------------- */
void trigger_Alarm(){

    static unsigned long prevmillis = 0;

    static bool buzzer_state = false;

    const unsigned long beep_interval = 100;

    digitalWrite(Red_led, HIGH);

    digitalWrite(Green_led, LOW);

    //Buzzer Beep Rate
    if(millis() - prevmillis >= beep_interval){

        prevmillis = millis();

        buzzer_state = !buzzer_state;

        digitalWrite(Buzzer, buzzer_state);
    }

}





/*================================================================================
=============================== Main Functions =============================
===========================================================================-*/


/* ------------------------- Setup Function --------------------------------*/
void setup() {

  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  while (!Serial);

  matrix.begin();

  wifi_connect();

  pinMode(Smoke_sensor, INPUT);

  pinMode(Flame_sensor, INPUT);

  pinMode(Green_led, OUTPUT);

  pinMode(Red_led, OUTPUT);

  pinMode(Buzzer, OUTPUT);

  digitalWrite(Green_led, HIGH);

  digitalWrite(Red_led, LOW);

  digitalWrite(Buzzer, LOW);
}


/* ---------------------------- Loop Function ---------------------------------- */
void loop() {

    if(WiFi.status() != WL_CONNECTED){

        wifi_reconnect();
    }

    if(fire_smoke_detect() == true){

       trigger_SMS();

       while(1){

          trigger_Alarm();
       }
    }
}