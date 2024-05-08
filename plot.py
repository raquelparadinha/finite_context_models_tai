import matplotlib.pyplot as plt

alphas = []
context_size = []
accuracy_gpt = []
accuracy_original = []

with open("results.txt","r") as f:
    for line in f:
        values = line.split(",")
        alphas.append(values[0])
        context_size.append(values[1])
        accuracy_gpt.append(values[2])
        accuracy_original.append(values[3])
        



accuracy_gpt_normalized = [float(a) / 100 for a in accuracy_gpt]
accuracy_original_normalized = [float(a) / 100 for a in accuracy_original]

path_gpt = "images/"

plt.scatter(alphas, context_size, c=accuracy_gpt_normalized, cmap='RdYlGn', s=100, alpha=0.5)
plt.colorbar(label='Accuracy GPT')
plt.xlabel('Alpha')
plt.ylabel('Context Size')
plt.title('Accuracy vs. Inputs')
plt.savefig('accuracy_gpt.png')
plt.close()

plt.scatter(alphas, context_size, c=accuracy_original_normalized, cmap='RdYlGn', s=100, alpha=0.5)
plt.colorbar(label='Accuracy Original')
plt.xlabel('Alpha')
plt.ylabel('Context Size')
plt.title('Accuracy vs. Inputs')
plt.savefig('accuracy_original.png')
plt.close()
