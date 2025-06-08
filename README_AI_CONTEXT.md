# KiwiBot AI Context Document

## 🤖 Project Overview
**KiwiBot** is a fully autonomous antweight (454g) combat robot with a 3-wheel holonomic (Kiwi) drive and hydraulic spike weapon. It uses dual ESP32-S3 cameras for opponent tracking and operates without human control during matches.

## 👤 Developer Context
- **Developer**: Working via Windows CMD and Git
- **Workflow**: Prefers full file outputs, regular git commits, command-line operations
- **Hardware Status**: NO HARDWARE YET - all development is mock/simulation
- **Development Style**: Incremental, test-driven, safety-focused

## 🎯 Robot Mission & Strategy
1. **Autonomous Operation**: No human control during matches
2. **Combat Strategy**: 
   - Detect opponent via dual cameras
   - Orbit to optimal position
   - Strike with hydraulic spike
   - Repeat until match ends
3. **Safety**: Multiple failsafes, contained weapon, SPARC/NERC compliant

## 📦 Hardware (On Order)
### Core Components
- **MCU**: Arduino Nano (ATmega328P) - 2KB RAM, 30KB Flash
- **Drive**: 3x N20 motors with custom omni wheels
- **Motor Driver**: DRV8833 dual H-bridge
- **Weapon**: Short-stroke solenoid (<20mm travel, <10J)
- **Vision**: 2x Seeed XIAO ESP32-S3 Sense (sending angle/range packets)
- **Power**: 2S LiPo 450mAh with 5A fuse
- **Radio**: Dead-man failsafe only (no active control)

### Pin Assignments (Planned)
- **Motors**: Pins 3,5 (Motor 0), 6,9 (Motor 1), 10,11 (Motor 2)
- **Strike Solenoid**: Pin 7
- **Right Camera Serial**: Pin 2 (SoftwareSerial RX)
- **Left Camera**: Hardware Serial (RX0)
- **Failsafe Input**: TBD (radio channel + lid switch)

## 💻 Software Architecture

### Current State (Commit e837d2a)
```
kiwibot-fw/
├── src/
│   └── main.cpp (72.1% RAM, needs interactive features)
├── lib/
│   ├── cam_mux/        ✓ Dual camera packet parser
│   ├── state_machine/  ✓ IDLE→ACQUIRE→ORBIT→STRIKE FSM
│   ├── drv8833_hal/    ✓ Motor control with orbit logic
│   ├── orbit_controller/ ✓ PID controller (Kp=0.5, Ki=0.1, Kd=0.05)
│   ├── strike_hal/     ✓ Weapon control with cooldown
│   ├── failsafe/       ✓ Kill switch ('k' command)
│   └── kiwi_ik/        ✓ Holonomic kinematics
├── test/               ⚠️ Tests broken (duplicate files issue)
└── tools/
    ├── fake_cam.py     ✓ Camera simulator
    └── bundle_project.py ✓ Code bundler
```

### Key Features Implemented
1. **State Machine**: Autonomous target tracking and engagement
2. **Dual Vision**: Left/right camera fusion, selects nearest target
3. **Mock Hardware**: All drivers have print statements for testing
4. **Safety Systems**: Failsafe, timeouts, weapon cooldown
5. **PID Control**: Orbit controller ready but needs tuning

### Memory Status
- **RAM**: 1477/2048 bytes (72.1%) - Getting tight!
- **Flash**: 11504/30720 bytes (37.4%) - Plenty of room

## 🔧 Current Issues

### 1. Tests Won't Compile
- Multiple `main()` definitions
- Old test files conflicting with new structure
- Need to properly isolate test cases

### 2. Missing Interactive Features
The enhanced `main.cpp` with serial commands exists but isn't saved:
- 't' - Inject test targets
- 'r' - Reset state machine  
- 'd' - Dump system state
- 'v' - Toggle verbose mode

### 3. No Hardware Testing
Everything is mocked - will need careful bring-up when parts arrive

## 📝 Next Steps (Priority Order)

### Immediate (No Hardware Needed)
1. **Save Interactive main.cpp** - The code exists in artifacts but needs to be saved
2. **Fix Test Structure** - Clean up test organization  
3. **Add EEPROM Logging** - Store match events for post-analysis

### When Hardware Arrives
1. **Test One System at a Time** - Motors → Vision → Weapon
2. **Verify Failsafes** - Must stop in <300ms
3. **Tune PID** - Start conservative, increase gradually
4. **Validate Power** - Check current draw, brownout protection

### Future Enhancements
1. **Target Prediction** - Lead moving opponents
2. **Multiple Strategies** - Aggressive/defensive modes
3. **Match Recording** - EEPROM event log
4. **Simulation Mode** - Virtual opponent for testing

## 🛠️ Development Commands

### Build & Upload
```bash
pio run                  # Build firmware
pio run -t upload        # Upload to Nano
pio device monitor       # Open serial monitor
pio test                 # Run tests (currently broken)
pio run -t clean         # Clean build files
```

### Git Workflow
```bash
git add -A
git commit -m "descriptive message"
git push origin main
```

### Project Analysis
```bash
python tools\bundle_project.py  # Bundle all code
dir /s /b                       # List all files
type filename.cpp | find "search"  # Search in files
```

## ⚠️ Critical Constraints

### Memory Limits
- **2KB RAM**: Already at 72%, careful with strings/buffers
- **No dynamic allocation**: Everything must be static
- **SoftwareSerial**: 250kbps is pushing limits on AVR

### Timing Requirements  
- **Control Loop**: Must run >100Hz for good response
- **Failsafe**: <300ms shutdown required by rules
- **Strike Cooldown**: 500ms minimum between strikes

### Safety Requirements
- Radio failsafe on dedicated channel
- Lid switch hardware interrupt
- Weapon must never leave chassis
- All logged on GitHub: https://github.com/SwanRonSun/kiwibot-fw

## 💡 Design Philosophy
1. **Simplicity**: AVR constraints force elegant solutions
2. **Testability**: Everything mockable without hardware
3. **Safety First**: Multiple redundant failsafes
4. **Incremental**: Small, tested changes via Git

## 🎮 Testing Without Hardware
Use the serial monitor commands (once saved) to simulate matches:
1. Press 't', enter angle: 45, range: 800 (acquire target)
2. Watch state transitions in output
3. Press 't', enter angle: 5, range: 100 (trigger strike)
4. Press 'd' to see full system state

## 📚 Key Decisions Made
1. **Kiwi Drive**: 3 wheels at 90°, 210°, 330° for holonomic motion
2. **Dual Cameras**: Redundancy and wider field of view
3. **Simple Protocol**: 4-byte packets (angle, range) from cameras  
4. **Mock First**: All hardware abstracted for testing
5. **PID Control**: For smooth orbit radius maintenance

## 🔗 Resources
- **GitHub**: https://github.com/SwanRonSun/kiwibot-fw
- **Platform**: PlatformIO + Arduino framework
- **Rules**: SPARC antweight regulations (454g limit)

---

*This robot fights autonomously using vision and strategy. The human operator only provides power and failsafe.*