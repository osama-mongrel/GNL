*This project has been created as part of the 42 curriculum by oben-el.*

# Get Next Line

## 📜 Description

**Get Next Line** is a foundational project at **1337** (part of the 42 Network). It challenges students to implement a function that reads and returns a line from a file descriptor, ensuring efficient memory usage and support for multiple file descriptors. This project is divided into a mandatory part and a bonus part.

---

## 🚀 Features

### Mandatory Part
- Reads from a file descriptor line by line.
- Handles customizable buffer sizes.
- Manages memory efficiently to prevent leaks.

### Bonus Part
- Supports multiple file descriptors simultaneously.
- Adds robustness for additional edge cases.

---

## 📂 Repository Structure

```plaintext
|-- get_next_line.c                 // Core function implementation
|-- get_next_line.h                 // Header file
|-- get_next_line_utils.c           // Helper functions
|-- get_next_line_bonus.c           // Bonus implementation for multiple file descriptors
|-- get_next_line_bonus.h           // Bonus header file
|-- get_next_line_utils_bonus.c     // Helper functions for the bonus part
```

---

## 🛠️ How to Use

### 1. Clone the Repository
```bash
git clone url get_next_line
cd Get_next_line
```

### 2. Integrate into Your Project

- For the **mandatory part**, use:
  - `get_next_line.c`
  - `get_next_line.h`
  - `get_next_line_utils.c`

- For the **bonus part**, use:
  - `get_next_line_bonus.c`
  - `get_next_line_bonus.h`
  - `get_next_line_utils_bonus.c`

### 3. Compile and Run

The `BUFFER_SIZE` must be defined during compilation using the `-D BUFFER_SIZE=<value>` flag.

#### Mandatory Part Compilation
```bash
gcc -D BUFFER_SIZE=42 your_main_file.c get_next_line.c get_next_line_utils.c -o your_program
```

#### Bonus Part Compilation
```bash
gcc -D BUFFER_SIZE=42 your_main_file.c get_next_line_bonus.c get_next_line_utils_bonus.c -o your_program
```

Replace `42` with your desired buffer size.

---

## 📖 Usage Example

Here’s a usage example for both the mandatory and bonus parts:

### Mandatory Example
```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void) {
    int fd = open("example.txt", O_RDONLY);
    char *line;

    while ((line = get_next_line(fd))) {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

### Bonus Example
```c
#include "get_next_line_bonus.h"
#include <fcntl.h>
#include <stdio.h>

int main(void) {
    int fd1 = open("file1.txt", O_RDONLY);
    int fd2 = open("file2.txt", O_RDONLY);
    char *line;

    while ((line = get_next_line(fd1))) {
        printf("File1: %s", line);
        free(line);
    }

    while ((line = get_next_line(fd2))) {
        printf("File2: %s", line);
        free(line);
    }

    close(fd1);
    close(fd2);
    return (0);
}
```

---

## Resources

- For file handling: https://www.codequoi.com/en/handling-a-file-by-its-descriptor-in-c/
- For Static variables: https://www.codequoi.com/en/local-global-static-variables-in-c/
- No AI was used here - only Ctrl+C and Ctrl+V.
