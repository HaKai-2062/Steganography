# Steganography Tool

## Note
This tool was made only for educational purposes. It may corrupt the file so make backup beforehand.

## How it works?
Steganography involves hiding data (mostly text) into an image file secretly to keep confidentiality of the message.

It works on the principle of least significant bit manipulation technique and data is stored (in last 2 bits of rgb color channels) sequentially without any encryption so it is quite easy to infer what is hidden.

> Currently, only supports 24 bit bitmap (bmp) and text that has to be embedded must be ASCII because each byte of character is taken at a time.

## Tools used
- [QT](https://www.qt.io/)
- [CMake](https://cmake.org/)
- [Visual Studio](https://visualstudio.microsoft.com/)

## Inspiration
[Secrets Hidden in Images (Steganography) - Computerphile](https://www.youtube.com/watch?v=TWEXCYQKyDc)

## Future Goal
- Usage of discrete cosine transform technique.
- Some encrpytion/decryption before storing data.