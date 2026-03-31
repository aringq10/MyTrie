#ifndef TRIE_H
#define TRIE_H

#include <stdexcept>
#include <string>
#include <memory>

namespace MyTrie {

    /**
     * @brief Custom exception for Trie-specific logic errors.
     */
    class TrieException : public std::runtime_error {
        public:
            explicit TrieException(const std::string& message) 
                : std::runtime_error("Trie Error: " + message) {}
    };

    /**
     * @brief A Prefix Tree (Trie) for string storage and prefix lookup.
     *
     * Implements the Pimpl idiom. Supports basic string operations, deep
     * copying and lexicographical comparison between different Trie instances.
     */
    class Trie {
        public:
            Trie();
            ~Trie();

            /** @brief Performs a deep copy of the entire tree structure. */
            Trie(const Trie& other);
            Trie& operator=(const Trie& other);

            /** @brief Returns the total count of unique words stored in the Trie. */
            int size() const noexcept;

            /** @brief Returns string of all words in Trie separated by newlines.*/
            std::string toString() const;

            /** @brief Writes each word in Trie to os separated by newlines. */
            friend std::ostream& operator<<(std::ostream& os, const Trie& trie);

            /**
             * @brief Inserts word into the Trie.
             * * @param word The string to be stored.
             * * @throw std::invalid_argument If word is empty.
             */
            void insert(const std::string& word);

            /**
             * @brief Removes word from the Trie.
             * * @param word The string to be removed.
             * * @throw std::invalid_argument If word is empty.
             * * @throw MyTrie::TrieException If Trie does not contain word.
             */
            void remove(const std::string& word);

            /**
             * @brief Returns true if Trie contains word.
             * * @param word The string to be searched.
             */
            bool contains(const std::string& word) const noexcept;

            /** @brief Deletes all nodes and resets the Trie to an empty state. */
            void clear();

            /** @brief Shorthand for insert() */
            Trie& operator+=(const std::string& word);

            /** @brief Shorthand for remove() */
            Trie& operator-=(const std::string& word);

            /** @brief Shorthand for contains() */
            bool operator[](const std::string& word) const noexcept;

            /** @brief Shorthand for clear() */
            void operator!();

            /** @brief Returns true if this Trie is lexicographically "equal" to the other. */
            bool operator==(const Trie& other) const noexcept;

            /** @brief Returns true if this Trie is not lexicographically "equal" to the other. */
            bool operator!=(const Trie& other) const noexcept;

            /** @brief Returns true if this Trie is lexicographically "smaller" than the other. */
            bool operator<(const Trie& other) const noexcept;

            /** @brief Returns true if this Trie is lexicographically "smaller" or "equal" to the other. */
            bool operator<=(const Trie& other) const noexcept;

            /** @brief Returns true if this Trie is lexicographically "bigger" than the other. */
            bool operator>(const Trie& other) const noexcept;

            /** @brief Returns true if this Trie is lexicographically "bigger" or "equal" to the other. */
            bool operator>=(const Trie& other) const noexcept;

        private:
            class Impl;
            std::unique_ptr<Impl> pImpl;
    };
}

#endif
