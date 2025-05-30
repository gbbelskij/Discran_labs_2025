import matplotlib.pyplot as plt
import numpy as np

pattern_counts = [1000, 500, 100]
text_sizes = [10000000, 250000, 10000]

aho_times = [0.199397, 0.0471974, 0.0046727]
naive_times = [8.75666, 1.14342, 0.0160003]

# Логарифмы по основанию 10
log_pattern_counts = np.log10(pattern_counts)
log_text_sizes = np.log10(text_sizes)
log_aho_times = np.log10(aho_times)
log_naive_times = np.log10(naive_times)

plt.figure(figsize=(10, 6))
plt.plot(log_pattern_counts, log_aho_times, marker='o', label='Aho-Corasick')
plt.plot(log_pattern_counts, log_naive_times, marker='o', label='Naive search')
plt.xlabel('log10(Patterns)')
plt.ylabel('log10(time)')
plt.title('Сравнение времени работы алгоритмов при разном количестве паттернов')
plt.legend()
plt.grid(True)
plt.xticks(log_pattern_counts, [f'$10^{{{int(np.log10(x))}}}$' for x in pattern_counts])
plt.show()

plt.figure(figsize=(10, 6))
plt.plot(log_text_sizes, log_aho_times, marker='o', label='Aho-Corasick')
plt.plot(log_text_sizes, log_naive_times, marker='o', label='Naive search')
plt.xlabel('log10(text)')
plt.ylabel('log10(time)')
plt.title('Сравнение времени работы алгоритмов при разном размере текста')
plt.legend()
plt.grid(True)
plt.xticks(log_text_sizes, [f'$10^{{{int(np.log10(x))}}}$' for x in text_sizes])
plt.show()