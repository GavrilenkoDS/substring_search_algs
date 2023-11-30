#include <iostream>
#include <vector>

// ??????? ??? ?????????? Z-??????? ??????
std::vector<int> calculateZ(const std::string& s) {
    int n = s.length();
    std::vector<int> z(n, 0);
    int l = 0, r = 0;

    for (int i = 1; i < n; ++i) {
        if (i <= r) {
            z[i] = std::min(r - i + 1, z[i - l]);
        }

        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            ++z[i];
        }

        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }

    return z;
}

// ???????? ???????
std::vector<int> colussiSearch(const std::string& text, const std::string& pattern) {
    std::string concatenated = pattern + "$" + text;
    std::vector<int> z = calculateZ(concatenated);
    std::vector<int> occurrences;

    for (int i = pattern.length() + 1; i < concatenated.length(); ++i) {
        if (z[i] == pattern.length()) {
            occurrences.push_back(i - pattern.length() - 1);
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
    std::vector<int> result = colussiSearch(text, pattern);
	auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "time: " << duration.count() << std::endl;
    /*std::cout << "ColussiSearch\npattern: " << pattern << " text: " << text << ": ";
    for (int index : result) {
        std::cout << index << " ";
    }*/
    std::cout << std::endl;

    return 0;
}

