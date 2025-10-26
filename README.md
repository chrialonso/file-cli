## Features

- **File Operations**: Create, read, edit, and delete files
- **Smart Search**: Find files by name or search content using regex patterns
- **Interactive Editing**: Append content to files with a simple line-by-line editor
- **Robust Error Handling**: Detailed error messages for file system, input, and regex operations

## Commands

| Command | Description |
|---------|-------------|
| `help` | Display all available commands |
| `list` | List all files in current directory |
| `create [filename]` | Create a new file |
| `read [filename]` | Display file contents with line numbers |
| `add [filename]` | Append content to a file interactively |
| `find [pattern]` | Find files matching a regex pattern |
| `search [pattern]` | Search file contents recursively for a pattern |
| `delete [filename]` | Delete a file |
| `exit` | Exit the program |

### Example Session
```
> create notes.txt
[SUCCESS] File 'notes.txt' created.

> add notes.txt
Editing file: 'notes.txt'
Enter a blank space to exit.
> This is my first note
> This is my second note
>  

> read notes.txt
[INFO] Reading file: 'notes.txt'
1: This is my first note
2: This is my second note
[INFO] Total lines: 2

> search note
[INFO] This will search all files recursively, continue? (y/n): y

/home/user/projects/file-cli/notes.txt
1: This is my first note
2: This is my second note


```
