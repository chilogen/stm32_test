# Build STM32 Develop Enviroment on Linux (Ubuntu 18.04)

## dependcy OR Keyword

* Clion

* STM32CubeMx

* ST-Link v2

* Ubuntu 18.04

* HAL Library

* Openbcd


## install and configure
### arm-none-eabi

> arm-none-eabi is a cross complie toolchains for arm

**install by repository**

```
apt install binutils-arm-none-eabi gcc-arm-none-eabi libisl15 libnewlib-arm-none-eabi libnewlib-dev libstdc++-arm-none-eabi-newlib
```

**install by binary file**

* download from [arm developer](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)

* unpark and copy to any path you want

* set System Environment (Note: also set Env in Clion-CMake or CMake can not find toolchains)


### openocd

**install by repository**

```
apt install libftdi1-2 libhidapi-hidraw0 libjaylink0 libjim0.77 libusb-0.1-4 openocd

apt install libusb-1.0-0-dev libusb-1.0-doc
```

**install by source**

* download from [sourceforge](https://nchc.dl.sourceforge.net/project/openocd/openocd/0.10.0/openocd-0.10.0.zip)

* ./configure --prefix=/opt/openocd --enable-stlink

* if gcc version higher or equal to v7, Makefile have to be modified

```
# add CFLAGS:
 -Wno-implicit-fallthrough -Werror=format-truncation=0 -Werror=format-overflow=0
```

* make && make install


* Customize board Config

```
# new file in /usr/share/openocd/script/board/warshipv3.cfg
# because command line is 
# `openocd -f interface/stlink-v2.cfg -f target/stm32f1x_stlink.cfg`

source [find interface/stlink-v2.cfg]

transport select hla_swd

source [find target/stm32f1x_stlink.cfg]

reset_config srst_only
```

## st-link driver

* fllow the [README](https://github.com/texane/stlink/blob/master/doc/compiling.md) from [gihub](https://github.com/texane/stlink)

* set system env 

* set ldconfig

## Clion

* download from jetbrain and unpack

## STM32CubeMx

* as normal