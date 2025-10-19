# kiln_controller
 
# Kiln Controller

Arduino-based kiln controller for ceramic kilns, glass fusing, and other high-temperature applications. Features multi-segment programmable firing curves with EEPROM storage and automatic power failure recovery.

## Features

- 🔥 **Multi-segment firing programs** - Create complex firing curves with heating, holding, and controlled cooling ramps
- 💾 **EEPROM storage** - Save multiple firing programs (limited by: programs × segments × 4 < 1000 bytes)
- ⚡ **Power failure recovery** - Automatically resumes firing after power interruption
- 🎯 **Hot start capability** - Can start firing mid-curve when kiln is already preheated
- 🛡️ **Multiple safety features** - Temperature limits, error detection, thermal runaway protection
- 🕹️ **Simple interface** - Analog joystick navigation with LCD1602 display
- ⏱️ **Long-term stability** - Auto-restart after extended runtime to prevent millis() overflow

## Hardware Requirements

### Core Components
- **Arduino board** (Uno or compatible - requires same number of pins as Uno)
- **MAX31856 thermocouple amplifier** (default: Type S thermocouple)
- **LCD1602 display** (16x2 character LCD)
- **Analog joystick** with button
- **SSR (Solid State Relay)** or contactor for heating element control
- **3x LEDs** for status indication
- **Buzzer** for audio alerts
- **Type S thermocouple** (or other type with code modification)

### Wiring
Components can be connected with jumper wires, though soldering is recommended for reliability in high-temperature environments.

## Software Requirements

### Arduino Libraries
```cpp
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Adafruit_MAX31856.h>
```

Install via Arduino Library Manager:
- LiquidCrystal (built-in)
- EEPROM (built-in)
- Adafruit MAX31856

## Installation

1. Clone this repository:
```bash
git clone https://github.com/yourusername/kiln_controller.git
```

2. Install required libraries in Arduino IDE

3. Open `kiln_controller.ino` in Arduino IDE

4. Upload to your Arduino board

## Configuration

### Changing Thermocouple Type
Edit the thermocouple type definition (single character change):
```cpp
// In the code, change from Type S to desired type
max31856.setThermocoupleType(MAX31856_TCTYPE_K);  // For Type K
```

### Maximum Temperature
Default max: 1280°C (adjustable based on your kiln and thermocouple):
```cpp
#define MAX_TEMP 1280  // Change as needed
```

### Adapting to Other Temperature Sensors
The code can be modified to work with other sensors (e.g., MAX6675, MAX31855) by replacing the sensor initialization and reading functions.

## Usage

### Navigation
- **Joystick UP/DOWN** - Navigate through menu options
- **Joystick RIGHT** - Confirm/accept
- **Joystick LEFT** - Cancel/go back
- **Joystick BUTTON** - Emergency stop (jump to program stop menu from anywhere) / Exit service menu and save changes
- **Status LEDs** - Indicate controller state
- **Buzzer** - Audio feedback and alerts

### Service Menu Access
Hidden service menu for advanced configuration:
1. Navigate to "Cost" menu item (firing cost calculation)
2. Move joystick LEFT three times rapidly
3. Service menu opens with PID tuning and system parameters

**Service Menu Settings:**
- **Kp, Ki, Kd** - PID controller parameters
- **Heating cycle time** - Duration of heating control cycle
- **PID update cycle time** - How often PID calculations refresh
- **Kiln power rating** - For energy consumption calculations
- **Energy unit cost** - For firing cost estimation

Exit service menu with joystick button to save changes.

### Firing Programs
Programs are stored in EEPROM with the following structure:
- **Segments** - Each program consists of multiple segments
- **Ramp types**:
  - Positive slope (a > 0): Heating
  - Zero slope (a = 0): Hold temperature
  - Negative slope (a < 0): Controlled cooling

### Hot Start Feature
If the kiln is already hot when starting a program, the controller will:
- Detect current temperature
- Find appropriate point on firing curve
- Resume from that point (not from zero)

## Safety Features

⚠️ **Important**: All safety features are software-based. If the controller crashes with SSR pin HIGH, heating will continue. Always use external safety mechanisms (kiln sitter, external over-temperature protection, etc.).

### Built-in Protections
- **Maximum temperature limit** (1280°C default)
- **Thermal runaway detection** - Aborts if actual temp differs from target by >100°C during heating
- **Heating failure detection** - Aborts if no temperature rise in SKIP mode after several minutes
- **Cold junction monitoring** - Alerts if MAX31856 reference junction exceeds 70°C
- **Sensor error handling** - Stops firing after 30 consecutive failed thermocouple readings

## PID Control

Features custom lightweight PID implementation (just a few lines of code) optimized for kiln control.

## Limitations & Known Issues

- EEPROM space limits number of programs and segments: `programs × segments × 4 < 1000 bytes` (24 bytes reserved for settings)
- Software-only safety features - external hardware protection strongly recommended
- Some minor improvements needed (see TODO section in code)

## Testing

This controller was tested and used on a small test kiln. Extensive real-world testing is recommended before use with valuable kiln loads.

## Why This Project?

Built out of necessity for a reliable, feature-complete kiln controller with precise firing curve control. Started as a learning project but evolved into a fully functional controller that handles everything from simple bisque firings to complex multi-segment glaze programs.

## Future Development

This was the most ambitious Arduino project at the time of creation. A newer version with touchscreen display has since been developed. This version remains functional and stable for those preferring the simpler joystick interface.

## Contributing

This is a personal project shared for educational purposes. Feel free to fork and adapt for your needs. Issues and suggestions are welcome.

## License

MIT

## Disclaimer

⚠️ **WARNING**: Kilns operate at extremely high temperatures and present serious fire and safety hazards. This controller is provided as-is with no warranties. Users assume all risk. Always follow proper kiln safety procedures, never leave a firing kiln unattended, and ensure proper ventilation. The author is not responsible for any damage, injury, or loss resulting from use of this controller.

## Author

Built by a maker who needed a kiln controller and decided to build one from scratch.

---

**Questions?** Open an issue on GitHub.

**Like this project?** Give it a ⭐!
