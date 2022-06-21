rem http://timestamp.comodoca.com/authenticode
rem http://tsa.starfieldtech.com

cd "C:\Users\Scott\Documents\Embarcadero\Studio\Projects\YahCoLoRiZe\B_English\YahCoLoRiZe"
dir
"C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x86\signtool.exe" timestamp /t "http://timestamp.comodoca.com/authenticode" Colorize.exe
pause
"C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x86\signtool.exe" timestamp /t "http://timestamp.comodoca.com/authenticode" *.dll
pause
