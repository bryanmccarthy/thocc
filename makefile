CC := gcc

CFLAGS := -framework IOKit -framework CoreFoundation -framework AudioToolbox

SOURCES := main.c

OUT := main

all: $(OUT)

$(OUT) := $(CC) -o $(OUT) $(SOURCES) $(CFLAGS)

clean:
	rm -f $(OUT)
