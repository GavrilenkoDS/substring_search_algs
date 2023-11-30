#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>

const int PRIME = 101;  // Простое число для вычисления хеша

// Вычисление хеша для строки
int calculateHash(const std::string& str, int length) {
    int hashValue = 0;
    for (int i = 0; i < length; ++i) {
        hashValue += str[i] * pow(PRIME, i);
    }
    return hashValue;
}

// Пересчет хеша при изменении символа
int recalculateHash(int oldHash, char oldChar, char newChar, int length) {
    int newHash = oldHash - oldChar;
    newHash /= PRIME;
    newHash += newChar * pow(PRIME, length - 1);
    return newHash;
}

// Поиск подстроки в тексте с использованием алгоритма Рабина-Карпа
std::vector<int> rabinKarpSearch(const std::string& text, const std::string& pattern) {
    int n = text.length();
    int m = pattern.length();
    int patternHash = calculateHash(pattern, m);

    std::vector<int> occurrences;

    #pragma omp parallel for
    for (int i = 0; i <= n - m; ++i) {
        int textHash = calculateHash(text.substr(i, m), m);

        if (textHash == patternHash) {
            bool match = true;
            for (int j = 0; j < m; ++j) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                #pragma omp critical
                occurrences.push_back(i);
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
    std::vector<int> result = rabinKarpSearch(text, pattern);
	auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "time: " << duration.count() << std::endl;
    /*std::cout << "Rabin-Karp\npattern: " << pattern << " text: " << text << ": ";
    for (int index : result) {
        std::cout << index << " ";
    }*/
    std::cout << std::endl;

    return 0;
}

