#include <BleMouse.h>
#include <Wire.h>

BleMouse bleMouse("ValoGun", "DIY", 100);

const int TRIGGER_PIN = 4;
const int MPU_ADDR = 0x68; 
bool wasPressed = false;

// --- TUNING VARIABLES ---
// 1. Lower sensitivity for finer control (was 0.005, now 0.0015)
float SENSITIVITY = 0.0015; 

// 2. Deadzone: Ignores micro-shakes. 
// If your crosshair still drifts when holding still, increase this to 300 or 400.
int DEADZONE = 200;         

// Calibration offsets
long offsetX = 0, offsetY = 0, offsetZ = 0;

void setup() {
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  
  Wire.begin(21, 22); 
  
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);  
  Wire.write(0);     
  Wire.endTransmission(true);
  
  Serial.println("Calibrating Gyro... KEEP IT PERFECTLY STILL!");
  
  // Take 200 readings while sitting still to find the natural drift
  for (int i = 0; i < 200; i++) {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 6, true);
    
    int16_t gX = Wire.read() << 8 | Wire.read();
    int16_t gY = Wire.read() << 8 | Wire.read();
    int16_t gZ = Wire.read() << 8 | Wire.read();
    
    offsetX += gX;
    offsetY += gY;
    offsetZ += gZ;
    delay(10); // 2 seconds total calibration time
  }
  
  // Calculate the average offset
  offsetX /= 200;
  offsetY /= 200;
  offsetZ /= 200;
  
  bleMouse.begin();
  Serial.println("Bluetooth Mouse ready! Connect to 'ValoGun'.");
}

void loop() {
  if (bleMouse.isConnected()) {
    
    // --- 1. HANDLE TRIGGER ---
    bool isPressed = (digitalRead(TRIGGER_PIN) == LOW); 
    if (isPressed && !wasPressed) {
      bleMouse.press(MOUSE_LEFT);
      wasPressed = true;
    } else if (!isPressed && wasPressed) {
      bleMouse.release(MOUSE_LEFT);
      wasPressed = false;
    }

    // --- 2. HANDLE GYROSCOPE ---
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x43); 
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 6, true); 

    int16_t rawX = Wire.read() << 8 | Wire.read();
    int16_t rawY = Wire.read() << 8 | Wire.read();
    int16_t rawZ = Wire.read() << 8 | Wire.read();

    // Subtract the calibration offset to stop resting drift
    long gyroX = rawX - offsetX;
    long gyroY = rawY - offsetY;
    long gyroZ = rawZ - offsetZ;

    // Apply Deadzone to filter out hand shakes
    if (abs(gyroX) < DEADZONE) gyroX = 0;
    if (abs(gyroY) < DEADZONE) gyroY = 0;
    if (abs(gyroZ) < DEADZONE) gyroZ = 0;

    // Map to mouse movement
    int moveX = -gyroZ * SENSITIVITY; // Yaw (Left/Right)
    int moveY = gyroY * SENSITIVITY;  // Pitch (Up/Down)

    // Only send Bluetooth data if there is actual movement
    if (moveX != 0 || moveY != 0) {
      bleMouse.move(moveX, moveY);
    }
  }
  delay(10); 
}