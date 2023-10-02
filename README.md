# tail
## A demonstration implementation of the Linux tail command.

This shows an example of low level operating system programming that I use for instructional purposes.

There are two options:
- Output the last N lines of a file.
- Continuously output the content of a file that is growing.

### Output Last N Lines

1. Position to the end of the file (without reading anything)
2. Start reading backwards, counting `newline` characters
3. When enough lines have been read, output the lines

The key is **not** reading all the contents of the file, which
yields a large performance gain which increases as the file size grows.

### Continuously Output Lines as a File Grows

1. Read and output the contents of the file until the end is reached
2. Sleep for a while
3. Reset the `end-of-file` indication so that Step 1 can resume

