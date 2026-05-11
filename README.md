🚀 ESP32 Smart Captive Portal with Multi-Webhook Integration
A professional-grade ESP32 project demonstrating the power of Captive Portals combined with remote data logging via Discord and Telegram webhooks. This project showcases how embedded systems can interact with modern API services in real-time.
🌟 Features
• 🌐 Captive Portal Technology: Automatically redirects users to a custom authentication/setup page upon connection.
• 📡 Dual-Mode Operation: Runs as an Access Point (AP) while maintaining a Station (STA) connection to the internet.
• 🤖 Discord Integration: Sends captured data instantly to your Discord server via Webhooks.
• 📱 Telegram Integration: Real-time notifications delivered straight to your Telegram bot.
• ⚡ Asynchronous Handling: Efficient DNS and Web Server management for a smooth user experience.
🛠️ Hardware Requirements
• ESP32 Development Board (NodeMCU, DevKit, etc.)
• USB Cable for programming.
• Stable Power Source (ESP32 requires consistent current in AP+STA mode).
⚙️ How It Works
1.	WiFi AP: The ESP32 creates a WiFi network named System_Recovery_Update.
2.	DNS Hijacking: The DNS server intercepts all requests and redirects them to the local hosted loginPage.
3.	Data Extraction: When a user submits the form, the data is captured via a POST request.
4.	Remote Logging: The ESP32 uses SSL/TLS (WiFiClientSecure) to securely push the data to your configured Discord and Telegram channels.
📜 License
read it from LICENSE.
⚠️ Disclaimer
This project is for educational and research purposes only. The author is not responsible for any misuse or damage caused by this software. Always ensure you have explicit permission before testing on any network or device.
Regarding the WiFi Name:
In the code I gave you, the name is set here:
WiFi.softAP("System_Recovery_Update");
You can change the text inside the quotes to whatever "Original Name" you want the network to have.
