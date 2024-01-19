rbxaltunlocker:.\src\main.cpp .\res\rbxaltunlocker.ico
	cl /O2 /DNDEBUG /EHsc /Feout\rbxaltunlocker.exe /Foout\rbxaltunlocker.obj ./src/main.cpp ./res/icon.res
icon:.\res\icon.rc .\res\rbxaltunlocker.ico
	rc ./res/icon.rc