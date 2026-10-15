# **Reinstalling MAME for PC-G850 Emulator**

[https://gemini.google.com/app/e9458456534c67aa](https://gemini.google.com/app/e9458456534c67aa)

*User prompt: i need to reinstall mame on linux to run pc-g850 emulator.*

Response: To reinstall MAME cleanly on Ubuntu and launch the Sharp PC-G850 pocket computer driver, follow these steps:

### **Step 1: Completely Remove the Existing MAME Installation**

Remove any existing APT packages, Flatpak, or Snap versions along with lingering system configuration files:

`# Remove APT version and configurations`  
`sudo apt purge --autoremove mame mame-data`

`# Remove Snap version (if installed)`  
`sudo snap remove mame`

`# Remove Flatpak version (if installed)`  
`flatpak uninstall org.mamedev.MAME`

### **Step 2: Install MAME**

Install the official MAME package via APT:

`sudo apt update`  
`sudo apt install mame`

### **Step 3: Configure Paths & ROM Directory**

Generate a fresh local configuration file and create the required directory structure:

`# Navigate to home directory`  
`cd ~`

`# Create local MAME directory and ROM folder`  
`mkdir -p ~/.mame/roms`

`# Generate a fresh local configuration file`  
`mame -createconfig`

**Verification:** Run ls \-la \~/.mame to verify that mame.ini was created successfully.

### **Step 4: Add the PC-G850 ROM**

MAME requires the specific Sharp PC-G850 system ROM zip file (typically named pcg850.zip) to emulate the device.

> 1. Obtain pcg850.zip.  
> 2. Move the .zip archive into your ROMs directory without extracting it:

`mv /path/to/downloaded/pcg850.zip ~/.mame/roms/`

**Verification:** Confirm the file is placed correctly by running ls \~/.mame/roms/. You should see pcg850.zip listed.

### **Step 5: Launch the PC-G850 Emulator**

Run MAME targeting the pcg850 system driver:

`mame pcg850`

Useful execution flags:

> * **Windowed mode:** mame pcg850 \-window  
> * **Skip info screens:** mame pcg850 \-skip\_gameinfo

**Sources**

> 1. [MAME:Installing \- SpecNext Wiki](https://wiki.specnext.dev/MAME:Installing?utm_source=gemini)

---

