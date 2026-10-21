# Project Synthia
Project Synthia is a digital synthesizer that is designed based upon the Suzuki Omnichord. This repository contains the firmware code for the microcontroller as well as the hardware schematics.


# Project Synthia Firmware

A brief description of what this firmware does, the target application, and the primary protocol used (e.g., "An ESP32-based firmware for monitoring soil moisture via MQTT").

## Hardware Requirements

* **Microcontroller:** [e.g., STM32F405RG / ESP32-WROOM-32E]
* **Development Board/PCB:** NUCLEO-G431KB
* **External Peripherals:** 
* **Debugger/Programmer:**

### Pin Mapping
The pins are mapped to the following variable values. It is currently understood that on the NUCLEO-G431KB, the PB9 pin does not exist.

| Variable | Pin | Pin Type | IO |
| :--- | :--- | :--- | :--- |
| majorChords | PA7 | GPIO | Output |
| minorChords | PB0 | GPIO | Output |
| seventhChords | PA9 | GPIO | Output |
| DAC_0 | PF0 | GPIO | Output |
| DAC_1 | PF1 | GPIO | Output |
| DAC_2 | PA8 | GPIO | Output |
| DAC_3 | PA0 | GPIO | Output |
| DAC_4 | PA1 | GPIO | Output |
| DAC_5 | PA4 | GPIO | Output |
| DAC_6 | PA5 | GPIO | Output |
| DAC_7 | PA6 | GPIO | Output |
| tempoControl | PB9 | Analog | Input |
| rhythmSwitch | PA10 | GPIO | Input |
| rhythmMatrix[4] | PA11 | GPIO | Input |
| fontSwitch | PA12 | GPIO | Input |
| fontMatrix[4] | PA15 | GPIO | Input |
| chordMatrix_0 | PB4 | GPIO | Input |
| chordMatrix_1 | PB5 | GPIO | Input |
| chordMatrix_2 | PB6 | GPIO | Input |
| chordMatrix_3 | PB7 | GPIO | Input |

## Getting Started

### Prerequisites & Toolchain
* **IDE/Framework:** STM32CubeIDE, STM32CubeMX
* **Compiler:** 
* **Dependencies:** N/A

### Building the Firmware
```bash
# Clone the repository
git clone https://github.com/Engineering-House/Project-Synthia.git
cd Project-Synthia/Synthia-Firmware
```

In the STM32CubeIDE, after opening the firmware folder detailed above, clean and build the project from the Project menu.

##  Flashing & Debugging

There is currently no flashing and debugging process. Please update this when needed.

## Configuration

Explain how users can modify settings before compiling (e.g., Wi-Fi credentials, baud rates, calibration constants).
* **Configuration File:** 
* **Key Parameters:**
## Testing & Debugging

