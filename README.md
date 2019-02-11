# YahCoLoRiZe-rad-studio
Port of the YahCoLoRiZe IRC-chat color-text processor to Embarcadero RAD Studio C++ Builder 10.2 Rio (Community edition for Windows)

YahCoLoRiZe was originally ANSI only and then I ported it to Unicode the hard way, using an old development environment (Borland C++ Builder 4). When the free version of RAD Studio (Tokyo) came out, I started to port the project but it was fairly daunting. I had to build a new Edit control and scrap the old TaeRichEdit component. the new edit component that goes with YahCoLoRiZe-rad-studio is YcEdit-rad-studio. There are a lot of demo, help, spelling dictionaries and other ancillary files for YahCoLoRiZe that are not included here but are in the old YahCoLoRiZe repository.

The project first release is 7.48 and I've solved the major performance issue. It was traced to the TProgressBar. For RAD Studio I found that I need to check to see if the progress position has changed before setting the property. Otherwise; it eats huge amounts of processing time.

