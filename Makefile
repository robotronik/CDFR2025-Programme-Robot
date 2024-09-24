CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -O0 -static $(INCLUDE_DIR)
LDFLAGS = -Lrplidar_sdk/output/Linux/Release
LDLIBS = pigpio/pigpio.o pigpio/command.o -pthread -li2c -lrt -lpthread -lsl_lidar_sdk 

INCLUDE_DIR = -Iinclude
INCLUDE_DIR += -Irplidar_sdk/sdk/include
INCLUDE_DIR += -Irplidar_sdk/sdk/src
INCLUDE_DIR += -Ipigpio
INCLUDE_DIR += -I../librairie-commune/include


BINDIR = bin
TARGET = $(BINDIR)/programCDFR
TEST_TARGET = $(BINDIR)/tests

SRCDIR = src
SRCDIR_LIBCOM = ../librairie-commune/src
SRCDIR_TEST = tests

OBJDIR = obj
OBJDIR_MAIN = obj/main_obj
OBJDIR_LIBCOM = obj/lib_com_obj
OBJDIR_TEST = obj/test_obj


SRC = $(wildcard $(SRCDIR)/*.cpp)
SRC_LIB_COM = $(wildcard $(SRCDIR_LIBCOM)/*.cpp)

OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR_MAIN)/%.o,$(SRC))
OBJ += $(patsubst $(SRCDIR_LIBCOM)/%.cpp,$(OBJDIR_LIBCOM)/%.o,$(SRC_LIB_COM)) 

SRC_NO_MAIN = $(filter-out $(SRCDIR)/main.cpp,$(SRC))
SRC_TEST = $(wildcard $(SRCDIR_TEST)/*.cpp)
OBJ_NO_MAIN = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR_MAIN)/%.o,$(SRC_NO_MAIN))
OBJ_NO_MAIN += $(patsubst $(SRCDIR_LIBCOM)/%.cpp,$(OBJDIR_LIBCOM)/%.o,$(SRC_LIB_COM)) 
TEST_OBJ = $(patsubst $(SRCDIR_TEST)/%.cpp,$(OBJDIR_TEST)/%.o,$(SRC_TEST))


DEPENDS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR_MAIN)/%.d,$(SRC)) 
DEPENDS += $(patsubst $(SRCDIR_LIBCOM)/%.cpp,$(OBJDIR_LIBCOM)/%.d,$(SRC_LIB_COM)) 
DEPENDS += $(patsubst $(SRCDIR_TEST)/%.cpp,$(OBJDIR_TEST)/%.d,$(SRC_TEST)) 


.PHONY: all clean tests clean-all deploy run

all: $(BINDIR) build_pigpio build_lidarLib $(TARGET) $(TEST_TARGET)
	@echo "Compilation terminée. Exécutez '(cd $(BINDIR) && ./programCDFR)' pour exécuter le programme."

check:
	@echo $(addsuffix /I2CDevice.cpp, $(SRC_DIRS))

$(TARGET): $(OBJ) | $(BINDIR)
	@echo "--------------------------------- Compilation du programme principal... ---------------------------------"
	@echo " APP  $@"
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(TEST_TARGET): $(OBJ_NO_MAIN) $(TEST_OBJ) | $(BINDIR)
	@echo "--------------------------------- Compilation des tests... ---------------------------------"
	@echo " APP  $@"
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

-include $(DEPENDS)

$(OBJDIR_MAIN)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR_MAIN)
	@echo " CXX  $@"
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJDIR_LIBCOM)/%.o: $(SRCDIR_LIBCOM)/%.cpp | $(OBJDIR_LIBCOM)
	@echo " CXX  $@"
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJDIR_TEST)/%.o: $(SRCDIR_TEST)/%.cpp | $(OBJDIR_TEST)
	@echo " CXX  $@"
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJDIR_LIBCOM):
	@echo " DIR  $@"
	@mkdir -p $@

$(OBJDIR_MAIN):
	@echo " DIR  $@"
	@mkdir -p $@

$(OBJDIR_TEST):
	@echo " DIR  $@"
	@mkdir -p $@

$(BINDIR):
	@echo " DIR  $@"
	@mkdir -p $@

tests: $(TEST_TARGET)
	@echo "--------------------------------- Exécution des tests... ---------------------------------"
	./$(TEST_TARGET)

# Define the lidarLib target
build_lidarLib:
	@echo "--------------------------------- Compilation de lidarLib... ---------------------------------"
	$(MAKE) -C rplidar_sdk

# Define the pigpio target
build_pigpio:
	@echo "--------------------------------- Compilation de pigpio... ---------------------------------"
	$(MAKE) -C pigpio pigpio.o command.o
# sudo




# Cross-compiler and flags for Raspberry Pi (ARM architecture)
CROSS_COMPILE_PREFIX = aarch64-linux-gnu
ARM_CXX = $(CROSS_COMPILE_PREFIX)-g++

# Raspberry Pi Deployment Info
PI_USER = pi
PI_HOST = 192.168.1.47
PI_DIR = /home/$(PI_USER)/CDFR2025

# Define the ARM target and object directory for cross-compilation
ARMBINDIR = arm_bin
ARM_OBJDIR = arm_obj
ARM_TARGET = $(ARMBINDIR)/programCDFR

ARM_OBJ = $(patsubst $(SRCDIR)/%.cpp,$(ARM_OBJDIR)/%.o,$(SRC))


# Compile all object files for ARM
$(ARM_OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(ARM_OBJDIR)
	$(ARM_CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Create the ARM object directory
$(ARM_OBJDIR):
	mkdir -p $@

# Create the ARM binary directory
$(ARMBINDIR):
	mkdir -p $@

# Cross-compile and link for Raspberry Pi
$(ARM_TARGET): $(ARM_OBJ) | $(ARMBINDIR)
	$(ARM_CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)


# Deploy target
deploy: build_arm_lidarLib build_arm_pigpio $(ARM_TARGET)
	@echo "--------------------------------- Deploiement vers le Raspberry Pi... ---------------------------------" 
	ssh $(PI_USER)@$(PI_HOST) 'mkdir -p $(PI_DIR)'
	scp -r ./$(ARMBINDIR) $(PI_USER)@$(PI_HOST):$(PI_DIR)

run: deploy
	ssh $(PI_USER)@$(PI_HOST) '$(PI_DIR)/$(ARM_TARGET)'

# Define the lidarLib target
build_arm_lidarLib:
	@echo "--------------------------------- Compilation de lidarLib pour ARM64... ---------------------------------"
	@cd rplidar_sdk && \
	chmod +x ./cross_compile.sh && \
	CROSS_COMPILE_PREFIX=$(CROSS_COMPILE_PREFIX) ./cross_compile.sh

# Define the pigpio target
build_arm_pigpio:
	@echo "--------------------------------- Compilation de pigpio pour ARM64... ---------------------------------"
	$(MAKE) CROSS_PREFIX=$(CROSS_COMPILE_PREFIX)- -C pigpio pigpio.o command.o
#sudo





clean:
	@echo "--------------------------------- Nettoyage... ---------------------------------"
	rm -rf $(OBJDIR) $(ARM_OBJDIR) $(ARM_OBJDIR) $(BINDIR) $(ARMBINDIR)

# Clean lidarLib specifically
clean-lidarLib:
	@echo "--------------------------------- Nettoyage de lidarLib... ---------------------------------"
	$(MAKE) -C rplidar_sdk clean

# Clean pigpio specifically
clean-pigpio:
	@echo "--------------------------------- Nettoyage de pigpio... ---------------------------------"
	$(MAKE) -C pigpio clean

# Combined clean for all
clean-all: clean clean-lidarLib clean-pigpio