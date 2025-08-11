# BlackholeSimulator

A sophisticated 3D spacetime visualization application built with modern C++ and OpenGL, demonstrating SOLID principles and clean architecture.

## 🌌 Features

- **3D Spacetime Curvature Visualization**: Interactive grid showing gravitational field strength with proper depth warping
- **Multiple Camera Modes**: Free-flight and game-style camera controls with smooth transitions
- **Real-time Gravity Simulation**: Physics-based gravitational body interactions
- **Interactive UI**: Camera mode switching with readable line-based text rendering
- **SOLID Architecture**: Clean, maintainable, and extensible codebase following all five SOLID principles

## 🎮 Controls

- **M**: Open camera mode menu
- **Mouse**: Hold left button and drag to rotate camera
- **W/S**: Move forward/backward
- **A/D**: Move left/right  
- **Q/E**: Move up/down (Free-flight) or adjust eye height (Game-style)
- **Shift+W/S**: Zoom in/out (Free-flight mode only)
- **ESC**: Exit application

## 🏗️ Architecture

Built following SOLID principles with clear separation of concerns:

### Core Components

- **Camera System**: Interface-based camera implementations (Free-flight, Game-style) with `CameraController`
- **Gravity Renderer**: 3D spacetime visualization with aligned grid and body positioning
- **UI Renderer**: 2D overlay system with interactive menus using line-based text rendering
- **Physics Simulation**: Gravitational body management and force calculations

### SOLID Principles Implementation

- **Single Responsibility**: Each class handles one specific aspect (rendering, camera, UI, physics)
- **Open/Closed**: Easy to extend with new camera types or renderers without modifying existing code
- **Liskov Substitution**: Camera implementations are fully interchangeable through interfaces
- **Interface Segregation**: Focused interfaces (`ICamera`, `IRenderer`, `IWindow`) for specific needs
- **Dependency Inversion**: High-level modules depend on abstractions, not concrete implementations

## 📁 Project Structure

```
BlackholeSimulator/
├── main.cpp                    # Modern application entry point with dependency injection
├── include/                    # Header files
│   ├── Camera.h               # Camera system interfaces and implementations
│   ├── GravityRenderer.h      # 3D spacetime visualization renderer
│   ├── UIRenderer.h           # 2D UI overlay system with line-based text
│   ├── GravityBody.h          # Physics body representation
│   ├── GravityGrid.h          # Gravitational field grid calculations
│   ├── GravitySimulation.h    # Physics simulation coordinator
│   ├── Application.h          # Main application framework
│   ├── GLFWWindow.h           # GLFW window management
│   └── [Supporting files]     # Vec2.h, WindowProperties.h, interfaces
├── src/                       # Implementation files
│   ├── Camera.cpp             # Camera system with Free-flight and Game-style modes
│   ├── GravityRenderer.cpp    # 3D rendering with coordinate-aligned visualization
│   ├── UIRenderer.cpp         # UI rendering with line-based character system
│   ├── GravityBody.cpp        # Physics body behavior and properties
│   ├── GravityGrid.cpp        # Grid calculations and gravitational field strength
│   ├── GravitySimulation.cpp  # Simulation management and updates
│   └── [Supporting files]     # Application.cpp, GLFWWindow.cpp, BasicRenderer.cpp
├── .gitignore                 # Comprehensive build and IDE file exclusions
├── README.md                  # This documentation
└── SOLID_REFACTORING.md       # Detailed architecture evolution
```

## 🛠️ Building

### Prerequisites

- **MSYS2** with UCRT64 environment
- **GLEW**: `pacman -S mingw-w64-ucrt-x86_64-glew`
- **GLFW**: `pacman -S mingw-w64-ucrt-x86_64-glfw`
- **OpenGL**: Included with graphics drivers

### Build Process

#### Using VS Code (Recommended)
1. Open project in VS Code
2. Use the configured build task (`Ctrl+Shift+P` → "Tasks: Run Build Task")
3. Run `.\BlackholeSimulator.exe`

#### Command Line
```bash
g++ -fdiagnostics-color=always -g \
    -I D:/Applications/msys64/ucrt64/include \
    -I . \
    main.cpp src/*.cpp \
    -L D:/Applications/msys64/ucrt64/lib \
    -lglew32 -lglfw3 -lopengl32 -lgdi32 \
    -o BlackholeSimulator.exe
```

## 🎯 What You'll See

- **3D Warped Grid**: Spacetime mesh showing gravitational fields with proper depth
  - Blue areas: Low gravitational force
  - Red areas: High gravitational force
  - Grid warps downward around massive objects
- **Yellow Spheres**: Gravitational bodies creating the field distortion
- **Interactive Camera**: Smooth movement and rotation with two distinct modes
- **Fixed-Position Menu**: Dropdown for camera mode switching (no longer follows cursor)

## 🔧 Technical Highlights

### Modern C++ Features
- **RAII**: Automatic resource management
- **Smart Pointers**: Memory safety with shared_ptr/unique_ptr
- **Interface Design**: Abstract base classes for extensibility
- **Separation of Concerns**: Each component has a single, well-defined responsibility

### Graphics Pipeline
- **3D Perspective Rendering**: Proper depth testing and projection matrices
- **2D Overlay System**: UI elements rendered on top with orthographic projection
- **Coordinate System Consistency**: Grid and bodies perfectly aligned in 3D space
- **Line-Based Text Rendering**: Custom character drawing using OpenGL primitives

### Problem Solutions
- **Grid Alignment**: Fixed coordinate mapping between gravity bodies and visualization grid
- **UI Positioning**: Changed from cursor-following to fixed-position menu for usability
- **Text Rendering**: Implemented line-based character system for readable menu text

## 🚀 Future Extensions

The SOLID architecture supports easy addition of:
- **New Physics**: N-body simulations, relativistic effects, particle dynamics
- **Enhanced Graphics**: Shader-based rendering, particle effects, lighting, shadows
- **Interactive Elements**: Body placement, parameter adjustment, real-time editing
- **Data Export**: Simulation recording, screenshot capture, data analysis tools
- **New Camera Types**: Orbital camera, tracking camera, cinematic camera paths

## 📚 Documentation

- **SOLID_REFACTORING.md**: Detailed information about architecture evolution and SOLID principles implementation
- **Code Comments**: Comprehensive documentation throughout the codebase
- **Git History**: Clean commit history showing development progression

## 🎓 Learning Value

This project demonstrates:
- **Clean Architecture**: Real-world application of SOLID principles
- **3D Graphics Programming**: OpenGL immediate mode with proper 3D transformations
- **Component Design**: Separation of rendering, physics, UI, and camera systems
- **Interface Design**: How to create extensible, maintainable C++ applications

## 📸 Screenshots

The application displays:
1. A 3D spacetime grid that warps around gravitational bodies
2. Yellow spheres representing massive objects
3. Color-coded gravitational field strength
4. Interactive camera controls with smooth transitions
5. Fixed-position menu system for mode switching

## 📄 License

Open source project available under the MIT License.

---

**Note**: This is a complete, working 3D spacetime visualization with proper SOLID architecture. All major rendering and interaction issues have been resolved, including grid-body alignment, menu positioning, and text readability.
