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

    delete rootNode;
    rootNode = nullptr;
}

node *buildTree(map<char, int> &table)
{
    priority_queue<node *, vector<node *>, compare> nodes;

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

void printMessage(string &encode, node *&root)
{
    size_t index = 0;
    string output = "";

    while (index < encode.length())
    {
        node *curr = root;

        while (curr->letter == '*')
        {
            if (encode[index] == '0')
                curr = curr->left;
            else
                curr = curr->right;

            index++;
        }

        output += curr->letter;
    }

    cout << output << endl;
}

int main()
{
    map<char, int> freqs; // Store the frequencies

    string chars;
    getline(cin, chars); //Get line of characters entered
    int num;

    for (size_t i = 0; i < chars.length(); i += 2) // Every second char is what we want to take
    {
        cin >> num;
        freqs[chars[i]] = num;
    }

    string encodeStr;
    cin >> encodeStr;

    node *rootNode = buildTree(freqs);
    printMessage(encodeStr, rootNode);
    return 0;
}