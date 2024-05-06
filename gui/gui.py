import tkinter as tk
from tkinter import filedialog

def open_file(input):
    filepath = filedialog.askopenfilename(filetypes=[("Text files", "*.txt")])
    input.delete(0, tk.END)  # Clear the current entry
    input.insert(0, filepath)  # Insert the new file path

def open_folder(input):
    folderpath = filedialog.askdirectory()  # Opens a dialog to choose a directory
    input.delete(0, tk.END)  # Clear the current entry
    input.insert(0, folderpath) 

app = tk.Tk()
app.title('Identifier of Human or ChatGPT written text')

# Label and entry for the file input
file_label = tk.Label(app, text="Select file to analyse:")
file_label.grid(row=0, column=0, sticky='w')
file_entry = tk.Entry(app, width=50)
file_entry.grid(row=0, column=1)
file_btn = tk.Button(app, text="Browse", command=lambda: open_file(file_entry))
file_btn.grid(row=0, column=2)

rh_label = tk.Label(app, text="Select Human files collection:")
rh_label.grid(row=1, column=0, sticky='w')
rh_entry = tk.Entry(app, width=50)
rh_entry.grid(row=1, column=1)
rh_btn = tk.Button(app, text="Browse", command=lambda: open_folder(rh_entry))
rh_btn.grid(row=1, column=2)

rc_label = tk.Label(app, text="Select ChatGPT files collection:")
rc_label.grid(row=2, column=0, sticky='w')
rc_entry = tk.Entry(app, width=50)
rc_entry.grid(row=2, column=1)
rc_btn = tk.Button(app, text="Browse", command=lambda: open_folder(rc_entry))
rc_btn.grid(row=2, column=2)

# Label and entry for the first number input
k_label = tk.Label(app, text="Evaluation window size (k): <integer>")
k_label.grid(row=3, column=0, sticky='w')
k_entry = tk.Entry(app)
k_entry.grid(row=3, column=1)

# Label and entry for the second number input
alpha_label = tk.Label(app, text="Smoothing parameter (alpha): <double>")
alpha_label.grid(row=4, column=0, sticky='w')
alpha_entry = tk.Entry(app)
alpha_entry.grid(row=4, column=1)

def process_inputs():
    with open("inputs.txt", "w") as f:
        f.write(rh_entry.get() + '\n')
        f.write(rc_entry.get() + '\n')
        f.write(file_entry.get() + '\n')
        f.write(alpha_entry.get() + '\n')
        # f.write(k_entry.get() + '\n')
    app.destroy()

submit_btn = tk.Button(app, text="Submit", command=process_inputs)
submit_btn.grid(row=6, column=1)

app.mainloop()
