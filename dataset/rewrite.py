import os
from openai import OpenAI
from dotenv import load_dotenv

load_dotenv()

OPENAI_API_KEY = os.getenv('OPENAI_KEY')
client = OpenAI(api_key=OPENAI_API_KEY)

IN_FOLDER = 'human/'
OUT_FOLDER = 'chatted/'
APPEND = '_clean.txt'

MAX_TOKENS = 800

def main():
    print("Start")
    for file in os.listdir(IN_FOLDER):
        sections = []
        print("Check", file)
        if file.endswith(APPEND):
            print("Processing file " + file)
            text = ''
            with open(IN_FOLDER + file, 'r') as f:  
                text = f.read()
            sec = ''
            for line in text.split('\n'):
                if len(sec) + len(line) < MAX_TOKENS * 4:
                    sec += line + '\n'
                else:
                    sections.append(sec)
                    sec = ''

            checkpoint_file = OUT_FOLDER + '/checkpoint_' + file
            checkpoint = 0
            if os.path.exists(checkpoint_file):
                with open(checkpoint_file, 'r') as f:
                    checkpoint = int(f.readlines()[-1])

            n_sections = len(text) // (MAX_TOKENS * 4)
            with open(OUT_FOLDER + file[:len(APPEND)] + '.txt', 'w') as f:
                f.write('')

            for i, sec in enumerate(sections):
                if checkpoint > i:
                    print(f'Skipping {file} section {i}/{n_sections}')
                    continue

                print(f'Processing {file} section {i}/{n_sections}')
                rewrite_text = rewrite(sec, 'gpt-3.5-turbo')
                with open(OUT_FOLDER + file[:len(APPEND)] + '.txt', 'a') as f:
                    f.write(rewrite_text + '\n')
                with open(checkpoint_file, 'a') as f:
                    f.write(str(i) + '\n')
            
            os.remove(checkpoint_file)
            print("Done with file " + file)

def rewrite(human_text, model):
    response = client.chat.completions.create(
    model=model,
    messages=[
        {
        "role": "user",
        "content": f'Rewrite the following text in Portugal\'s portuguese, maintaining roughly the same number of characters:{human_text}'
        }
    ],
    temperature=1,
    max_tokens=2048,
    top_p=1,
    frequency_penalty=0,
    presence_penalty=0
    )
    return response.choices[0].message.content

if __name__ == '__main__':
    main()