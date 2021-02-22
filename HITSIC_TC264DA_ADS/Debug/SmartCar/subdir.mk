################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SmartCar/SmartCar_ADC.c \
../SmartCar/SmartCar_Assert.c \
../SmartCar/SmartCar_Encoder.c \
../SmartCar/SmartCar_Eru.c \
../SmartCar/SmartCar_EruDma.c \
../SmartCar/SmartCar_FONT.c \
../SmartCar/SmartCar_Flash.c \
../SmartCar/SmartCar_GPIO.c \
../SmartCar/SmartCar_Hardware_IIC.c \
../SmartCar/SmartCar_MPU.c \
../SmartCar/SmartCar_MT9V034.c \
../SmartCar/SmartCar_Oled.c \
../SmartCar/SmartCar_PIT.c \
../SmartCar/SmartCar_Pwm.c \
../SmartCar/SmartCar_Soft_IIC.c \
../SmartCar/SmartCar_Systick.c \
../SmartCar/SmartCar_Uart.c \
../SmartCar/SmartCar_Upload.c \
../SmartCar/common.c 

OBJS += \
./SmartCar/SmartCar_ADC.o \
./SmartCar/SmartCar_Assert.o \
./SmartCar/SmartCar_Encoder.o \
./SmartCar/SmartCar_Eru.o \
./SmartCar/SmartCar_EruDma.o \
./SmartCar/SmartCar_FONT.o \
./SmartCar/SmartCar_Flash.o \
./SmartCar/SmartCar_GPIO.o \
./SmartCar/SmartCar_Hardware_IIC.o \
./SmartCar/SmartCar_MPU.o \
./SmartCar/SmartCar_MT9V034.o \
./SmartCar/SmartCar_Oled.o \
./SmartCar/SmartCar_PIT.o \
./SmartCar/SmartCar_Pwm.o \
./SmartCar/SmartCar_Soft_IIC.o \
./SmartCar/SmartCar_Systick.o \
./SmartCar/SmartCar_Uart.o \
./SmartCar/SmartCar_Upload.o \
./SmartCar/common.o 

COMPILED_SRCS += \
./SmartCar/SmartCar_ADC.src \
./SmartCar/SmartCar_Assert.src \
./SmartCar/SmartCar_Encoder.src \
./SmartCar/SmartCar_Eru.src \
./SmartCar/SmartCar_EruDma.src \
./SmartCar/SmartCar_FONT.src \
./SmartCar/SmartCar_Flash.src \
./SmartCar/SmartCar_GPIO.src \
./SmartCar/SmartCar_Hardware_IIC.src \
./SmartCar/SmartCar_MPU.src \
./SmartCar/SmartCar_MT9V034.src \
./SmartCar/SmartCar_Oled.src \
./SmartCar/SmartCar_PIT.src \
./SmartCar/SmartCar_Pwm.src \
./SmartCar/SmartCar_Soft_IIC.src \
./SmartCar/SmartCar_Systick.src \
./SmartCar/SmartCar_Uart.src \
./SmartCar/SmartCar_Upload.src \
./SmartCar/common.src 

C_DEPS += \
./SmartCar/SmartCar_ADC.d \
./SmartCar/SmartCar_Assert.d \
./SmartCar/SmartCar_Encoder.d \
./SmartCar/SmartCar_Eru.d \
./SmartCar/SmartCar_EruDma.d \
./SmartCar/SmartCar_FONT.d \
./SmartCar/SmartCar_Flash.d \
./SmartCar/SmartCar_GPIO.d \
./SmartCar/SmartCar_Hardware_IIC.d \
./SmartCar/SmartCar_MPU.d \
./SmartCar/SmartCar_MT9V034.d \
./SmartCar/SmartCar_Oled.d \
./SmartCar/SmartCar_PIT.d \
./SmartCar/SmartCar_Pwm.d \
./SmartCar/SmartCar_Soft_IIC.d \
./SmartCar/SmartCar_Systick.d \
./SmartCar/SmartCar_Uart.d \
./SmartCar/SmartCar_Upload.d \
./SmartCar/common.d 


# Each subdirectory must supply rules for building sources it contributes
SmartCar/%.src: ../SmartCar/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/Infra/Platform/Tricore/Compilers" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Multican/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/Infra/Platform" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Cif/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Hssl/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Cpu/Trap" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/Service/CpuGeneric/If/Ccu6If" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Dsadc/Dsadc" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Port" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Stm/Timer" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Dts/Dts" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Eth" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Flash" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Vadc" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Msc" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Qspi/SpiMaster" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Scu/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/Service/CpuGeneric/SysSe/Comm" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/Service/CpuGeneric/SysSe/Math" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/Infra/Platform/Tricore" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Gtm/Trig" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Gtm/Tim" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Ccu6/TimerWithTrigger" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Emem" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Mtu" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/Infra" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Fft" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/I2c/I2c" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Asclin/Asc" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/Service/CpuGeneric/SysSe" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Flash/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/Service/CpuGeneric/If" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Psi5" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Cpu" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Fce/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Stm/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Msc/Msc" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Vadc/Adc" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Asclin" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Gtm/Tom/Pwm" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Gtm/Atom" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Port/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Psi5/Psi5" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Eray" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Qspi/SpiSlave" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Ccu6/Icu" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Cpu/CStart" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Hssl" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Cif" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Eth/Phy_Pef7071" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Hssl/Hssl" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Iom/Driver" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Multican/Can" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Psi5s/Psi5s" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Fft/Fft" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Ccu6/PwmHl" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Iom/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/_Lib" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Gtm/Tom/Timer" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Sent" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Eray/Eray" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Gpt12/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Dma" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Fce/Crc" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Qspi" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/Infra/Sfr" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/Infra/Sfr/TC26B" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/Service/CpuGeneric/SysSe/Bsp" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/Service/CpuGeneric/SysSe/General" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Cpu/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Dts" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Src" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Dma/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Cif/Cam" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Src/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Asclin/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/I2c/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Configurations" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/_Lib/DataHandling" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Sent/Sent" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Ccu6/Timer" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Psi5/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Psi5s" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Emem/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Ccu6/PwmBc" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Iom" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Ccu6/TPwm" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/Service/CpuGeneric" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Multican" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Mtu/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/Infra/Sfr/TC26B/_Reg" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Gtm/Tom/PwmHl" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Dma/Dma" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Gtm/Atom/Timer" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Ccu6/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/Service/CpuGeneric/SysSe/Time" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Dsadc/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Cpu/Irq" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Gtm" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Ccu6" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Gpt12/IncrEnc" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Psi5s/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Scu" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/_Lib/InternalMux" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Stm" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Dsadc/Rdc" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Vadc/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Dts/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/Service" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Eth/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Smu" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/_PinMap" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Asclin/Lin" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/Service/CpuGeneric/StdIf" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Dsadc" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Fce" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Gtm/Atom/PwmHl" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Qspi/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Gtm/Tom" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Gtm/Tim/In" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Msc/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Configurations/Debug" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Fft/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Gtm/Atom/Pwm" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/Service/CpuGeneric/_Utilities" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Gtm/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Smu/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/I2c" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/source" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Asclin/Spi" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Eray/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Port/Io" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/_Impl" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/Libraries/iLLD/TC26B/Tricore/Sent/Std" -I"E:/Fork/HITSIC_TC264DA/HITSIC_TC264DA_ADS/SmartCar" --iso=99 --c++14 --language=+volatile --anachronisms --fp-model=3 --fp-model=c --fp-model=f --fp-model=l --fp-model=n --fp-model=r --fp-model=z -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file=$(@:.src=.d) --misrac-version=2012 -N0 -Z0 -Y0 2>&1; sed -i -e '/ctc\\include/d' -e '/Libraries\\iLLD/d' -e '/Libraries\\Infra/d' -e 's/\(.*\)".*\\HITSIC_TC264DA_ADS\(\\.*\)"/\1\.\.\2/g' -e 's/\\/\//g' $(@:.src=.d) && \
	echo $(@:.src=.d) generated
	@echo 'Finished building: $<'
	@echo ' '

SmartCar/%.o: ./SmartCar/%.src
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


