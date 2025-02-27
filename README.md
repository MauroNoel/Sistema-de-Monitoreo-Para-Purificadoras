# IoT Monitoring System for water purification plant

## 📌 Project Overview
This project is an IoT-based system designed for real-time monitoring of water levels and flow rates in raw water storage tanks at a purification plant. The system utilizes microcontrollers, sensors, and a secure web platform to collect, process, and display data, enabling efficient water management.

## 🔧 Features
- **Real-time Monitoring**: Tracks water level and flow rate continuously.
- **Wi-Fi Connectivity**: Sends data securely to a remote server using encrypted HTTP.
- **Database Integration**: Stores data in a MySQL database for historical analysis.
- **Web Visualization**: Displays real-time and historical data using a Blazor-based dashboard.
- **Alert System**: Generates notifications for critical water levels.

## 🛠️ Technologies Used
- **Microcontrollers**: Arduino, ESP8266, ESP32
- **Sensors**: Ultrasonic sensors, Flow meters
- **Communication**: Secure HTTP (encrypted), MySQL Connector
- **Backend**: MySQL database hosted on AWS
- **Frontend**: Blazor web application

## 🚀 Installation & Setup
### 1️⃣ Hardware Setup
1. Connect the ultrasonic sensors and flow meters to the ESP32.
2. Set up the microcontroller with appropriate power supply and Wi-Fi connectivity.

### 2️⃣ Software Setup
1. **Microcontroller Programming**
   - Upload the Arduino/ESP32 firmware.
   - Configure Wi-Fi credentials and server endpoints.
2. **Server & Database**
   - Deploy MySQL on AWS.
   - Create necessary tables for storing water level and flow data.
3. **Web Application**
   - Deploy the Blazor application.
   - Connect it to the database for real-time visualization.

## 📊 Data Flow
1. Sensors measure water level and flow.
2. Microcontroller processes data and sends it via encrypted HTTP.
3. Server stores the data in a MySQL database.
4. Blazor web app retrieves and visualizes data.
5. Alerts are generated if thresholds are exceeded.

## 📝 Future Improvements
- Implement mobile app support.
- Add predictive analytics for water usage.
- Improve energy efficiency with low-power modes.

## 📩 Contact
For any inquiries, feel free to reach out!


