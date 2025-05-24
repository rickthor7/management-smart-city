#include <iostream>
#include <string>

using namespace std;

// Constants for Trie (simple lowercase alphabet)
const int ALPHABET_SIZE = 26;
const int MAX_SUGGESTIONS = 5; // Max suggestions for Trie autocomplete

// Forward declarations (karena ada dependensi silang atau untuk kerapian)
class ManualLinkedList;
class ManualQueue;
class ManualStack;
class NaryTreeNode;
class NaryTree;
class TrieNode;
class Trie;
class GraphVertexNode;
class GraphEdgeNode;
class Graph;
class CitizenManagement;
class TrafficManagement;
class UrbanStructure;
class NetworkService;
class SmartCitySimulator;

// --- Utility Functions (jika diperlukan, misal untuk string to int sederhana) ---
// Karena tidak boleh ada library lain, konversi string ke int manual jika diperlukan,
// tapi untuk contoh ini, kita akan coba hindari kebutuhan konversi kompleks.
// Untuk Dijkstra, kita akan menggunakan ID integer secara langsung.

// --- Helper structure for Dijkstra's Priority Queue (implemented as sorted linked list) ---
struct DijkstraNode {
    int vertexID;
    int distance;
    DijkstraNode* next;

    DijkstraNode(int id, int dist) : vertexID(id), distance(dist), next(NULL) {}
};

// --- CORE IMPLEMENTATION: Manual Doubly Circular Linked List with Sentinel ---
struct NodeLL {
    // Generic data placeholder - for simplicity, we'll define specific data in entities
    int id;         // Example: Citizen ID, Vehicle ID
    string name;    // Example: Citizen Name, Street Name
    string data1;   // Additional data field 1
    string data2;   // Additional data field 2

    NodeLL* next;
    NodeLL* prev;

    NodeLL(int i = 0, string n = "", string d1 = "", string d2 = "") : id(i), name(n), data1(d1), data2(d2), next(NULL), prev(NULL) {}
};

class ManualLinkedList {
public:
    NodeLL* sentinel;
    int count;

    ManualLinkedList() {
        sentinel = new NodeLL(-1, "SENTINEL"); // Sentinel node
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        count = 0;
    }

    ~ManualLinkedList() {
        NodeLL* current = sentinel->next;
        while (current != sentinel) {
            NodeLL* temp = current;
            current = current->next;
            delete temp;
        }
        delete sentinel;
    }

    bool isEmpty() const {
        return count == 0; // Or sentinel->next == sentinel;
    }

    int getSize() const {
        return count;
    }

    void insertAtTail(int id, string name, string data1 = "", string data2 = "") {
        NodeLL* newNode = new NodeLL(id, name, data1, data2);
        newNode->next = sentinel;
        newNode->prev = sentinel->prev;
        sentinel->prev->next = newNode;
        sentinel->prev = newNode;
        count++;
    }

    void insertAtHead(int id, string name, string data1 = "", string data2 = "") {
        NodeLL* newNode = new NodeLL(id, name, data1, data2);
        newNode->next = sentinel->next;
        newNode->prev = sentinel;
        sentinel->next->prev = newNode;
        sentinel->next = newNode;
        count++;
    }

    NodeLL* findNode(int id) {
        NodeLL* current = sentinel->next;
        while (current != sentinel) {
            if (current->id == id) {
                return current;
            }
            current = current->next;
        }
        return NULL; // Not found
    }

    bool deleteNode(int id) {
        NodeLL* nodeToDelete = findNode(id);
        if (nodeToDelete) {
            nodeToDelete->prev->next = nodeToDelete->next;
            nodeToDelete->next->prev = nodeToDelete->prev;
            delete nodeToDelete;
            count--;
            return true;
        }
        return false; // Not found
    }

    void displayList() const {
        if (isEmpty()) {
            cout << "List is empty." << endl;
            return;
        }
        NodeLL* current = sentinel->next;
        while (current != sentinel) {
            cout << "ID: " << current->id << ", Name: " << current->name;
            if (!current->data1.empty()) cout << ", Data1: " << current->data1;
            if (!current->data2.empty()) cout << ", Data2: " << current->data2;
            cout << endl;
            current = current->next;
        }
    }

    // AMBITIOUS FEATURE: Manual Insertion Sort (for linked list, useful for PQ in Dijkstra)
    // This specific sort is for DijkstraNode, not generic NodeLL.
    // A more generic sort would require function pointers or templates (not using templates here for simplicity)
    void sortedInsertDijkstra(DijkstraNode** headRef, DijkstraNode* newNode) {
        DijkstraNode* current;
        if (*headRef == NULL || (*headRef)->distance >= newNode->distance) {
            newNode->next = *headRef;
            *headRef = newNode;
        } else {
            current = *headRef;
            while (current->next != NULL && current->next->distance < newNode->distance) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
    }
};

// --- CORE IMPLEMENTATION: Manual Queue (using ManualLinkedList's principles) ---
// For simplicity, Queue will use a singly linked list structure internally,
// rather than directly reusing ManualLinkedList to avoid type conflicts for queue items.
struct QueueNode {
    // For traffic simulation, a vehicle might have a license plate and type
    string licensePlate;
    string vehicleType; // e.g., CAR, BUS, EMERGENCY
    int priority; // For priority queue, 0 is highest

    QueueNode* next;

    QueueNode(string lp, string vt, int p = 1) : licensePlate(lp), vehicleType(vt), priority(p), next(NULL) {}
};

class ManualQueue {
public:
    QueueNode* front;
    QueueNode* rear;
    int count;

    ManualQueue() : front(NULL), rear(NULL), count(0) {}

    ~ManualQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    bool isEmpty() const {
        return front == NULL;
    }

    int getSize() const {
        return count;
    }

    // Basic Enqueue
    void enqueue(string lp, string vt) {
        QueueNode* newNode = new QueueNode(lp, vt);
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        count++;
    }

    // AMBITIOUS FEATURE: Priority Enqueue (simple version, inserts based on priority)
    // Lower number means higher priority. Assumes queue is kept sorted by priority.
    // This makes it a simple priority queue. For real PQ, heap is better.
    void priorityEnqueue(string lp, string vt, int prio) {
        QueueNode* newNode = new QueueNode(lp, vt, prio);
        count++;

        if (isEmpty() || prio < front->priority) {
            newNode->next = front;
            front = newNode;
            if (rear == NULL) rear = front; // if was empty
            return;
        }

        QueueNode* current = front;
        while (current->next != NULL && current->next->priority <= prio) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
        if(newNode->next == NULL) rear = newNode; // if inserted at end
    }


    QueueNode* dequeue() {
        if (isEmpty()) {
            return NULL;
        }
        QueueNode* temp = front;
        front = front->next;
        if (front == NULL) { // Queue becomes empty
            rear = NULL;
        }
        temp->next = NULL; // Isolate the node
        count--;
        return temp; // Caller should delete this node
    }

    QueueNode* peek() const {
        return front;
    }

    void displayQueue() const {
        if (isEmpty()) {
            cout << "Queue is empty." << endl;
            return;
        }
        QueueNode* current = front;
        cout << "Vehicles in Queue (Front to Rear):" << endl;
        while (current != NULL) {
            cout << "  Plate: " << current->licensePlate << ", Type: " << current->vehicleType << ", Prio: " << current->priority << endl;
            current = current->next;
        }
    }
};

// --- CORE IMPLEMENTATION: Manual Stack (using ManualLinkedList's principles) ---
// Also using a simple singly linked list for stack items.
struct StackNode {
    string data; // Example: Parked Car License Plate, Undo Command
    StackNode* next;

    StackNode(string d) : data(d), next(NULL) {}
};

class ManualStack {
public:
    StackNode* top;
    int count;

    ManualStack() : top(NULL), count(0) {}

    ~ManualStack() {
        while (!isEmpty()) {
            pop();
        }
    }

    bool isEmpty() const {
        return top == NULL;
    }

    int getSize() const {
        return count;
    }

    void push(string data) {
        StackNode* newNode = new StackNode(data);
        newNode->next = top;
        top = newNode;
        count++;
    }

    string pop() {
        if (isEmpty()) {
            return "ERROR_EMPTY_STACK";
        }
        StackNode* temp = top;
        string poppedData = temp->data;
        top = top->next;
        delete temp;
        count--;
        return poppedData;
    }

    string peek() const {
        if (isEmpty()) {
            return "ERROR_EMPTY_STACK";
        }
        return top->data;
    }

    void displayStack() const {
        if (isEmpty()) {
            cout << "Stack is empty." << endl;
            return;
        }
        StackNode* current = top;
        cout << "Items in Stack (Top to Bottom):" << endl;
        while (current != NULL) {
            cout << "  " << current->data << endl;
            current = current->next;
        }
    }
};

// --- CORE IMPLEMENTATION: N-Ary Tree (Left-Child, Right-Sibling) ---
struct NaryTreeNode {
    string name; // Name of the department, area, etc.
    NaryTreeNode* firstChild;
    NaryTreeNode* nextSibling;

    NaryTreeNode(string n) : name(n), firstChild(NULL), nextSibling(NULL) {}

    ~NaryTreeNode() { // Recursive destructor for children and siblings
        delete firstChild; // This will trigger child's destructor chain
        delete nextSibling; // This will trigger sibling's destructor chain
    }
};

class NaryTree {
public:
    NaryTreeNode* root;

    NaryTree(string rootName) {
        root = new NaryTreeNode(rootName);
    }

    ~NaryTree() {
        delete root; // This will start the recursive deletion
    }

    // Helper to find a node (could be improved, e.g. BFS/DFS search)
    NaryTreeNode* findNodeRecursive(NaryTreeNode* current, const string& name) {
        if (!current) return NULL;
        if (current->name == name) return current;

        NaryTreeNode* foundNode = findNodeRecursive(current->firstChild, name);
        if (foundNode) return foundNode;

        return findNodeRecursive(current->nextSibling, name);
    }
    
    NaryTreeNode* findNode(const string& name) {
        if (root->name == name) return root;
        return findNodeRecursive(root->firstChild, name); // Search children first
    }


    void addChild(const string& parentName, const string& childName) {
        NaryTreeNode* parentNode = findNode(parentName);
        if (!parentNode) {
            cout << "Error: Parent node '" << parentName << "' not found for N-Ary Tree." << endl;
            return;
        }

        NaryTreeNode* newNode = new NaryTreeNode(childName);
        if (!parentNode->firstChild) { // No children yet
            parentNode->firstChild = newNode;
        } else {
            NaryTreeNode* sibling = parentNode->firstChild;
            while (sibling->nextSibling) {
                sibling = sibling->nextSibling;
            }
            sibling->nextSibling = newNode;
        }
    }

    void displayTreeRecursive(NaryTreeNode* node, int depth) {
        if (!node) return;
        for (int i = 0; i < depth; ++i) cout << "  "; // Indentation
        cout << "- " << node->name << endl;

        displayTreeRecursive(node->firstChild, depth + 1);
        displayTreeRecursive(node->nextSibling, depth); // Siblings are at the same depth relative to parent call
    }

    void displayTree() {
        cout << "N-Ary Tree Structure:" << endl;
        if(root) {
             cout << root->name << endl;
             displayTreeRecursive(root->firstChild, 1);
        } else {
            cout << "Tree is empty." << endl;
        }
    }
};

// --- AMBITIOUS FEATURE: Trie (Prefix Tree) ---
struct TrieNode {
    TrieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;
    string wordStored; // Store the full word for easy suggestion retrieval

    TrieNode() {
        isEndOfWord = false;
        wordStored = "";
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            children[i] = NULL;
        }
    }

    // Destructor for TrieNode to delete children
    ~TrieNode() {
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            delete children[i];
        }
    }
};

class Trie {
public:
    TrieNode* root;

    Trie() {
        root = new TrieNode();
    }
    ~Trie() {
        delete root;
    }

    void insert(string word) {
        TrieNode* current = root;
        for (int i = 0; i < word.length(); ++i) {
            // Basic lowercase conversion and check
            char ch = word[i];
            if (ch >= 'A' && ch <= 'Z') ch = ch - 'A' + 'a';
            if (ch < 'a' || ch > 'z') continue; // Skip non-alpha

            int index = ch - 'a';
            if (!current->children[index]) {
                current->children[index] = new TrieNode();
            }
            current = current->children[index];
        }
        current->isEndOfWord = true;
        current->wordStored = word; // Store the original (or processed) word
    }

    bool search(string word) {
        TrieNode* current = root;
        for (int i = 0; i < word.length(); ++i) {
            char ch = word[i];
            if (ch >= 'A' && ch <= 'Z') ch = ch - 'A' + 'a';
            if (ch < 'a' || ch > 'z') return false;

            int index = ch - 'a';
            if (!current->children[index]) {
                return false;
            }
            current = current->children[index];
        }
        return (current != NULL && current->isEndOfWord);
    }

    // Helper for suggestions
    void getSuggestionsRecursive(TrieNode* node, string currentPrefix, ManualLinkedList* suggestionsList, int& count) {
        if (!node || count >= MAX_SUGGESTIONS) {
            return;
        }

        if (node->isEndOfWord) {
            suggestionsList->insertAtTail(count + 1, node->wordStored); // Use ID for suggestion number
            count++;
        }

        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            if (node->children[i]) {
                // char nextChar = 'a' + i; // Not strictly needed for this list structure
                getSuggestionsRecursive(node->children[i], currentPrefix /* + nextChar */, suggestionsList, count);
            }
        }
    }

    ManualLinkedList* getSuggestions(string prefix) {
        TrieNode* current = root;
        for (int i = 0; i < prefix.length(); ++i) {
            char ch = prefix[i];
            if (ch >= 'A' && ch <= 'Z') ch = ch - 'A' + 'a';
            if (ch < 'a' || ch > 'z') return new ManualLinkedList(); // Empty list for invalid prefix

            int index = ch - 'a';
            if (!current->children[index]) {
                return new ManualLinkedList(); // Prefix not found
            }
            current = current->children[index];
        }

        ManualLinkedList* suggestionsList = new ManualLinkedList();
        int suggestionCount = 0;
        getSuggestionsRecursive(current, prefix, suggestionsList, suggestionCount);
        return suggestionsList;
    }
};


// --- CORE IMPLEMENTATION: Graph (Adjacency List using ManualLinkedList) ---
// Edge node for adjacency list
struct GraphEdgeNode {
    int toVertexID;
    int weight;
    GraphEdgeNode* next;

    GraphEdgeNode(int to, int w) : toVertexID(to), weight(w), next(NULL) {}
};

// Vertex node for graph
struct GraphVertexNode {
    int id;
    string name; // e.g., Intersection Name, Location Name
    GraphEdgeNode* adjListHead; // Head of the adjacency list (linked list of edges)
    GraphVertexNode* nextVertex; // To link vertices in the graph's main list of vertices

    // For Dijkstra
    int distFromSource;
    bool visited;


    GraphVertexNode(int i, string n) : id(i), name(n), adjListHead(NULL), nextVertex(NULL), distFromSource(2147483647), visited(false) {} // 2147483647 ~ infinity

    ~GraphVertexNode() {
        GraphEdgeNode* currentEdge = adjListHead;
        while (currentEdge) {
            GraphEdgeNode* tempEdge = currentEdge;
            currentEdge = currentEdge->next;
            delete tempEdge;
        }
    }
};

class Graph {
public:
    GraphVertexNode* vertexListHead; // Head of the linked list of all vertices in the graph
    int numVertices;

    Graph() : vertexListHead(NULL), numVertices(0) {}

    ~Graph() {
        GraphVertexNode* currentVertex = vertexListHead;
        while (currentVertex) {
            GraphVertexNode* tempVertex = currentVertex;
            currentVertex = currentVertex->nextVertex;
            delete tempVertex; // This will call ~GraphVertexNode() which deletes its adj list
        }
    }

    GraphVertexNode* findVertex(int id) {
        GraphVertexNode* current = vertexListHead;
        while (current) {
            if (current->id == id) {
                return current;
            }
            current = current->nextVertex;
        }
        return NULL;
    }

    void addVertex(int id, string name) {
        if (findVertex(id)) {
            cout << "Error: Vertex with ID " << id << " already exists." << endl;
            return;
        }
        GraphVertexNode* newNode = new GraphVertexNode(id, name);
        newNode->nextVertex = vertexListHead; // Add to head of vertex list
        vertexListHead = newNode;
        numVertices++;
    }

    void addEdge(int fromID, int toID, int weight) {
        GraphVertexNode* fromVertex = findVertex(fromID);
        GraphVertexNode* toVertex = findVertex(toID);

        if (!fromVertex || !toVertex) {
            cout << "Error: One or both vertices not found for edge (" << fromID << " -> " << toID << ")." << endl;
            return;
        }

        // Add edge from fromID to toID
        GraphEdgeNode* newEdge = new GraphEdgeNode(toID, weight);
        newEdge->next = fromVertex->adjListHead;
        fromVertex->adjListHead = newEdge;

        // For an undirected graph, add the reverse edge too
        // GraphEdgeNode* reverseEdge = new GraphEdgeNode(fromID, weight);
        // reverseEdge->next = toVertex->adjListHead;
        // toVertex->adjListHead = reverseEdge;
    }

    void displayGraph() {
        cout << "Graph Adjacency List:" << endl;
        GraphVertexNode* currentVertex = vertexListHead;
        if (!currentVertex) {
            cout << "Graph is empty." << endl;
            return;
        }
        while (currentVertex) {
            cout << "Vertex " << currentVertex->id << " (" << currentVertex->name << "): ";
            GraphEdgeNode* currentEdge = currentVertex->adjListHead;
            if (!currentEdge) {
                cout << "-> No outgoing edges";
            }
            while (currentEdge) {
                cout << "-> " << currentEdge->toVertexID << "(w:" << currentEdge->weight << ") ";
                currentEdge = currentEdge->next;
            }
            cout << endl;
            currentVertex = currentVertex->nextVertex;
        }
    }

    // --- AMBITIOUS FEATURE: Breadth-First Search (BFS) ---
    void BFS(int startVertexID) {
        GraphVertexNode* startNode = findVertex(startVertexID);
        if (!startNode) {
            cout << "BFS Error: Start vertex " << startVertexID << " not found." << endl;
            return;
        }

        // Manual visited tracking (could be bool array if IDs are 0 to N-1, or use map/set if allowed)
        // For this pure C++ version, we'll add a 'visited' flag to GraphVertexNode
        // Reset visited flags for all nodes
        GraphVertexNode* tempV = vertexListHead;
        while(tempV) { tempV->visited = false; tempV = tempV->nextVertex; }


        ManualQueue q; // Our own queue
        cout << "BFS starting from vertex " << startVertexID << ": ";

        startNode->visited = true;
        q.enqueue(to_string(startVertexID), "VERTEX"); // Store ID as string for queue data

        while (!q.isEmpty()) {
            QueueNode* currentQNode = q.dequeue();
            int currentVertexID = stoi(currentQNode->licensePlate); // Re-use field, convert back
            delete currentQNode;

            cout << currentVertexID << " ";

            GraphVertexNode* u = findVertex(currentVertexID);
            GraphEdgeNode* edge = u->adjListHead;
            while (edge) {
                GraphVertexNode* v = findVertex(edge->toVertexID);
                if (v && !v->visited) {
                    v->visited = true;
                    q.enqueue(to_string(v->id), "VERTEX");
                }
                edge = edge->next;
            }
        }
        cout << endl;
    }

    // --- AMBITIOUS FEATURE: Depth-First Search (DFS) ---
    void DFSUtil(GraphVertexNode* v, bool visited[]) { // visited array needs to be sized by max vertex ID or use map
        // For pure C++, using the node's visited flag
        v->visited = true;
        cout << v->id << " ";

        GraphEdgeNode* edge = v->adjListHead;
        while(edge) {
            GraphVertexNode* adjV = findVertex(edge->toVertexID);
            if(adjV && !adjV->visited) {
                DFSUtil(adjV, visited); // Recursive call
            }
            edge = edge->next;
        }
    }

    void DFS(int startVertexID) {
        GraphVertexNode* startNode = findVertex(startVertexID);
        if (!startNode) {
            cout << "DFS Error: Start vertex " << startVertexID << " not found." << endl;
            return;
        }
        // Reset visited flags
        GraphVertexNode* tempV = vertexListHead;
        while(tempV) { tempV->visited = false; tempV = tempV->nextVertex; }

        cout << "DFS starting from vertex " << startVertexID << ": ";
        DFSUtil(startNode, NULL); // Visited array not strictly needed if using node flags
        cout << endl;
    }

    // --- AMBITIOUS FEATURE: Dijkstra's Algorithm (using sorted linked list as PQ) ---
    void Dijkstra(int startVertexID) {
        GraphVertexNode* startNode = findVertex(startVertexID);
        if (!startNode) {
            cout << "Dijkstra Error: Start vertex " << startVertexID << " not found." << endl;
            return;
        }

        // 1. Initialize distances and visited status
        GraphVertexNode* currentV = vertexListHead;
        while (currentV) {
            currentV->distFromSource = (currentV->id == startVertexID) ? 0 : 2147483647;
            currentV->visited = false;
            currentV = currentV->nextVertex;
        }

        // 2. "Priority Queue" - Implemented as a sorted linked list of DijkstraNode
        DijkstraNode* pqHead = NULL; // Head of our sorted linked list PQ
        ManualLinkedList dijkstraPQHelper; // Just to use its sortedInsertDijkstra method

        dijkstraPQHelper.sortedInsertDijkstra(&pqHead, new DijkstraNode(startVertexID, 0));

        cout << "Dijkstra's Shortest Paths from Vertex " << startVertexID << ":" << endl;

        while (pqHead != NULL) {
            // Extract min from our "PQ"
            DijkstraNode* minNode = pqHead;
            pqHead = pqHead->next; // Dequeue

            GraphVertexNode* u = findVertex(minNode->vertexID);

            if (!u || u->visited) { // Already processed or invalid
                delete minNode;
                continue;
            }
            u->visited = true;

            // For outputting path info (optional, complex to reconstruct full path without parent pointers)
            // cout << "  Processing vertex " << u->id << " (current shortest_dist: " << u->distFromSource << ")" << endl;


            // Iterate over neighbors
            GraphEdgeNode* edge = u->adjListHead;
            while (edge) {
                GraphVertexNode* v = findVertex(edge->toVertexID);
                if (v && !v->visited) {
                    int newDist = u->distFromSource + edge->weight;
                    if (newDist < v->distFromSource) {
                        v->distFromSource = newDist;
                        // Add/Update v in PQ
                        // For sorted list, this means removing old if exists and inserting new.
                        // Simpler: just insert. Duplicates will be handled by visited flag or later extraction.
                        dijkstraPQHelper.sortedInsertDijkstra(&pqHead, new DijkstraNode(v->id, newDist));
                    }
                }
                edge = edge->next;
            }
            delete minNode; // Clean up the dequeued node from PQ
        }

        // Print results
        cout << "Vertex distances from " << startVertexID << ":" << endl;
        currentV = vertexListHead;
        while (currentV) {
            cout << "  To " << currentV->id << " (" << currentV->name << "): ";
            if (currentV->distFromSource == 2147483647) {
                cout << "Not Reachable" << endl;
            } else {
                cout << currentV->distFromSource << endl;
            }
            currentV = currentV->nextVertex;
        }
    }
};


// --- Smart City Entity Definitions ---
struct Citizen {
    int id;
    string name;
    string address;
    // Other relevant data, e.g., utility IDs, can be linked via ManualLinkedList if needed
    Citizen(int i=0, string n="", string addr=""): id(i), name(n), address(addr){}
};

struct VehicleEntity { // To differentiate from QueueNode for traffic
    string licensePlate;
    string type; // CAR, BUS, EMERGENCY_VEHICLE
    string ownerCitizenName; // Link to citizen

    VehicleEntity(string lp = "", string t = "", string owner = "") : licensePlate(lp), type(t), ownerCitizenName(owner) {}
};


// --- Smart City Module Implementations ---
class CitizenManagement {
public:
    ManualLinkedList citizenList; // Stores Citizen objects (conceptually, NodeLL will hold citizen data)
    Trie citizenNameTrie; // For quick name search/autocomplete

    CitizenManagement() {}

    void addCitizen(int id, string name, string address) {
        if (citizenList.findNode(id)) {
            cout << "Citizen with ID " << id << " already exists." << endl;
            return;
        }
        citizenList.insertAtTail(id, name, address); // Using NodeLL fields for citizen data
        citizenNameTrie.insert(name);
        cout << "Citizen " << name << " added." << endl;
    }

    void findCitizen(int id) {
        NodeLL* citizenNode = citizenList.findNode(id);
        if (citizenNode) {
            cout << "Citizen Found: ID=" << citizenNode->id << ", Name=" << citizenNode->name << ", Address=" << citizenNode->data1 << endl;
        } else {
            cout << "Citizen with ID " << id << " not found." << endl;
        }
    }

    void displayAllCitizens() {
        cout << "\n--- All Citizens ---" << endl;
        citizenList.displayList();
    }

    void suggestCitizenNames(string prefix) {
        cout << "\n--- Citizen Name Suggestions for prefix '" << prefix << "' ---" << endl;
        ManualLinkedList* suggestions = citizenNameTrie.getSuggestions(prefix);
        if (suggestions->isEmpty()) {
            cout << "No suggestions found." << endl;
        } else {
            suggestions->displayList();
        }
        delete suggestions; // Clean up the suggestions list
    }
};

class TrafficManagement {
public:
    ManualQueue intersectionQueueA; // Example: An intersection with a priority queue
    // Graph* cityRoadNetwork; // Pointer to the main city graph

    TrafficManagement(/*Graph* roadNetwork*/) {
        // cityRoadNetwork = roadNetwork;
    }

    void vehicleArrives(string plate, string type, int priority = 1) { // default priority 1
        if (type == "EMERGENCY") { // Simple rule for emergency
            priority = 0;
        }
        intersectionQueueA.priorityEnqueue(plate, type, priority);
        cout << "Vehicle " << plate << " (" << type << ", Prio " << priority <<") arrived at intersection A." << endl;
    }

    void vehicleDeparts() {
        if (intersectionQueueA.isEmpty()) {
            cout << "No vehicles to depart from intersection A." << endl;
            return;
        }
        QueueNode* departed = intersectionQueueA.dequeue();
        cout << "Vehicle " << departed->licensePlate << " (" << departed->vehicleType << ") departed from intersection A." << endl;
        delete departed;
    }

    void displayIntersectionQueue() {
        cout << "\n--- Intersection A Queue Status ---" << endl;
        intersectionQueueA.displayQueue();
    }
};

class ParkingManagement {
public:
    ManualStack parkingLotTower1;
    int capacity;

    ParkingManagement(int cap = 10) : capacity(cap) {}

    void parkVehicle(string licensePlate) {
        if (parkingLotTower1.getSize() >= capacity) {
            cout << "Parking lot Tower 1 is full. Cannot park " << licensePlate << "." << endl;
            return;
        }
        parkingLotTower1.push(licensePlate);
        cout << "Vehicle " << licensePlate << " parked in Tower 1. Spots left: " << capacity - parkingLotTower1.getSize() << endl;
    }

    void unparkVehicle() {
        if (parkingLotTower1.isEmpty()) {
            cout << "Parking lot Tower 1 is empty. No vehicle to unpark." << endl;
            return;
        }
        string unparkedPlate = parkingLotTower1.pop();
        cout << "Vehicle " << unparkedPlate << " unparked from Tower 1. Spots left: " << capacity - parkingLotTower1.getSize() << endl;
    }

    void displayParkingLotStatus() {
        cout << "\n--- Parking Lot Tower 1 Status (Capacity: " << capacity << ") ---" << endl;
        parkingLotTower1.displayStack();
        cout << "Vehicles Parked: " << parkingLotTower1.getSize() << endl;
        cout << "Spots Available: " << capacity - parkingLotTower1.getSize() << endl;
    }
};


class UrbanStructure {
public:
    NaryTree cityHierarchy;

    UrbanStructure(string cityName) : cityHierarchy(cityName) {}

    void addDepartment(string parentDept, string newDept) {
        cityHierarchy.addChild(parentDept, newDept);
        cout << "Department '" << newDept << "' added under '" << parentDept << "'." << endl;
    }

    void displayCityHierarchy() {
        cout << "\n--- City Hierarchy ---" << endl;
        cityHierarchy.displayTree();
    }
};

class NetworkService {
public:
    Graph* cityGraph; // Reference to the main city graph

    NetworkService(Graph* graph) : cityGraph(graph) {}

    void findShortestPath(int startID, int endID) { // endID not used in current Dijkstra output
        if (!cityGraph) {
            cout << "Error: City graph not initialized for Network Service." << endl;
            return;
        }
        cout << "\n--- Finding Shortest Paths (Dijkstra) ---" << endl;
        cityGraph->Dijkstra(startID);
    }

    void performBFS(int startID) {
        if (!cityGraph) {
            cout << "Error: City graph not initialized for Network Service." << endl;
            return;
        }
        cout << "\n--- Performing BFS ---" << endl;
        cityGraph->BFS(startID);
    }
    
    void performDFS(int startID) {
        if (!cityGraph) {
            cout << "Error: City graph not initialized for Network Service." << endl;
            return;
        }
        cout << "\n--- Performing DFS ---" << endl;
        cityGraph->DFS(startID);
    }
};


// --- Main Smart City Simulator Class ---
class SmartCitySimulator {
public:
    CitizenManagement citizenModule;
    TrafficManagement trafficModule;
    ParkingManagement parkingModule;
    UrbanStructure structureModule;
    NetworkService networkModule;
    Graph cityRoadNetwork; // The actual graph instance

    SmartCitySimulator(string cityName) : structureModule(cityName), networkModule(&cityRoadNetwork) /*, trafficModule(&cityRoadNetwork)*/ {
        cout << "Smart City Simulator for '" << cityName << "' initialized." << endl;
        initializeDefaultCityData();
    }

    ~SmartCitySimulator() {
        cout << "Smart City Simulator shutting down." << endl;
        // Destructors of member objects will be called automatically.
        // The cityRoadNetwork (Graph instance) will also be cleaned up by its destructor.
    }

    void initializeDefaultCityData() {
        cout << "\nInitializing default city data..." << endl;
        // Citizens
        citizenModule.addCitizen(101, "Adi Nugraha", "Jl. Merdeka 1");
        citizenModule.addCitizen(102, "Bunga Citra", "Jl. Sudirman 20");
        citizenModule.addCitizen(103, "Charlie Van Houten", "Jl. Diponegoro 5A");

        // Urban Structure
        structureModule.addDepartment("Kota Smartvana", "Dinas Kependudukan");
        structureModule.addDepartment("Kota Smartvana", "Dinas Perhubungan");
        structureModule.addDepartment("Dinas Perhubungan", "Seksi Lalu Lintas");
        structureModule.addDepartment("Dinas Perhubungan", "Seksi Parkir");

        // Road Network (Vertices are locations/intersections)
        cityRoadNetwork.addVertex(1, "Simpang A");
        cityRoadNetwork.addVertex(2, "Simpang B");
        cityRoadNetwork.addVertex(3, "Kantor Walikota");
        cityRoadNetwork.addVertex(4, "Pasar Sentral");
        cityRoadNetwork.addVertex(5, "Rumah Sakit Umum");
        cityRoadNetwork.addVertex(6, "Area Parkir Pusat");


        // Roads (Edges) - (from, to, weight/distance)
        cityRoadNetwork.addEdge(1, 2, 5); // A -> B, weight 5
        cityRoadNetwork.addEdge(2, 1, 5); // B -> A (if two-way)
        cityRoadNetwork.addEdge(1, 4, 3); // A -> Pasar
        cityRoadNetwork.addEdge(4, 1, 3);
        cityRoadNetwork.addEdge(2, 3, 7); // B -> Kantor Walikota
        cityRoadNetwork.addEdge(3, 2, 7);
        cityRoadNetwork.addEdge(2, 5, 4); // B -> RS Umum
        cityRoadNetwork.addEdge(5, 2, 4);
        cityRoadNetwork.addEdge(4, 5, 6); // Pasar -> RS Umum
        cityRoadNetwork.addEdge(5, 4, 6);
        cityRoadNetwork.addEdge(3, 6, 2); // Kantor -> Parkir Pusat
        cityRoadNetwork.addEdge(6, 3, 2);
        cityRoadNetwork.addEdge(4, 6, 4); // Pasar -> Parkir Pusat
        cityRoadNetwork.addEdge(6, 4, 4);


        cout << "Default data initialization complete." << endl;
    }


    void displayMainMenu() {
        cout << "\n===== Smart City Simulator Menu =====" << endl;
        cout << "1. Citizen Management" << endl;
        cout << "2. Traffic Management" << endl;
        cout << "3. Parking Management" << endl;
        cout << "4. Urban Structure" << endl;
        cout << "5. Network Services (Graph)" << endl;
        cout << "0. Exit Simulator" << endl;
        cout << "===================================" << endl;
        cout << "Enter your choice: ";
    }

    void citizenMenu() {
        int choice;
        do {
            cout << "\n--- Citizen Management Menu ---" << endl;
            cout << "1. Add Citizen" << endl;
            cout << "2. Find Citizen by ID" << endl;
            cout << "3. Display All Citizens" << endl;
            cout << "4. Suggest Citizen Names (Trie)" << endl;
            cout << "0. Back to Main Menu" << endl;
            cout << "Enter choice: ";
            cin >> choice;
            cin.ignore(); // consume newline

            int id;
            string name, address, prefix;

            switch (choice) {
                case 1:
                    cout << "Enter ID: "; cin >> id; cin.ignore();
                    cout << "Enter Name: "; getline(cin, name);
                    cout << "Enter Address: "; getline(cin, address);
                    citizenModule.addCitizen(id, name, address);
                    break;
                case 2:
                    cout << "Enter ID to find: "; cin >> id; cin.ignore();
                    citizenModule.findCitizen(id);
                    break;
                case 3:
                    citizenModule.displayAllCitizens();
                    break;
                case 4:
                    cout << "Enter name prefix for suggestions: "; getline(cin, prefix);
                    citizenModule.suggestCitizenNames(prefix);
                    break;
                case 0: break;
                default: cout << "Invalid choice. Try again." << endl;
            }
        } while (choice != 0);
    }

    void trafficMenu() {
         int choice;
        do {
            cout << "\n--- Traffic Management Menu ---" << endl;
            cout << "1. Vehicle Arrives at Intersection A" << endl;
            cout << "2. Vehicle Departs from Intersection A" << endl;
            cout << "3. Display Intersection A Queue" << endl;
            cout << "0. Back to Main Menu" << endl;
            cout << "Enter choice: ";
            cin >> choice;
            cin.ignore();

            string plate, type;
            int priority;

            switch (choice) {
                case 1:
                    cout << "Enter License Plate: "; getline(cin, plate);
                    cout << "Enter Vehicle Type (CAR, BUS, EMERGENCY): "; getline(cin, type);
                    // Simple priority assignment based on type
                    if (type == "EMERGENCY") priority = 0;
                    else if (type == "BUS") priority = 1;
                    else priority = 2;
                    trafficModule.vehicleArrives(plate, type, priority);
                    break;
                case 2:
                    trafficModule.vehicleDeparts();
                    break;
                case 3:
                    trafficModule.displayIntersectionQueue();
                    break;
                case 0: break;
                default: cout << "Invalid choice. Try again." << endl;
            }
        } while (choice != 0);
    }

    void parkingMenu() {
        int choice;
        do {
            cout << "\n--- Parking Management Menu ---" << endl;
            cout << "1. Park Vehicle in Tower 1" << endl;
            cout << "2. Unpark Last Vehicle from Tower 1" << endl;
            cout << "3. Display Tower 1 Status" << endl;
            cout << "0. Back to Main Menu" << endl;
            cout << "Enter choice: ";
            cin >> choice;
            cin.ignore();
            string plate;
            switch (choice) {
                case 1:
                    cout << "Enter License Plate to Park: "; getline(cin, plate);
                    parkingModule.parkVehicle(plate);
                    break;
                case 2:
                    parkingModule.unparkVehicle();
                    break;
                case 3:
                    parkingModule.displayParkingLotStatus();
                    break;
                case 0: break;
                default: cout << "Invalid choice. Try again." << endl;
            }
        } while (choice != 0);
    }
    
    void structureMenu() {
        int choice;
        do {
            cout << "\n--- Urban Structure Menu ---" << endl;
            cout << "1. Add Department/Sub-unit" << endl;
            cout << "2. Display City Hierarchy" << endl;
            cout << "0. Back to Main Menu" << endl;
            cout << "Enter choice: ";
            cin >> choice;
            cin.ignore();
            string parent, child;
            switch (choice) {
                case 1:
                    cout << "Enter Parent Department/Unit Name: "; getline(cin, parent);
                    cout << "Enter New Department/Sub-unit Name: "; getline(cin, child);
                    structureModule.addDepartment(parent, child);
                    break;
                case 2:
                    structureModule.displayCityHierarchy();
                    break;
                case 0: break;
                default: cout << "Invalid choice. Try again." << endl;
            }
        } while (choice != 0);
    }

    void networkMenu() {
        int choice;
        do {
            cout << "\n--- Network Services Menu ---" << endl;
            cout << "1. Display Road Network (Graph)" << endl;
            cout << "2. Find Shortest Paths from Location (Dijkstra)" << endl;
            cout << "3. Perform BFS Traversal from Location" << endl;
            cout << "4. Perform DFS Traversal from Location" << endl;
            cout << "0. Back to Main Menu" << endl;
            cout << "Enter choice: ";
            cin >> choice;
            cin.ignore();
            int startID;
            switch (choice) {
                case 1:
                    cityRoadNetwork.displayGraph();
                    break;
                case 2:
                    cout << "Enter Start Location ID for Dijkstra: "; cin >> startID; cin.ignore();
                    networkModule.findShortestPath(startID, -1); // endID not used here
                    break;
                case 3:
                    cout << "Enter Start Location ID for BFS: "; cin >> startID; cin.ignore();
                    networkModule.performBFS(startID);
                    break;
                case 4:
                    cout << "Enter Start Location ID for DFS: "; cin >> startID; cin.ignore();
                    networkModule.performDFS(startID);
                    break;

                case 0: break;
                default: cout << "Invalid choice. Try again." << endl;
            }
        } while (choice != 0);
    }


    void run() {
        int choice;
        do {
            displayMainMenu();
            cin >> choice;
            cin.ignore(); // consume the newline character after reading integer

            switch (choice) {
                case 1: citizenMenu(); break;
                case 2: trafficMenu(); break;
                case 3: parkingMenu(); break;
                case 4: structureMenu(); break;
                case 5: networkMenu(); break;
                case 0: cout << "Exiting Smart City Simulator. Goodbye!" << endl; break;
                default: cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 0);
    }
};


int main() {
    // Basic check for new failure (though modern C++ throws bad_alloc)
    SmartCitySimulator* myCity = NULL;
    try {
       myCity  = new SmartCitySimulator("Kota Smartvana");
    } catch(...) { // Catch any potential error from new, though unlikely with just iostream/string
        cout << "FATAL ERROR: Could not allocate memory for SmartCitySimulator." << endl;
        return 1;
    }
    
    if (myCity) {
        myCity->run();
        delete myCity; // Ensure destructor is called
    } else {
         cout << "FATAL ERROR: SmartCitySimulator pointer is null after allocation attempt." << endl;
        return 1;
    }

    return 0;
}
