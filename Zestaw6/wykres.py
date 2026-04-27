import matplotlib.pyplot as plt

moves = []

# Czytamy plik wygenerowany przez C++
try:
    with open("ruchy.txt", "r") as file:
        for line in file:
            x, y = map(int, line.split())
            moves.append((x, y))
except FileNotFoundError:
    print("Brak pliku ruchy.txt! Odpal najpierw program w C++.")
    exit()

# Rozdzielamy na osie X i Y
x = [m[0] for m in moves]
y = [m[1] for m in moves]

plt.figure(figsize=(8, 8))
plt.plot(x, y, marker='o', linestyle='-', color='b', markersize=8)

# Numerujemy kroki
for i, (xi, yi) in enumerate(moves):
    plt.text(xi, yi + 0.1, str(i + 1), color='red', fontsize=10, ha='center')

plt.grid(True)
# Ustaw rozmiar siatki na taki jak Twoje N (u Ciebie to 7)
plt.xticks(range(7))
plt.yticks(range(7))
plt.title("Zautomatyzowana Trasa Skoczka")
plt.show()