*This project has been created as part of the 42 curriculum by oben-el.*

# get_next_line

## Description

**get_next_line** is a C function that returns one line at a time from a file descriptor:

```c
char *get_next_line(int fd);
```

Calling it repeatedly in a loop reads the whole content of the file descriptor, one line per call, until it returns `NULL`.

- The returned line **includes the terminating `\n`**, except for the last line of a file that does not end with a newline.
- It returns `NULL` when there is nothing left to read, or when an error occurs (invalid fd, `read()` failure, allocation failure).
- It works on regular files as well as on the standard input and other file descriptors such as pipes.
- Only `read`, `malloc` and `free` are used. There are no global variables, no `lseek`, and no `libft`.

The goal of the project is to learn about **static variables** (state that survives between calls), **file descriptors**, **buffered reading** with `read()`, and **dynamic memory management**.

The **bonus** version does the same thing but can read from **several file descriptors at the same time**, using **only one static variable**. You can alternate between fd 3, 4 and 5 and each one keeps its own reading position.

### Files

| File | Content |
|------|---------|
| `get_next_line.c` | Mandatory: `get_next_line()` and its reading / parsing helpers |
| `get_next_line_utils.c` | Mandatory: string helpers (`ft_strlen`, `ft_strjoin`, `ft_strchr`) |
| `get_next_line.h` | Mandatory: prototypes, includes, and the default `BUFFER_SIZE` |
| `get_next_line_bonus.c` | Bonus: multi-fd version of `get_next_line()` |
| `get_next_line_utils_bonus.c` | Bonus: string helpers |
| `get_next_line_bonus.h` | Bonus: prototypes, includes, and the default `BUFFER_SIZE` |

## Instructions

### Compilation

The buffer size is set at compile time with `-D BUFFER_SIZE=n`. If the flag is omitted, a default value of `42` is used.

Mandatory part:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c
```

Bonus part:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main.c
```

Without the flag, the default `BUFFER_SIZE` of 42 is used:

```bash
cc -Wall -Wextra -Werror get_next_line.c get_next_line_utils.c main.c
```

The files for the mandatory part and the bonus part must not be compiled together, since they define the same function names.

### Usage example

```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

int main(int argc, char **argv)
{
    int     fd;
    char    *line;

    if (argc != 2)
        return (1);
    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
        return (1);
    while ((line = get_next_line(fd)))
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

Reading from the standard input works the same way with `fd = 0`.

With the bonus version (include `get_next_line_bonus.h`), calls on different file descriptors can be mixed freely:

```c
line = get_next_line(fd1);   /* first line of file 1  */
line = get_next_line(fd2);   /* first line of file 2  */
line = get_next_line(fd1);   /* second line of file 1 */
```

The caller owns each returned line and must `free()` it.

### Testing

Both versions were tested with several values of `BUFFER_SIZE` (1, 2, 3, 5, 42, 9999, 10000000) on:

- an empty file, a file containing only `\n`, and several consecutive `\n`;
- files with and without a final newline;
- a 10,000-character line and a file with 5,000 lines;
- the standard input (pipe);
- an invalid fd (`-1`, a closed fd) and a directory (read error);
- repeated calls after the end of the file;
- memory errors and leaks, using `-fsanitize=address,undefined`.

Additional tests for the bonus version:

- three files read in an interleaved way, in forward and reverse order, with one file ending without a newline;
- files read at the same time as the standard input;
- 200 file descriptors open at once and read alternately;
- fds outside the supported range (`-1`, `1024`).

## Algorithm

### Mandatory part

The function keeps the data it has read but not yet returned in a single `static char *saved`. Each call does three things:

1. **Read until a line is available** (`read_from_file`).
   A buffer of `BUFFER_SIZE + 1` bytes is allocated. While `saved` does not contain a `\n` (checked with `ft_strchr`), `read()` is called and the freshly read chunk is appended to `saved` with `ft_strjoin`. The loop stops as soon as a `\n` is stored, or when `read()` returns `0` (end of file). A return of `-1` frees everything and the function returns `NULL`. The buffer is freed on every exit path.

2. **Extract the line** (`get_line`).
   A new string is allocated and filled with the characters of `saved` up to and including the first `\n` (or up to the end of the string if there is none).

3. **Keep the remainder** (`get_rest`).
   Everything after the returned line is copied to a new string that replaces `saved`. If nothing remains, `saved` is freed and set to `NULL`, which is what makes the next call return `NULL` at the end of the file.

### Bonus part

The bonus version keeps the same three steps, but the single static variable becomes an array of pointers:

```c
static char *saved[1024];
```

`saved[fd]` holds the leftover data of file descriptor `fd`. Every step of the algorithm works on `saved[fd]` only, so reading one file descriptor never touches the state of another. Since this is still **one** static variable, the "only one static variable" constraint is respected. A call with `fd < 0` or `fd >= 1024` returns `NULL`.

### Why this design

- **A static variable is the only way to remember the leftover data** between two calls without a global variable. Since `read()` returns fixed-size chunks that usually do not stop on a line boundary, the part of the chunk that follows the `\n` must be kept for the next call.
- **Reading stops as soon as a newline is found.** The function never reads the whole file in advance, as the subject asks, so each call reads as little as possible.
- **Any `BUFFER_SIZE` works.** With `1`, `read()` is called once per byte and the loop simply runs more often. With a very large value, a single `read()` can return several lines (or the whole file), and the extra data stays in `saved` for the following calls. The line length is independent of `BUFFER_SIZE` because `saved` grows through `ft_strjoin`.
- **An array indexed by the fd keeps the bonus simple.** Looking up the state of a file descriptor is a direct array access, so there is no list to search or to clean up. The cost is a fixed 1024-entry array (8 KB on a 64-bit system), which matches the usual per-process limit on open file descriptors.
- **Cleanup is explicit on every path.** On a read error, an allocation failure, or the end of the file, the buffer and `saved` are freed and `saved` is reset to `NULL`, so the next call starts from a clean state.
- **Trade-off:** `ft_strjoin` allocates a new string on each chunk, so a very long line read with a small `BUFFER_SIZE` is copied many times (quadratic cost). This keeps the code simple and the ownership of memory clear.

## Resources

### References

- `man 2 read`, `man 3 malloc`, `man 3 free`, `man 2 open`
- *The C Programming Language*, Kernighan & Ritchie: static storage duration
- GNU C Library manual: low-level input/output and file descriptors
- 42 subject: *Get Next Line* (v14.3)
- AddressSanitizer / LeakSanitizer documentation (Clang/GCC)

### Use of AI

AI (Claude) was used **only for verification and documentation**, not to design or write the algorithm:

- **Testing:** generating test programs (edge cases, several `BUFFER_SIZE` values, invalid fds, stdin, and several file descriptors read in an interleaved way) and running them with AddressSanitizer/LeakSanitizer and the Norm checker. This helped to spot compiler errors (an unused variable, and an array assignment in the bonus) and a memory leak in my own code.
- **Documentation:** drafting the structure of this README. I reviewed and corrected it to match my implementation.

The `get_next_line` code itself (static-variable logic, helpers, memory handling) was written by me, and I am able to explain every part of it.
