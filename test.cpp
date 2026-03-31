#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "trie.h"

using namespace MyTrie;

void log(std::ostream& f, const std::string& msg) {
    std::cout << msg << std::endl;
    f << msg << std::endl;
}

void run_test(std::ostream& f, const std::string& testName, bool condition) {
    log(f, "TEST: " + testName + " -> " + (condition ? "PASSED" : "FAILED"));
}

int main() {
    std::ofstream logFile("log.txt");
    if (!logFile.is_open()) return 1;

    log(logFile, "==========================================");
    log(logFile, "TRIE PROJECT DIAGNOSTIC TEST SUITE");
    log(logFile, "==========================================");

    Trie t;

    // Test 1: Insertion and Size
    t += "test";
    t += "testing";
    run_test(logFile, "Size after 2 insertions", t.size() == 2);

    // Test 2: Duplicate Prevention (Logic Check)
    try {
        t += "test";
        run_test(logFile, "Duplicate insertion protection", false);
    } catch (const TrieException& e) {
        log(logFile, "Caught expected TrieException: " + std::string(e.what()));
        run_test(logFile, "Duplicate insertion protection", true);
    }

    // Test 3: Invalid Arguments
    try {
        t += "";
        run_test(logFile, "Empty string protection", false);
    } catch (const std::invalid_argument& e) {
        log(logFile, "Caught expected invalid_argument: " + std::string(e.what()));
        run_test(logFile, "Empty string protection", true);
    }

    // Test 4: Removal Logic
    t -= "test";
    run_test(logFile, "Size after removal", t.size() == 1);
    run_test(logFile, "Contains removed word", t["test"] == false);

    // Test 5: Deep Copy Independence
    Trie* original = new Trie();
    *original += "independent";
    Trie copy = *original;
    delete original;
    run_test(logFile, "Deep copy persistence after original deletion", copy["independent"]);

    // Test 6: Lexicographical Sorting
    Trie a, b;
    a += "apple";
    b += "banana";
    run_test(logFile, "Lexicographical comparison (apple < banana)", a < b);

    log(logFile, "--- Stress Testing Internal Node Integrity ---");
    std::vector<std::string> words = {"alpha", "beta", "gamma", "delta", "epsilon", "zeta", "eta", "theta"};
    for (const auto& w : words) {
        t += w;
        log(logFile, "Logged insertion of node: " + w + " [OK]");
    }
    
    run_test(logFile, "Final state check", t.size() == 9);
    log(logFile, "Final Trie Structure Contents:");
    log(logFile, t.toString());
    
    log(logFile, "==========================================");
    log(logFile, "ALL TESTS COMPLETED");
    log(logFile, "==========================================");

    logFile.close();
    return 0;
}
