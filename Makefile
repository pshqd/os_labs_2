CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -pthread -I./deps

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
    RPATH = -Wl,-rpath,@loader_path/deps
else
    RPATH = -Wl,-rpath,'$$ORIGIN/deps'
endif

LDFLAGS  = -L./deps -lcaesar -pthread $(RPATH)

TARGET   = securecopy
SRCS     = src/main.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

test: all
	echo "Hello World" > /tmp/test_in.txt
	./securecopy /tmp/test_in.txt /tmp/test_enc.txt K
	./securecopy /tmp/test_enc.txt /tmp/test_dec.txt K
	diff /tmp/test_in.txt /tmp/test_dec.txt && echo "TEST PASSED" || echo "TEST FAILED"
