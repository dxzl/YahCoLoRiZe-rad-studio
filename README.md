# YahCoLoRiZe-rad-studio
Port of the YahCoLoRiZe IRC-chat color-text processor to Embarcadero RAD Studio C++ Builder for Windows.

This project builds with Embarcadero RAD Studio 10.4 Community Edition

Free compiler: [Embarcadero C++ Builder 10.4 Community Edition](https://www.embarcadero.com/products/cbuilder/starter)

The new edit component that goes with YahCoLoRiZe-rad-studio is YcEdit-rad-studio. There are a lot of demo, help, spelling dictionaries and other ancillary files for YahCoLoRiZe that are not included here but are in the old YahCoLoRiZe repository.

The project first release is 7.48 and I've solved the major performance issue. It was traced to the TProgressBar. For RAD Studio I found that I need to check to see if the progress position has actually changed before setting the Position property.

To build YahCoLoRiZe, first install C++ Builder Community: http://docwiki.embarcadero.com/RADStudio/Rio/en/Community_Edition

Download https://github.com/dxzl/YcEdit-rad-studio component. In RAD Studio, load the YcEditPackage.cbproj project-file. Right-click on YcEditPackage.bpl at the right and choose Install from the pop-up menu.

Download this repository. Load Colorize.cbproj into RAD Studio and build it. The output exe file will be in Win32\Release.