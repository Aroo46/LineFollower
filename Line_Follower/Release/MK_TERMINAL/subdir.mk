################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MK_TERMINAL/mk_term.c 

OBJS += \
./MK_TERMINAL/mk_term.o 

C_DEPS += \
./MK_TERMINAL/mk_term.d 


# Each subdirectory must supply rules for building sources it contributes
MK_TERMINAL/%.o: ../MK_TERMINAL/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=11059200UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


