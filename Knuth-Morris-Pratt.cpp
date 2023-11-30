#include <iostream>
#include <vector>

// ??????? ??? ?????????? ???????-??????? ??????
std::vector<int> calculatePrefixFunction(const std::string& pattern) {
    int m = pattern.length();
    std::vector<int> prefixFunction(m, 0);
    int k = 0;

    for (int i = 1; i < m; ++i) {
        while (k > 0 && pattern[i] != pattern[k]) {
            k = prefixFunction[k - 1];
        }

        if (pattern[i] == pattern[k]) {
            ++k;
        }

        prefixFunction[i] = k;
    }

    return prefixFunction;
}

// ???????? ?????-???????-??????
std::vector<int> kmpSearch(const std::string& text, const std::string& pattern) {
    int n = text.length();
    int m = pattern.length();
    std::vector<int> prefixFunction = calculatePrefixFunction(pattern);
    std::vector<int> occurrences;

    int q = 0;

    for (int i = 0; i < n; ++i) {
        while (q > 0 && pattern[q] != text[i]) {
            q = prefixFunction[q - 1];
        }

        if (pattern[q] == text[i]) {
            ++q;
        }

        if (q == m) {
            occurrences.push_back(i - m + 1);
            q = prefixFunction[q - 1];
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
    std::string pattern = "abc";
	auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> result = kmpSearch(text, pattern);
	auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "time: " << duration.count() << std::endl;
    /*std::cout << "KMC\npattern: " << pattern << " text: " << text << ": ";
    for (int index : result) {
        std::cout << index << " ";
    }*/
    std::cout << std::endl;

    return 0;
}

