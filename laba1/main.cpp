#include <iostream>
#include <chrono>

struct TEntry {
    char number[9];  // 9 потому что ещё 1 на \0
    u_int64_t value;
};

int GetCharIndex(char c) {  // получаем индекс символа, буквы или цифры
    if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    }
    return c - '0' + 26;
}

void RadixSort(TEntry* entries, int numStrings) {
    for (int j = 7; j >= 0; j--) {
        TEntry* sorted = new TEntry[numStrings];  // выделяем память для отсортированных элементов
        int count[36] = {0};

        for (int i = 0; i < numStrings; i++) {  // 1 этап, подсчитываем сколько каких символов
            int index = GetCharIndex(entries[i].number[j]);
            count[index]++;
        }

        for (int i = 1; i < 36; i++) {  // 2 этап, накопительно подсчитываем все числа с 1 этапа
            count[i] += count[i - 1];
        }

        for (int i = numStrings - 1; i >= 0; i--) {  // 3 этап, по полученным индексам расставляем наши элементы
            int index = GetCharIndex(entries[i].number[j]);
            sorted[--count[index]] = entries[i];
        }

        for (int i = 0; i < numStrings; i++) {
            entries[i] = sorted[i];
        }

        delete[] sorted;  // освобождаем память
    }
}

int main() {
    std::cin.tie(0);
    std::cout.tie(0);
    std::ios::sync_with_stdio(false);
    const int MAX_LENGTH = 256;  // длина строки
    int capacity = 10;  // вместимость массива
    int numStrings = 0;  // количество считанных строк

    TEntry* entries = new TEntry[capacity];  // выделяем память с помощью new

    if (entries == nullptr) {
        std::cout << "Memory allocation failed!" << '\n';
        return 1;
    }

    char buffer[MAX_LENGTH];

    // считываем строки с номерами автомобилей и значениями
    while (std::cin.getline(buffer, MAX_LENGTH)) {
        if (buffer[0] != '\0') {
            if (numStrings == capacity) {
                capacity *= 2;
                TEntry* temp = new TEntry[capacity];  // выделяем новый массив
                if (temp == nullptr) {
                    std::cout << "Memory reallocation failed!" << '\n';
                    delete[] entries;  // освобождаем старый массив
                    return 1;
                }
                // копируем данные из старого массива в новый
                for (int i = 0; i < numStrings; i++) {
                    temp[i] = entries[i];
                }
                delete[] entries;  // освобождаем старый массив
                entries = temp;
            }

            int i = 0;
            while (buffer[i] != '\0' && buffer[i] != '\t' && i < 8) {
                entries[numStrings].number[i] = buffer[i];
                i++;
            }
            entries[numStrings].number[i] = '\0';

            while (buffer[i] == '\t') {
                i++;
            }

            entries[numStrings].value = 0;
            while (buffer[i] != '\0') {
                entries[numStrings].value = entries[numStrings].value * 10 + (buffer[i] - '0');
                i++;
            }

            numStrings++;
        }
    }

    auto start = std::chrono::steady_clock::now();
    RadixSort(entries, numStrings);

    for (int i = 0; i < numStrings; i++) {
        std::cout << entries[i].number << '\t' << entries[i].value << '\n';
    }

    delete[] entries;  // освобождаем память
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    // std::cout << "Время работы программы: " << duration.count() << " миллисекунд." << std::endl;
    return 0;
}
