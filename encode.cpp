#include <iostream>
#include <map>
#include <vector>
#include <queue>

using namespace std;

class node // Our node class
{
public:
    node *left = nullptr;
    node *right = nullptr;

    char letter = '*';
    int freq = -1;

    node(){}; // Default Constructor

    node(char letter, int freq) // Create a node with it's values
    {
        this->letter = letter;
        this->freq = freq;
    }

    node(node *left, node *right, int freq) // Will be used by merge
    {
        this->left = left;
        this->right = right;
        this->freq = freq;
    }
};

struct compare
{
    /*
        This will be used by the priority queue
        to compare two nodes and see which is larger
    */
    bool operator()(node *&left, node *&right) const
    {
        if (left->freq > right->freq)
            return true;

        if (left->freq == right->freq)
            return left->letter > right->letter; // Comparing chars if they're equal frequency

        return false;
    }
};

void free(node *rootNode)
{
    if (rootNode == nullptr)
        return;

    if (rootNode->right == nullptr && rootNode->left == nullptr) // If it is a leaf
    {
        delete rootNode;
        return;
    }

    //Delete children first
    free(rootNode->left);
    free(rootNode->right);

    delete rootNode; // Delete this current node
    rootNode = nullptr;
}

node *buildTree(map<char, int> &table)
{
    priority_queue<node *, vector<node *>, compare> nodes; // Declaring our Priority Queue

    for (pair<char, int> value : table)
    {
        node *curr = new node(value.first, value.second);
        nodes.push(curr);
    } // Creates nodes from with values from the map and adds it to the pQueue

    while (nodes.size() > 1)
    {
        node *left = nodes.top();
        nodes.pop();
        node *right = nodes.top();
        nodes.pop();
        //Get the two nodes at the front of the queue

        node *mergeNode = new node(left, right, left->freq + right->freq);
        //Make the new frequency node
        nodes.push(mergeNode); //Add merged node to pQueue
    }

    return nodes.top();
}

void populateCodes(map<char, string> &codes, node *rootNode, string currpath = "")
{
    if (!rootNode) // Checking if rootNode is null
        return;

    if (rootNode->letter != '*') // If char isn't "*" then we know it's a letter node
        codes[rootNode->letter] = currpath;

    populateCodes(codes, rootNode->left, currpath + "0");
    populateCodes(codes, rootNode->right, currpath + "1");
}

void Display(string &line, map<char, string> &codes, map<char, int> &freq)
{
    size_t origCount = line.length() * 8;
    string binString; // The encoded string

    for (char &letter : line)
        binString += codes[letter];

    for (pair<char, int> val : freq)
        cout << val.first << " ";

    cout << endl;

    for (pair<char, int> val : freq)
        cout << val.second << " ";

    cout << endl;

    cout << binString << endl;
    cout << "Total Bits (Original):" << origCount << endl;
    cout << "Total Bits (Coded):" << binString.length() << endl;
}

int main()
{
    string line;
    getline(cin, line); // Get the string to encode

    map<char, int> freqTable;

    for (char letter : line)
        freqTable[letter]++;
    /*
        This for loop builds the table. With a map, when you have no entry
        for a certain character, it'll make an entry and assign a value of
        0 to it. If you already do have a value, it'll simply increment it by 1.
    */

    node *rootNode = buildTree(freqTable);

    map<char, string> codes;         // This will store the codes for each character
    populateCodes(codes, rootNode);  // Add each char and their corressponding code to the codes map
    free(rootNode);                  // Free's memory taken by tree nodes
    Display(line, codes, freqTable); // Display the output
    return 0;
}