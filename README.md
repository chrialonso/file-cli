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

## Motivation

I built this project to explore C++ error handling patterns inspired by Rust's `Result<T, E>` while also exploring file system operations and regex pattern matching. After learning about Rust's approach to making errors explicit and impossible to ignore, I wanted to see how I could implement this for my work in progress CLI file manager.

The goal was to create a system where:
- Errors are values, not exceptions
- The compiler helps prevent bugs (using `[[nodiscard]]`)
- Function signatures tell you exactly what can go wrong

## Design Decisions
#### Type-Safe Error Handling
Instead of exceptions, I use `enum class` error types with explicit return values:
```cpp
std::pair<std::vector<std::string>, FileError> readFileLines(const std::string& filename);
```

**Benefits:**
- **Predictable control flow**: Errors are values, not control flow jumps
- **Better performance**: No exception overhead
- **Compile-time safety**: Can't accidentally mix different error types
- **Self-documenting**: Function signatures show they can fail

#### `[[nodiscard]]` Attribute
All functions returning errors or resources are marked `[[nodiscard]]`:
```cpp
[[nodiscard]]
FileError createFile(const std::string& filename);
```

This produces compiler warnings if you forget to check results:
```cpp
createFile("test.txt");  // Compiler warning: ignoring return value
```

Catches bugs at compile-time instead of runtime.

**Rust:**
```rust
fn read_file(path: &str) -> Result<String, IoError>
```

**My C++ equivalent:**
```cpp
std::pair<std::vector<std::string>, FileError> readFileLines(const std::string& filename)
```

Combined with `[[nodiscard]]`, this forces me to handle errors at compile-time, similar to how Rust's `?` operator makes error handling explicit.

#### Structured Bindings (C++17)
Unpacking of `std::pair` returns:
```cpp
auto [file, err] = parseCommand(input);
if (!handleInputError(err)) return;
```

More readable than `result.first` / `result.second`.

#### Filesystem Library
Uses `std::filesystem` (C++17) instead of platform-specific APIs:
- Cross-platform compatibility
- RAII-style error handling with `std::error_code`
- Type-safe path operations

### Architecture Patterns

#### Separation of Concerns
Each module has a single responsibility:
- `input_utils`: Input parsing and validation
- `file_utils`: File system operations
- `regex_utils`: Pattern matching logic
- `errors`: Error enums and handling
- `commands`: Main command dispatcher

#### Error Recovery
Three-tier error handling:
1. **Detection**: Functions return typed errors
2. **Display**: `matchXError()` functions provide user-friendly messages
3. **Recovery**: `handleXError()` functions return `bool` for control flow

## What I Learned

- Designing error handling systems without exceptions
- Working with `std::filesystem` and its error handling
- Using regex for pattern matching in a real application
- Structuring a multi-file C++ project with clear module boundaries
