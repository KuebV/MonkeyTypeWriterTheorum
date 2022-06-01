#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <thread>
#include <stdio.h>
#include <chrono>
#include <string>
#include <sstream>
#include <vector>

// Look at me go, I'll actually comment my code, cause why not


// Define the chars allowed
char arr[29] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's','t', 'u', 'v', 'w', 'x', 'y', 'z', ',', '.', ' ' };
std::string globalString;
static bool stringFound = false;

// Really, really not recommended, unless you want to fill up a hard drive in a couple hours
static bool generateAllFiles = false;


std::string generateFileName(int index, int threadID, const std::string& ext) {
    std::ostringstream result;
    result << "document" << index << "thread" << threadID << ext;
    return result.str();
}

void monkeyType(int threadID) {

    srand(threadID);
    // Amount of documents iterated through
    int documents = 1;


    while (!stringFound) {

        // Start a stopwatch
        auto stopwatchStart = std::chrono::high_resolution_clock::now();
        std::string str = "";
        for (int i = 0; i < 32000; i++) {   
            int randNum = rand() % 29;
            char randChar = arr[randNum];
            str += randChar;
        }

        // Stop and calculate the stopwatch duration
        auto stopwatchStop = std::chrono::high_resolution_clock::now();
        auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(stopwatchStop - stopwatchStart);

        // Updates the status every 1000 documents completed
        if (documents % 1000 == 0)
            std::cout << "[Monkey: " << threadID << "] has completed " << documents << " documents in " << microseconds.count() << " ms\n\n";

        //If the string is found, then stop the process.
        if (str.find(globalString) != std::string::npos) {
            std::ofstream file(generateFileName(documents, threadID, ".txt").c_str());
            if (file.is_open()) {
                file << "Document: " << documents << " with ThreadID: " << threadID << " has found \"" << globalString << "\"\n";
                file << str << "\n";
                file.close();
            }
            stringFound = true;
            exit(0);
        }

        if (generateAllFiles) {
            std::ofstream file(generateFileName(documents, threadID, ".txt").c_str());
            if (file.is_open()) {
                file << "Document: " << documents << " with ThreadID: " << threadID << " has found \"" << globalString << "\"\n";
                file << str << "\n";
                file.close();
            }
        }

        documents++;
    }
}

int main()
{
    // User Input for what the program is looking for
    std::cout << "What are you looking for?\n";
    std::getline(std::cin, globalString);

    std::cout << "Program will look for \"" << globalString << "\"\n";
    std::vector<std::thread> vec;
    std::size_t threads = 2;
    for (std::size_t i = 0; i < threads; ++i) {
        vec.push_back(std::thread(&monkeyType, i + 1));

    }
    for (auto& th : vec) {
        th.join();
    }
    return 0;

}



