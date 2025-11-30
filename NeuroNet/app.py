import tkinter as tk
from tkinter import filedialog, messagebox
import neuronet_core
import networkx as nx
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

class App:
    def __init__(self, root):
        self.root = root
        self.root.title("NeuroNet v2.0 - Visualización Avanzada")
        self.root.geometry("1100x800") 
        
        self.bg_color = "#2c3e50" 
        self.panel_color = "#ecf0f1"
        self.root.configure(bg=self.bg_color)

        self.engine = neuronet_core.Motor()
        self.loaded = False
        
        control_frame = tk.Frame(root, bg=self.panel_color, pady=15, padx=15)
        control_frame.pack(fill=tk.X, side=tk.TOP)

        btn_style = {"font": ("Segoe UI", 10, "bold"), "bd": 0, "relief": "flat", "padx": 15, "pady": 5}

        tk.Label(control_frame, text="1. Datos:", bg=self.panel_color, font=("Segoe UI", 10, "bold")).pack(side=tk.LEFT)
        tk.Button(control_frame, text="📂 Cargar Dataset", command=self.load, bg="#e67e22", fg="white", **btn_style).pack(side=tk.LEFT, padx=5)
        
        self.lbl_info = tk.Label(control_frame, text="Sin datos", bg="white", fg="#7f8c8d", width=25, relief="sunken")
        self.lbl_info.pack(side=tk.LEFT, padx=10)

        tk.Label(control_frame, text="| 2. Análisis:", bg=self.panel_color, font=("Segoe UI", 10, "bold")).pack(side=tk.LEFT, padx=5)
        tk.Button(control_frame, text="⚡ Nodo Crítico", command=self.crit, bg="#2980b9", fg="white", **btn_style).pack(side=tk.LEFT, padx=5)

        tk.Label(control_frame, text="| 3. Visualización:", bg=self.panel_color, font=("Segoe UI", 10, "bold")).pack(side=tk.LEFT, padx=5)
        
        tk.Label(control_frame, text="Inicio:", bg=self.panel_color).pack(side=tk.LEFT)
        self.entry_start = tk.Entry(control_frame, width=6, justify="center", font=("Arial", 10))
        self.entry_start.pack(side=tk.LEFT, padx=2)
        
        tk.Label(control_frame, text="Prof:", bg=self.panel_color).pack(side=tk.LEFT)
        self.entry_depth = tk.Entry(control_frame, width=4, justify="center", font=("Arial", 10))
        self.entry_depth.insert(0, "1")
        self.entry_depth.pack(side=tk.LEFT, padx=2)

        tk.Button(control_frame, text="▶ Simular", command=self.sim, bg="#27ae60", fg="white", **btn_style).pack(side=tk.LEFT, padx=10)
        
        self.viz_frame = tk.Frame(root, bg=self.bg_color)
        self.viz_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

    def load(self):
        filename = filedialog.askopenfilename(filetypes=[("Text Files", "*.txt")])
        if filename: 
            try:
                self.engine.cargar(filename)
                self.loaded = True
                n, m = self.engine.stats()
                self.lbl_info.config(text=f"Nodos: {n} | Aristas: {m}")
                messagebox.showinfo("Carga Completa", f"Dataset cargado en memoria RAM.\nMotor C++ listo.")
            except Exception as e:
                messagebox.showerror("Error", str(e))

    def crit(self):
        if self.loaded:
            c = self.engine.critico()
            messagebox.showinfo("Nodo Crítico", f"El nodo más conectado es el ID: {c}")
            self.entry_start.delete(0, tk.END)
            self.entry_start.insert(0, str(c))
        else:
            messagebox.showwarning("Atención", "Primero carga un archivo.")

    def sim(self):
        if not self.loaded: return
        try:
            start_node = int(self.entry_start.get())
            depth = int(self.entry_depth.get())
            
            nodes_found = self.engine.simular(start_node, depth)
            
            self.draw_graph(nodes_found, start_node)
            
        except ValueError:
            messagebox.showerror("Error", "Números inválidos.")

    def draw_graph(self, nodes, center):
        for widget in self.viz_frame.winfo_children(): widget.destroy()

        G = nx.Graph()
        neighbors = []
        for n in nodes: 
            if n != center: 
                G.add_edge(center, n)
                neighbors.append(n)
        
        fig = plt.Figure(figsize=(8, 6), dpi=100, facecolor='#2c3e50') 
        ax = fig.add_subplot(111)
        ax.set_facecolor('#2c3e50') 
        
        pos = nx.spring_layout(G, k=0.15, iterations=20)
        
        
        nx.draw_networkx_edges(G, pos, ax=ax, alpha=0.1, edge_color="white")

        nx.draw_networkx_nodes(G, pos, nodelist=neighbors, 
                               node_color="#3498db",
                               node_size=30,         
                               alpha=0.6,            
                               ax=ax)

        nx.draw_networkx_nodes(G, pos, nodelist=[center], 
                               node_color="#e74c3c",
                               node_size=800,       
                               edgecolors="white",   
                               linewidths=2,
                               label=f"Centro {center}", 
                               ax=ax)
        
        ax.set_title(f"Propagación desde Nodo {center} ({len(nodes)} afectados)", color="white", fontsize=12)
        ax.axis('off') 

        canvas = FigureCanvasTkAgg(fig, self.viz_frame)
        canvas.draw()
        canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

if __name__ == "__main__":
    root = tk.Tk()
    app = App(root)
    root.mainloop()