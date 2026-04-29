import matplotlib.pyplot as plt
import os

# Ścieżka do pliku z ruchami
# Ustawiona na "../ruchy.txt", bo CLion odpala programy w folderze cmake-build-debug
file_path = "../ruchy.txt"

moves = []

# 1. Wczytywanie danych
try:
    with open(file_path, "r") as file:
        for line in file:
            x_val, y_val = map(int, line.split())
            moves.append((x_val, y_val))
except FileNotFoundError:
    print(f"Błąd: Nie znaleziono pliku {os.path.abspath(file_path)}")
    print("Upewnij się, że najpierw odpaliłeś program w C++!")
    exit()

# 2. Przygotowanie osi
x = [m[0] for m in moves]
y = [m[1] for m in moves]

# 3. Dynamiczne obliczanie rozmiaru planszy N
N = max(max(x), max(y)) + 1

# 4. Tworzenie grafiki
# Skalujemy rozmiar okna w zależności od wielkości szachownicy
plt.figure(figsize=(max(6, N), max(6, N)))

# Rysujemy linię trasy (ciemny granat)
plt.plot(x, y, marker='o', linestyle='-', color='#2c3e50',
         markersize=8, linewidth=2, zorder=1)

# 5. Numerowanie kroków (czerwone numery nad punktami)
for i, (xi, yi) in enumerate(moves):
    plt.text(xi, yi + 0.15, str(i + 1), color='#e74c3c',
             fontweight='bold', fontsize=9, ha='center', zorder=2)

# 6. Stylizacja szachownicy
plt.grid(True, linestyle='--', alpha=0.6)
plt.xticks(range(N))
plt.yticks(range(N))
plt.gca().set_aspect('equal', adjustable='box')
plt.title(f"Trasa Skoczka Szachowego ({N}x{N})", fontsize=14, pad=20)
plt.savefig("../trasa_skoczka.png")
