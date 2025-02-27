# Caution
1. **Do not unplug trrs cable** when keyboard is connected with power. It will destory keyboard.
2. Main side is **LEFT**, connect usb to left MCU when using.  


# How to use

1. Install qmk - **https://docs.qmk.fm/newbs_getting_started**
2. clone this repository into **${Home}/qmk_firmware/keyboards**
   
   make sure the path looks like **${Home}/qmk_firmware/keyboards/charybdis**
3. compile
   
   ```bash
   qmk compile -kb charybdis/3x5/v2/pico -km via
   ```
4. go to **${Home}/qmk_firmware/.build**  and copy .uf2 file
5. double click a push button under the MCU mounted in keyboard
6. paste .uf2 file into keyboard
7. You need to clone .uf2 file to each sides
8. Each sides use same .uf2 file

# Keymap

to change keymaps,

go to

**3x5/keymaps/via/keymap.c**

and change keymaps as you want

# Origin Repository

https://github.com/qmk/qmk_firmware/tree/master/keyboards/bastardkb/charybdis

