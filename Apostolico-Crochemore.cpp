#include <iostream>
#include <vector>

std::vector<int> apostolicoCrochemoreSearch(const std::string& text, const std::string& pattern) {
    std::vector<int> occurrences;
    int textLength = text.length();
    int patternLength = pattern.length();


    std::vector<int> compressed(patternLength, 0);
    int i = 1, j = 0;
    while (i < patternLength) {
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


    i = 0; j = 0;
    while (i < textLength) {
        if (text[i] == pattern[j]) {
            ++i; ++j;
            if (j == patternLength) {
                occurrences.push_back(i - j);
                j = compressed[j - 1];
            }
        } else {
            if (j != 0) {
                j = compressed[j - 1];
            } else {
                ++i;
            }
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

    std::vector<int> result = apostolicoCrochemoreSearch(text, pattern);
    
	auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "time: " << duration.count() << std::endl;

    /*std::cout << "Apostolico-Crochemore\npattern: " << pattern << " text: " << text << ": ";
    for (int index : result) {
        std::cout << index << " ";
    }*/
    std::cout << std::endl;

    return 0;
}

