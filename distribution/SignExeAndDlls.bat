cd "C:\Users\Scott\Documents\Embarcadero\Studio\Projects\YahCoLoRiZe\B_English\YahCoLoRiZe"
dir
set /p pass=Enter Pass:
rem Remove digital signature:
rem "C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x86\signtool.exe" remove /?
rem "C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x86\signtool.exe" remove /s /v "Colorize.exe"
rem "C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x86\signtool.exe" remove /s /v "*.dll"
rem Add digital signature:
"C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x86\signtool.exe" sign /v /f "C:\Users\Scott\Documents\Embarcadero\Studio\Projects\YahCoLoRiZe\Cert\NewCert\DtsUWP.pfx" /p %pass% Colorize.exe
"C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x86\signtool.exe" sign /v /f "C:\Users\Scott\Documents\Embarcadero\Studio\Projects\YahCoLoRiZe\Cert\NewCert\DtsUWP.pfx" /p %pass% *.dll
pause
