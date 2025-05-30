#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <sstream>
#include <algorithm>
#include <tuple>
#include <cstdint>


struct TNode {
    std::map<uint32_t, TNode*> Children;
    TNode* Fail;
    std::vector<int> Outputs;
    TNode() : Fail(nullptr) {}
};

class TAhoCorasick {
public:
    TAhoCorasick() {
        Root = new TNode();
        Root->Fail = Root;
    }

    void AddPattern(const std::vector<uint32_t>& pattern, int index) {
        TNode* current = Root;
        for (const auto& num : pattern) {
            if (!current->Children.count(num)) {
                current->Children[num] = new TNode();
            }
            current = current->Children[num];
        }
        current->Outputs.push_back(index);
        PatternLengths.push_back(pattern.size());
    }

    void BuildFailLinks() {
        std::queue<TNode*> q;
        for (auto& pair : Root->Children) {
            TNode* child = pair.second;
            child->Fail = Root;
            q.push(child);
        }

        while (!q.empty()) {
            TNode* current = q.front();
            q.pop();

            for (auto& pair : current->Children) {
                uint32_t num = pair.first;
                TNode* child = pair.second;
                TNode* failNode = current->Fail;

                while (true) {
                    if (failNode->Children.count(num)) {
                        child->Fail = failNode->Children[num];
                        break;
                    }
                    if (failNode == Root) {
                        child->Fail = Root;
                        break;
                    }
                    failNode = failNode->Fail;
                }

                child->Outputs.insert(child->Outputs.end(),
                    child->Fail->Outputs.begin(),
                    child->Fail->Outputs.end());

                q.push(child);
            }
        }
    }

    void Search(const std::vector<uint32_t>& text, const std::vector<std::pair<int, int>>& positions) {
        TNode* current = Root;
        for (size_t i = 0; i < text.size(); ++i) {
            uint32_t num = text[i];

            while (true) {
                if (current->Children.count(num)) {
                    current = current->Children[num];
                    break;
                }
                if (current == Root) {
                    break;
                }
                current = current->Fail;
            }

            for (int patternId : current->Outputs) {
                int pat_len = PatternLengths[patternId];
                int pos = static_cast<int>(i) - pat_len + 1;
                if (pos >= 0) {
                    std::cout << positions[pos].first << ", "
                              << positions[pos].second << ", "
                              << patternId + 1 << "\n";
                }
            }
        }
    }

private:
    TNode* Root;
    std::vector<int> PatternLengths;
};

// Для примера: простой наивный поиск
void naive_search(
    const std::vector<std::vector<uint32_t>>& patterns,
    const std::vector<std::vector<uint32_t>>& text_lines)
{
    for (size_t line_num = 0; line_num < text_lines.size(); ++line_num) {
        const auto& line = text_lines[line_num];
        for (size_t word_pos = 0; word_pos < line.size(); ++word_pos) {
            for (size_t pat_id = 0; pat_id < patterns.size(); ++pat_id) {
                const auto& pat = patterns[pat_id];
                if (word_pos + pat.size() <= line.size()) {
                    bool match = true;
                    for (size_t k = 0; k < pat.size(); ++k) {
                        if (line[word_pos + k] != pat[k]) {
                            match = false;
                            break;
                        }
                    }
                    if (match) {
                        // std::cout << (line_num + 1) << ", " << (word_pos + 1) << ", " << (pat_id + 1) << "\n";
                    }
                }
            }
        }
    }
}

// --- Парсинг входных данных ---
std::vector<uint32_t> parse_numbers(const std::string& line) {
    std::vector<uint32_t> res;
    std::istringstream iss(line);
    std::string token;
    while (iss >> token) {
        res.push_back(static_cast<uint32_t>(std::stoul(token)));
    }
    return res;
}

int main() {
    // Чтение образцов
    std::vector<std::vector<uint32_t>> patterns;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) break;
        auto nums = parse_numbers(line);
        if (!nums.empty()) patterns.push_back(nums);
    }

    // Чтение текста
    std::vector<std::vector<uint32_t>> text_lines;
    while (std::getline(std::cin, line)) {
        auto nums = parse_numbers(line);
        text_lines.push_back(nums);
    }

    // --- Замер времени для Ахо-Корасик ---
    auto start_aho = std::chrono::high_resolution_clock::now();
    {
        // --- Здесь вставьте ваш рабочий TAhoCorasick и вызовите поиск ---
        // Пример: 
        TAhoCorasick ac;
        for (int i = 0; i < patterns.size(); ++i) ac.AddPattern(patterns[i], i);
        ac.BuildFailLinks();
        // Собираем весь текст в один массив и позиции, как в вашем рабочем коде
        std::vector<uint32_t> text;
        std::vector<std::pair<int, int>> positions;
        for (int line_num = 0; line_num < text_lines.size(); ++line_num) {
            for (int i = 0; i < text_lines[line_num].size(); ++i) {
                text.push_back(text_lines[line_num][i]);
                positions.emplace_back(line_num + 1, i + 1);
            }
        }
        ac.Search(text, positions);
    }
    auto end_aho = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_aho = end_aho - start_aho;
    std::cout << "Aho-Corasick: " << diff_aho.count() << " seconds\n";

    // --- Замер времени для наивного поиска ---
    auto start_naive = std::chrono::high_resolution_clock::now();
    naive_search(patterns, text_lines);
    auto end_naive = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_naive = end_naive - start_naive;
    std::cout << "Naive search: " << diff_naive.count() << " seconds\n";

    return 0;
}
