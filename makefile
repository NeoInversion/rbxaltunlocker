CC := $(shell echo g++||echo cl)
RC := $(shell echo windres||echo rc)

ifeq ($(CC),g++)
	CXXFLAGS := -o ./out/rbxaltunlocker.exe -O3 -Wl,--subsystem,windows ./src/main.cpp ./res/icon.res 
else ifeq ($(CC),cl)
	CXXFLAGS := /Fe:out\rbxaltunlocker.exe /Fo:out\main.obj /O2 /DNDEBUG /EHsc ./src/main.cpp ./res/icon.res
endif

ifeq ($(RC),windres)
	RCFLAGS := ./res/icon.rc -O coff -o ./res/icon.res
else ifeq ($(RC),rc)
	RCFLAGS := ./res/icon.rc
endif

rbxaltunlocker: icon
	$(CC) $(CXXFLAGS)
icon:
	$(RC) $(RCFLAGS)