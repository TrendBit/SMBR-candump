# SMBR-candump
Program similar to linux utility candump but with more information specific to SMBR CAN BUS communication. Codes used in application messages are from [SMBR-CANBUS-Codes](https://github.com/TrendBit/SMBR-CANBUS-Codes) repository. For cross-compilation is used docker container based on `debian:bookworm-slim`.

# Getting started
Compilation and installation on RPi
```zsh  
git clone https://github.com/TrendBit/SMBR-candump.git --recursive
cd SMBR-candump  
mkdir -p build && cd build
cmake ..
sudo make install
```

Cross-compilation on host machine:
```zsh  
git clone git@github.com:TrendBit/SMBR-candump.git  
cd SMBR-candump  
make  
scp candump pi@192.168.x.x:~  
```  

Run: `can-watch <can_interface>`

Execution and example with `cansend can0 0b000051#04`:
```zsh  
./candump can0
    Time        ID        Message           Module          Instance     Length    Data
[00001.342]  0b000051   Ping_request    Control_board      Exclusive        1      0x04 
[00001.695]  0b010051  Ping_response    Control_board      Exclusive        1      0x04
```  

# Libraries used
- [emio](https://github.com/Viatorus/emio) by Toni Neubert [MIT licence]
- [magic_enum](https://github.com/Neargye/magic_enum) by Daniil Goncharov [MIT licence]
