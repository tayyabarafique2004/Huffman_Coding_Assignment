#include <iostream>
#include <unordered_map>
#include <string>
#include <limits>
using namespace std;

/*
 * Explanation
 * 1. when string is given to program it calculates frequency of each character
 * 2.priority queue is used to construct huffman tree
 * 3. unique characters node are created and inserted into the pqueue
 * 4. then it removes two nodes with lowest frequencies , merge  into one and then reinsert
 * 5. we encode by replacing them with their huffman codes 
 * 6. decode by traversing and converting bits back into characters 
 * 7. then we compare orignal and encoded size of string
 */

// Node structure for Huffman Tree
struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;
    Node* next;

    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr), next(nullptr) {}
};

// Linked list priority queue
class PriorityQueue {
private:
    Node* head;

public:
    PriorityQueue() : head(nullptr) {}

    // Insert node into priority queue
    void insert(Node* newNode) {
        if (!head || newNode->freq < head->freq) {
            newNode->next = head;
            head = newNode;
        }
        else {
            Node* current = head;
            while (current->next && current->next->freq <= newNode->freq) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
    }

    // Remove and return the node with the smallest frequency
    Node* remove() {
        if (!head) return nullptr;
        Node* temp = head;
        head = head->next;
        temp->next = nullptr;
        return temp;
    }

    // Check if the queue is empty
    bool isEmpty() {
        return head == nullptr;
    }

    Node* getHead() {
        return head;
    }
};

// Generate Huffman codes recursively
void generateCodes(Node* root, string code, unordered_map<char, string>& huffmanCodes) {
    if (!root) return;

    // Leaf node condition
    if (!root->left && !root->right) {
        huffmanCodes[root->ch] = code;
    }

    generateCodes(root->left, code + "0", huffmanCodes);
    generateCodes(root->right, code + "1", huffmanCodes);
}

// Encode the input string
string encode(const string& text, unordered_map<char, string>& huffmanCodes) {
    string encodedString;
    for (char ch : text) {
        encodedString += huffmanCodes[ch];
    }
    return encodedString;
}

// Decode the encoded string
string decode(const string& encodedString, Node* root) {
    string decodedString;
    Node* current = root;

    for (char bit : encodedString) {
        if (!current) break;

        current = (bit == '0') ? current->left : current->right;

        if (current && !current->left && !current->right) {
            decodedString += current->ch;
            current = root; // Reset to root for the next character
        }
    }

    return decodedString;
}
void clrscr() {
#ifdef _WIN32
    system("cls"); 
#else
    system("clear"); 
#endif
}
int main() {
    while (true) {
       
        cout << "\nEnter a string to encode (or type 'exit' to quit): ";
        string text;
        getline(cin, text);

        // Check if user wants to exit
        if (text == "exit") {
            break;
        }

        // Step 1: Calculate frequency of each character
        unordered_map<char, int> freqMap;
        for (char ch : text) {
            freqMap[ch]++;
        }

        cout << "\nFrequency Table:\n";
        for (const auto& pair : freqMap) {
            cout << pair.first << ": " << pair.second << endl;
        }

        // Step 2: Build Huffman Tree using priority queue (linked list)
        PriorityQueue pq;
        for (const auto& pair : freqMap) {
            pq.insert(new Node(pair.first, pair.second));
        }

        while (pq.getHead() && pq.getHead()->next) {
            Node* left = pq.remove();
            Node* right = pq.remove();

            Node* merged = new Node('\0', left->freq + right->freq);
            merged->left = left;
            merged->right = right;

            pq.insert(merged);
        }

        Node* root = pq.remove();

        // Step 3: Generate Huffman Codes
        unordered_map<char, string> huffmanCodes;
        generateCodes(root, "", huffmanCodes);

        cout << "\nCharacter | Frequency | Huffman Code\n";
        cout << "------------------------------------\n";
        for (const auto& pair : huffmanCodes) {
            cout << "    " << pair.first << "       |     " << freqMap[pair.first] << "      | " << pair.second << endl;
        }

        // Step 4: Encode the input string
        string encodedString = encode(text, huffmanCodes);
        cout << "\nOriginal String: " << text << endl;
        cout << "Encoded String: " << encodedString << endl;

        // Step 5: Decode the encoded string
        string decodedString = decode(encodedString, root);
        cout << "Decoded String: " << decodedString << endl;

        // Step 6: Compression analysis
        int originalSize = text.size() * 8; // Each character is 8 bits
        int encodedSize = encodedString.size();
        cout << "\nOriginal Size: " << originalSize << " bits" << endl;
        cout << "Encoded Size: " << encodedSize << " bits" << endl;
        cout << "Compression Ratio: " << ((double)encodedSize / originalSize) * 100 << "%\n";
    }

    return 0;
}
