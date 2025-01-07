#include <iostream>
#include <cstring>
using namespace std;

const int ALPHABET_SIZE = 26; // uppercase as well
const int MAX_SUGGESTIONS = 10; // change this to get more suggestions

// Trie Node structure
class TreeNode {
public:
    TreeNode* children[ALPHABET_SIZE]; // Array of children (size 26 for English letters)
    bool isEndOfWord;

    TreeNode() {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            children[i] = nullptr;
        }
        isEndOfWord = false;
    }
};

// Trie class for inserting words and getting suggestions
class Tree {
private:
    TreeNode* root;

public:
    Tree() {
        root = new TreeNode();
    }

    // Insert a word into the Trie
    void insert(string word) {
        TreeNode* node = root;
        for (char c : word) {
            int index = c - 'a'; // Convert char to index (0-25)
            if (node->children[index] == nullptr) {
                node->children[index] = new TreeNode();
            }
            node = node->children[index];
        }
        node->isEndOfWord = true;
    }

    // Helper function to collect all words starting with a given prefix
    void getSuggestionsHelper(TreeNode* node, string prefix, string suggestions[], int& count) {
        if (node->isEndOfWord) {
            if (count < MAX_SUGGESTIONS) {
                suggestions[count++] = prefix;
            }
        }

        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (node->children[i] != nullptr) {
                getSuggestionsHelper(node->children[i], prefix + char('a' + i), suggestions, count);
            }
        }
    }

    // Get all suggestions starting with a given prefix
    int getSuggestions(string prefix, string suggestions[]) {
        TreeNode* node = root;
        int count = 0;

        // Traverse the Trie for the given prefix
        for (char c : prefix) {
            int index = c - 'a';
            if (node->children[index] == nullptr) {
                return 0; // No suggestions available for this prefix
            }
            node = node->children[index];
        }

        // Collect all words starting with the prefix
        getSuggestionsHelper(node, prefix, suggestions, count);
        return count;
    }
};
