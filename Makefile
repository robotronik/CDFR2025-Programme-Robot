CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -O0 -static -Iinclude -Irplidar_sdk/sdk/include -Irplidar_sdk/sdk/src -Ipigpio
LDFLAGS = -Lrplidar_sdk/output/Linux/Release
LDLIBS = pigpio/pigpio.o pigpio/command.o -pthread -li2c -lrt -lpthread -lsl_lidar_sdk 

BINDIR = bin
TARGET = $(BINDIR)/programCDFR
TEST_TARGET = $(BINDIR)/tests
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

all: $(BINDIR) build_pigpio build_lidarLib $(TARGET) $(TEST_TARGET) copy_html
	@echo "Compilation terminée. Exécutez '(cd $(BINDIR) && ./programCDFR)' pour exécuter le programme."

$(TARGET): $(OBJ) | $(BINDIR)
	@echo "--------------------------------- Compilation du programme principal... ---------------------------------"
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(TEST_TARGET): $(OBJ_NO_MAIN) $(TEST_OBJ) | $(BINDIR)
	@echo "--------------------------------- Compilation des tests... ---------------------------------"
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

$(BINDIR):
	mkdir -p $@

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
PI_USER = robotronik
PI_HOST = raspitronik.local
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
	@echo "--------------------------------- Compilation du programme principal... ---------------------------------"
	$(ARM_CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)


# Deploy target
deploy: build_arm_lidarLib build_arm_pigpio $(ARM_TARGET) copy_html_arm
	@echo "--------------------------------- Deploiement vers le Raspberry Pi... ---------------------------------" 
	ssh $(PI_USER)@$(PI_HOST) 'mkdir -p $(PI_DIR)'
	rsync -av --progress ./$(ARMBINDIR) $(PI_USER)@$(PI_HOST):$(PI_DIR)

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




# Rule to copy the HTML directory to bin
copy_html: | $(BINDIR)
	cp -r html $(BINDIR)
# Rule to copy the HTML directory to the arm bin
copy_html_arm: | $(ARMBINDIR)
	cp -r html $(ARMBINDIR)


clean:
	@echo "--------------------------------- Nettoyage... ---------------------------------"
	rm -rf $(OBJDIR) $(TESTOBJDIR) $(ARM_OBJDIR) $(ARM_OBJDIR) $(BINDIR) $(ARMBINDIR)

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