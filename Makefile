CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -O0 -Iinclude -Irplidar_sdk/sdk/include -Irplidar_sdk/sdk/src -Ipigpio/sdk/include -Ipigpio #-Issd1306_oled_sdk/src -Issd1306_oled_sdk/include 
LDFLAGS = -Lrplidar_sdk/output/Linux/Release -Lpigpio/sdk/bin -Lpigpio/sdk/lib -Lpigpio
LDLIBS = -pthread -li2c -lrt -lpthread -lsl_lidar_sdk -lpigpio -Wl,-rpath=./

TARGET = bin/programCDFR
TEST_TARGET = bin/runTests
SRCDIR = src
TESTDIR = tests
OBJDIR = obj
TESTOBJDIR = test_obj

SRC = $(wildcard $(SRCDIR)/*.cpp)
SRC_NO_MAIN = $(filter-out $(SRCDIR)/main.cpp,$(SRC))
TEST_SRC = $(wildcard $(TESTDIR)/*.cpp)
OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))
OBJ_NO_MAIN = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC_NO_MAIN))
TEST_OBJ = $(patsubst $(TESTDIR)/%.cpp,$(TESTOBJDIR)/%.o,$(TEST_SRC))
DEPENDS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.d,$(SRC)) $(patsubst $(TESTDIR)/%.cpp,$(TESTOBJDIR)/%.d,$(TEST_SRC))

.PHONY: all clean tests clean-all deploy run

all: bin build_pigpio build_lidarLib $(TARGET) $(TEST_TARGET)
	@echo "Compilation terminée. Exécutez '(cd bin && ./programCDFR)' pour exécuter le programme."

$(TARGET): $(OBJ) | bin
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(TEST_TARGET): $(OBJ_NO_MAIN) $(TEST_OBJ) | bin
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

-include $(DEPENDS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(TESTOBJDIR)/%.o: $(TESTDIR)/%.cpp | $(TESTOBJDIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJDIR):
	mkdir -p $@

$(TESTOBJDIR):
	mkdir -p $@

bin:
	mkdir -p bin

tests: $(TEST_TARGET)
	@echo "--------------------------------- Exécution des tests... ---------------------------------"
	./$(TEST_TARGET)

# Define the lidarLib target
build_lidarLib:
	@echo "--------------------------------- Building lidarLib... ---------------------------------"
	$(MAKE) -C rplidar_sdk

# Define the pigpio target
build_pigpio:
	@echo "--------------------------------- Building pigpio... ---------------------------------"
	sudo $(MAKE) -C pigpio install
	cp ./pigpio/sdk/lib/libpigpio.so.1 ./bin/








# Cross-compiler and flags for Raspberry Pi (ARM architecture)
CROSS_COMPILE_PREFIX = arm-linux-gnueabihf
ARM_CXX = $(CROSS_COMPILE_PREFIX)-g++

# Raspberry Pi Deployment Info
PI_USER = pi
PI_HOST = 192.168.1.47
PI_DIR = /home/pi/CDFR2025

# Define the ARM target and object directory for cross-compilation
ARM_OBJDIR = arm_obj
ARM_TARGET = arm_bin/programCDFR

ARM_OBJ = $(patsubst $(SRCDIR)/%.cpp,$(ARM_OBJDIR)/%.o,$(SRC))

# Compile all object files for ARM
$(ARM_OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(ARM_OBJDIR)
	$(ARM_CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Create the ARM object directory
$(ARM_OBJDIR):
	mkdir -p $@

# Cross-compile and link for Raspberry Pi
$(ARM_TARGET): $(ARM_OBJ) | arm_bin
	$(ARM_CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# Create the ARM binary directory
arm_bin:
	mkdir -p arm_bin

# Deploy target
deploy: clean-all arm_bin build_arm_lidarLib build_arm_pigpio $(ARM_TARGET)
	@echo "--------------------------------- Deploying to Raspberry Pi... ---------------------------------" 
	ssh $(PI_USER)@$(PI_HOST) 'mkdir -p $(PI_DIR)'
	scp -r ./arm_bin $(PI_USER)@$(PI_HOST):$(PI_DIR)

run: deploy
	ssh $(PI_USER)@$(PI_HOST) '$(PI_DIR)/$(TARGET)'

# Define the lidarLib target
build_arm_lidarLib:
	@echo "--------------------------------- Building lidarLib for ARM... ---------------------------------"
	@cd rplidar_sdk && \
	chmod +x ./cross_compile.sh && \
	CROSS_COMPILE_PREFIX=arm-linux-gnueabihf ./cross_compile.sh

# Define the pigpio target
build_arm_pigpio:
	@echo "--------------------------------- Building pigpio for ARM... ---------------------------------"
	sudo $(MAKE) CROSS_PREFIX=$(CROSS_COMPILE_PREFIX)- -C pigpio install
	cp ./pigpio/sdk/lib/libpigpio.so.1 ./arm_bin/





clean:
	@echo "--------------------------------- Cleaning... ---------------------------------"
	rm -rf $(OBJDIR) $(TESTOBJDIR) $(ARM_OBJDIR) $(ARM_OBJDIR) bin/ arm_bin/

# Clean lidarLib specifically
clean-lidarLib:
	@echo "--------------------------------- Cleaning lidarLib... ---------------------------------"
	$(MAKE) -C rplidar_sdk clean

# Clean pigpio specifically
clean-pigpio:
	@echo "--------------------------------- Cleaning pigpio... ---------------------------------"
	$(MAKE) -C pigpio clean

# Combined clean for all
clean-all: clean clean-lidarLib clean-pigpio