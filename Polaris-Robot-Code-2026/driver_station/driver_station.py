#!/usr/bin/env python3
"""
Polaris Driver Station  v2
GitHub-dark themed control UI with live tuning
"""

import tkinter as tk
from tkinter import ttk
import serial
import serial.tools.list_ports
import threading
import queue

# ── Colour palette (GitHub-dark inspired) ──────────────────────────
BG_DARK   = '#0d1117'
BG_CARD   = '#161b22'
BG_INPUT  = '#0d1117'
BORDER    = '#30363d'
TXT       = "#4798e9"
TXT_DIM   = '#8b949e'
GREEN     = '#3fb950'
RED       = '#f85149'
YELLOW    = '#d29922'
BLUE      = '#58a6ff'
ACCENT    = '#1f6feb'


class DriverStation:
    # ----------------------------------------------------------------
    def __init__(self):
        self.root = tk.Tk()
        self.root.title("Polaris Driver Station")
        self.root.geometry("740x780")
        self.root.configure(bg=BG_DARK)
        self.root.minsize(620, 720)

        self.serial_port = None
        self.enabled = False
        self.running = True
        self.rx_queue = queue.Queue()
        self.sliders = {}

        self.setup_ui()
        self.refresh_ports()
        self.process_rx_queue()

        # Auto-connect if Teensy found
        port = self.port_var.get().strip()
        if port and 'usbmodem' in port:
            self.log("Teensy detected – auto-connecting…")
            self.connect()

    # ----------------------------------------------------------------
    #  UI
    # ----------------------------------------------------------------
    def setup_ui(self):
        style = ttk.Style()
        style.theme_use('clam')
        style.configure('TCombobox', fieldbackground=BG_INPUT, background=BG_CARD,
                        foreground=TXT, arrowcolor=TXT)

        # Click anywhere outside an Entry → return focus to root
        self.root.bind('<Button-1>', self._on_root_click)

        # ── Connection card ────────────────────────────────────────
        conn = self._card(self.root)
        conn.pack(fill='x', padx=12, pady=(12, 4))

        tk.Label(conn, text="Port", fg=TXT_DIM, bg=BG_CARD,
                 font=('Arial', 11)).pack(side='left')

        self.port_var = tk.StringVar()
        self.port_combo = ttk.Combobox(conn, textvariable=self.port_var, width=22)
        self.port_combo.pack(side='left', padx=6)

        self.refresh_btn = tk.Button(conn, text="↻", command=self.refresh_ports,
                                     bg=BG_CARD, fg=TXT, activebackground=BORDER,
                                     activeforeground=TXT, bd=0, width=3,
                                     font=('Arial', 13))
        self.refresh_btn.pack(side='left', padx=2)

        self.connect_btn = tk.Button(conn, text="Connect", command=self.toggle_connection,
                                     bg=ACCENT, fg=TXT, activebackground='#1a5cc7',
                                     activeforeground=TXT, bd=0, width=10,
                                     font=('Arial', 11, 'bold'))
        self.connect_btn.pack(side='left', padx=6)

        self.conn_status = tk.Label(conn, text="● Disconnected", fg=RED,
                                    bg=BG_CARD, font=('Arial', 10))
        self.conn_status.pack(side='right')

        # ── Enable / Disable ───────────────────────────────────────
        btn_row = tk.Frame(self.root, bg=BG_DARK)
        btn_row.pack(fill='x', padx=12, pady=6)

        self.enable_btn = tk.Button(btn_row, text="ENABLE",
                                    command=self.enable_robot,
                                    bg='#238636', fg=TXT,
                                    activebackground='#2ea043', activeforeground=TXT,
                                    font=('Arial', 18, 'bold'),
                                    width=12, height=2, bd=0,
                                    state='disabled')
        self.enable_btn.pack(side='left', padx=(0, 6))

        self.disable_btn = tk.Button(btn_row, text="DISABLE",
                                     command=self.disable_robot,
                                     bg='#da3633', fg=TXT,
                                     activebackground='#f85149', activeforeground=TXT,
                                     font=('Arial', 18, 'bold'),
                                     width=12, height=2, bd=0,
                                     state='disabled')
        self.disable_btn.pack(side='right', padx=(6, 0))

        # ── Status banner ──────────────────────────────────────────
        self.status_frame = tk.Frame(self.root, bg=BG_CARD,
                                     highlightbackground=BORDER,
                                     highlightthickness=1)
        self.status_frame.pack(fill='x', padx=12, pady=4)

        self.robot_status = tk.Label(self.status_frame, text="DISABLED",
                                     fg=RED, bg=BG_CARD,
                                     font=('Arial', 26, 'bold'))
        self.robot_status.pack(pady=8)

        # ── Tuning card ────────────────────────────────────────────
        tune_header = tk.Label(self.root, text="⚙  Live Tuning", fg=YELLOW,
                               bg=BG_DARK, font=('Arial', 13, 'bold'))
        tune_header.pack(anchor='w', padx=14, pady=(10, 0))

        tune = self._card(self.root)
        tune.pack(fill='x', padx=12, pady=4)

        params = [
            ('VLIMIT',   'Voltage Limit',  0.0,  12.0,  1.5,  0.1, 'V'),
            ('SPEED',    'Swing Speed',     0.5,  50.0,  3.0,  0.5, 'rad/s'),
            ('SWING_MS', 'Swing Time',      50,  10000, 5000,   50, 'ms'),
            ('HOLD_MS',  'Hold Time',        0,  10000,  500,   50, 'ms'),
            ('RAMP_MS',  'Ramp Up Time',     0,  5000,   500,   50, 'ms'),
        ]

        for name, label, mn, mx, default, res, unit in params:
            row = tk.Frame(tune, bg=BG_CARD)
            row.pack(fill='x', padx=10, pady=4)

            tk.Label(row, text=f"{label}:", fg=TXT, bg=BG_CARD,
                     font=('Arial', 11), width=14, anchor='w').pack(side='left')

            tk.Label(row, text=unit, fg=TXT_DIM, bg=BG_CARD,
                     font=('Arial', 10), width=5).pack(side='right')

            val_var = tk.StringVar(value=str(default))
            val_entry = tk.Entry(row, textvariable=val_var, fg=GREEN, bg=BG_INPUT,
                                 font=('Courier', 11, 'bold'), width=8,
                                 insertbackground=GREEN, justify='center',
                                 relief='flat', bd=2,
                                 highlightbackground=BORDER, highlightthickness=1)
            val_entry.pack(side='right', padx=(0, 4))
            val_entry.bind('<Return>',
                           lambda e, n=name, vv=val_var, lo=mn, hi=mx:
                           self._on_entry_submit(n, vv, lo, hi))

            slider = tk.Scale(row, from_=mn, to=mx, resolution=res,
                              orient='horizontal', bg=BG_CARD, fg=TXT,
                              troughcolor=BG_INPUT, highlightthickness=0,
                              showvalue=False, length=250,
                              activebackground=BLUE,
                              command=lambda v, n=name, vv=val_var:
                              self._on_slider(n, v, vv))
            slider.set(default)
            slider.pack(side='left', fill='x', expand=True, padx=5)

            self.sliders[name] = {'slider': slider, 'var': val_var,
                                  'entry': val_entry, 'min': mn, 'max': mx}

        # ── Direction picker ───────────────────────────────────────
        dir_row = tk.Frame(tune, bg=BG_CARD)
        dir_row.pack(fill='x', padx=10, pady=4)

        tk.Label(dir_row, text="Direction:", fg=TXT, bg=BG_CARD,
                 font=('Arial', 11), width=14, anchor='w').pack(side='left')

        self.dir_var = tk.StringVar(value="Forward")
        dir_fwd = tk.Radiobutton(dir_row, text="Forward", variable=self.dir_var,
                                  value="Forward", bg=BG_CARD, fg=GREEN,
                                  selectcolor=BG_INPUT, activebackground=BG_CARD,
                                  activeforeground=GREEN, font=('Arial', 11),
                                  command=lambda: self._on_dir_change())
        dir_fwd.pack(side='left', padx=8)

        dir_rev = tk.Radiobutton(dir_row, text="Reverse", variable=self.dir_var,
                                  value="Reverse", bg=BG_CARD, fg=RED,
                                  selectcolor=BG_INPUT, activebackground=BG_CARD,
                                  activeforeground=RED, font=('Arial', 11),
                                  command=lambda: self._on_dir_change())
        dir_rev.pack(side='left', padx=8)

        # ── Sequence mode picker ──────────────────────────────────
        seq_row = tk.Frame(tune, bg=BG_CARD)
        seq_row.pack(fill='x', padx=10, pady=4)

        tk.Label(seq_row, text="Sequence:", fg=TXT, bg=BG_CARD,
                 font=('Arial', 11), width=14, anchor='w').pack(side='left')

        self.seq_var = tk.StringVar(value="Swing")
        seq_options = [("Swing", "0"), ("Repeat", "1"), ("Single Shot", "2")]
        for label_text, val in seq_options:
            rb = tk.Radiobutton(seq_row, text=label_text, variable=self.seq_var,
                                value=label_text, bg=BG_CARD, fg=BLUE,
                                selectcolor=BG_INPUT, activebackground=BG_CARD,
                                activeforeground=BLUE, font=('Arial', 11),
                                command=lambda: self._on_seq_change())
            rb.pack(side='left', padx=6)

        fetch_row = tk.Frame(tune, bg=BG_CARD)
        fetch_row.pack(fill='x', padx=10, pady=(2, 8))
        self.fetch_btn = tk.Button(fetch_row, text="↓ Fetch Values from Teensy",
                                   command=self.fetch_values,
                                   bg=BG_CARD, fg=BLUE, activebackground=BORDER,
                                   activeforeground=BLUE, bd=0,
                                   font=('Arial', 11), state='disabled')
        self.fetch_btn.pack(side='left')

        # ── Console card ───────────────────────────────────────────
        tk.Label(self.root, text="Console", fg=TXT, bg=BG_DARK,
                 font=('Arial', 12, 'bold')).pack(anchor='w', padx=14)

        con_card = self._card(self.root)
        con_card.pack(fill='both', expand=True, padx=12, pady=4)

        self.console = tk.Text(con_card, bg=BG_INPUT, fg=GREEN,
                               font=('Courier', 10), height=8,
                               state='disabled', bd=0,
                               insertbackground=GREEN)
        self.console.pack(side='left', fill='both', expand=True)

        sb = ttk.Scrollbar(con_card, command=self.console.yview)
        sb.pack(side='right', fill='y')
        self.console.config(yscrollcommand=sb.set)

        # ── Command entry ──────────────────────────────────────────
        cmd_row = tk.Frame(self.root, bg=BG_DARK)
        cmd_row.pack(fill='x', padx=12, pady=(4, 10))

        tk.Label(cmd_row, text="Send:", fg=TXT_DIM, bg=BG_DARK,
                 font=('Arial', 11)).pack(side='left')

        self.cmd_entry = tk.Entry(cmd_row, bg=BG_INPUT, fg=TXT,
                                  font=('Courier', 11), width=40,
                                  insertbackground=TXT, relief='flat', bd=2,
                                  highlightbackground=BORDER, highlightthickness=1)
        self.cmd_entry.pack(side='left', padx=6, fill='x', expand=True)
        self.cmd_entry.bind('<Return>', self.send_command)

        self.send_btn = tk.Button(cmd_row, text="Send", command=self.send_command,
                                  bg=ACCENT, fg=TXT, activebackground='#1a5cc7',
                                  activeforeground=TXT,
                                  bd=0, font=('Arial', 11, 'bold'), state='disabled')
        self.send_btn.pack(side='right')

        # ── Keyboard shortcuts ─────────────────────────────────────
        self.root.bind('<Escape>', lambda e: self.disable_robot())
        self.root.bind('<space>',  lambda e: None
                       if isinstance(e.widget, (tk.Entry, ttk.Combobox))
                       else self.disable_robot())
        self.root.bind('<Return>', lambda e: None
                       if isinstance(e.widget, (tk.Entry, ttk.Combobox))
                       else self.disable_robot())

    # ── helpers ────────────────────────────────────────────────────
    def _card(self, parent):
        """Return a rounded-look card frame."""
        return tk.Frame(parent, bg=BG_CARD,
                        highlightbackground=BORDER, highlightthickness=1)

    def _on_root_click(self, event):
        """Click outside an Entry → defocus it so shortcuts work."""
        w = event.widget
        if not isinstance(w, (tk.Entry, ttk.Combobox)):
            self.root.focus_set()

    # ── slider / entry callbacks ───────────────────────────────────
    def _on_slider(self, param_name, value, val_var):
        entry = self.sliders[param_name].get('entry')
        if entry and entry == self.root.focus_get():
            return
        val_var.set(value)
        self.send_serial(f"SET:{param_name}:{value}")

    def _on_entry_submit(self, param_name, val_var, min_val, max_val):
        """Enter pressed inside a tuning entry."""
        try:
            val = float(val_var.get())
            val = max(min_val, min(max_val, val))
            val_var.set(str(val))
            self.sliders[param_name]['slider'].set(val)
            self.send_serial(f"SET:{param_name}:{val}")
        except ValueError:
            self.log(f"Invalid value for {param_name}")
        self.root.focus_set()

    def _on_dir_change(self):
        val = 1 if self.dir_var.get() == "Forward" else -1
        self.send_serial(f"SET:DIR:{val}")
        self.log(f"Direction → {self.dir_var.get()}")

    def _on_seq_change(self):
        seq_map = {"Swing": 0, "Repeat": 1, "Single Shot": 2}
        val = seq_map.get(self.seq_var.get(), 0)
        self.send_serial(f"SET:SEQ:{val}")
        self.log(f"Sequence → {self.seq_var.get()}")

    def fetch_values(self):
        self.send_serial("GET")
        self.log(">>> Fetching values…")

    # ── serial / connection ────────────────────────────────────────
    def refresh_ports(self):
        all_ports = serial.tools.list_ports.comports()
        ports = [p.device for p in all_ports
                 if 'Bluetooth' not in p.device and 'debug' not in p.device]
        self.port_combo['values'] = ports
        if ports:
            teensy = [p for p in ports if 'usbmodem' in p]
            self.port_var.set(teensy[0] if teensy else ports[0])
            self.log(f"Found ports: {ports}")
        else:
            self.port_var.set("")
            self.log("No serial ports found! Is Teensy plugged in?")

    def toggle_connection(self):
        if self.serial_port:
            self.disconnect()
        else:
            self.connect()

    def connect(self):
        port = self.port_var.get().strip()
        if not port:
            self.log("No port selected! Click ↻ to refresh.")
            return
        try:
            self.serial_port = serial.Serial(port, 115200, timeout=0.1)
            self.conn_status.config(text="● Connected", fg=GREEN)
            self.connect_btn.config(text="Disconnect")
            for w in (self.enable_btn, self.disable_btn, self.send_btn, self.fetch_btn):
                w.config(state='normal')
            self.log(f"Connected to {port}")
            self.read_thread = threading.Thread(target=self.read_serial, daemon=True)
            self.read_thread.start()
            self.root.after(500, self.fetch_values)
        except Exception as e:
            self.log(f"Connection failed: {e}")

    def disconnect(self):
        if self.serial_port:
            self.serial_port.close()
            self.serial_port = None
        self.conn_status.config(text="● Disconnected", fg=RED)
        self.connect_btn.config(text="Connect")
        for w in (self.enable_btn, self.disable_btn, self.send_btn, self.fetch_btn):
            w.config(state='disabled')
        self.log("Disconnected")

    def read_serial(self):
        while self.running and self.serial_port:
            try:
                if self.serial_port.in_waiting:
                    line = self.serial_port.readline().decode('utf-8', errors='ignore').strip()
                    if line:
                        self.rx_queue.put(line)
            except Exception:
                break

    def process_rx_queue(self):
        while not self.rx_queue.empty():
            line = self.rx_queue.get()
            self.log(line)
            self._parse_value_update(line)
        self.root.after(50, self.process_rx_queue)

    def _parse_value_update(self, line):
        for param in self.sliders:
            for sep in (':', '='):
                prefix = f"{param}{sep}"
                if line.startswith(prefix):
                    try:
                        val = float(line[len(prefix):])
                        self.sliders[param]['slider'].set(val)
                        self.sliders[param]['var'].set(str(val))
                    except ValueError:
                        pass
                    return
        # Sync DIR radio buttons
        for sep in (':', '='):
            if line.startswith(f"DIR{sep}"):
                try:
                    val = int(float(line[len(f"DIR{sep}"):]))
                    self.dir_var.set("Forward" if val >= 0 else "Reverse")
                except ValueError:
                    pass
                return
        # Sync SEQ radio buttons
        for sep in (':', '='):
            if line.startswith(f"SEQ{sep}"):
                try:
                    val = int(float(line[len(f"SEQ{sep}"):]))
                    seq_rmap = {0: "Swing", 1: "Repeat", 2: "Single Shot"}
                    self.seq_var.set(seq_rmap.get(val, "Swing"))
                except ValueError:
                    pass
                return

    # ── enable / disable ───────────────────────────────────────────
    def enable_robot(self):
        self.enabled = True
        self.robot_status.config(text="ENABLED", fg=GREEN)
        self.status_frame.config(bg='#0f2d15')
        self.robot_status.config(bg='#0f2d15')
        self.send_serial("ENABLE")
        self.log(">>> ROBOT ENABLED")

    def disable_robot(self):
        self.enabled = False
        self.robot_status.config(text="DISABLED", fg=RED)
        self.status_frame.config(bg=BG_CARD)
        self.robot_status.config(bg=BG_CARD)
        self.send_serial("DISABLE")
        self.log(">>> ROBOT DISABLED")

    # ── send / log ─────────────────────────────────────────────────
    def send_command(self, event=None):
        cmd = self.cmd_entry.get().strip()
        if cmd:
            self.send_serial(cmd)
            self.log(f">>> {cmd}")
            self.cmd_entry.delete(0, 'end')

    def send_serial(self, data):
        if self.serial_port:
            try:
                self.serial_port.write(f"{data}\n".encode())
            except Exception as e:
                self.log(f"Send error: {e}")

    def log(self, message):
        self.console.config(state='normal')
        self.console.insert('end', message + '\n')
        self.console.see('end')
        lines = int(self.console.index('end-1c').split('.')[0])
        if lines > 500:
            self.console.delete('1.0', '100.0')
        self.console.config(state='disabled')

    # ── lifecycle ──────────────────────────────────────────────────
    def run(self):
        self.root.protocol("WM_DELETE_WINDOW", self.on_close)
        self.root.mainloop()

    def on_close(self):
        self.running = False
        self.disconnect()
        self.root.destroy()


if __name__ == "__main__":
    app = DriverStation()
    app.run()
