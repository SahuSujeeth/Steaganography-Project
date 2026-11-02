# LSB-Based Image Steganography in C

## Project Overview

LSB-Based Image Steganography is a C programming project that demonstrates how secret information can be hidden inside a BMP image using the **Least Significant Bit (LSB)** technique.

The project supports both encoding and decoding:
- **Encoding:** Hides a secret file inside a BMP image.
- **Decoding:** Extracts the hidden file from the stego image and reconstructs the original secret file.

The primary objective is to understand bitwise operations, file handling, pointers, structures, and command-line arguments through a practical C project.

## How It Works

The Least Significant Bit (LSB) technique modifies the least significant bit of image data to store secret information.

For example:

```text
Original Pixel Byte: 01100100
Modified Pixel Byte: 01100101
```

Only the least significant bit changes, resulting in a very small pixel value difference that is generally not visually noticeable.

The project stores secret information in the image using LSB encoding and retrieves it through decoding.

## Features

- Encode secret files into BMP images.
- Decode and recover hidden secret files.
- Store and retrieve secret file extensions.
- Store and retrieve secret file sizes.
- Preserve the original secret file contents.
- Support text and binary secret files.
- Command-line based execution.

## Technologies Used

- C Programming
- File Handling
- Bitwise Operators
- Pointers
- Structures
- Command-Line Arguments
- BMP Image Processing

## Project Structure

```text
LSB-Based-Image-Steganography/
│
├── encode.c
├── encode.h
├── decode.c
├── decode.h
├── common.h
├── types.h
├── test_encode.c
├── secret.txt
├── beautiful.bmp
├── output.bmp
└── README.md
```

## Compilation

Compile the project using GCC:

```bash
gcc decode.c encode.c test_encode.c -o steganography
```

## Encoding

### Command Format

```bash
./steganography -e <source_image.bmp> <secret_file> <stego_image.bmp>
```

### Example

```bash
./steganography -e beautiful.bmp secret.txt output.bmp
```

### Output

The secret file is embedded inside the generated `output.bmp` image.

## Decoding

### Command Format

```bash
./steganography -d <stego_image.bmp> <output_filename>
```

### Example

```bash
./steganography -d output.bmp result
```

The recovered file will be created using the decoded secret file extension.

For example:

```text
result.txt
```

## Verification

Compare the original secret file with the recovered file:

```bash
cmp secret.txt result.txt
```

If the command produces no output, both files are identical byte by byte.

## Example Secret Message

```text
Bro really decoded my secrets.
```

## Learning Outcomes

Through this project, I gained practical experience in:

- Understanding LSB image steganography.
- Performing bitwise operations for data encoding and decoding.
- Working with file pointers and binary file operations.
- Using `fread()`, `fwrite()`, `fseek()`, and `fgetc()`.
- Implementing command-line argument validation.
- Handling secret file sizes and extensions.
- Understanding BMP image structure and pixel data.

## Future Improvements

- Support additional image formats such as PNG.
- Improve validation for different BMP formats.
- Develop a graphical user interface.
- Enhance error handling and image capacity validation.

## Author

**Sujeeth**

ECE Student | C Programming | Embedded Systems Enthusiast

## Acknowledgment

This project was developed as part of my C programming learning journey at ECEP / Emertxe.

---

⭐ If you find this project interesting, feel free to explore the repository!