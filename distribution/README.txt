Unzip YahCoLoRiZe.zip and Release.zip folders...

YahCoLoRiZe Windows MSI Installer Project:

Use Visual Studio to build the installer, and Embarcadero C++ Builder Community to build YahCoLoRiZe.

In YahCoLoRiZe Installer.vdproj set:

DetectNewerInstalledVersions=FALSE
RemovePreviousVersions=TRUE

(When you re-run the same YahCoLoRiZe32.msi that's already installed we get nice prompt to repair or uninstall... OK!)

Be sure to copy the new Colorize.exe into this folder...

To update to a new version, change Version (say from 1.73.0 to 1.74.0) AND when it prompts to change the product code select YES.
The compiled version will install over the previous version.

If only updating the installer, not the main program, you would change 1.74.0 to 1.74.1 and get a new product code.

!!!! You MUST copy/paste the new product-code into the arguments field of the uninstall shortcut!!!!
(right-click the "YahCoLoRiZe Installer" project, choose View->File System, click User's Programs Menu->YahCoLoRiZe then the Uninstall YahCoLoRiZe Shortcut)

First run SignExeAndDlls.bat to sign, then TimestampExesAndDlls.bat (online), then run SignInstaller.bat then TimestampInstaller.bat (online).

Modify the .bat files as needed to fit your installation. You will need Windows signtool.exe which is in the Windows 10 SDK.
