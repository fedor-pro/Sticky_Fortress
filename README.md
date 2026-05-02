A clone of dwarf fortress on C with using Raylib. Right now in prototype stage.

# How to run if you have linux

First, you need to install raylib on your system directories

Debian: 
'''
sudo apt update
sudo apt install libraylib-dev
'''

Arch Linux / Manjaro:
'''
sudo pacman -S raylib
'''

Fedora:
'''
sudo dnf install raylib-devel
'''

Then, you need to go prevTesting folder and write this command in console: "make clean". Then write "make run". Game will start. 

## Common errors

1) Program crashed with error "Not found ./logs/latest.log" or similar: check that the logs/ folder exists in the Sticky_Fortress/prevTesting folder and that it contains the latest.log file. If they are missing, create them yourself. 