# steganography
# C Steganography

A C-based image steganography project that uses the **Least Significant Bit (LSB)** technique to hide secret data inside BMP images.

## 📌 Overview

Steganography is the technique of hiding information inside another medium without visibly changing the original content.

This project implements image steganography in **C programming** by modifying the Least Significant Bits of pixel data in a BMP image to store secret information.

The project focuses on understanding:

* File handling in C
* Binary file operations
* BMP image structure
* Bitwise operations
* Least Significant Bit (LSB) encoding
* Structures and enumerations
* Modular C programming

## ⚙️ How It Works

The encoding process follows these basic steps:

```text
Secret File
     │
     ▼
Read Secret Data
     │
     ▼
Read BMP Image
     │
     ▼
Check Image Capacity
     │
     ▼
Encode Data into LSBs
     │
     ▼
Generate Stego Image
```

The secret information is embedded into the BMP image by modifying the least significant bits of image bytes.

Because only the least significant bits are modified, the visual appearance of the image remains almost unchanged.

## 🧩 Project Structure

```text
steganography/
│
├── encode.c
├── encode.h
├── common.h
├── types.h
├── test_encode.c
├── beautiful.bmp
├── secret.txt
└── README.md
```

### File Description

| File            | Description                                          |
| --------------- | ---------------------------------------------------- |
| `encode.c`      | Contains encoding-related function implementations   |
| `encode.h`      | Contains encoding structures and function prototypes |
| `common.h`      | Contains common definitions such as the magic string |
| `types.h`       | Contains user-defined data types and status enums    |
| `test_encode.c` | Test/driver source for the encoding project          |
| `beautiful.bmp` | BMP image used for testing                           |
| `secret.txt`    | Sample secret data used for testing                  |

## 🔧 Technologies Used

* **Language:** C
* **Concept:** Image Steganography
* **Technique:** Least Significant Bit (LSB)
* **Image Format:** BMP
* **Development Environment:** Linux / VS Code
* **Version Control:** Git & GitHub

## 🧠 C Concepts Used

This project provides practical experience with:

* Structures
* Enumerations
* Pointers
* File pointers
* `fopen()`
* `fread()`
* `fwrite()`
* `fseek()`
* File handling
* Bitwise operations
* Command-line arguments
* Function prototypes
* Modular programming

## 🚀 Current Implementation

The current implementation includes functionality for:

* Opening source image, secret file and stego image files
* Reading BMP image dimensions
* Calculating image capacity
* Maintaining encoding information using `EncodeInfo`
* Defining encoding-related function interfaces
* Defining project status and operation types

## 🛠️ Future Improvements

Planned improvements include:

* Complete secret data encoding
* Magic string encoding
* Secret file extension encoding
* Secret file size encoding
* Secret file data encoding
* BMP header handling
* Remaining image data copying
* Complete decoding functionality
* Error handling and validation
* Testing with different BMP images and secret files

## 🎯 Learning Objective

The main objective of this project is to strengthen **C programming, file handling, pointers, structures, bitwise operations, and low-level data manipulation** through a practical embedded/software-oriented project.

## 👨‍💻 Author

**Jeeva**

Aspiring Embedded Systems Engineer
C | Embedded C | Embedded Linux | Firmware Development

## 📄 License

This project is intended for educational and learning purposes.
