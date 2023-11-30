#include <iostream>
#include <vector>
#include <algorithm>

const int ALPHABET_SIZE = 256;


void badCharHeuristic(const std::string& pattern, std::vector<int>& badChar) {
    int patternLength = pattern.length();

    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        badChar[i] = -1;  // Инициализация массива сдвигов значением -1
    }

    for (int i = 0; i < patternLength; ++i) {
        badChar[static_cast<int>(pattern[i])] = i;
    }
}

std::vector<int> boyerMooreSearch(const std::string& text, const std::string& pattern) {
    std::vector<int> occurrences;
    int textLength = text.length();
    int patternLength = pattern.length();

    std::vector<int> badChar(ALPHABET_SIZE, 0);
    badCharHeuristic(pattern, badChar);

    int shift = 0;
    while (shift <= textLength - patternLength) {
        int j = patternLength - 1;

        while (j >= 0 && pattern[j] == text[shift + j]) {
            --j;
        }

        if (j < 0) {
            // Найдено вхождение
            occurrences.push_back(shift);
            shift += (shift + patternLength < textLength) ? patternLength - badChar[text[shift + patternLength]] : 1;
        } else {
            // Сдвиг по правилу "плохого символа"
            shift += std::max(1, j - badChar[text[shift + j]]);
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

    std::vector<int> result = boyerMooreSearch(text, pattern);

	auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "time: " << duration.count() << std::endl;

    /*std::cout << "BOYER-MOORE\npattern: " << pattern << " text: " << text << ": ";
    for (int index : result) {
        std::cout << index << " ";
    }*/
    std::cout << std::endl;

    return 0;
}

