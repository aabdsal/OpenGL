# Desarrollo de las prácticas en macOS

Este documento sirve como guía para configurar y desarrollar las prácticas de Informática Gráfica en macOS.

## Tabla de Equivalencias

| Aspecto | Windows  | macOS  |
|---------|-------------------|-------------------|
| **IDE** | Visual Studio (MSVC) | VS Code |
| **Compilador** | MSVC (`cl.exe`) | Clang (`clang++`) |
| **Gestión Proyecto** | `.sln`, `.vcxproj` | `.vscode/tasks.json` o Makefile |
| **Librerías** | DLLs locales | System Frameworks (`-framework OpenGL -framework GLUT`) |
| **Includes** | `<codebase.h>` | `<GLUT/glut.h>` (Nativo) |
| **Sistema Ventanas** | GLUT / freeglut | GLUT (Nativo de macOS) |

## Preparación del Entorno

### Instalación de Herramientas
1.  **Xcode Command Line Tools**: Necesario para tener `clang++`.
     Abre una terminal y ejecuta:
    ```bash
    xcode-select --install
    ```
2. **Homebrew (Gestor de paquetes):** Abre una terminal y pega el siguiente comando:
    ```bash
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    ```
3.  **VS Code**: Descárgalo de [code.visualstudio.com](https://code.visualstudio.com/).
4.  **Extensión C/C++**: En VS Code, instala la extensión oficial de Microsoft (ms-vscode.cpptools).
5.  **Librería Freeimage**: En la terminal, ejecuta este comando para instalar Freeimage, ```brew install freeimage```

## Compilación y Ejecución

**Desde Terminal Manualmente:**
Si prefieres no usar tareas de VS Code:
```bash
g++ main.cpp -o main -I/opt/homebrew/include -L/opt/homebrew/lib -lfreeimage -framework GLUT -framework OpenGL -Wno-deprecated
./main
```
**Desde VS Code:** 

Para compilar y ejecutar cómodamente, crea una carpeta llamada `.vscode` en tu directorio raíz y añade los siguientes archivos:

### `tasks.json` 
Este archivo define como compilar tu archivo.
Crea `.vscode/tasks.json`:

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "type": "cppbuild",
            "label": "Compilar OpenGL",
            "command": "/usr/bin/clang++",
            "args": [
                "-std=c++17",
                "-stdlib=libc++",
                "-fdiagnostics-color=always",
                "-g",
                "${file}",
                "-o",
                "${fileDirname}/${fileBasenameNoExtension}",
                "-I${workspaceFolder}/codebase",
                "-I/opt/homebrew/include",
                "-L/opt/homebrew/lib",
                "-lfreeimage",
                "-framework", "GLUT",
                "-framework", "OpenGL",
                "-Wno-deprecated"
            ],
            "options": {
                "cwd": "${fileDirname}"
            },
            "problemMatcher": ["$gcc"],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "detail": "Compilador clang++ con frameworks OpenGL y GLUT"
        }
    ]
}
```

### `launch.json`
Este archivo define cómo ejecutar y depurar tu programa una vez compilado.
Crea `.vscode/launch.json`:

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Ejecutar OpenGL (Debug)",
            "type": "cppdbg",
            "request": "launch",
            "program": "${fileDirname}/${fileBasenameNoExtension}",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${fileDirname}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "lldb",
            "preLaunchTask": "Compilar OpenGL"
        }
    ]
}
```

### `c_cpp_properties.json`
Ayuda a VS Code a encontrar las librerías para que el autocompletado funcione bien. 
Realmente no es necesario este archivo.
Crea `.vscode/c_cpp_properties.json`:

```json
{
    "configurations": [
        {
            "name": "Mac",
            "includePath": [
                "${workspaceFolder}/**"
                "/opt/homebrew/include"
            ],
            "defines": [
                "__APPLE__"
            ],
            "macFrameworkPath": [
                "/System/Library/Frameworks",
                "/Library/Frameworks"
            ],
            "compilerPath": "/usr/bin/clang++",
            "cStandard": "c17",
            "cppStandard": "c++17",
            "intelliSenseMode": "macos-clang-x64"
        }
    ],
    "version": 4
}
```

Ahora desde el boton de Run & Debug deberías de poder compilar y ejecutar el programa.


## Extra
Para que el código funcione tanto en Windows como en macOS, se pueden añadir directivas de preprocesador en `Primer_Proyecto.cpp`:

```cpp
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <codebase.h>
#endif
```