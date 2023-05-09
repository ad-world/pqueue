/*
Algorithm Analysis
- n is the number of elements in the priority queue
Implementation 1:
insert: 
time: O(n)
This is because if something with lowest priority is added, it will iterate to the end of the linked list (n items) before being added.
space:

deleteMax:
time: O(1)
does not depend on the size of the list, only removes the first node if it exists
space:

Implementation 2:
insert: O(1) 
time:
Amortized runtime since array most likely wont go through realloc frequently for large n
space:

deleteMax: O(1)
time:
space:

Implementation 3:
let i be highest priority
insert: 
time: O(i)
worst-case since runtime is O(i) when vector resizing and reallocation has to happen
space:

deleteMax: 
time: O(1)
space:



*/
#include <string>
#include <iostream>
#include <cstdlib>
#include <utility>
#include <vector>
#include <queue>

using namespace std;
int timestamp = 0;

// IMPLEMENTATION 1
class Node
{
public:
    int data;
    int priority;
    Node *next;

    Node(int data, int priority) : data{data}, priority{priority}, next{nullptr} {}
};

class LinkedList
{
    Node *head;
    int size;

public:
    LinkedList();
    void insert(int priority);
    Node *findMax();
    void printMax();
    void deleteMax();
    void printQueue();
    void clearQueue();
    void reset();
    void lookup(int index);
    ~LinkedList();
};

void LinkedList::insert(int priority)
{
    timestamp++;
    Node *node = new Node(timestamp, priority);
    Node *start = head;
    if (start == nullptr)
    {
        head = node;
        return;
    }
    else if (priority > start->priority)
    {
        node->next = head;
        head = node;
    }
    else
    {
        while (start->next != nullptr && start->next->priority >= priority)
        {
            start = start->next;
        }
        node->next = start->next;
        start->next = node;
    }
    size++;
}

Node *LinkedList::findMax()
{
    return head;
}

LinkedList::LinkedList()
{
    head = nullptr;
    size = 0;
}

void LinkedList::printMax()
{
    if (head)
    {
        cout << head->priority << " " << head->data << '\n';
    }
}

void LinkedList::deleteMax()
{

    if (size >= 0)
    {
        this->printMax();
        if (head->next == nullptr)
        {
            delete head;
            head = nullptr;
        }
        else
        {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
        size--;
    }
}

void LinkedList::printQueue()
{
    Node *start = head;
    while (start)
    {
        cout << start->priority << "(" << start->data << ")"
             << " ";
        start = start->next;
    }
    cout << endl;
}

void LinkedList::reset()
{
    while (head)
    {
        if (head->next == nullptr)
        {
            delete head;
            head = nullptr;
        }
        else
        {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
    }
    size = 0;
}

void LinkedList::lookup(int index)
{
    if (index == 0)
    {
        cout << head->data << endl;
        return;
    }
    int count = 0;
    Node *temp = head;

    while (count != index)
    {
        temp = temp->next;
        count++;
    }

    cout << temp->data << endl;
    return;
}

LinkedList::~LinkedList()
{
    while (head)
    {
        if (head->next == nullptr)
        {
            delete head;
            head = nullptr;
        }
        else
        {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
    }
    size = 0;
}

// IMPLEMENTATION 2

class HeapNode
{
    int priority;
    int timestamp;

public:
    HeapNode(int p, int t) : priority{p}, timestamp{t} {}
    int getPriority()
    {
        return priority;
    }
    int getTimestamp()
    {
        return timestamp;
    }
};

class MaxHeap
{
    HeapNode *arr;
    int size;
    int max_size;

public:
    MaxHeap();
    void insert(int priority);
    void deleteMax();
    int parent(int index);
    void fixUp(int index);
    void fixDown(int index);
    int left(int index);
    int right(int index);
    void printMax();
    void printQueue();
    void reset();
    void lookup(int index);
    ~MaxHeap()
    {
        free(arr);
    }
};

MaxHeap::MaxHeap()
{
    size = 0;
    max_size = 1;
}

void MaxHeap::printQueue()
{
    for (int i = 0; i < size; i++)
    {
        cout << arr[i].getPriority() << "(" << arr[i].getTimestamp() << ") ";
    }
    cout << endl;
}

void MaxHeap::insert(int priority)
{
    if (size == 0)
    {
        arr = (HeapNode *)malloc(max_size * sizeof(HeapNode));
    }

    timestamp++;
    HeapNode h = HeapNode(priority, timestamp);

    if (size == max_size)
    {
        max_size *= 2;
        arr = (HeapNode *)realloc(arr, max_size * sizeof(HeapNode));
    }

    arr[size] = h;
    size++;

    this->fixUp(size - 1);
}

int MaxHeap::parent(int index)
{
    return (index - 1) / 2;
}

void MaxHeap::fixUp(int index)
{
    while (parent(index) != -1 && (arr[parent(index)].getPriority() < arr[index].getPriority()))
    {
        HeapNode temp1 = arr[parent(index)];
        HeapNode temp2 = arr[index];
        arr[parent(index)] = temp2;
        arr[index] = temp1;

        index = parent(index);
    }
}

int MaxHeap::left(int index)
{
    return (index * 2) + 1;
}

int MaxHeap::right(int index)
{
    return (index * 2) + 2;
}

void MaxHeap::fixDown(int index)
{

    while (left(index) < size && right(index) < size)
    {
        int leftChildIndex = left(index);
        if (leftChildIndex != size && arr[leftChildIndex + 1].getPriority() > arr[leftChildIndex].getPriority())
        {
            leftChildIndex += 1;
        }
        if (arr[index].getPriority() >= arr[leftChildIndex].getPriority())
            break;

        HeapNode temp1 = arr[index];
        HeapNode temp2 = arr[leftChildIndex];
        arr[index] = temp2;
        arr[leftChildIndex] = temp1;

        // swap(arr[index], arr[leftChildIndex]);
        index = leftChildIndex;
    }
}

void MaxHeap::printMax()
{
    if (size > 0)
    {
        cout << arr[0].getPriority() << " " << arr[0].getTimestamp();
        cout << endl;
    }
}

void MaxHeap::deleteMax()
{
    if (size > 0)
    {
        this->printMax();
        HeapNode max = arr[0];
        arr[0] = arr[size - 1];
        size = size - 1;

        this->fixDown(0);
    }
}

void MaxHeap::reset()
{
    if (size > 0)
    {
        arr = (HeapNode *)realloc(arr, 0);
    }
    size = 0;
    max_size = 1;
}

void MaxHeap::lookup(int index)
{
    cout << arr[index].getTimestamp() << endl;
}

// IMPLEMENTATION 3

class VectorQueue
{
    vector<queue<int>> vec;
    int size;
    int highest_priority;

public:
    VectorQueue();
    void insert(int priority);
    int deleteMax();
    void printMax();
    void printQueue();
    void reset();
    void lookup(int index);
};

VectorQueue::VectorQueue()
{
    vec.resize(0);
    size = 0;
    highest_priority = 0;
}

void VectorQueue::insert(int priority)
{
    if (priority > highest_priority)
    {
        vec.resize(priority + 1);
        highest_priority = priority;
    }

    timestamp++;

    if (vec[priority].empty())
    {
        vec[priority] = queue<int>();
    }

    vec[priority].push(timestamp);
    size++;
}

int VectorQueue::deleteMax()
{
    if (size > 0)
    {
        int value = vec[highest_priority].front();
        this->printMax();
        vec[highest_priority].pop();
        if (vec[highest_priority].empty())
        {
            for (int i = vec.size() - 1; i > 0; i--)
            {
                if (!vec[i].empty())
                {
                    highest_priority = i;
                    vec.resize(i + 1);
                    break;
                }

                if (i == 0)
                {
                    vec.resize(1);
                    highest_priority = 0;
                    break;
                }
            }
        }
        size--;
        return value;
    }
    return 0;
}

void VectorQueue::printMax()
{
    if (size != 0)
    {
        cout << highest_priority << " " << vec[highest_priority].front() << endl;
    }
}

void VectorQueue::printQueue()
{
    for (int i = vec.size() - 1; i > 0; i--)
    {
        if (!vec[i].empty())
        {
            cout << i << " "
                 << "(" << vec[i].size() << ") ";
        }
    }
    cout << endl;
}

void VectorQueue::reset()
{
    vec.resize(0);
    size = 0;
    highest_priority = 0;
}

void VectorQueue::lookup(int index)
{
    if (index > highest_priority)
    {
        cout << 0 << endl;
    }
    else
    {
        cout << vec[index].size() << endl;
    }
}

// DRIVER

int main()
{
    LinkedList imp1;
    MaxHeap imp2;
    VectorQueue imp3;

    string s;
    int n;
    int priority;
    int i;
    while (cin >> s)
    {
        if (s == "i")
        {
            cin >> n;
            cin >> priority;
            if (n == 1)
                imp1.insert(priority);
            if (n == 2)
                imp2.insert(priority);
            if (n == 3)
                imp3.insert(priority);
        }
        else if (s == "d")
        {
            cin >> n;
            if (n == 1)
                imp1.deleteMax();
            if (n == 2)
                imp2.deleteMax();
            if (n == 3)
                imp3.deleteMax();
        }
        else if (s == "f")
        {
            cin >> n;
            if (n == 1)
                imp1.printMax();
            if (n == 2)
                imp2.printMax();
            if (n == 3)
                imp3.printMax();
        }
        else if (s == "l")
        {
            cin >> n;
            cin >> i;
            if (n == 1)
                imp1.lookup(i);
            if (n == 2)
                imp2.lookup(i);
            if (n == 3)
                imp3.lookup(i);
        }
        else if (s == "p")
        {
            cin >> n;
            if (n == 1)
                imp1.printQueue();
            if (n == 2)
                imp2.printQueue();
            if (n == 3)
                imp3.printQueue();
        }
        else if (s == "r")
        {
            imp1.reset();
            imp2.reset();
            imp3.reset();
            // do something;
            // don't care rn
        }
        else if (s == "x")
        {
            imp1.reset();
            imp2.reset();
            imp3.reset();
            break;
        }
        else
        {
            break;
        }
    }

    return 0;
}
