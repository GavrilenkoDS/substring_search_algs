#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>

struct Suffix {
    int index;
    std::string suffix;

    Suffix(int i, const std::string& s) : index(i), suffix(s) {}
};

std::vector<int> buildSuffixArray(const std::string& text) {
    int n = text.length();
    std::vector<Suffix> suffixes;
    for (int i = 0; i < n; ++i) {
        suffixes.emplace_back(i, text.substr(i));
    }

    std::sort(suffixes.begin(), suffixes.end(), [](const Suffix& a, const Suffix& b) {
        return a.suffix < b.suffix;
    });

    std::vector<int> suffixArray(n);
    for (int i = 0; i < n; ++i) {
        suffixArray[i] = suffixes[i].index;
    }

    return suffixArray;
}

std::vector<int> searchSubstring(const std::string& text, const std::string& pattern, const std::vector<int>& suffixArray) {
    int n = text.length();
    int m = pattern.length();
    std::vector<int> occurrences;

    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        if (text.compare(suffixArray[i], m, pattern) == 0) {
            #pragma omp critical
            occurrences.push_back(suffixArray[i]);
        }
    }

    return occurrences;
}
#include <chrono>
int main() {
    std::string text = "ababcababcabcabc";
    int repeatCount = 10;


    for (int i = 0; i < repeatCount; ++i) {
        text += text;
    }
    std::string pattern = "abc";
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> suffixArray = buildSuffixArray(text);

    std::vector<int> result = searchSubstring(text, pattern, suffixArray);

	auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "time: " << duration.count() << std::endl; 

    /*std::cout << "Suffix Array\npattern: " << pattern << " text: " << text << ": ";
    for (int index : result) {
        std::cout << index << " ";
    }*/
    std::cout << std::endl;

    return 0;
}

