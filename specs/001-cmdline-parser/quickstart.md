# Quick Start Guide: Command Line Parser Library

**Library Name**: `carsenal::cmdline_parser`  
**Version**: 1.0.0  
**Purpose**: Modern C++ command line argument parsing with fluent API

## Installation

### As CMake Subproject
```cmake
# In your CMakeLists.txt
add_subdirectory(path/to/carsenal/library/cmdline_parser)
target_link_libraries(your_target PRIVATE carsenal::cmdline_parser)
target_include_directories(your_target PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/carsenal/cmdline_parser)
```

### Header-Only Usage
```cpp
// Copy include/carsenal/cmdline_parser/*.hpp to your project
#include "carsenal/cmdline_parser/parser.hpp"
```

## Basic Usage

### Simple Program with Options
```cpp
#include <carsenal/cmdline_parser/parser.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    using namespace cars::cmdline_parser;
    
    // Create parser with builder pattern
    auto parser = App("myprogram", "A sample program")
        .add_option("input,i", "Input file path")
            ->required()
            ->check(FileExists())
        .add_option("output,o", "Output file path")
            ->default_value("output.txt")
        .add_flag("verbose,v", "Enable verbose output")
        .add_option("count", "Number of iterations")
            ->type<int>()
            ->default_value(1)
            ->check(Range(1, 100));
    
    // Parse command line
    auto result = parser.parse(argc, argv);
    
    if (!result) {
        std::cerr << "Error: " << result.error() << std::endl;
        std::cerr << parser.help() << std::endl;
        return 1;
    }
    
    // Access parsed values
    auto& values = result.value();
    std::string input = values.get<std::string>("input");
    std::string output = values.get<std::string>("output");
    bool verbose = values.get<bool>("verbose");
    int count = values.get<int>("count");
    
    // Use values...
    if (verbose) {
        std::cout << "Processing " << input << " to " << output 
                  << " (" << count << " iterations)" << std::endl;
    }
    
    return 0;
}
```

### Compile and Run
```bash
# Compile
g++ -std=c++17 -I/path/to/include main.cpp -o myprogram

# Run with help
./myprogram --help

# Run with options
./myprogram --input data.txt --output results.txt --verbose --count 5

# Short options work too
./myprogram -i data.txt -o results.txt -v -c 5
```

## Common Patterns

### Boolean Flags
```cpp
App("program")
    .add_flag("debug,d", "Enable debug mode")
    .add_flag("quiet,q", "Suppress output")
    .add_flag("force,f", "Force operation");
```

### Required vs Optional Options
```cpp
App("program")
    .add_option("required", "Must be provided")->required()
    .add_option("optional", "Can be omitted")->default_value("default");
```

### Type-Safe Parsing
```cpp
App("program")
    .add_option("int", "Integer value")->type<int>()
    .add_option("float", "Floating point")->type<double>()
    .add_option("string", "Text value")->type<std::string>()
    .add_option("path", "File path")->type<std::filesystem::path>();
```

### Validation
```cpp
App("program")
    .add_option("port", "Port number")
        ->type<int>()
        ->check(Range(1, 65535))
    .add_option("email", "Email address")
        ->check(Regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$"))
    .add_option("level", "Log level")
        ->check(Choice({"debug", "info", "warning", "error"}));
```

### Subcommands (Git-style)
```cpp
auto app = App("git", "Version control system");
    
auto clone = app.add_subcommand("clone", "Clone repository")
    .add_option("repository", "Repository URL")->required()
    .add_option("directory", "Target directory");
    
auto commit = app.add_subcommand("commit", "Record changes")
    .add_flag("all,a", "Commit all changed files")
    .add_option("message,m", "Commit message")->required();
    
auto push = app.add_subcommand("push", "Update remote refs")
    .add_option("remote", "Remote name")->default_value("origin")
    .add_flag("force", "Force push");
```

### Positional Arguments
```cpp
App("program", "Process files")
    .add_positional("files", "Files to process")
        ->type<std::vector<std::string>>()
        ->required();
```

## Error Handling

### Check Parse Result
```cpp
auto result = parser.parse(argc, argv);
if (!result) {
    // Handle specific error types
    if (result.error().type() == ErrorType::MissingRequired) {
        std::cerr << "Missing required option: " << result.error().option() << std::endl;
    } else if (result.error().type() == ErrorType::ValidationFailed) {
        std::cerr << "Validation failed: " << result.error().message() << std::endl;
    }
    return 1;
}
```

### Exception-Based Alternative
```cpp
try {
    auto values = parser.parse(argc, argv).value();
    // Use values...
} catch (const ParseError& e) {
    std::cerr << "Parse error: " << e.what() << std::endl;
    return 1;
} catch (const ConfigError& e) {
    std::cerr << "Configuration error: " << e.what() << std::endl;
    return 1;
}
```

## Advanced Features

### Custom Callbacks
```cpp
App("program")
    .add_option("config", "Configuration file")
        ->callback([](const std::string& path) {
            // Custom processing when option is parsed
            load_configuration(path);
        });
```

### Environment Variable Fallback
```cpp
App("program")
    .add_option("api-key", "API key for service")
        ->env("MYAPP_API_KEY")
        ->required();
```

### Configuration File Support
```cpp
auto parser = App("program")
    .add_option("config", "Config file")->default_value("config.ini");
    
// Can parse from config file
auto result = parser.parse_config("config.ini");
```

### Help Text Customization
```cpp
auto parser = App("program", "My awesome program")
    .version("1.2.3")
    .footer("See documentation at https://example.com/docs");
    
// Generate help
std::cout << parser.help() << std::endl;
```

## Integration Examples

### With Existing Code
```cpp
struct Config {
    std::string input;
    std::string output;
    bool verbose;
    int threads;
};

Config parse_args(int argc, char* argv[]) {
    auto parser = App("converter", "File format converter")
        .add_option("input,i", "Input file")->required()
        .add_option("output,o", "Output file")->required()
        .add_flag("verbose,v", "Verbose output")
        .add_option("threads,t", "Number of threads")->type<int>()->default_value(4);
    
    auto result = parser.parse(argc, argv);
    if (!result) {
        throw std::runtime_error(result.error().message());
    }
    
    auto& values = result.value();
    return Config{
        .input = values.get<std::string>("input"),
        .output = values.get<std::string>("output"),
        .verbose = values.get<bool>("verbose"),
        .threads = values.get<int>("threads")
    };
}
```

## Best Practices

1. **Always provide help**: Use `--help` flag automatically
2. **Validate early**: Use validation constraints for robust programs
3. **Use meaningful defaults**: Reduce required options when possible
4. **Support both long and short**: `--verbose` and `-v`
5. **Handle errors gracefully**: Provide clear error messages
6. **Test parsing**: Include command line parsing in your tests

## Next Steps

1. Explore the full API reference in header files
2. Check out example programs in `examples/` directory
3. Read the detailed architecture documentation
4. Contribute improvements via pull requests

## Getting Help

- API Documentation: `include/carsenal/cmdline_parser/` headers
- Examples: `library/cmdline_parser/examples/`
- Tests: `library/cmdline_parser/tests/`
- Issues: GitHub repository issues page