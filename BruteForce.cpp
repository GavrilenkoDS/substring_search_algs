#include <iostream>
#include <vector>

std::vector<int> bruteForceSearch(const std::string& text, const std::string& pattern) {
    std::vector<int> occurrences;

    int textLength = text.length();
    int patternLength = pattern.length();

    for (int i = 0; i <= textLength - patternLength; ++i) {
        bool match = true;
        for (int j = 0; j < patternLength; ++j) {
            if (text[i + j] != pattern[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            occurrences.push_back(i);
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

    std::vector<int> result = bruteForceSearch(text, pattern);

	auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "time: " << duration.count() << std::endl;
	/*
    std::cout << "BRUTE FORCE\npattern: " << pattern << " text: " << text << ": ";
    for (int index : result) {
        std::cout << index << " ";
    }*/
    std::cout << std::endl;

    return 0;
}

