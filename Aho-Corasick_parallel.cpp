#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

const int ALPHABET_SIZE = 26;

struct TrieNode {
    TrieNode* children[ALPHABET_SIZE];
    TrieNode* fail;
    std::vector<int> output;

    TrieNode() {
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            children[i] = nullptr;
        }
        fail = nullptr;
    }
};

void insertTrie(TrieNode* root, const std::string& keyword, int keywordIndex) {
    TrieNode* node = root;
    for (char c : keyword) {
        int index = c - 'a';
        if (!node->children[index]) {
            node->children[index] = new TrieNode();
        }
        node = node->children[index];
    }
    node->output.push_back(keywordIndex);
}

void buildAhoCorasick(TrieNode* root) {
    std::queue<TrieNode*> q;
    root->fail = nullptr;

    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (root->children[i]) {
            root->children[i]->fail = root;
            q.push(root->children[i]);
        } else {
            root->children[i] = root;
        }
    }

    while (!q.empty()) {
        TrieNode* current = q.front();
        q.pop();

        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            if (current->children[i]) {
                TrieNode* next = current->children[i];
                q.push(next);

                TrieNode* failNode = current->fail;
                while (failNode->children[i] == nullptr && failNode != root) {
                    failNode = failNode->fail;
                }
                next->fail = failNode->children[i] ? failNode->children[i] : root;

                next->output.insert(next->output.end(), next->fail->output.begin(), next->fail->output.end());
            }
        }
    }
}

std::vector<int> ahoCorasickSearch(const std::string& text, TrieNode* root, const std::vector<std::string>& keywords) {
    std::vector<int> occurrences;
    TrieNode* currentState = root;

    #pragma omp parallel for
    for (int i = 0; i < text.length(); ++i) {
        int index = text[i] - 'a';

        while (currentState->children[index] == nullptr && currentState != root) {
            currentState = currentState->fail;
        }

        currentState = currentState->children[index] ? currentState->children[index] : root;

        for (int outputIndex : currentState->output) {
            #pragma omp critical
            occurrences.push_back(i - keywords[outputIndex].length() + 1);
        }
    }

    return occurrences;
}
#include <chrono>
int main() {
    std::string text = "ababcababcabcabc";
    int repeatCount = 20;


    for (int i = 0; i < repeatCount; ++i) {
        text += text;
    }
    std::vector<std::string> keywords = {"abc"};
	auto start = std::chrono::high_resolution_clock::now();
    TrieNode* root = new TrieNode();

    for (int i = 0; i < keywords.size(); ++i) {
        insertTrie(root, keywords[i], i);
    }

    buildAhoCorasick(root);

    std::vector<int> result = ahoCorasickSearch(text, root, keywords);
 	auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "time: " << duration.count() << std::endl; 
    /*std::cout << "AhoCorasick\npattern: " << keywords[0] << " text: " << text << ": ";
    for (int index : result) {
        std::cout << index << " ";
    }*/
    std::cout << std::endl;

    return 0;
}

