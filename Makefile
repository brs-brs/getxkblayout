CC = gcc
CFLAGS = -I/usr/include
LDFLAGS = -lX11 -lxkbfile

SRC_DIR = ./src
BUILD_DIR = ./build
INSTALL_DIR = $(DEST_DIR)/usr/bin

TARGET = getxkblayout
SRC = $(SRC_DIR)/getxkblayout.c
OBJ = $(BUILD_DIR)/$(TARGET)

DEPENDENCY_HEADER = /usr/include/X11/extensions/XKBrules.h

.PHONY: all install uninstall clean check-dependency

all: check-dependency $(OBJ)

check-dependency:
	@if [ ! -f $(DEPENDENCY_HEADER) ]; then \
		echo "Error: $(DEPENDENCY_HEADER) not found."; \
		echo "Please install the necessary dependency."; \
		echo "You can install it using: sudo apt-get install libxkbfile-dev"; \
		exit 1; \
	fi

$(OBJ): $(SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(OBJ) $(SRC) $(LDFLAGS)

install: $(OBJ)
	install -D $(OBJ) $(INSTALL_DIR)/$(TARGET)

uninstall:
	rm -f $(INSTALL_DIR)/$(TARGET)

clean:
	rm -f $(OBJ)
	rm -rf $(BUILD_DIR)
