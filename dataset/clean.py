import os

IN_FOLDER = 'human/'
OUT_FOLDER = 'human/'
APPEND = '_clean'

def main():
    for file in os.listdir(IN_FOLDER):
        if file.endswith(APPEND + '.txt'):
            continue
        
        lines = []
        with open(IN_FOLDER + file, 'r') as f:
            lines = f.readlines()
            
        filtered_lines = [l for l in lines if len(l) > 1]
        filtered_lines = [l for l in filtered_lines if not l.startswith('CAPÍTULO')]
        filtered_lines = [l for l in filtered_lines if l.replace('I', '').replace('V', '').replace('X', '').strip() != '']
        filtered_lines = [l for l in filtered_lines if l.replace('*', '').replace(' ', '').strip() != '']

        with open(OUT_FOLDER + file.replace('.txt', APPEND + '.txt'), 'w') as f:
            f.writelines(filtered_lines)


if __name__ == '__main__':
    main()