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
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<std::vector<uint32_t>> patterns;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) break;
        auto nums = parse_numbers(line);
        if (!nums.empty()) patterns.push_back(nums);
    }

    TAhoCorasick ac;
    for (int i = 0; i < static_cast<int>(patterns.size()); ++i)
        ac.AddPattern(patterns[i], i);

    ac.BuildFailLinks();

    int line_num = 0;
    while (std::getline(std::cin, line)) {
        ++line_num;
        std::vector<uint32_t> text = parse_numbers(line);
        std::vector<std::pair<int, int>> positions;
        for (int i = 0; i < static_cast<int>(text.size()); ++i) {
            positions.emplace_back(line_num, i + 1);
        }
        ac.Search(text, positions);
    }
    return 0;
}
