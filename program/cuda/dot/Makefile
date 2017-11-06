# export PATH := "/cygdrive/c/Program Files (x86)/Microsoft Visual Studio 12.0/VC/bin/":$(PATH)

INC_DIR := "c:\usr\work\sicp_exercise\cuda\book"

TARGET=dot

$(TARGET): $(TARGET).cu
	nvcc -o $@ $^ -I$(INC_DIR)

clean:
	rm -rf $(TARGET) $(TARGET).lib $(TARGET).exp
