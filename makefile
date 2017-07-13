# Test compilation makefile
#
# Copyright (c) 2016 Ryan Kurte
# This file is covered under the MIT license available at: https://opensource.org/licenses/MIT

.PHONY: setup efm32g efm32gg efm32zg efm32wg efm32lg efm32tg efm32hg ezr32lg ezr32wg efm32pg efr32fg

test: setup efm32g efm32gg efm32zg efm32wg efm32lg efm32tg efm32hg ezr32lg ezr32wg efm32pg efr32fg

setup:
	mkdir -p builds

#EFM32G Family
efm32g:
	cd builds; rm -rf $@; mkdir $@; cd $@; cmake -DDEVICE=EFM32G210F128 ../..; make

#EFM32GG Family
efm32gg:
	cd builds; rm -rf $@; mkdir $@; cd $@; cmake -DDEVICE=EFM32GG990F1024 ../..; make

#EFM32ZG Family
efm32zg:
	cd builds; rm -rf $@; mkdir $@; cd $@; cmake -DDEVICE=EFM32ZG222F32 ../..; make

#EFM32WG Family
efm32wg:
	cd builds; rm -rf $@; mkdir $@; cd $@; cmake -DDEVICE=EFM32WG990F256 ../..; make

#EFM32LG Family
efm32lg:
	cd builds; rm -rf $@; mkdir $@; cd $@; cmake -DDEVICE=EFM32LG990F256 ../..; make;

#EFM32TG Family
efm32tg:
	cd builds; rm -rf $@; mkdir $@; cd $@; cmake -DDEVICE=EFM32TG840F32 ../..; make;

#EFM32WG Family
efm32hg:
	cd builds; rm -rf $@; mkdir $@; cd $@; cmake -DDEVICE=EFM32HG322F64 ../..; make

#EFM32LG Family
ezr32lg:
	cd builds; rm -rf $@; mkdir $@; cd $@; cmake -DDEVICE=EZR32LG230F256R67 ../..; make;

#EFM32TG Family
ezr32wg:
	cd builds; rm -rf $@; mkdir $@; cd $@; cmake -DDEVICE=EZR32WG230F256R67 ../..; make;

#efm32pg Family
efm32pg:
	cd builds; rm -rf $@; mkdir $@; cd $@; cmake -DDEVICE=EFM32PG1B200F256GM48 ../..; make;


#EFR32FG Family
efr32fg:
	cd builds; rm -rf $@; mkdir $@; cd $@; cmake -DDEVICE=EFR32FG13P231F512GM48 ../..; make;


clean:
	rm -rf builds
