# export PATH := "/cygdrive/c/Program Files (x86)/Microsoft Visual Studio 12.0/VC/bin/":$(PATH)

TARGET=simple_kernel

$(TARGET): $(TARGET).cu
	nvcc -o $@ $^

clean:
	rm -rf $(TARGET) $(TARGET).lib $(TARGET).exp
