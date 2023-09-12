#include <ESP32Servo.h>    // me permite controlar el servomotor
#include <WiFi.h>          // me permite hacer la conexion wifi
#include <PubSubClient.h>  // me permite hacer una conexion con un servidor MQTT
#include <TaskScheduler.h> // me permitre crear tareas en paralelo

//----------------------------------------------------------------------------------------------------------------------
// definimos los pines de los motores

const int STBY = 14;
const int PWMA = 33;
const int AIN1 = 13;
const int AIN2 = 12;
const int PWMB = 25;
const int BIN1 = 27;
const int BIN2 = 26;

//----------------------------------------------------------------------------------------------------------------------
// definimos los pines para los sensores

int TRIGER1 = 5;
int ECO1    = 19;
int TRIGER2 = 21;
int ECO2    = 18;

//----------------------------------------------------------------------------------------------------------------------
// ultrasonicos

int valUltrasonico1;
int valUltrasonico2;

//----------------------------------------------------------------------------------------------------------------------
// creamos el objeto Sheduler

Scheduler scheduler;

//----------------------------------------------------------------------------------------------------------------------
// definimos loas pines del servo

const int SERVO = 2;

//----------------------------------------------------------------------------------------------------------------------
// creamos el objeto Servo

Servo miServo;

//----------------------------------------------------------------------------------------------------------------------
// direccion desde el servidor (Right, Left)

char m_direccion_S[50];
int direccion_S = 0;
//----------------------------------------------------------------------------------------------------------------------
// velocidad

char m_velocidad[50];
int velocidad = 0;

//----------------------------------------------------------------------------------------------------------------------
// direccion desde el servidor (Adelante, Atras, Stop)

char m_direccion_R[50];
int direccion_R = 0;

//----------------------------------------------------------------------------------------------------------------------
// variables para la conexion con el servidor

const char* ssid = "ANDERSON-MARIN";
const char* password = "Ander.marin_0";
const char* mqtt_server = "test.mosquitto.org";

//----------------------------------------------------------------------------------------------------------------------
// Instancia de la clase WiFiClient

WiFiClient espClient;

//----------------------------------------------------------------------------------------------------------------------
// Instancia de la clase PubSubClient

PubSubClient client(espClient);

//----------------------------------------------------------------------------------------------------------------------
// Función para conectarse a la red Wi-Fi

void setup_wifi() {

  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("direccion IP: ");
  Serial.println(WiFi.localIP());
}

//----------------------------------------------------------------------------------------------------------------------
// Funcion para reconectarse a la red Wi-Fi

void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexion MQTT...");
    // Creamos un cliente aleatorio
    String clientId = "ESP32-";
    clientId += String(random(0xffff), HEX);
    // intentamos conectarnos
    if (client.connect(clientId.c_str())) {
      Serial.println("conectado ");
      // subscribimos los topicos a los que nos vamos a conectar
      client.subscribe("velocidad_R");
      client.subscribe("direccion_R");
      client.subscribe("direccion_S");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" Espera 5 segundos ");
      // Esperamos 5 segundos y volvemos a intentar
      delay(5000);
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------
// Función callback para recibir datos del servidor

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido en el tópico: ");
  Serial.println(topic);

  // Guardamos el valor del topico como string
  String str_topico = String(topic);

  // hacemos la validacion del topico y extraemos su pyload
  if (str_topico == "direccion_S") {
    payload[length] = '\0';
    strcpy(m_direccion_S, (char*)payload);
    Serial.print("direccion recibida: ");
    Serial.println(m_direccion_S);
    direccion_S = atoi(String(m_direccion_S).c_str());
  }
  if (str_topico == "velocidad_R") {
    payload[length] = '\0';
    strcpy(m_velocidad, (char*)payload);
    Serial.print("velocidad recibida: ");
    Serial.println(m_velocidad);
    velocidad = atoi(String(m_velocidad).c_str());
  }
  if (str_topico == "direccion_R") {
    payload[length] = '\0';
    strcpy(m_direccion_R, (char*)payload);
    Serial.print("direccion recibida: ");
    Serial.println(m_direccion_R);
    direccion_R = atoi(String(m_direccion_R).c_str());
  }
}

//----------------------------------------------------------------------------------------------------------------------
// Función para calcular la distacia de los sensores

int calcularDistancia(int TRIGER, int ECO){
  digitalWrite(TRIGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGER, LOW);

  long duracion = pulseIn(ECO, HIGH);
  float distancia = duracion*0.0343/2;
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
  delay(1000);
  return distancia;
}

//----------------------------------------------------------------------------------------------------------------------
// funcion para leer los sensores

void leerSensores(){
 // Serial.println("leyendo sensores");
  //valUltrasonico1 = calcularDistancia(TRIGER1,ECO1);
  //valUltrasonico2 = calcularDistancia(TRIGER2,ECO2);
}

//----------------------------------------------------------------------------------------------------------------------
// funcion para controlar los motores

void controlMotores(){
//   Serial.println("motores");
//   int aux = velocidad;
//  if(valUltrasonico1 <= 25 || valUltrasonico2 <= 25){
//    velocidad = velocidad-10;
//   }else{
//    velocidad = aux;
//   }
}

//----------------------------------------------------------------------------------------------------------------------
// creamos las tareas y les asignamos una funcion a cada una

Task miTarea1(500, TASK_FOREVER, &leerSensores);
Task miTarea2(500, TASK_FOREVER, &controlMotores);

//----------------------------------------------------------------------------------------------------------------------
// configuracion

void setup() {
  // configuramos la conexion al servidor
  Serial.begin(115200);
  setup_wifi();  // esta funcion es la de conectarnos a la red wifi
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // configuramos el servo
  miServo.setPeriodHertz(50);
  miServo.attach(SERVO, 500, 2400);
  miServo.write(90);

  // definimos esos pines como salidas
  pinMode(STBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  // Agregamos las tareas al Scheduler (planificador de tareas)
  scheduler.addTask(miTarea1);
  scheduler.addTask(miTarea2);

  // Iniciamos las tareas
  miTarea1.enable();
  miTarea2.enable();
}

//----------------------------------------------------------------------------------------------------------------------
// Funcion para controlar el PWM

void pwm(int velocidad) {
  for (int dutyCycle = 0; dutyCycle <= velocidad; dutyCycle++) {
    analogWrite(PWMA, dutyCycle);
    analogWrite(PWMB, dutyCycle);
  }
}

//----------------------------------------------------------------------------------------------------------------------
// funcion para detener el motor

void motorStop() {
  digitalWrite(STBY, 0);
  delay(100);
}

//----------------------------------------------------------------------------------------------------------------------
// funcion para ir adelante

void motorAdelante() {
  digitalWrite(STBY, 1);
  // MOTOR 1
  digitalWrite(AIN1, 0);
  digitalWrite(AIN2, 1);
  // MOTOR 2
  digitalWrite(BIN1, 0);
  digitalWrite(BIN2, 1);
  delay(100);
}

//----------------------------------------------------------------------------------------------------------------------
// funcion para ir atras

void motorAtras() {
  digitalWrite(STBY, 1);
  // MOTOR 1
  digitalWrite(AIN1, 1);
  digitalWrite(AIN2, 0);
  // MOTOR 2
  digitalWrite(BIN1, 1);
  digitalWrite(BIN2, 0);
  delay(100);
}

//----------------------------------------------------------------------------------------------------------------------
// ciclo principal

void loop() {

  //----------------------------------------------------------------------------------------------------------------------
  // Ejecutamos el planificador de tareas
  scheduler.execute();

  //----------------------------------------------------------------------------------------------------------------------
  // Verificar si estamos conectados al servidor MQTT

  if (!client.connected()) {
    reconnect();
  }

  //----------------------------------------------------------------------------------------------------------------------
  // Escuchando los de datos de Node-RED

  client.loop();

  //----------------------------------------------------------------------------------------------------------------------
  //                                                 FUNCIONALIDAD                                                      //
  //----------------------------------------------------------------------------------------------------------------------

  // ir adelante si recibimos 1 en este topico
  if (direccion_R == 1) {
    Serial.println("Adelante");
    pwm(velocidad);
    motorAdelante();
  }

  //----------------------------------------------------------------------------------------------------------------------
  // detener el vehiculo si recibimos 0 en este topico

  if (direccion_R == 0) {
    Serial.println("Stop");
    pwm(velocidad);
    motorStop();
  }

  //----------------------------------------------------------------------------------------------------------------------
  // ir atras si recibimos 2 en este topico

  if (direccion_R == 2) {
    Serial.println("Atras");
    pwm(velocidad);
    motorAtras();
  }

  //----------------------------------------------------------------------------------------------------------------------
  // girar a la derecha si recibimos 1 en este topico

  if (direccion_S == 1) {
    Serial.println("derecha");
    miServo.write(75);
    delay(900);
    miServo.write(95);
  }

  //----------------------------------------------------------------------------------------------------------------------
  // girar a la izquierda si recibimos 2 en este topico

  if (direccion_S == 2) {
    Serial.println("izquierda");
    miServo.write(115);
    delay(900);
    miServo.write(95);
  }

  //----------------------------------------------------------------------------------------------------------------------

  direccion_S = 0;
}
