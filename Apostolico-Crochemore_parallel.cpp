#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

std::vector<int> apostolicoCrochemoreSearch(const std::string& text, const std::string& pattern) {
    std::vector<int> occurrences;
    int textLength = text.length();
    int patternLength = pattern.length();

    std::vector<int> compressed(patternLength, 0);

    for (int i = 1, j = 0; i < patternLength; ) {
        if (pattern[i] == pattern[j]) {
            compressed[i] = j + 1;
            ++i; ++j;
        } else {
            if (j != 0) {
                j = compressed[j - 1];
            } else {
                compressed[i] = 0;
                ++i;
            }
        }
    }

    #pragma omp parallel
    {
        std::vector<int> localOccurrences;

        #pragma omp for
        for (int i = 0; i < textLength; ++i) {
            int local_j = 0;
            int local_i = i;

            while (local_i < textLength) {
                if (text[local_i] == pattern[local_j]) {
                    ++local_i; ++local_j;
                    if (local_j == patternLength) {
                        localOccurrences.push_back(local_i - local_j);
                        local_j = compressed[local_j - 1];
                    }
                } else {
                    if (local_j != 0) {
                        local_j = compressed[local_j - 1];
                    } else {
                        ++local_i;
                    }
                }
            }
        }

        #pragma omp critical
        occurrences.insert(occurrences.end(), localOccurrences.begin(), localOccurrences.end());
    }

    return occurrences;
}

int main() {
    std::string text = "ababcababcabcabc";
    int repeatCount = 20;

    for (int i = 0; i < repeatCount; ++i) {
        text += text;
    }

    std::string pattern = "abc";

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<int> result = apostolicoCrochemoreSearch(text, pattern);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "time: " << duration.count() << std::endl;

    return 0;
}

