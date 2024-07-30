CC = gcc
CFLAGS = -I/usr/include
LDFLAGS = -lX11 -lxkbfile

SRC_DIR = ./src
BUILD_DIR = ./build
INSTALL_DIR = $(DESTDIR)/usr/bin
MAN_INSTALL_DIR = $(DESTDIR)/usr/share/man/man1

TARGET = getxkblayout
SRC = $(SRC_DIR)/getxkblayout.c
OBJ = $(BUILD_DIR)/$(TARGET)

MAN_FILE = getxkblayout.1
MAN_SRC = $(SRC_DIR)/$(MAN_FILE)
MAN_DEST = $(MAN_INSTALL_DIR)/$(MAN_FILE)

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
	install -D -m 644 $(MAN_SRC) $(MAN_DEST)

uninstall:
	rm -f $(INSTALL_DIR)/$(TARGET)
	rm -f $(MAN_DEST)

clean:
	rm -f $(OBJ)
	rm -rf $(BUILD_DIR)
