CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -O0 -Iinclude -Irplidar_sdk/sdk/include -Irplidar_sdk/sdk/src -Issd1306_oled_sdk/src -Issd1306_oled_sdk/include
LDFLAGS = -Lrplidar_sdk/output/Linux/Release
LDLIBS = -lsl_lidar_sdk -pthread -li2c -lpigpio -lrt -lpthread

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

.PHONY: all clean tests

all: lidarLib $(TARGET)
	@echo "Compilation terminée. Exécutez './$(TARGET)' pour exécuter le programme."

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

lidarLib:
	@echo "Compilation du sous-dossier lidarLib..."
	$(MAKE) -C rplidar_sdk

tests: $(TEST_TARGET)
	@echo "Exécution des tests..."
	./$(TEST_TARGET)

clean:
	rm -rf $(OBJDIR) $(TESTOBJDIR) bin/
