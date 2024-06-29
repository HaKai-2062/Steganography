# Steganography Tool
![I1](https://raw.githubusercontent.com/HaKai-2062/Steganography/main/resources/github/main.png)

## What is steganography?
Steganography involves hiding data (mostly text) into an image file secretly to keep confidentiality of the message.

It works on the principle of least significant bit manipulation technique and data is stored (in last 2 bits of rgb color channels) sequentially without any encryption so it is quite easy to infer what is hidden.

> Currently, only supports 24 bit bitmap (bmp) and text, that has to be embedded, must be ASCII because each byte of character is taken at a time.

## How it works?
- Download the zip file from Releases section or build the project.
- Open Steganography.exe and there will be 2 modes that can be selected: Read and Write.
- In Write mode, any plain 24 bitmap can be used and data from text file can be embedded inside it. A new bitmap file is made suffixed by _steganography.bmp. The length of data stream given should be noted after writing to get the data back precisely.
- In Read mode, the embedded file (suffixed by _steganography.bmp) is taken and data length is provided along with an empty text file. The embedded data is written back into that text file.

## Note
This tool was made only for educational purposes. It may corrupt the file so make backup beforehand.

## Tools used
- [QT](https://www.qt.io/)
- [CMake](https://cmake.org/)
- [Visual Studio](https://visualstudio.microsoft.com/)

## Inspiration
[Secrets Hidden in Images (Steganography) - Computerphile](https://www.youtube.com/watch?v=TWEXCYQKyDc)

## Future Goal
- Usage of discrete cosine transform technique.
- Some encrpytion/decryption before storing data.