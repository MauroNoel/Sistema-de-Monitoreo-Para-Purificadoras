#include "Credentials.h"
#include <Arduino.h>
#include <ESP32_MySQL.h>
#include <Adafruit_Sensor.h>

/* Configuracion Libreria ESP32_MySql*/
#define ESP32_MYSQL_DEBUG_PORT Serial
// Debug Level from 0 to 4
#define _ESP32_MYSQL_LOGLEVEL_ 1
#define USING_HOST_NAME true
#if USING_HOST_NAME
  // Optional using hostname
  char server[] = "XXXXXXXXXXXXXXXXXX"; // change to your server's hostname/URL
#else 
  IPAddress server(128, 100, 001, 010);
#endif
uint16_t server_port = 3306;    // MySQL server port (default : 3306)
char default_database[] = "tt";           //default DB
char default_table[]    = "mediciones";          //default table
//LevelSensor Pins
#define TRIG_PIN 22 // GPIO23 connected to Ultrasonic Sensor's TRIG pin
#define ECHO_PIN 23 // GPIO22 connected to Ultrasonic Sensor's ECHO pin
float duration_us, distance_cm;
//FlowSensor Pins
#define SENSOR  21

long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay =1000;

// Init conn         
ESP32_MySQL_Connection conn((Client *)&client);
ESP32_MySQL_Query *query_mem;
 

void Level(){
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(ECHO_PIN, HIGH);

  // calculate the distance
  distance_cm = 0.017 * duration_us;
  Serial.print(distance_cm);
if (distance_cm >= 31) {
    const float radio = 93.79; // cm
    float volumen = M_PI * pow(radio, 2) * (206 - distance_cm); // 22 es la distancia del sensor a los 4700 lts
    distance_cm = (volumen / 1000);
  //print the value to Serial Monitor
  Serial.print("distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  delay(500);
    }  
}

//Counter Flow Sensor
void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}
//FlowSensor Function  
void Flow()
{
  float calibrationFactor = 4.5;
  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    
    pulse1Sec = pulseCount;
    pulseCount = 0;
    //El flujo esta determinado por la lectura anterior mas el ciclo, al no ser exacto se resta la lectura anterior con el flujo, y se usa una factor de calibracion para ajustar la salida(4.5)
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();
    //Convertir a Litros por minuto
    flowMilliLitres = (flowRate / 60) * 1000;
    // Suma los mililitros pasados ​​en este segundo al total acumulado
    totalMilliLitres += flowMilliLitres;  
    // PRINTS
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");       // Print tab space
    // Imprime el total acumulado de litros fluídos desde el inicio.
    Serial.print("Output Liquid Quantity: ");
    Serial.print(totalMilliLitres);
    Serial.print("mL / ");
    Serial.print(totalMilliLitres / 1000);
    Serial.println("L");
  }
}


void runInsert(const char* sensor, float value)
{
  // Initiate the query class instance
  ESP32_MySQL_Query query_mem = ESP32_MySQL_Query(&conn);

  if (conn.connected())
  {
    // Query
    String INSERT_SQL = "INSERT INTO mediciones (valor, sensor) VALUES (" + 
                    String(value, 2) + ", '" + sensor + "')";
                    
    ESP32_MYSQL_DISPLAY(INSERT_SQL);
    
    // Execute the query
    if ( !query_mem.execute(INSERT_SQL.c_str()) )
    {
      ESP32_MYSQL_DISPLAY("Insert error");
    }
    else
    {
      ESP32_MYSQL_DISPLAY("Data Inserted.");
    }
  }
  else
  {
    ESP32_MYSQL_DISPLAY("Disconnected from Server. Can't insert.");
  }
}


void setup()
{
  Serial.begin(115200);
  
  //Configure LevelSensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);


//Configure FlowSensor
  pinMode(SENSOR, INPUT_PULLUP);

  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;
  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
  while (!Serial && millis() < 5000); // wait for serial port to connect
  ESP32_MYSQL_DISPLAY1("\nStarting Basic_Insert_ESP on", ARDUINO_BOARD);

  // Begin WiFi section
  ESP32_MYSQL_DISPLAY1("Connecting to", ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    ESP32_MYSQL_DISPLAY0(".");
  }
  // print out info about the connection:
  ESP32_MYSQL_DISPLAY1("Connected to network. My IP address is:", WiFi.localIP());
  ESP32_MYSQL_DISPLAY3("Connecting to SQL Server @", server, ", Port =", server_port);
  ESP32_MYSQL_DISPLAY5("User =", user, ", PW =", password, ", DB =", default_database);
}


void loop()
{
  Flow();
  Level();
  if ((millis() - lastTime) > timerDelay) {
    lastTime = millis();
  }

  ESP32_MYSQL_DISPLAY("Connecting...");
  
  //if (conn.connect(server, server_port, user, password))
  if (conn.connectNonBlocking(server, server_port, user, password, default_database) != RESULT_FAIL)
  {
    delay(500);
    
    // Insertar datos del sensor de nivel
    runInsert("NIVEL", distance_cm);
    
    // Insertar datos del sensor de flujo (tasa de flujo)
    runInsert("FLUJO_RATE", flowRate);
    
    // Insertar el total de mililitros
    runInsert("FLUJO_TOTAL", totalMilliLitres);
    
    conn.close(); // close the connection
  } 
  else 
  {
    ESP32_MYSQL_DISPLAY("\nConnect failed. Trying again on next iteration.");
  }
  
  ESP32_MYSQL_DISPLAY("\nSleeping...");
  ESP32_MYSQL_DISPLAY("================================================");
  delay(10000); // every 30 seconds
}