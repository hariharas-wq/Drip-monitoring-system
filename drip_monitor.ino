

#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
                          
#define FIREBASE_HOST "drip-monitor-default-rtdb.firebaseio.com"                     //Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "4y6pKoehhQX3uJcb5wKioIeWQIAXHGHOIb5yHJer" //Firebase Database Secret goes here

#define WIFI_SSID "Cena"                                               //WiFi SSID for which yout NodeMCU connects
#define WIFI_PASSWORD "Cena1130"                                      //Password of your wifi network 
 
const int trigPin = 12;
const int echoPin = 14;

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;
// Declare the Firebase Data object in the global scope
FirebaseData firebaseData;

// Declare global variable to store value
int val=0;


void setup() {

  Serial.begin(115200);                                                      // Select the same baud rate if you want to see the datas on Serial Monitor
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.println("Serial communication started\n\n");  
           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);


  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase

  Firebase.reconnectWiFi(true);
  delay(1000);
}

void loop() {

 // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
   int voltage = analogRead(A0);
  
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
// Firebase Error Handling And Reading Data From Specified Path ****************
val=distanceCm;
if (Firebase.setInt(firebaseData, "/ultrasonic", val)) {    // On successful Write operation, function returns 1  
               Serial.println("Ultrasonic Value Uploaded Successfully");
               Serial.print("Val = ");
               Serial.println(val);
               Serial.println("\n");
          
        

     }

else {        
    Serial.println(firebaseData.errorReason());
  }
  if (Firebase.setInt(firebaseData, "/ldr", voltage)) {    // On successful Write operation, function returns 1  
               Serial.println(" Ldr Value Uploaded Successfully");
               Serial.print("voltage = ");
               Serial.println(voltage);
               Serial.println("\n");
          
        

     }

else {        
    Serial.println(firebaseData.errorReason());
  }

 }


/* NOTE:
 *  To read value, command is ===> Firebase.getInt(firebaseData, "path"); variable = firebaseData.intData();
 *  
 *  Example                   ===>  Firebase.setInt(firebaseData, "/data", val); val = firebaseData.intData();
 */
 
 
 
 
 
 
 
 
