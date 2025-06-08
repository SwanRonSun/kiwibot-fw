#include <Arduino.h>
#include "cam_mux.h"
#include "state_machine.h"
#include "drv8833_hal.h"
#include "orbit_controller.h"
#include "failsafe.h"

// Performance monitoring
namespace {
  uint32_t loopCount = 0;
  uint32_t lastPerfReport = 0;
  uint32_t lastStatusReport = 0;
}

// Memory monitoring (AVR specific) - Fixed extern declarations
extern int __heap_start;
extern int *__brkval; 
int freeRam() {
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== KiwiBot Firmware v1.0 ===");
  Serial.print("Free RAM: ");
  Serial.print(freeRam());
  Serial.println(" bytes");
  
  // Initialize all subsystems
  DrvHal::init();
  CamMux::init();          // Left cam on RX0 @115200, right on D2 @250000
  StateMachine::init();
  
  // Initialize PID controller with conservative gains
  // These will need tuning once hardware is available
  OrbitController::init(0.5, 0.1, 0.05);  // Kp, Ki, Kd
  
  Serial.println("Setup complete!\n");
  Serial.println("Commands:");
  Serial.println("  'k' - Kill switch (failsafe)");
  Serial.println("  's' - Print camera statistics");
  Serial.println("  'h' - Print this help");
  Serial.println("");
  
  lastPerfReport = millis();
  lastStatusReport = millis();
}

void loop() {
  // Core control loop
  CamMux::poll();
  StateMachine::run();
  
  // Handle serial commands
  if (Serial.available()) {
    char cmd = Serial.read();
    switch(cmd) {
      case 's':
      case 'S':
        CamMux::printStats();
        break;
      case 'h':
      case 'H':
        Serial.println("\nCommands: k=kill, s=stats, h=help");
        break;
      // 'k' is handled by Failsafe::poll() inside StateMachine::run()
    }
  }
  
  // Performance monitoring (every second)
  loopCount++;
  if (millis() - lastPerfReport > 1000) {
    Serial.print("Performance: ");
    Serial.print(loopCount);
    Serial.print(" Hz, RAM: ");
    Serial.print(freeRam());
    Serial.println(" bytes");
    
    loopCount = 0;
    lastPerfReport = millis();
  }
  
  // Status report (every 2 seconds)
  if (millis() - lastStatusReport > 2000) {
    // Current state
    Serial.print("State: ");
    Serial.print(StateMachine::currentName());
    
    // Vision data
    Serial.print(" | Vision L(");
    Serial.print(CamMux::leftAngle());
    Serial.print("°,");
    Serial.print(CamMux::leftRange());
    Serial.print("mm) R(");
    Serial.print(CamMux::rightAngle());
    Serial.print("°,");
    Serial.print(CamMux::rightRange());
    Serial.print("mm) → Sel(");
    Serial.print(CamMux::selectedAngle());
    Serial.print("°,");
    Serial.print(CamMux::selectedRange());
    Serial.println("mm)");
    
    lastStatusReport = millis();
  }
}