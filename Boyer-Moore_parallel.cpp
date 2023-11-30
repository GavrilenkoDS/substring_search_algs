#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>

const int ALPHABET_SIZE = 256;

void badCharHeuristic(const std::string& pattern, std::vector<int>& badChar) {
    int patternLength = pattern.length();

    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        badChar[i] = -1;
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

    #pragma omp parallel
    {
        std::vector<int> privateOccurrences;
        int threadNum = omp_get_thread_num();

        #pragma omp for
        for (int shift = 0; shift <= textLength - patternLength; ++shift) {
            int j = patternLength - 1;

            while (j >= 0 && pattern[j] == text[shift + j]) {
                --j;
            }

            if (j < 0) {
                // Found an occurrence
                privateOccurrences.push_back(shift);

                // Increment shift by the pattern length
                shift += (shift + patternLength < textLength) ? patternLength - badChar[text[shift + patternLength]] : 1;
            } else {
                // Shift based on bad character heuristic
                shift += std::max(1, j - badChar[text[shift + j]]);
            }
        }

        // Combine private occurrences into the global occurrences vector
        #pragma omp critical
        occurrences.insert(occurrences.end(), privateOccurrences.begin(), privateOccurrences.end());
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

