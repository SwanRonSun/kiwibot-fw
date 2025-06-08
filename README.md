# KiwiBot - Autonomous Antweight Combat Robot

An autonomous 454g combat robot with holonomic drive and vision-based targeting.

## Quick Start

```bash
# Build firmware
pio run

# Run tests (currently broken - see issue #1)
pio test

# Monitor serial output
pio device monitor
```

## Project Status

🚧 **Hardware on order** - Currently running in mock/simulation mode

- ✅ Core firmware architecture complete
- ✅ State machine (IDLE → ACQUIRE → ORBIT → STRIKE)  
- ✅ Dual camera vision system (mocked)
- ✅ Holonomic kinematics implemented
- ✅ PID orbit controller ready
- ⏳ Waiting for hardware to arrive for testing
- ⚠️ Unit tests need restructuring

## Features

- **Fully Autonomous**: No human control during matches
- **Dual Vision**: Two ESP32-S3 cameras for opponent tracking
- **Kiwi Drive**: 3-wheel holonomic platform for agile movement
- **Hydraulic Strike**: Contained spike weapon with 500ms cooldown
- **Multiple Failsafes**: Radio dead-man, lid switch, software kill

## Documentation

- 📋 [AI Context Document](README_AI_CONTEXT.md) - Detailed technical overview
- 📋 [Safety Checklist](docs/safety_checklist.md) - Competition compliance

## Hardware Overview

- **Brain**: Arduino Nano (ATmega328P)
- **Motors**: 3x N20 with custom omni wheels
- **Vision**: 2x XIAO ESP32-S3 Sense modules
- **Weapon**: Short-stroke solenoid spike
- **Power**: 2S LiPo with 5A fuse

## Memory Usage

- RAM: 72.1% (1477/2048 bytes)
- Flash: 37.4% (11504/30720 bytes)

## License

Open source hardware and software for combat robotics research.