# Build Notes

Verified on Windows with Qt 6.11.1 MinGW 64-bit.

Command used:

```powershell
$env:PATH='C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.11.1\mingw_64\bin;' + $env:PATH
C:\Qt\6.11.1\mingw_64\bin\qmake.exe FlightBookingQt.pro -o Makefile
C:\Qt\Tools\mingw1310_64\bin\mingw32-make.exe -j2
```

Result: build succeeded and produced `release\FlightBookingQt.exe` during verification.

The generated Makefile/release/debug files were removed from the submitted source folder because they are build artifacts.