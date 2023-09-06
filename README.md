# CS-4800R

## Project Guide For Ren

### Important Files
`docs/meeting_notes.md` - Contains notes from our meetings

### Directory Structure
- `docs/` - Contains all documentation for the project, not organized yet
- `latex/` - Contains the writeup for the project
- `stm32_afsk/` - Contains the Embedded code for the project (This will change once I can think of a good name)
- `external_host/` - Contains the code for the external host

not so important directories

- `.vscode` - Included on purpose, it contains the settings for CMake and spell checking
- `stm32_cubeIDE_workspace` - For debugging in the STM32 Cube IDE (A sometimes useful eclipse based IDE)


### GitHub Project
https://github.com/users/joshua-jerred/projects/7/views/1

^ I may need to give you access to this.

***
***

# Notes for Me
Required Apt Packages:
- `cmake` - For CMake...
- `ninja-build` - For CMake to use a build system
- `gcc-arm-none-eabi` - For compiling for the STM32
- `stlink-tools` - For flashing the STM32 (st-flash)
- `gdb-multiarch` - For debugging the STM32

Run `ln -s /usr/bin/gdb-multiarch /usr/bin/arm-none-eabi-gdb` to add `arm-none-eabi-gdb` to the path