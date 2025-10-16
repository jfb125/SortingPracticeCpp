################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ArrayComposition.cpp \
../src/BlockOperations.cpp \
../src/BlockSort.cpp \
../src/BlockSortTest.cpp \
../src/Disorganizer.cpp \
../src/InitialOrdering.cpp \
../src/MessageList.cpp \
../src/OneTestResult.cpp \
../src/ResultOutput.cpp \
../src/SimpleRandomizer.cpp \
../src/SimpleRandomizer_test.cpp \
../src/SortAlgorithm.cpp \
../src/SortFailureLog.cpp \
../src/SortFailureLog_test.cpp \
../src/SortTest.cpp \
../src/SortTestMetrics.cpp \
../src/SortableObject.cpp \
../src/SortingPractice.cpp \
../src/SortingUtilities.cpp \
../src/StudentData.cpp \
../src/StudentDataArray.cpp \
../src/StudentDataArray_test.cpp \
../src/StudentDataGenerator.cpp \
../src/nChoosek.cpp \
../src/nChoosek_test.cpp 

CPP_DEPS += \
./src/ArrayComposition.d \
./src/BlockOperations.d \
./src/BlockSort.d \
./src/BlockSortTest.d \
./src/Disorganizer.d \
./src/InitialOrdering.d \
./src/MessageList.d \
./src/OneTestResult.d \
./src/ResultOutput.d \
./src/SimpleRandomizer.d \
./src/SimpleRandomizer_test.d \
./src/SortAlgorithm.d \
./src/SortFailureLog.d \
./src/SortFailureLog_test.d \
./src/SortTest.d \
./src/SortTestMetrics.d \
./src/SortableObject.d \
./src/SortingPractice.d \
./src/SortingUtilities.d \
./src/StudentData.d \
./src/StudentDataArray.d \
./src/StudentDataArray_test.d \
./src/StudentDataGenerator.d \
./src/nChoosek.d \
./src/nChoosek_test.d 

OBJS += \
./src/ArrayComposition.o \
./src/BlockOperations.o \
./src/BlockSort.o \
./src/BlockSortTest.o \
./src/Disorganizer.o \
./src/InitialOrdering.o \
./src/MessageList.o \
./src/OneTestResult.o \
./src/ResultOutput.o \
./src/SimpleRandomizer.o \
./src/SimpleRandomizer_test.o \
./src/SortAlgorithm.o \
./src/SortFailureLog.o \
./src/SortFailureLog_test.o \
./src/SortTest.o \
./src/SortTestMetrics.o \
./src/SortableObject.o \
./src/SortingPractice.o \
./src/SortingUtilities.o \
./src/StudentData.o \
./src/StudentDataArray.o \
./src/StudentDataArray_test.o \
./src/StudentDataGenerator.o \
./src/nChoosek.o \
./src/nChoosek_test.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/ArrayComposition.d ./src/ArrayComposition.o ./src/BlockOperations.d ./src/BlockOperations.o ./src/BlockSort.d ./src/BlockSort.o ./src/BlockSortTest.d ./src/BlockSortTest.o ./src/Disorganizer.d ./src/Disorganizer.o ./src/InitialOrdering.d ./src/InitialOrdering.o ./src/MessageList.d ./src/MessageList.o ./src/OneTestResult.d ./src/OneTestResult.o ./src/ResultOutput.d ./src/ResultOutput.o ./src/SimpleRandomizer.d ./src/SimpleRandomizer.o ./src/SimpleRandomizer_test.d ./src/SimpleRandomizer_test.o ./src/SortAlgorithm.d ./src/SortAlgorithm.o ./src/SortFailureLog.d ./src/SortFailureLog.o ./src/SortFailureLog_test.d ./src/SortFailureLog_test.o ./src/SortTest.d ./src/SortTest.o ./src/SortTestMetrics.d ./src/SortTestMetrics.o ./src/SortableObject.d ./src/SortableObject.o ./src/SortingPractice.d ./src/SortingPractice.o ./src/SortingUtilities.d ./src/SortingUtilities.o ./src/StudentData.d ./src/StudentData.o ./src/StudentDataArray.d ./src/StudentDataArray.o ./src/StudentDataArray_test.d ./src/StudentDataArray_test.o ./src/StudentDataGenerator.d ./src/StudentDataGenerator.o ./src/nChoosek.d ./src/nChoosek.o ./src/nChoosek_test.d ./src/nChoosek_test.o

.PHONY: clean-src

