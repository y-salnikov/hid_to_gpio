#!/bin/bash
openocd -f interface/stlink-v2.cfg \
-f target/stm32f4x.cfg \
-c "program build/bk_controller_usb_cubemx.elf verify reset exit"

