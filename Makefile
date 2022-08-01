TARGET=firm_extract

all: $(TARGET)

$(TARGET): $(TARGET).o
	gcc $(TARGET).c -o $@

clean:
	rm -f $(TARGET)
	rm -f *.o
	rm -f new_ubi.bin
