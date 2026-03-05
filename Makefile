CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -pthread -I./deps

# rpath: чтобы бинарник находил .so рядом без LD_LIBRARY_PATH
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
