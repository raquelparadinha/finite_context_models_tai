import pandas as pd

df = pd.read_csv('AI_Human.csv')

human_texts = df[df['generated'] == 1.0]['text'] 
human_texts = human_texts.sample(frac=1).reset_index(drop=True)

max_size = 5 * 1024 * 1024  
current_size = 0
index = 0  

with open('dataset/human/gpt_written_texts.txt', 'w', encoding='utf-8') as file:
    for i, text in enumerate(human_texts):
        text_with_newline = text + '\n'
        text_size = len(text_with_newline.encode('utf-8'))
        if current_size + text_size > max_size:
            index = i 
            break
        file.write(text_with_newline)
        current_size += text_size

for j in range(500):
    if index + j < len(human_texts): 
        with open(f'test/gpt/text_file_{j + 1}.txt', 'w', encoding='utf-8') as file:
            file.write(human_texts[index + j])
