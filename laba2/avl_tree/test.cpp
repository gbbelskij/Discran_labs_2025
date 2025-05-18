#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdint>


constexpr uint16_t MAX_STR_LEN = 256;

char makeLowercase(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

int8_t compareStrings(const char s1[MAX_STR_LEN + 1], uint16_t len1, 
                     const char s2[MAX_STR_LEN + 1], uint16_t len2) {
    uint16_t min_len = len1 < len2 ? len1 : len2;
    for (uint16_t i = 0; i < min_len; ++i) {
        if (s1[i] != s2[i]) {
            return s1[i] > s2[i] ? 1 : -1;
        }
    }
    if (len1 == len2) return 0;
    return len1 > len2 ? 1 : -1;
}

void resetString(char str[MAX_STR_LEN + 1]) {
    for (uint16_t i = 0; i <= MAX_STR_LEN; ++i) {
        str[i] = '\0';
    }
}

struct TreeNode {
    uint64_t depth;
    TreeNode* left_child;
    TreeNode* right_child;
    char data[MAX_STR_LEN + 1];
    uint16_t length;
    uint64_t value;
};

TreeNode* createNode(TreeNode* left, TreeNode* right, 
                    const char str[MAX_STR_LEN + 1], 
                    uint16_t size, uint64_t val) {
    TreeNode* node = new TreeNode;
    node->left_child = left;
    node->right_child = right;
    for (uint16_t i = 0; i <= MAX_STR_LEN; ++i) {
        node->data[i] = str[i];
    }
    node->length = size;
    node->value = val;
    node->depth = 1;
    return node;
}

void deleteTree(TreeNode* node) {
    if (!node) return;
    deleteTree(node->left_child);
    deleteTree(node->right_child);
    delete node;
}

uint64_t getDepth(const TreeNode* node) {
    return node ? node->depth : 0;
}

int8_t getBalanceFactor(const TreeNode* node) {
    return getDepth(node->right_child) - getDepth(node->left_child);
}

void updateDepth(TreeNode* node) {
    uint64_t left_depth = getDepth(node->left_child);
    uint64_t right_depth = getDepth(node->right_child);
    node->depth = (left_depth > right_depth ? left_depth : right_depth) + 1;
}

TreeNode* findNode(TreeNode* current, const char str[MAX_STR_LEN + 1], 
                  uint16_t str_len) {
    while (current) {
        int8_t cmp = compareStrings(current->data, current->length, str, str_len);
        if (cmp == 0) break;
        current = cmp > 0 ? current->left_child : current->right_child;
    }
    return current;
}

TreeNode* rotateRight(TreeNode* root) {
    TreeNode* new_root = root->left_child;
    root->left_child = new_root->right_child;
    new_root->right_child = root;
    updateDepth(root);
    updateDepth(new_root);
    return new_root;
}

TreeNode* rotateLeft(TreeNode* root) {
    TreeNode* new_root = root->right_child;
    root->right_child = new_root->left_child;
    new_root->left_child = root;
    updateDepth(root);
    updateDepth(new_root);
    return new_root;
}

TreeNode* balanceTree(TreeNode* node) {
    updateDepth(node);
    int8_t balance = getBalanceFactor(node);
    
    if (balance > 1) {
        if (getBalanceFactor(node->right_child) < 0) {
            node->right_child = rotateRight(node->right_child);
        }
        return rotateLeft(node);
    }
    if (balance < -1) {
        if (getBalanceFactor(node->left_child) > 0) {
            node->left_child = rotateLeft(node->left_child);
        }
        return rotateRight(node);
    }
    return node;
}

TreeNode* insertNode(TreeNode* parent, const char str[MAX_STR_LEN + 1], 
                    uint16_t str_len, uint64_t num) {
    int8_t cmp = compareStrings(parent->data, parent->length, str, str_len);
    
    if (cmp > 0) {
        parent->left_child = parent->left_child ? 
            insertNode(parent->left_child, str, str_len, num) : 
            createNode(nullptr, nullptr, str, str_len, num);
    } 
    else if (cmp < 0) {
        parent->right_child = parent->right_child ? 
            insertNode(parent->right_child, str, str_len, num) : 
            createNode(nullptr, nullptr, str, str_len, num);
    }
    
    return balanceTree(parent);
}

TreeNode* findMinimum(TreeNode* node) {
    while (node->left_child) {
        node = node->left_child;
    }
    return node;
}

TreeNode* removeMinimum(TreeNode* node) {
    if (!node->left_child) {
        return node->right_child;
    }
    node->left_child = removeMinimum(node->left_child);
    return balanceTree(node);
}

TreeNode* deleteNode(TreeNode* node, const char str[MAX_STR_LEN + 1], 
                    uint16_t str_len) {
    if (!node) return nullptr;
    
    int8_t cmp = compareStrings(node->data, node->length, str, str_len);
    
    if (cmp > 0) {
        node->left_child = deleteNode(node->left_child, str, str_len);
    } 
    else if (cmp < 0) {
        node->right_child = deleteNode(node->right_child, str, str_len);
    } 
    else {
        TreeNode* left = node->left_child;
        TreeNode* right = node->right_child;
        delete node;
        
        if (!right) return left;
        
        TreeNode* min = findMinimum(right);
        min->right_child = removeMinimum(right);
        min->left_child = left;
        return balanceTree(min);
    }
    
    return balanceTree(node);
}

void saveToFile(const TreeNode* node, FILE* file) {
    if (!node) return;
    std::fwrite(&node->length, sizeof(uint16_t), 1, file);
    std::fwrite(node->data, sizeof(char), MAX_STR_LEN + 1, file);
    std::fwrite(&node->value, sizeof(uint64_t), 1, file);
    saveToFile(node->left_child, file);
    saveToFile(node->right_child, file);
}

class BalancedDictionary {
    TreeNode* root;
    
public:
    BalancedDictionary() : root(nullptr) {}
    ~BalancedDictionary() { deleteTree(root); }
    
    TreeNode* search(const char str[MAX_STR_LEN + 1], uint16_t str_len) {
        return findNode(root, str, str_len);
    }
    
    bool add(const char str[MAX_STR_LEN + 1], uint16_t str_len, uint64_t num) {
        if (!root) {
            root = createNode(nullptr, nullptr, str, str_len, num);
            return true;
        }
        if (findNode(root, str, str_len)) {
            return false;
        }
        root = insertNode(root, str, str_len, num);
        return true;
    }
    
    bool remove(const char str[MAX_STR_LEN + 1], uint16_t str_len) {
        if (!findNode(root, str, str_len)) {
            return false;
        }
        root = deleteNode(root, str, str_len);
        return true;
    }
    
    void importData(FILE* file) {
        deleteTree(root);
        root = nullptr;
        uint16_t key_len;
        char key[MAX_STR_LEN + 1];
        uint64_t val;
        
        while (std::fread(&key_len, sizeof(uint16_t), 1, file) && 
               std::fread(key, sizeof(char), MAX_STR_LEN + 1, file) && 
               std::fread(&val, sizeof(uint64_t), 1, file)) {
            add(key, key_len, val);
        }
    }
    
    void exportData(FILE* file) {
        saveToFile(root, file);
    }
};



// Подключи сюда свой BalancedDictionary
// #include "your_avl_tree.hpp"

// constexpr uint16_t MAX_STR_LEN = 256;

// Генерация случайных строк
std::string random_string(size_t length) {
    static const char charset[] =
        "abcdefghijklmnopqrstuvwxyz";
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<> dist(0, sizeof(charset) - 2);

    std::string result;
    result.reserve(length);
    for (size_t i = 0; i < length; ++i)
        result += charset[dist(rng)];
    return result;
}

int main() {
    constexpr int N = 1000000;  // количество элементов
    constexpr int M = 100000;   // сколько потом искать/удалять

    // Сгенерируем данные
    std::vector<std::string> keys;
    std::vector<uint64_t> values;

    for (int i = 0; i < N; ++i) {
        keys.push_back(random_string(8));
        values.push_back(i);
    }

    std::vector<std::string> search_keys(keys.begin(), keys.begin() + M);

    // ======================= Тест std::map ========================
    std::map<std::string, uint64_t> std_map;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i)
        std_map[keys[i]] = values[i];
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "std::map insert time: " << std::chrono::duration<double>(end - start).count() << "s\n";

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < M; ++i)
        std_map.find(search_keys[i]);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "std::map search time: " << std::chrono::duration<double>(end - start).count() << "s\n";

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < M; ++i)
        std_map.erase(search_keys[i]);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "std::map erase time: " << std::chrono::duration<double>(end - start).count() << "s\n";


    // ==================== Тест BalancedDictionary =======================
    BalancedDictionary avl;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        char buf[MAX_STR_LEN + 1] = {};
        std::strncpy(buf, keys[i].c_str(), MAX_STR_LEN);
        avl.add(buf, keys[i].length(), values[i]);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "AVL insert time: " << std::chrono::duration<double>(end - start).count() << "s\n";

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < M; ++i) {
        char buf[MAX_STR_LEN + 1] = {};
        std::strncpy(buf, search_keys[i].c_str(), MAX_STR_LEN);
        avl.search(buf, search_keys[i].length());
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "AVL search time: " << std::chrono::duration<double>(end - start).count() << "s\n";

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < M; ++i) {
        char buf[MAX_STR_LEN + 1] = {};
        std::strncpy(buf, search_keys[i].c_str(), MAX_STR_LEN);
        avl.remove(buf, search_keys[i].length());
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "AVL erase time: " << std::chrono::duration<double>(end - start).count() << "s\n";

    return 0;
}