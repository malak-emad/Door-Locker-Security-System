# Dual Microcontroller-Based Door Locker Security System Using Password Authentication

## **Objective**
The objective of this project is to develop a secure and efficient smart door control system using two microcontrollers, **HMI_ECU** and **Control_ECU**, which communicate via **UART**. The system implements **password authentication**, stores data in **external EEPROM**, and integrates additional components such as a **PIR sensor, H-bridge motor control, and a buzzer** for enhanced functionality.

## **Project Overview**
This smart door control system is designed with two microcontrollers, one acting as the **Human-Machine Interface (HMI_ECU)** and the other as the **Control Unit (Control_ECU)**. Users interact with the system using an **LCD and keypad** to enter passwords, which are verified and stored using **external EEPROM**. The door is controlled by an **H-bridge circuit** connected to a **motor**, and the system includes a **PIR sensor for motion detection, a buzzer for alarms,** and password-protected access to system options like door unlocking and password changing.

## **Features**
1. **Password Protection** - Users can set and verify a password stored in external EEPROM.
2. **LCD and Keypad Interface** - Allows easy interaction for entering and managing passwords.
3. **UART Communication** - HMI_ECU sends and receives data to and from Control_ECU via UART.
4. **EEPROM Storage** - Passwords and system data are stored securely in an external EEPROM.
5. **Motorized Door Control** - The door is unlocked/locked using a motor driven by an H-bridge.
6. **Buzzer Alert** - The buzzer is activated for failed password attempts and system alerts.
7. **PIR Motion Sensor** - Detects motion to trigger door operations.
8. **Password Change Option** - Users can change the password after verification.
9. **Security Lock** - The system locks for one minute if the password is entered incorrectly three times consecutively.

## **Hardware Components & Connections**
### **HMI_ECU Connections:**
- **LCD (8-bit mode)**
  - RS pin → PC0
  - E (Enable) pin → PC1
  - Data Pins (D0-D7) → PA0 to PA7
- **Keypad (4x4)**
  - Rows → PB0, PB1, PB2, PB3
  - Columns → PB4, PB5, PB6, PB7
- **UART Communication**
  - TXD (Transmit Data) → RXD of Control_ECU
  - RXD (Receive Data) → TXD of Control_ECU

### **Control_ECU Connections:**
- **External EEPROM (I2C Communication)**
  - SCL (Serial Clock Line) → PC0
  - SDA (Serial Data Line) → PC1
- **Buzzer**
  - Connected to PC7
- **H-bridge Motor Driver**
  - Input 1 → PD6
  - Input 2 → PD7
  - Enable1 → PB3/OC0
- **Motor (for Door Control)**
  - Connected to the H-bridge motor driver
- **PIR Motion Sensor**
  - Connected to PC2

## **Installation & Setup**
1. **Clone the Repository:**
   ```sh
   git clone https://github.com/malak-emad/Door-Locker-Security-System.git
   ```
2. **Compile & Upload Code:**
   - Use **AVR Studio / Atmel Studio** or another suitable IDE to compile and upload the code to the microcontrollers.
3. **Hardware Setup:**
   - Connect all components as per the wiring diagram.
4. **Run the System:**
   - Power on the system and interact using the keypad and LCD.

## **Usage Instructions**
1. **Enter Password:** Input the predefined password using the keypad.
2. **Unlock Door:** Upon correct password entry, the motor unlocks the door.
3. **Incorrect Password Handling:**
   - If entered incorrectly 3 times, the system locks for 1 minute.
4. **Change Password:** Users can update their password after successful authentication.
5. **Buzzer Alerts:** The buzzer is triggered for incorrect passwords and security alerts.
6. **Motion Detection:** The PIR sensor detects movement and can trigger the door control system.


## **License**
This project is supervised by **Edges For Training**

