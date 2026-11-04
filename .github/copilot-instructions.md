# Instrucciones del Proyecto E04-ProjectStart

## Descripción del Proyecto
Este proyecto implementa máquinas de estado tanto en software como en firmware para el manejo de menús y teclados matriciales 4x4.

## Estructura del Proyecto
- **Firmware**: Código embebido para controlar teclados matriciales y máquinas de estado
- **Software**: Interfaz de usuario con máquinas de estado para menús
- **Documentacion**: Diagramas de estados, especificaciones y documentación del proyecto

## Roles y Responsabilidades
- **Dev-1**: Máquina de estados del menú del software
- **Dev-2**: Máquina de estados del teclado matricial 4x4 y su implementación en firmware
- **Dev-3**: Máquina de estados del menú del firmware

## Máquinas de Estado - Requisitos
Cada máquina de estado debe incluir:
1. Diagrama de estados (imagen en formato PNG/JPG)
2. Descripción de transiciones y estados
3. Comentarios claros en el código
4. Funciones de entrada/salida bien documentadas

### Estados Esperados en el Software
- Menú principal
- Leer estado de sensores
- Activar actuadores
- Cargar nuevo usuario
- Consultar registro de usuarios

## Convenciones de Código
- Nombres descriptivos en inglés para funciones y variables
- Prefijo `stateMachine_` para funciones relacionadas a máquinas de estado
- Prefijo `menu_` para funciones de menú
- Prefijo `keyboard_` para funciones del teclado
- Documentar cambios de estado en comentarios

## Archivos Esperados

### Firmware
```
firmware/
├── keyboard/
│   ├── keyboard.c
│   └── keyboard.h
├── menu/
│   ├── menu.c
│   └── menu.h
└── ...
```

### Software
```
software/
├── menu/
│   ├── menu.c/menu.py (según el lenguaje)
│   └── menu.h (si aplica)
└── ...
```

## Git Workflow
- **Rama principal**: develop
- **Formato de commits**: `E05-FiniteMachineState Dev[1-3]` (al finalizar cada fase)
- **Proceso**: Commit → Push → Pull request a develop → Merge tras revisión

## Documentación
- Todos los diagramas de máquinas de estado deben subirse a `Documentacion/`
- Nombrar los diagramas de forma descriptiva: `stateMachine_[component]_[devX].png`
- Actualizar este archivo conforme avance el proyecto

## Testing
- Las máquinas de estado deben ser probadas con diferentes secuencias de entrada
- Documentar casos de prueba en `Documentacion/`

## Contexto Adicional
Este repositorio es de un proyecto dinamico de 3 desarrolladores trabajando en paralelo. Cada uno tiene asignada una parte especifica del proyecto. En la documentacion se agregan nuevas actividades a medida que avanza el proyecto. Copilot debe sugerir cambios acordes a estos objetivos y ayudar a mantener el proyecto organizado.