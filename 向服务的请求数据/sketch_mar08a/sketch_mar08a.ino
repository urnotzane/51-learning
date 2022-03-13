

#include <ESP8266WiFi.h>        // 本程序使用 ESP8266WiFi库
#include <ESP8266HTTPClient.h>

const char* ssid     = "coder-visitor";      // 连接WiFi名（此处使用taichi-maker为示例）
// 请将您需要连接的WiFi名填入引号中
const char* password = "";

const char* host = "192.168.50.125"; // 网络服务器地址
const int httpPort = 80;            // http端口80
#define SERVER_IP "192.168.50.125"

void setup() {
  Serial.begin(9600);   // 启动串口通讯


  WiFi.begin(ssid, password);                  // 启动网络连接
  Serial.print("Connecting to ");              // 串口监视器输出网络连接信息
  Serial.print(ssid); Serial.println(" ...");  // 告知用户NodeMCU正在尝试WiFi连接

  int i = 0;                                   // 这一段程序语句用于检查WiFi是否连接成功
  while (WiFi.status() != WL_CONNECTED) {      // WiFi.status()函数的返回值是由NodeMCU的WiFi连接状态所决定的。
    delay(1000);                               // 如果WiFi连接成功则返回值为WL_CONNECTED
    Serial.print(i++); Serial.print(' ');      // 此处通过While循环让NodeMCU每隔一秒钟检查一次WiFi.status()函数返回值
  }                                       // 一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。这也是
  // 此处while循环判断是否跳出循环的条件。
  // WiFi连接成功后将通过串口监视器输出连接成功信息
  Serial.println('\n');                     // WiFi连接成功后
  Serial.print("Connected to ");            // NodeMCU将通过串口监视器输出。
  Serial.println(WiFi.SSID());              // 连接的WiFI名称
  Serial.print("IP address:\t");            // 以及
  Serial.println(WiFi.localIP());           // NodeMCU的IP地址

}

void loop() {
  // 发送HTTP请求
  httpRequest();

  delay(3000);
}



// 向服务器发送HTTP请求，请求信息中包含json信息
void httpRequest() {
  // 建立WiFi客户端对象，对象名称client
  WiFiClient client;

  // 重点1： 建立JSON，此JSON包含需要发送的信息
  String payloadJson = "{\"info\": {\"name\": \"taichimaker\",\"url\": \"www.taichi-maker.com\",\"email\": \"taichimaker@163.com\"},\"digital_pin\": {\"d1\": \"";
  payloadJson += String(digitalRead(D1));
  payloadJson += "\",\"d2\": \"";
  payloadJson += String(digitalRead(D2));
  payloadJson += "\",\"d3\": \"";
  payloadJson += String(digitalRead(D3));
  payloadJson += "\"},\"analog_pin\": {\"a0\": \"";
  payloadJson += String(analogRead(A0));
  payloadJson += "\"}}";

  // 通过串口输出连接服务器名称以便查阅连接服务器的网址
  Serial.print("Connecting to ");
  Serial.print(host);

  if (client.connect(host, httpPort)) {
    Serial.println(" Success!");            // 连接成功后串口输出“Success”信息

    // 发送请求
    client.println("POST /test HTTP/1.1");
    client.println("Content-type: application/json");
    client.print("Host: ");
    client.println(host);
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(payloadJson.length());
    client.println();
    client.println(payloadJson);

    Serial.println("Web Server Response:"); // 通过串口监视输出服务器响应信息
    while (client.connected() || client.available()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    }
  } else {   // 如果连接不成功则通过串口输出“连接失败”信息
    Serial.println(" failed!");
  }

  client.stop();                      // 断开与服务器的连接
  Serial.print("Disconnected from "); // 并且通过串口输出断开连接信息
  Serial.println(host);
}
