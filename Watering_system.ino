int resval = 0;
int buzzer = 5;
int HumidSensor = 4;
int pompa = 12;
int wilgoc;



/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com  
*********/

// Import required libraries
#ifdef ESP32
  #include <WiFi.h>
  #include <ESPAsyncWebServer.h>
#else
  #include <Arduino.h>
  #include <ESP8266WiFi.h>
  #include <Hash.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
#endif

// Variables to store temperature values
String temperatureF = "";
String temperatureC = "";

// Timer variables
unsigned long lastTime = 0;  
unsigned long timerDelay = 1000;

// Replace with your network credentials
//const char* ssid = "x";
//const char* password = "x";
const char* ssid = "iPhone (Kuba)";
const char* password = "hahaha123";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .ds-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>Stacja automatycznego podlewania</h2>
  <p>
    <i class="fas fa-water" style="color:#059e8a;"></i> 
    <span class="ds-labels">Poziom wody w zbiorniku : </span> 
    <span id="temperaturec">%TEMPERATUREC%</span> 
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperaturec").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperaturec", true);
  xhttp.send();
}, 10000) ;
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperaturef").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperaturef", true);
  xhttp.send();
}, 10000) ;
</script>
</html>)rawliteral";

// Replaces placeholder with DS18B20 values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATUREC"){
    return temperatureC;
  }
  return String();
}

void setup(){
  
  Serial.begin(9600);
  Serial.println();

  pinMode(buzzer, OUTPUT);
  pinMode(HumidSensor, INPUT);
  pinMode(pompa, OUTPUT);

    resval = analogRead(A0);
    
    if(resval < 200){
      temperatureC = "pusto";
    }
    else if(resval >= 200 && resval < 400){
      temperatureC = "stan niski";
    }
    else if (resval >= 400 && resval < 450){
      temperatureC = "stan sredni";
    }
  else{
  temperatureC = "stan wysoki";
  }
  



  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperaturec", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", temperatureC.c_str());
  });
  server.on("/temperaturef", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", temperatureF.c_str());
  });
  // Start server
  server.begin();
}
 
void loop(){
  if ((millis() - lastTime) > timerDelay) {
                          wilgoc = digitalRead(HumidSensor);
    resval = analogRead(A0);
    if(resval < 200){
      temperatureC = "pusto";
    }
    else if(resval >= 200 && resval < 400){
      temperatureC = "stan niski";
    }
    else if (resval >= 400 && resval < 450){
      temperatureC = "stan sredni";
    }
  else{
  temperatureC = "stan wysoki";
  }

                    if(resval >= 200){
                        if(wilgoc == HIGH){
                        digitalWrite(pompa, HIGH);
                        }
                        else{
                          digitalWrite(pompa, LOW);
                        }
                      }
                        else{
                          digitalWrite(pompa, LOW);
                        }
                      
                      if(resval < 200){
                        digitalWrite(buzzer, HIGH);
                      }
                      else{
                        digitalWrite(buzzer, LOW);
                      }
                      
    lastTime = millis();
  }  
}
