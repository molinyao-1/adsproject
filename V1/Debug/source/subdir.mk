################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Cpu0_Main.c \
../source/Cpu1_Main.c \
../source/EMIT_List.c \
../source/EMIT_Pitmgr.c \
../source/SmartCar_Isr.c \
../source/VA_get.c \
../source/adrc.c \
../source/beacon_control.c \
../source/beacon_control_v2.c \
../source/image.c \
../source/menu.c \
../source/testsome1.c 

OBJS += \
./source/Cpu0_Main.o \
./source/Cpu1_Main.o \
./source/EMIT_List.o \
./source/EMIT_Pitmgr.o \
./source/SmartCar_Isr.o \
./source/VA_get.o \
./source/adrc.o \
./source/beacon_control.o \
./source/beacon_control_v2.o \
./source/image.o \
./source/menu.o \
./source/testsome1.o 

COMPILED_SRCS += \
./source/Cpu0_Main.src \
./source/Cpu1_Main.src \
./source/EMIT_List.src \
./source/EMIT_Pitmgr.src \
./source/SmartCar_Isr.src \
./source/VA_get.src \
./source/adrc.src \
./source/beacon_control.src \
./source/beacon_control_v2.src \
./source/image.src \
./source/menu.src \
./source/testsome1.src 

C_DEPS += \
./source/Cpu0_Main.d \
./source/Cpu1_Main.d \
./source/EMIT_List.d \
./source/EMIT_Pitmgr.d \
./source/SmartCar_Isr.d \
./source/VA_get.d \
./source/adrc.d \
./source/beacon_control.d \
./source/beacon_control_v2.d \
./source/image.d \
./source/menu.d \
./source/testsome1.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.src: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb -I"D:/mmc/editingproject/Libraries/Infra/Platform/Tricore/Compilers" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Multican/Std" -I"D:/mmc/editingproject/Libraries/Infra/Platform" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Cif/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Hssl/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Cpu/Trap" -I"D:/mmc/editingproject/Libraries/Service/CpuGeneric/If/Ccu6If" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Dsadc/Dsadc" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Port" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Stm/Timer" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Dts/Dts" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Eth" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Flash" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Vadc" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Msc" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Qspi/SpiMaster" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Scu/Std" -I"D:/mmc/editingproject/Libraries/Service/CpuGeneric/SysSe/Comm" -I"D:/mmc/editingproject/Libraries/Service/CpuGeneric/SysSe/Math" -I"D:/mmc/editingproject/Libraries/Infra/Platform/Tricore" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Gtm/Trig" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Gtm/Tim" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Ccu6/TimerWithTrigger" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Emem" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Mtu" -I"D:/mmc/editingproject/Libraries/Infra" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Fft" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/I2c/I2c" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Asclin/Asc" -I"D:/mmc/editingproject/Libraries/Service/CpuGeneric/SysSe" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Flash/Std" -I"D:/mmc/editingproject/Libraries/Service/CpuGeneric/If" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Psi5" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Cpu" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Fce/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Stm/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Msc/Msc" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Vadc/Adc" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Asclin" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Gtm/Tom/Pwm" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Gtm/Atom" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Port/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Psi5/Psi5" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Eray" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Qspi/SpiSlave" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Ccu6/Icu" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Cpu/CStart" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Hssl" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Cif" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Eth/Phy_Pef7071" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Hssl/Hssl" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Iom/Driver" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Multican/Can" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Psi5s/Psi5s" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Fft/Fft" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Ccu6/PwmHl" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Iom/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/_Lib" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Gtm/Tom/Timer" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Sent" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Eray/Eray" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Gpt12/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Dma" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Fce/Crc" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Qspi" -I"D:/mmc/editingproject/Libraries/Infra/Sfr" -I"D:/mmc/editingproject/Libraries/Infra/Sfr/TC26B" -I"D:/mmc/editingproject/Libraries/Service/CpuGeneric/SysSe/Bsp" -I"D:/mmc/editingproject/Libraries/Service/CpuGeneric/SysSe/General" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Cpu/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Dts" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Src" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Dma/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Cif/Cam" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Src/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Asclin/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/I2c/Std" -I"D:/mmc/editingproject/Configurations" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/_Lib/DataHandling" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Sent/Sent" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Ccu6/Timer" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Psi5/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Psi5s" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Emem/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Ccu6/PwmBc" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Iom" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Ccu6/TPwm" -I"D:/mmc/editingproject/Libraries/Service/CpuGeneric" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Multican" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Mtu/Std" -I"D:/mmc/editingproject/Libraries/Infra/Sfr/TC26B/_Reg" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Gtm/Tom/PwmHl" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Dma/Dma" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Gtm/Atom/Timer" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Ccu6/Std" -I"D:/mmc/editingproject/Libraries/Service/CpuGeneric/SysSe/Time" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Dsadc/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Cpu/Irq" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Gtm" -I"D:/mmc/editingproject/Libraries/iLLD" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Ccu6" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Gpt12/IncrEnc" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Psi5s/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Scu" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/_Lib/InternalMux" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Stm" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Dsadc/Rdc" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Vadc/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Dts/Std" -I"D:/mmc/editingproject/Libraries/Service" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Eth/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Smu" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/_PinMap" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Asclin/Lin" -I"D:/mmc/editingproject/Libraries/Service/CpuGeneric/StdIf" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Dsadc" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Fce" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Gtm/Atom/PwmHl" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Qspi/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Gtm/Tom" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Gtm/Tim/In" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Msc/Std" -I"D:/mmc/editingproject/Configurations/Debug" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Fft/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Gtm/Atom/Pwm" -I"D:/mmc/editingproject/Libraries/Service/CpuGeneric/_Utilities" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Gtm/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Smu/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/I2c" -I"D:/mmc/editingproject/Libraries" -I"D:/mmc/editingproject/source" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Asclin/Spi" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Eray/Std" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Port/Io" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/_Impl" -I"D:/mmc/editingproject/Libraries/iLLD/TC26B/Tricore/Sent/Std" -I"D:/mmc/editingproject/SmartCar" --iso=99 --c++14 --language=+volatile --anachronisms --fp-model=3 --fp-model=c --fp-model=f --fp-model=l --fp-model=n --fp-model=r --fp-model=z -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file=$(@:.src=.d) --misrac-version=2012 -N0 -Z0 -Y0 2>&1; sed -i -e '/ctc\\include/d' -e '/Libraries\\iLLD/d' -e '/Libraries\\Infra/d' -e 's/\(.*\)".*\\editingproject\(\\.*\)"/\1\.\.\2/g' -e 's/\\/\//g' $(@:.src=.d) && \
	echo $(@:.src=.d) generated
	@echo 'Finished building: $<'
	@echo ' '

source/%.o: ./source/%.src
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


