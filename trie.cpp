#include <map>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include "trie.h"

namespace MyTrie {
    class Trie::Impl {
        public:
            struct Node {
                std::map<char, std::unique_ptr<Node>> children;
                bool isEndOfWord = false;

                std::unique_ptr<Node> clone() const {
                    auto newNode = std::make_unique<Node>();
                    newNode->isEndOfWord = this->isEndOfWord;
                    for (auto const& [ch, childPtr] : children) {
                        newNode->children[ch] = childPtr->clone();
                    }
                    return newNode;
                }

                bool isEqual(const Node* other) const {
                    if (!other || isEndOfWord != other->isEndOfWord || children.size() != other->children.size()) {
                        return false;
                    }
                    for (auto const& [ch, childPtr] : children) {
                        auto it = other->children.find(ch);
                        if (it == other->children.end() || !childPtr->isEqual(it->second.get())) {
                            return false;
                        }
                    }
                    return true;
                }
            };

            int size = 0;

            std::unique_ptr<Node> root;

            Impl() : root(std::make_unique<Node>()) {}

            bool removeHelper(Node* current, const std::string& word, size_t index) {
                if (index == word.length()) {
                    if (!current->isEndOfWord) return false;
                    current->isEndOfWord = false;
                    return current->children.empty();
                }

                char ch = word[index];
                if (current->children.find(ch) == current->children.end()) return false;

                bool shouldDeleteChild = removeHelper(current->children[ch].get(), word, index + 1);

                if (shouldDeleteChild) {
                    current->children.erase(ch);
                    return current->children.empty() && !current->isEndOfWord;
                }
                return false;
            }

            static int compare(const Node* a, const Node* b) {
                if (a == b) return 0;
                if (a == nullptr) return -1;
                if (b == nullptr) return 1;

                if (a->isEndOfWord != b->isEndOfWord) {
                    return a->isEndOfWord ? -1 : 1; 
                }

                auto itA = a->children.begin();
                auto itB = b->children.begin();

                while (itA != a->children.end() && itB != b->children.end()) {
                    if (itA->first < itB->first) return -1;
                    if (itA->first > itB->first) return 1;

                    int res = compare(itA->second.get(), itB->second.get());
                    if (res != 0) return res;

                    ++itA; ++itB;
                }

                if (a->children.size() < b->children.size()) return -1;
                if (a->children.size() > b->children.size()) return 1;

                return 0;
            }

            void printRecursive(Node* node, std::string prefix, std::ostream& os) {
                if (node->isEndOfWord) {
                    os << prefix << "\n";
                }
                for (auto const& [ch, child] : node->children) {
                    printRecursive(child.get(), prefix + ch, os);
                }
            }
    };

    Trie::Trie() : pImpl(std::make_unique<Impl>()) {}

    Trie::~Trie() = default;

    Trie::Trie(const Trie& other) : pImpl(std::make_unique<Impl>()) {
        if (other.pImpl->root) {
            pImpl->root = other.pImpl->root->clone();
            pImpl->size = other.pImpl->size;
        }
    }

    Trie& Trie::operator=(const Trie& other) {
        if (this != &other) {
            auto newRoot = other.pImpl->root->clone(); 
            pImpl->root = std::move(newRoot);
            pImpl->size = other.pImpl->size;
        }
        return *this;
    }

    int Trie::size() const noexcept {
        return pImpl->size;
    }

    std::string Trie::toString() const {
        std::stringstream ss;
        pImpl->printRecursive(pImpl->root.get(), "", ss);
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const Trie& trie) {
        trie.pImpl->printRecursive(trie.pImpl->root.get(), "", os);
        return os;
    }

    void Trie::insert(const std::string& word) {
        if (word.empty()) {
            throw std::invalid_argument("Trie does not support empty strings.");
        }

        if (contains(word)) {
            throw TrieException("Attempted to insert existing word '" + word + "'.");
        }

        Impl::Node* curr = pImpl->root.get();
        for (char ch : word) {
            if (curr->children.find(ch) == curr->children.end()) {
                curr->children[ch] = std::make_unique<Impl::Node>();
            }
            curr = curr->children[ch].get();
        }
        if (!curr->isEndOfWord) pImpl->size++;
        curr->isEndOfWord = true;
    }

    bool Trie::contains(const std::string& word) const noexcept {
        Impl::Node* curr = pImpl->root.get();
        for (char ch : word) {
            if (curr->children.find(ch) == curr->children.end()) return false;
            curr = curr->children[ch].get();
        }
        return curr->isEndOfWord;
    }

    void Trie::remove(const std::string& word) {
        if (word.empty()) {
            throw std::invalid_argument("Word cannot be empty.");
        }

        if (!contains(word)) {
            throw TrieException("Attempted to remove word '" + word + "' which does not exist.");
        }

        pImpl->removeHelper(pImpl->root.get(), word, 0);
        pImpl->size--;
    }

    void Trie::clear() {
        pImpl->root = std::make_unique<Impl::Node>();
        pImpl->size = 0;
    }

    Trie& Trie::operator+=(const std::string& word) {
        insert(word); return *this;
    }

    Trie& Trie::operator-=(const std::string& word) {
        remove(word); return *this;
    }

    bool Trie::operator[](const std::string& word) const noexcept {
        return contains(word);
    }

    void Trie::operator!() {
        clear();
    }

    bool Trie::operator==(const Trie& other) const noexcept {
        return Impl::compare(pImpl->root.get(), other.pImpl->root.get()) == 0;
    }

    bool Trie::operator!=(const Trie& other) const noexcept {
        return !(*this == other);
    }

    bool Trie::operator<(const Trie& other) const noexcept {
        return Impl::compare(pImpl->root.get(), other.pImpl->root.get()) < 0;
    }

    bool Trie::operator>(const Trie& other) const noexcept {
        return Impl::compare(pImpl->root.get(), other.pImpl->root.get()) > 0;
    }

    bool Trie::operator<=(const Trie& other) const noexcept {
        return Impl::compare(pImpl->root.get(), other.pImpl->root.get()) <= 0;
    }

    bool Trie::operator>=(const Trie& other) const noexcept {
        return Impl::compare(pImpl->root.get(), other.pImpl->root.get()) >= 0;
    }
}
