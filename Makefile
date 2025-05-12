CC = gcc
CFLAGS = -Wall -g #warnings and debug info
LDFLAGS =
RM = rm -f

SRC_DIR = src
SOURCES = $(SRC_DIR)/System.c
EXECUTABLE = restaurant_system

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	@echo "Building $(EXECUTABLE) from $(SOURCES)..."
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE) $(LDFLAGS)
	@echo "Project built: $(EXECUTABLE)"


clean:
	@echo "Cleaning..."
	$(RM) $(EXECUTABLE) *.o
	@echo "Cleanup complete."

