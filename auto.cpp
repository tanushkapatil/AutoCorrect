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

// Function to calculate the Levenshtein Distance (edit distance) between two words
int levenshteinDistance(const string& str1, const string& str2) {
    int len1 = str1.size();
    int len2 = str2.size();

    int dp[len1 + 1][len2 + 1];  // 2D array for storing distances

    // Initialize the distance array
    for (int i = 0; i <= len1; i++) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= len2; j++) {
        dp[0][j] = j;
    }

    // Calculate the minimum distance
    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
            dp[i][j] = min(min(dp[i - 1][j] + 1, dp[i][j - 1] + 1), dp[i - 1][j - 1] + cost);
        }
    }

    return dp[len1][len2];
}

int main() {
    Tree tree;

    // Sample words to insert into the Tree
    tree.insert("apple");
    tree.insert("apply");
    tree.insert("app");
    tree.insert("banana");
    tree.insert("band");
    tree.insert("bandit");

    string prefix;
    cout << "Enter a word to check or autocomplete: ";
    cin >> prefix;

    // Array to store suggestions
    string suggestions[MAX_SUGGESTIONS];
    int suggestionCount = tree.getSuggestions(prefix, suggestions);

    if (suggestionCount == 0) {
        cout << "No suggestions found for prefix: " << prefix << endl;
    } else {
        cout << "Suggestions: ";
        for (int i = 0; i < suggestionCount; i++) {
            cout << suggestions[i] << " ";
        }
        cout << endl;
    }

    // Optionally: Suggest corrections if the word is misspelled
    string corrections[MAX_SUGGESTIONS];
    int correctionCount = 0;

    // Use Levenshtein distance to find possible corrections
    for (int i = 0; i < suggestionCount; i++) {
        if (levenshteinDistance(prefix, suggestions[i]) <= 2) {
            corrections[correctionCount++] = suggestions[i];
        }
    }

    if (correctionCount > 0) {
        cout << "Did you mean: ";
        for (int i = 0; i < correctionCount; i++) {
            cout << corrections[i] << " ";
        }
        cout << endl;
    }

    return 0;
}
