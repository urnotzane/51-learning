/*
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : 3_2_3_Pin_State_Display
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : CYNO朔
日期/Date（YYYYMMDD）     : 20191107
程序目的/Purpose          : 使用NodeMCU建立基本服务器。该页面将会自动刷新并且显示NodeMCU
                           的D3引脚状态。NodeMCU开发板上的FLASH按键可以控制D3引脚的电平。
                           没有按下该按键时D3引脚将会保持高电平状态。当按下该按键后，
                           D3引脚会变为低电平。
-----------------------------------------------------------------------
修订历史/Revision History
日期/Date    作者/Author      参考号/Ref    修订说明/Revision Description
***********************************************************************/

#include <ESP8266WiFi.h>      // 本程序使用 ESP8266WiFi库
#include <ESP8266WebServer.h> //  ESP8266WebServer库

#define buttonPin D3 // 按钮引脚D3

const char *ssid = "coder-visitor"; // 连接WiFi名（此处使用taichi-maker为示例）
                                    // 请将您需要连接的WiFi名填入引号中
const char *password = "";

ESP8266WebServer esp8266_server(80); // 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）

bool pinState; // 存储引脚状态用变量

void setup()
{
  Serial.begin(9600); // 启动串口通讯

  pinMode(buttonPin, INPUT_PULLUP); // 将按键引脚设置为输入上拉模式

  WiFi.begin(ssid, password);     // 启动网络连接
  Serial.print("Connecting to "); // 串口监视器输出网络连接信息
  Serial.print(ssid);
  Serial.println(" ..."); // 告知用户NodeMCU正在尝试WiFi连接

  int i = 0; // 这一段程序语句用于检查WiFi是否连接成功
  while (WiFi.status() != WL_CONNECTED)
  {              // WiFi.status()函数的返回值是由NodeMCU的WiFi连接状态所决定的。
    delay(1000); // 如果WiFi连接成功则返回值为WL_CONNECTED
    Serial.print(i++);
    Serial.print(' '); // 此处通过While循环让NodeMCU每隔一秒钟检查一次WiFi.status()函数返回值
  }                    // 一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。这也是
                       // 此处while循环判断是否跳出循环的条件。
  // WiFi连接成功后将通过串口监视器输出连接成功信息
  Serial.println('\n');           // WiFi连接成功后
  Serial.print("Connected to ");  // NodeMCU将通过串口监视器输出。
  Serial.println(WiFi.SSID());    // 连接的WiFI名称
  Serial.print("IP address:\t");  // 以及
  Serial.println(WiFi.localIP()); // NodeMCU的IP地址

  esp8266_server.begin();                    // 启动网站服务
  esp8266_server.on("/", handleRoot);        // 设置服务器根目录即'/'的函数'handleRoot'
  esp8266_server.onNotFound(handleNotFound); // 设置处理404情况的函数'handleNotFound'

  Serial.println("HTTP esp8266_server started"); //  告知用户ESP8266网络服务功能已经启动
}

void loop()
{
  esp8266_server.handleClient();     // 处理http服务器访问
  pinState = digitalRead(buttonPin); // 获取引脚状态
}

/* 以下函数处理网站首页的访问请求。此函数为本示例程序重点1
详细讲解请参见太极创客网站《零基础入门学用物联网》
第3章-第2节“通过网络服务将开发板引脚状态显示在网页中”的说明讲解。*/
void handleRoot()
{
  String displayPinState; // 存储按键状态的字符串变量

  if (pinState == HIGH)
  {                                         // 当按键引脚D3为高电平
    displayPinState = "Button State: HIGH"; // 字符串赋值高电平信息
  }
  else
  {                                        // 当按键引脚D3为低电平
    displayPinState = "Button State: LOW"; // 字符串赋值低电平信息
  }

  esp8266_server.send(200, "text/plain", displayPinState);
  // 向浏览器发送按键状态信息
}

// 设置处理404情况的函数'handleNotFound'
void handleNotFound()
{                                                           // 当浏览器请求的网络资源无法在服务器找到时，
  esp8266_server.send(404, "text/plain", "404: Not found"); // NodeMCU将调用此函数。
}
