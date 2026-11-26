# E04 - Proyecto de máquinas de estados

Aplicación de consola en C, interfaz gráfica en Python y firmware para explorar
máquinas de estados en un sistema con teclado y sensores.

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

## Interfaz gráfica (PC)

Programa en Python (tkinter + pyserial) que reemplaza al menú de consola de SoftwareC.

Funciones:
- Lectura de temperatura y humedad cada 1 s (comando `S`).
- Botones para encender y apagar el LED (comandos `E` y `A`).
- Sección "Datos de usuario".

Cómo ejecutarla:

    pip install pyserial
    python SoftwareC\Interfaz\interfaz.py

Elegir el puerto COM del conector UART de la placa y presionar "Conectar".
Solo un programa puede usar el puerto a la vez (cerrar el monitor serie antes).

## Protocolo serie (9600 8N1)

| Comando | Respuesta |
|---------|-----------|
| `E`     | `OK` (enciende el LED) |
| `A`     | `OK` (apaga el LED) |
| `T`     | `T:24.0` |
| `H`     | `H:55.0` |
| `S`     | `T:24.0;H:55.0` |
| otro / falla del sensor | `ERROR` |

## Pines (ESP32-S3-DevKitC-1)

| Función | GPIO |
|---------|------|
| DHT11 (señal) | 15 |
| PWM temperatura (0–50 °C → 0–255) | 13 |
| PWM humedad (0–100 % → 0–255) | 38 |
| Teclado, filas | 4, 5, 6, 7 |
| Teclado, columnas | 8, 9, 10, 11 |
| LED integrado | 48 |

El DHT11 se alimenta con 3V3 y GND de la placa.