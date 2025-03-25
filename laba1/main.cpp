#include "iostream"


struct Entry {
    char number[9];  // 9 потмоу что еще 1 на \0
    u_int64_t value;
};


int getCharIndex(char c) {  // получаем индекс символа, буквы или цифры
    if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    }
    return c - '0' + 26;
}


void radixSort(Entry* entries, int numStrings) {
    for (int j = 7; j >= 0; j--) {
        Entry* sorted = (Entry*)malloc(numStrings * sizeof(Entry));  // храненим отсортированные элементы
        int count[36] = {0};

        
        for (int i = 0; i < numStrings; i++) {  // 1 этап, подсчитываем сколько каких символов
            int index = getCharIndex(entries[i].number[j]);
            count[index]++;
        }
        
        
        for (int i = 1; i < 36; i++) {  // 2 этап, накопительно подсчитываем все числа с 1 этапа
            count[i] += count[i - 1];
        }
        

        for (int i = numStrings - 1; i >= 0; i--) {  // 3 этап, по полученным индексам расставляем наши элементы
            int index = getCharIndex(entries[i].number[j]);
            sorted[--count[index]] = entries[i];
        }
        

        for (int i = 0; i < numStrings; i++) {
            entries[i] = sorted[i];
        }

        free(sorted);
    }
}

int main() {
    const int MaxLength = 256;  // длина строки
    int capacity = 10;  // вместимость массива
    int numStrings = 0;  // количество считанных строк

    Entry* entries = (Entry*)malloc(capacity * sizeof(Entry));
    if (entries == nullptr) {
        std::cout << "Memory allocation failed!" << '\n';
        return 1;
    }

    char buffer[MaxLength];

    // считываем строки с номерами автомобилей и значениями
    while (std::cin.getline(buffer, MaxLength)) {
        if (buffer[0] != '\0') {
            if (numStrings == capacity) {
                capacity *= 2;
                Entry* temp = (Entry*)realloc(entries, capacity * sizeof(Entry));
                if (temp == nullptr) {
                    std::cout << "Memory reallocation failed!" << '\n';
                    free(entries);
                    return 1;
                }
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

    radixSort(entries, numStrings);

    for (int i = 0; i < numStrings; i++) {
        std::cout << entries[i].number << '\t' << entries[i].value << '\n';
    }

    free(entries);

    return 0;
}
