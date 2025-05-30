import random

# Параметры теста
num_patterns = 1000      # Количество образцов
pattern_length = 5       # Длина каждого образца
num_lines = 1000         # Количество строк в тексте
line_length = 1000       # Количество чисел в строке

with open("test_large.txt", "w") as f:
    # Генерация образцов
    for _ in range(num_patterns):
        pattern = [str(random.randint(1, 1000000)) for _ in range(pattern_length)]
        f.write(" ".join(pattern) + "\n")
    f.write("\n")
    # Генерация текста
    for _ in range(num_lines):
        line = [str(random.randint(1, 1000000)) for _ in range(line_length)]
        f.write(" ".join(line) + "\n")