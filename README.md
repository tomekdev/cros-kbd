# cros-kbd
Small application written in C/C++ for X-independent backlight management using key combination in ASUS Chromebook C201

# Compilation
Simply run make. The binary will be named cros-kbd

# Usage
Run cros-kbd as root. It will try to detect your Chromebook keyboard by probing for 'cros_ec' device name. Then it will try to open corresponding event device in /dev/input/event* and will read every keypress. Don't worry, it ignores all keypresses except Search button and Brightness+/-. Now you can adjust your screen brightness using Search+Brightness+ and Search+Brightness-
