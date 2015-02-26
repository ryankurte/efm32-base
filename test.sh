#!/bin/bash
#Test compilation script

mkdir builds
cd builds

#EFM32G Family
rm -rf efm32g
mkdir efm32g
cd efm32g
cmake -DDEVICE=EFM32G210F128 ../..
make
cd ..

#EFM32GG Family
rm -rf efm32gg
mkdir efm32gg
cd efm32gg
cmake -DDEVICE=EFM32GG990F1024 ../..
make
cd ..

#EFM32ZG Family
rm -rf efm32zg
mkdir efm32zg
cd efm32zg
cmake -DDEVICE=EFM32ZG222F32 ../..
make
cd ..

#EFM32WG Family
rm -rf efm32wg
mkdir efm32wg
cd efm32wg
cmake -DDEVICE=EFM32WG990F256 ../..
make
cd ..

#EFM32LG Family
rm -rf efm32lg
mkdir efm32lg
cd efm32lg
cmake -DDEVICE=EFM32LG990F256 ../..
make
cd ..

#EFM32TG Family
rm -rf efm32tg
mkdir efm32tg
cd efm32tg
cmake -DDEVICE=EFM32TG840F32 ../..
make
cd ..

cd ..

