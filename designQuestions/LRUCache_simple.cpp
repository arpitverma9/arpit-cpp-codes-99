#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <utility>
#include <string>

using namespace std;

class File
{
public:
    string data = "empty file";
};

class Node
{

public:
    string key;
    // File *value;

    Node *next;
    Node *prev;

    Node() = default;

    Node(string k)
        : key(k)
    {
    }
};

class LRUCache
{
    Node *head; //-->>DLL
    unordered_map<string, pair<File, Node *>> map;
    int capacity = 0;
    int occupied = 0;
    Node *tail;

private:
    void removeNode(Node *node)
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void addToFront(Node *node)
    {
        Node *temp = head->next;

        node->next = temp;
        temp->prev = node;

        head->next = node;
        node->prev = head;
    }

    void moveToFront(Node *node)
    {
        removeNode(node);
        addToFront(node);
    }

public:
    LRUCache(int capacity_) : capacity(capacity_)
    {
        head = new Node();
        tail = new Node();

        head->next = tail;
        tail->prev = head;
    }

    void put(string key_, File value_)
    {
        // return 1 means succcessfully put , 0 means some issue
        if (map.find(key_) == map.end()) // IF KEY NOT IN MAP ALREADY
        {
            Node *node = new Node(key_);

            addToFront(node);

            map[key_] = std::make_pair(value_, node);

            // Capacity exceeded
            if (map.size() > capacity)
            {
                Node *lru = tail->prev;

                removeNode(lru);

                map.erase(lru->key);

                delete lru;
            }
        }
        else
        {
            // if the key exists in map;
            Node *curr = map[key_].second;

            map[key_].first = value_;

            moveToFront(curr);

            return;
        }
    }

    File get(string key)
    {
        auto it = map.find(key);

        if (it == map.end())
        {
            cout << "Invalid key\n";
            return File();
        }

        Node *node = it->second.second;

        // get() also counts as recent usage
        moveToFront(node);

        return it->second.first;
    }

    ~LRUCache()
    {
        Node *curr = head;

        while (curr)
        {
            Node *next = curr->next;
            delete curr;
            curr = next;
        }
    }
};

int main()
{
    LRUCache cache(3);

    File f1, f2, f3, f4;

    f1.data = "ABC data";
    f2.data = "PQR data";
    f3.data = "XYZ data";
    f4.data = "NEW data";

    cache.put("abc", f1);
    cache.put("pqr", f2);
    cache.put("xyz", f3);

    cout << cache.get("abc").data << endl; // abc becomes MRU

    cache.put("new", f4); // should evict pqr

    cout << cache.get("abc").data << endl; // found
    cout << cache.get("xyz").data << endl; // found
    cout << cache.get("pqr").data << endl; // should print invalid key

    return 0;
}