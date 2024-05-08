import pandas as pd
import os

# Load the dataset
df = pd.read_csv('AI_Human.csv')

# Filter rows where 'generated' column is 0.0
human_texts = df[df['generated'] == 1.0]['text']  # Replace 'text' with your actual text column name

# Shuffle the human_texts to randomize order
human_texts = human_texts.sample(frac=1).reset_index(drop=True)

# Save the main text file with a limit of 5 MB
max_size = 5 * 1024 * 1024  # 5 MB in bytes
current_size = 0
index = 0  # To track where to start for the next file

with open('dataset/gpt/gpt_written_texts.txt', 'w', encoding='utf-8') as file:
    for i, text in enumerate(human_texts):
        text_with_newline = text + '\n'
        text_size = len(text_with_newline.encode('utf-8'))
        if current_size + text_size > max_size:
            index = i  # Save the index where this text would exceed the limit
            break
        file.write(text_with_newline)
        current_size += text_size

# Now, create 10 separate text files for the next 10 texts
for j in range(10):
    if index + j < len(human_texts):  # Check if there are enough texts left
        with open(f'test/gpt/text_file_{j + 1}.txt', 'w', encoding='utf-8') as file:
            file.write(human_texts[index + j])
