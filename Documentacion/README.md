# E04 - Proyecto de máquinas de estados

Aplicación de consola en C y firmware para explorar máquinas de estados en un
sistema con teclado y sensores. El proyecto contiene la máquina de estados del
menú del software y componentes para comunicación serie con el firmware.

## SoftwareC

El ejecutable `GES` usa `SoftwareC/src/main.c` como punto de entrada. En el
arranque inicializa la comunicación serie, los módulos de sensores y el menú
implementado en `menuState.c`. El superloop lee teclas sin esperar Enter: ESC
finaliza la aplicación y las demás teclas se envían a la máquina de estados.

| Tecla | Acción |
| --- | --- |
| 1 | Solicitar lectura de sensores por serie |
| 2 | Mostrar el mensaje de activación de LEDs |
| 3 | Mostrar la opción de datos de usuario |
| 4 | Enviar el comando serie `E` (encender LED) |
| 5 | Enviar el comando serie `A` (apagar LED) |
| ESC | Salir |

La lectura de sensores usa el comando serie `S`; las opciones 2 y 3 son
demostrativas y actualmente no controlan hardware ni consultan registros. El
módulo de temperatura se inicializa, pero todavía no tiene una opción propia en
el menú. La configuración serie actual está fijada en `SoftwareC/src/serial.c`
(COM1, 9600 baudios), por lo que las acciones que dependen del firmware
requieren ese puerto y el dispositivo conectado.

## Estructura relevante

- `SoftwareC/src/main.c`: inicialización y superloop de consola.
- `SoftwareC/src/menuState.c`: estados y acciones del menú activo.
- `SoftwareC/src/Sensores.c` y `SoftwareC/src/SensorTemperatura.c`: módulos de
  sensores.
- `SoftwareC/src/serial.c` y `SoftwareC/src/rs232.c`: interfaz serie y su
  implementación de bajo nivel.
- `SoftwareC/inc/`: interfaces públicas de los módulos.
- `Firmware/`: código del firmware.
- `Documentacion/`: documentación y diagramas del proyecto.

`SoftwareC/src/menu.c` conserva una implementación simple anterior, pero no es
el menú utilizado por `main.c`.

## Compilar y ejecutar SoftwareC

Se requiere CMake y un compilador C compatible con el entorno Windows y con
`<conio.h>`.

Desde la raíz del repositorio, en PowerShell:

```powershell
cmake -S SoftwareC -B SoftwareC/build
cmake --build SoftwareC/build
```

Ejecuta `GES.exe` dentro de `SoftwareC/build`; la ruta exacta puede incluir una
subcarpeta de configuración como `Debug`, según el generador de CMake elegido.
Para compilar con CMake es posible seleccionar explícitamente un generador y
un compilador con las opciones habituales de CMake.