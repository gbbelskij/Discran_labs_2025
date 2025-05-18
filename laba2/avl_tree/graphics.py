import matplotlib.pyplot as plt
import numpy as np

# Данные
N_values = [10_000, 100_000, 1_000_000]

map_insert = [0.0176024, 0.0740294, 1.4053]
map_search = [0.000969953, 0.0065599, 0.132782]
map_erase =  [0.00150853, 0.00960739, 0.222105]

avl_insert = [0.0382857, 0.192351, 2.56781]
avl_search = [0.000497966, 0.00340816, 0.0683824]
avl_erase =  [0.00205475, 0.0168482, 0.188421]

# Функция построения графика
def plot_comparison(title, y_map, y_avl, ylabel):
    x = np.arange(len(N_values))  # позиции на оси X
    width = 0.35

    fig, ax = plt.subplots()
    ax.bar(x - width/2, y_map, width, label='std::map', color='steelblue')
    ax.bar(x + width/2, y_avl, width, label='AVL tree', color='darkorange')

    ax.set_xlabel('Количество элементов (N)')
    ax.set_ylabel(ylabel)
    ax.set_title(title)
    ax.set_xticks(x)
    ax.set_xticklabels([f'{n:,}' for n in N_values])
    ax.legend()
    ax.grid(True, linestyle='--', alpha=0.5)

    plt.tight_layout()
    plt.savefig(f"{title}.png")

# Построение графиков
plot_comparison('Время вставки (insert)', map_insert, avl_insert, 'Время (сек)')
plot_comparison('Время поиска (search)', map_search, avl_search, 'Время (сек)')
plot_comparison('Время удаления (erase)', map_erase, avl_erase, 'Время (сек)')
