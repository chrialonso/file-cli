## Features

- **File Operations**: Create, read, edit, and delete files
- **Smart Search**: Find files by name or search content using regex patterns
- **Configurable Search**: Control search behavior with flags (file size limits, match counts, directory depth)
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
| `search [pattern] [flags]` | Search file contents recursively with optional flags |
| `delete [filename]` | Delete a file |
| `exit` | Exit the program |

### Example Session
```
> create notes.txt
[INFO] File 'notes.txt' created.

> add notes.txt
Editing file: 'notes.txt'
Enter !exit to exit.
> This is my first note
> This is my second note
>  

> read notes.txt
[INFO] Reading file: 'notes.txt'
1: This is my first note
2: This is my second note
[INFO] Total lines: 2

> search TODO

/home/user/project/src/main.cpp
15: // TODO: Add error handling
42: // TODO: Refactor this function

> search myFunction() --max-file-size=2MB --max-depth=2
[results]

> search ERROR --max-global-matches=5 --max-matches-per-file=2
[results]

> find \.cpp$
./src/main.cpp
./src/commands.cpp
./src/errors.cpp
```

## Search Flags
The `search` command supports optional flags for fine-grained control:

| Flag | Description | Default |
|------|-------------|---------|
| `--max-file-size=<size><unit>` | Skip files larger than specified (KB, MB, GB) | 5MB |
| `--max-global-matches=<number>` | Maximum total matches across all files | 500 |
| `--max-matches-per-file=<number>` | Maximum matches to show per file | 500 |
| `--max-depth=<number>` | Maximum directory recursion depth | unlimited |

Use `help search` for detailed flag information.

## Motivation

I built this project to explore C++ error handling patterns inspired by Rust's `Result<T, E>` while also exploring file system operations and regex pattern matching. After learning about Rust's approach to making errors explicit and impossible to ignore, I wanted to see how I could implement this for my work in progress CLI file manager.

The goal was to create a system where:
- Errors are values, not exceptions
- The compiler helps prevent bugs (using `[[nodiscard]]`)
- Function signatures tell you exactly what can go wrong

## Design Decisions
### Type-Safe Error Handling
Instead of exceptions, I use `enum class` error types with explicit return values:
```cpp
std::pair<std::vector<std::string>, FileError> readFileLines(const std::string& filename);
```

**Benefits:**
- **Predictable control flow**: Errors are values, not control flow jumps
- **Better performance**: No exception overhead
- **Compile-time safety**: Can't accidentally mix different error types
- **Self-documenting**: Function signatures show they can fail

### `[[nodiscard]]` Attribute
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

### Structured Bindings (C++17)
Unpacking of `std::pair` returns:
```cpp
auto [file, err] = parseCommand(input);
if (!handleInputError(err)) return;
```
More readable than `result.first` / `result.second`.

### Zero-Copy String Handling
Used `std::string_view` for efficient string parsing without unnecessary copies, inspired by Rust's `&str`:

**Rust:**
```rust
fn extract_query(s: &str) -> &str;
fn tokenize(s: &str) -> Vec<&str>;
```
**My C++ equivalent:**
```cpp
std::string_view extractQuery(std::string_view sv);
std::vector<std::string_view> tokenize(std::string_view sv);
```

**Benefits:**
- **Zero allocations**: Views into existing strings instead of copies
- **Faster parsing**: No memory allocation overhead for temporary strings
- **Non-owning semantics**: Like Rust's borrows, views don't own their data
- **Read-only safety**: Can't accidentally modify the original string


### Filesystem Library
Uses `std::filesystem` (C++17) instead of platform-specific APIs:
- Cross-platform compatibility
- RAII-style error handling with `std::error_code`
- Type-safe path operations
  
### Flag Parsing System

Implemented a custom flag parser for the search command that handles:
- Multiple flags per command
- Value + unit parsing (`--max-file-size=5MB`)
- Numeric-only values (`--max-depth=3`)
- Validation and error reporting
  
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
- Building a flexible command-line argument parser
- Using `std::string_view` to avoid allocations, similar to Rust's `&str`
- Structuring a multi-file C++ project with clear module boundaries
