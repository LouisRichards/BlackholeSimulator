# BlackholeSimulator

A modern C++ OpenGL application demonstrating SOLID principles and clean architecture for graphics programming.

## 🏗️ Architecture

This project showcases a well-structured OpenGL application that has been refactored from a monolithic design into a modular architecture following all five SOLID principles:

### SOLID Principles Implementation

- **Single Responsibility Principle (SRP)**: Each class has one reason to change
  - `WindowProperties`: Window configuration data only
  - `GLFWWindow`: GLFW window management only  
  - `BasicRenderer`: OpenGL rendering operations only
  - `Application`: Application coordination only

- **Open/Closed Principle (OCP)**: Open for extension, closed for modification
  - Abstract interfaces allow new implementations without changing existing code
  - New renderers can implement `IRenderer`
  - New window systems can implement `IWindow`

- **Liskov Substitution Principle (LSP)**: Derived classes are substitutable for base classes
  - `GLFWWindow` can replace `IWindow` anywhere
  - `BasicRenderer` can replace `IRenderer` anywhere

- **Interface Segregation Principle (ISP)**: Focused, specific interfaces
  - `IWindow`: Only window management methods
  - `IRenderer`: Only rendering methods

- **Dependency Inversion Principle (DIP)**: Depend on abstractions, not concretions
  - `Application` depends on `IWindow` and `IRenderer` interfaces
  - Dependencies are injected via constructor

## 📁 Project Structure

```
BlackholeSimulator/
├── main.cpp                    # Entry point with dependency injection
├── include/                    # Header files
│   ├── WindowProperties.h      # Window configuration structure
│   ├── IWindow.h              # Abstract window interface
│   ├── IRenderer.h            # Abstract renderer interface
│   ├── GLFWWindow.h           # GLFW window implementation
│   ├── BasicRenderer.h        # Basic OpenGL renderer
│   └── Application.h          # Main application coordinator
├── src/                       # Source files
│   ├── GLFWWindow.cpp         # GLFW window implementation
│   ├── BasicRenderer.cpp      # Basic renderer implementation
│   └── Application.cpp        # Application logic
└── .vscode/                   # VS Code configuration
    ├── tasks.json             # Build configuration
    └── c_cpp_properties.json  # IntelliSense configuration
```

## 🛠️ Building

### Prerequisites

- **MSYS2** with UCRT64 environment
- **GLEW** library: `pacman -S mingw-w64-ucrt-x86_64-glew`
- **GLFW** library: `pacman -S mingw-w64-ucrt-x86_64-glfw`
- **g++** compiler (included in MSYS2)

### Build Commands

#### Using VS Code
1. Open the project in VS Code
2. Press `Ctrl+Shift+P` and run "Tasks: Run Build Task"
3. The executable will be created as `BlackholeSimulator.exe`

#### Using Command Line
```bash
g++ -fdiagnostics-color=always -g \
    -I D:/Applications/msys64/ucrt64/include \
    -I . \
    main.cpp src/Application.cpp src/GLFWWindow.cpp src/BasicRenderer.cpp \
    -L D:/Applications/msys64/ucrt64/lib \
    -lglew32 -lglfw3 -lopengl32 -lgdi32 \
    -o BlackholeSimulator.exe
```

## 🚀 Running

After building, simply run:
```bash
./BlackholeSimulator.exe
```

You should see a window with a dark blue background, demonstrating the basic rendering pipeline.

## 📚 Documentation

The codebase includes comprehensive Doxygen documentation:

- **File-level documentation**: Purpose and overview of each file
- **Class-level documentation**: Role in SOLID architecture
- **Method documentation**: Parameters, return values, and behavior
- **Architecture explanations**: How SOLID principles are applied

### Key Concepts Explained

- **Double Buffering**: `swapBuffers()` prevents visual artifacts by swapping front/back buffers
- **Event-Driven Programming**: `pollEvents()` processes user input and window events
- **Dependency Injection**: Application accepts interfaces, not concrete implementations

## 🎯 Benefits of This Architecture

1. **Maintainability**: Easy to understand and modify individual components
2. **Testability**: Dependencies can be mocked for unit testing
3. **Extensibility**: Add new renderers or window systems without breaking existing code
4. **Reusability**: Components can be used in different contexts
5. **Loose Coupling**: Changes to one component don't affect others

## 🔄 Future Extensions

This architecture makes it easy to add:

- **New Renderers**: Vulkan renderer, Ray-tracing renderer, Software renderer
- **New Window Systems**: SDL window, Win32 window, Wayland window
- **Input Handling**: Keyboard and mouse input management
- **Scene Management**: 3D scene graphs and object hierarchies
- **Asset Loading**: Model and texture loading systems

## 🤝 Contributing

When contributing, please maintain the SOLID principles:

1. Keep classes focused on a single responsibility
2. Use interfaces for extensibility
3. Inject dependencies rather than hard-coding them
4. Write comprehensive documentation
5. Follow the established naming conventions

## 📄 License

This project is open source and available under the MIT License.
A small blackhole simulator in cpp
