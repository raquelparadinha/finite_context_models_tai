import tkinter as tk
from tkinter import filedialog
import customtkinter as ctk
from subprocess import Popen, PIPE

# Function to open a file
def open_file(input):
    filepath = filedialog.askopenfilename(filetypes=[("Text files", "*.txt")])
    input.delete(0, ctk.END)
    input.insert(0, filepath)

# Function to open a folder
def open_folder(input):
    folderpath = filedialog.askdirectory()
    input.delete(0, ctk.END)
    input.insert(0, folderpath)

# Main application window setup
app = ctk.CTk()
app.title('Identifier of Human or ChatGPT written text')
app.geometry('1000x600')

# Set appearance mode and color theme
ctk.set_appearance_mode('light')
ctk.set_default_color_theme('green')

# Widgets creation with consistent styling
widget_width = 400

# Message Label
message_label = ctk.CTkLabel(app, text="", font=('Arial', 16))
message_label.place(relx=0.5, rely=0.1, anchor='center')

# Grid layout configuration for central alignment
app.grid_rowconfigure(0, weight=1)
app.grid_rowconfigure(7, weight=1)
app.grid_columnconfigure(0, weight=1)
app.grid_columnconfigure(3, weight=1)

file_label = ctk.CTkLabel(app, text="Select file to analyse:")
file_label.grid(row=1, column=1, sticky='e', padx=10, pady=10)
file_entry = ctk.CTkEntry(app, width=widget_width, placeholder_text="File path")
file_entry.grid(row=1, column=2, padx=10)
file_btn = ctk.CTkButton(app, text="Browse", command=lambda: open_file(file_entry), fg_color="grey")
file_btn.grid(row=1, column=3, padx=10)

rh_label = ctk.CTkLabel(app, text="Select Human files collection:")
rh_label.grid(row=2, column=1, sticky='e', padx=10, pady=10)
rh_entry = ctk.CTkEntry(app, width=widget_width, placeholder_text="Folder path")
rh_entry.grid(row=2, column=2, padx=10)
rh_btn = ctk.CTkButton(app, text="Browse", command=lambda: open_folder(rh_entry), fg_color="grey")
rh_btn.grid(row=2, column=3, padx=10)

rc_label = ctk.CTkLabel(app, text="Select ChatGPT files collection:")
rc_label.grid(row=3, column=1, sticky='e', padx=10, pady=10)
rc_entry = ctk.CTkEntry(app, width=widget_width, placeholder_text="Folder path")
rc_entry.grid(row=3, column=2, padx=10)
rc_btn = ctk.CTkButton(app, text="Browse", command=lambda: open_folder(rc_entry), fg_color="grey")
rc_btn.grid(row=3, column=3, padx=10)

k_label = ctk.CTkLabel(app, text="Markov model order ([1, 10]):")
k_label.grid(row=4, column=1, sticky='e', padx=10, pady=10)
k_entry = ctk.CTkEntry(app, width=widget_width, placeholder_text="Integer value")
k_entry.grid(row=4, column=2, padx=10)

alpha_label = ctk.CTkLabel(app, text="Smoothing parameter (]0, 1[):")
alpha_label.grid(row=5, column=1, sticky='e', padx=10, pady=10)
alpha_entry = ctk.CTkEntry(app, width=widget_width, placeholder_text="Double value")
alpha_entry.grid(row=5, column=2, padx=10)

# Process inputs and close the application
def process_inputs():
    process = Popen(["./../bin/markov_model_classifier", rh_entry.get(), rc_entry.get(), file_entry.get(), alpha_entry.get(), k_entry.get()], stdout=PIPE, stderr=PIPE)
    output, errors = process.communicate()
    
    if process.returncode == 0:
        print("C++ program output:", output)
        response = output.decode(errors='ignore').split("\n")
        message_label.configure(text=response[0])
    else:
        print("Error:", errors.decode())


# Function to reset all input fields
def reset_inputs():
    file_entry.delete(0, ctk.END)
    file_entry.configure(placeholder_text="File path")
    rh_entry.delete(0, ctk.END)
    rh_entry.configure(placeholder_text="Folder path")
    rc_entry.delete(0, ctk.END)
    rc_entry.configure(placeholder_text="Folder path")
    k_entry.delete(0, ctk.END)
    k_entry.configure(placeholder_text="Integer value")
    alpha_entry.delete(0, ctk.END)
    alpha_entry.configure(placeholder_text="Double value")
    message_label.configure(text="")

submit_btn = ctk.CTkButton(app, text="Submit", command=process_inputs, fg_color="#008000")
submit_btn.grid(row=6, column=2, pady=20)

reset_btn = ctk.CTkButton(app, text="Reset", command=reset_inputs, fg_color="#8B0000", hover_color="#990000")
reset_btn.grid(row=6, column=3, pady=20)

app.mainloop()
