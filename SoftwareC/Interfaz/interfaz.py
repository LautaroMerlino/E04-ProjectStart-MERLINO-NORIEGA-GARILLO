
import tkinter as tk
from tkinter import ttk, messagebox
from threading import Thread, Event
from queue import Queue, Empty
import time

import serial
from serial.tools import list_ports

BAUDRATE = 9600
READ_INTERVAL_MS = 1000

USER_INFO = (
    "UTN FRSFCO - Informática II\n\n"
    "Integrantes: Merlino, Noriega, Garillo\n"
    "Placa: ESP32-S3 - Sensor: DHT11"
)


class SensorMonitor:

    def __init__(self, window):
        self.window = window
        self.serial_port = None
        self.closing = False

        self.reader_thread = None
        self.stop_event = Event()
        self.data_queue = Queue()
        self.command_queue = Queue()

        self.window.title("Monitor de temperatura y humedad")
        self.window.geometry("600x400")
        self.window.minsize(500, 350)

        self.port_var = tk.StringVar()
        self.status_var = tk.StringVar(value="Desconectado")
        self.temperature_var = tk.StringVar(value="--.- °C")
        self.humidity_var = tk.StringVar(value="--.- %")

        self._build_interface()
        self.refresh_ports()

        self.window.protocol("WM_DELETE_WINDOW", self.close_window)

        self.window.after(100, self.process_queue)

    def _build_interface(self):

        header = ttk.Frame(self.window, padding=15)
        header.pack(fill=tk.X)

        ttk.Label(
            header,
            text="MONITOR DE TEMPERATURA Y HUMEDAD",
            font=("Arial", 15, "bold")
        ).pack()

        ttk.Label(
            header,
            text="Sistema de monitoreo ESP32"
        ).pack(pady=(5, 0))

        controls = ttk.LabelFrame(
            self.window,
            text="Conexión serial",
            padding=10
        )
        controls.pack(fill=tk.X, padx=15, pady=5)

        ttk.Label(controls, text="Puerto COM:").grid(
            row=0, column=0, padx=5
        )

        self.port_menu = ttk.Combobox(
            controls,
            textvariable=self.port_var,
            state="readonly",
            width=12
        )
        self.port_menu.grid(row=0, column=1, padx=5)

        self.refresh_button = ttk.Button(
            controls,
            text="Actualizar",
            command=self.refresh_ports
        )
        self.refresh_button.grid(row=0, column=2, padx=5)

        self.connect_button = ttk.Button(
            controls,
            text="Conectar",
            command=self.connect
        )
        self.connect_button.grid(row=0, column=3, padx=5)

        self.disconnect_button = ttk.Button(
            controls,
            text="Desconectar",
            command=self.disconnect,
            state=tk.DISABLED
        )
        self.disconnect_button.grid(row=0, column=4, padx=5)

        measurements = ttk.Frame(self.window, padding=15)
        measurements.pack(fill=tk.BOTH, expand=True)

        measurements.columnconfigure(0, weight=1)
        measurements.columnconfigure(1, weight=1)

        temperature_frame = ttk.LabelFrame(
            measurements,
            text="Temperatura",
            padding=15
        )
        temperature_frame.grid(
            row=0, column=0, padx=8, sticky="nsew"
        )

        ttk.Label(
            temperature_frame,
            text="🌡",
            font=("Arial", 28)
        ).pack(pady=5)

        ttk.Label(
            temperature_frame,
            textvariable=self.temperature_var,
            font=("Arial", 25, "bold"),
            foreground="#D66A32"
        ).pack(pady=10)

        humidity_frame = ttk.LabelFrame(
            measurements,
            text="Humedad relativa",
            padding=15
        )
        humidity_frame.grid(
            row=0, column=1, padx=8, sticky="nsew"
        )

        ttk.Label(
            humidity_frame,
            text="💧",
            font=("Arial", 28)
        ).pack(pady=5)

        ttk.Label(
            humidity_frame,
            textvariable=self.humidity_var,
            font=("Arial", 25, "bold"),
            foreground="#278CCB"
        ).pack(pady=10)

        actions = ttk.LabelFrame(
            self.window,
            text="Acciones",
            padding=10
        )
        actions.pack(fill=tk.X, padx=15, pady=5)

        self.led_on_button = ttk.Button(
            actions,
            text="Encender LED",
            command=lambda: self.send_command(b"E"),
            state=tk.DISABLED
        )
        self.led_on_button.grid(row=0, column=0, padx=5)

        self.led_off_button = ttk.Button(
            actions,
            text="Apagar LED",
            command=lambda: self.send_command(b"A"),
            state=tk.DISABLED
        )
        self.led_off_button.grid(row=0, column=1, padx=5)

        self.user_button = ttk.Button(
            actions,
            text="Datos de usuario",
            command=self.show_user_data
        )
        self.user_button.grid(row=0, column=2, padx=5)

        status_frame = ttk.Frame(self.window, padding=10)
        status_frame.pack(fill=tk.X, side=tk.BOTTOM)

        self.status_label = ttk.Label(
            status_frame,
            textvariable=self.status_var,
            anchor=tk.W,
            relief=tk.SUNKEN,
            padding=5
        )
        self.status_label.pack(fill=tk.X)

    def refresh_ports(self):

        ports = [port.device for port in list_ports.comports()]

        self.port_menu.configure(values=ports)

        if self.port_var.get() not in ports:
            self.port_var.set(ports[0] if ports else "")

        if self.serial_port is None:
            self.status_var.set(
                f"{len(ports)} puerto(s) disponible(s)"
                if ports else "No se encontraron puertos seriales"
            )

    def connect(self):

        port = self.port_var.get()

        if not port:
            messagebox.showwarning(
                "Conexión",
                "Seleccioná un puerto COM."
            )
            return

        self.connect_button.configure(state=tk.DISABLED)
        self.refresh_button.configure(state=tk.DISABLED)
        self.port_menu.configure(state=tk.DISABLED)
        self.disconnect_button.configure(state=tk.NORMAL)
        self.led_on_button.configure(state=tk.NORMAL)
        self.led_off_button.configure(state=tk.NORMAL)

        self.status_var.set(f"Conectando a {port}...")

        self.stop_event.clear()

        self.reader_thread = Thread(
            target=self.serial_worker,
            args=(port,),
            daemon=True
        )
        self.reader_thread.start()

    def serial_worker(self, port_name):

        port = None

        try:
            port = serial.Serial(
                port_name,
                BAUDRATE,
                timeout=0.2,
                write_timeout=0.5
            )

            self.serial_port = port

            # Esperar el reinicio del ESP32
            if self.stop_event.wait(2):
                return

            port.reset_input_buffer()

            self.data_queue.put(("status", f"Conectado a {port_name} - {BAUDRATE} baudios"))

            while not self.stop_event.is_set():

                self.process_commands(port)

                port.write(b'S')

                # Esperar una respuesta sin bloquear la interfaz
                deadline = time.monotonic() + 2

                while not self.stop_event.is_set():
                    if time.monotonic() >= deadline:
                        self.data_queue.put(("status", "Esperando respuesta del sensor..."))
                        break

                    line = port.readline()

                    if not line:
                        continue

                    line = line.decode("utf-8", errors="replace").strip()

                    if line == "ERROR":
                        self.data_queue.put(("status", "Error en la lectura del sensor"))
                        break

                    if line.startswith("T:") and ";H:" in line:
                        temperature, humidity = self.parse_data(line)

                        if temperature is not None and humidity is not None:
                            self.data_queue.put(
                                ("data", temperature, humidity)
                            )
                        break

                # Esperar hasta la proxima lectura, atendiendo los botones del LED
                for _ in range(READ_INTERVAL_MS // 100):
                    if self.stop_event.is_set():
                        break
                    self.process_commands(port)
                    self.stop_event.wait(0.1)

        except (serial.SerialException, OSError) as error:
            self.data_queue.put(("error", str(error)))

        finally:
            if port is not None:
                try:
                    if port.is_open:
                        port.close()
                except (serial.SerialException, OSError):
                    pass

            self.serial_port = None
            self.data_queue.put(("disconnected",))

    def process_queue(self):

        try:
            while True:
                item = self.data_queue.get_nowait()

                if item[0] == "status":
                    self.status_var.set(item[1])

                elif item[0] == "data":
                    self.temperature_var.set(f"{item[1]:.1f} °C")
                    self.humidity_var.set(f"{item[2]:.1f} %")
                    self.status_var.set("Conectado - Recibiendo datos")

                elif item[0] == "error":
                    self.status_var.set(f"Error serial: {item[1]}")

                elif item[0] == "disconnected":
                    self.serial_port = None

                    if not self.closing:
                        self.port_menu.configure(state="readonly")
                        self.refresh_button.configure(state=tk.NORMAL)
                        self.connect_button.configure(state=tk.NORMAL)
                        self.disconnect_button.configure(state=tk.DISABLED)
                        self.led_on_button.configure(state=tk.DISABLED)
                        self.led_off_button.configure(state=tk.DISABLED)

                        self.status_var.set("Desconectado")

        except Empty:
            pass

        if not self.closing:
            self.window.after(100, self.process_queue)

    def send_command(self, command):
        self.command_queue.put(command)

    def process_commands(self, port):
        while not self.stop_event.is_set():
            try:
                command = self.command_queue.get_nowait()
            except Empty:
                return

            port.reset_input_buffer()
            port.write(command)

            deadline = time.monotonic() + 1
            answer = None
            while time.monotonic() < deadline and not self.stop_event.is_set():
                line = port.readline().decode("utf-8", errors="replace").strip()
                if line in ("OK", "ERROR"):
                    answer = line
                    break

            if answer == "OK":
                text = "LED encendido" if command == b"E" else "LED apagado"
                self.data_queue.put(("status", text))
            else:
                self.data_queue.put(("status", "El ESP32 no confirmo el comando del LED"))

    def show_user_data(self):
        messagebox.showinfo(
            "Datos de usuario",
            USER_INFO
        )

    def parse_data(self, data):

        try:
            parts = data.split(";")

            temperature = float(parts[0].split(":")[1])
            humidity = float(parts[1].split(":")[1])

            if not 0 <= humidity <= 100:
                return None, None

            return temperature, humidity

        except (ValueError, IndexError):
            return None, None

    def disconnect(self):

        self.stop_event.set()
        while not self.command_queue.empty():
            self.command_queue.get_nowait()

        self.temperature_var.set("--.- °C")
        self.humidity_var.set("--.- %")

        self.status_var.set("Desconectando...")

        self.disconnect_button.configure(state=tk.DISABLED)

    def close_window(self):

        self.closing = True
        self.stop_event.set()

        self.window.destroy()


def main():

    window = tk.Tk()
    monitor = SensorMonitor(window)
    monitor.closing = False
    window.mainloop()


if __name__ == "__main__":
    main()
