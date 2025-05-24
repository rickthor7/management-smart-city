#include <iostream>
#include <string>

using namespace std;

// Konstanta untuk Trie (alfabet huruf kecil sederhana)
const int ALPHABET_SIZE = 26;
const int MAX_SUGGESTIONS = 5; // Jumlah maksimal saran untuk autocompletion Trie

// Deklarasi forward (karena ada dependensi silang atau untuk kerapian)
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

// --- Fungsi Utilitas (jika diperlukan, misal untuk string ke int sederhana) ---
// Karena tidak boleh ada library lain, konversi string ke int manual jika diperlukan,
// tapi untuk contoh ini, kita akan coba hindari kebutuhan konversi kompleks.
// Untuk Dijkstra, kita akan menggunakan ID integer secara langsung.

// --- Struktur Helper untuk Priority Queue Dijkstra (diimplementasikan sebagai sorted linked list) ---
struct DijkstraNode {
    int vertexID;
    int distance;
    DijkstraNode* next;

    DijkstraNode(int id, int dist) : vertexID(id), distance(dist), next(NULL) {}
};

// --- IMPLEMENTASI INTI: Manual Doubly Circular Linked List dengan Sentinel ---
struct NodeLL {
    // Placeholder data generik - untuk kesederhanaan, kita akan definisikan data spesifik di entitas
    int id;         // Contoh: ID Penduduk, ID Kendaraan
    string name;    // Contoh: Nama Penduduk, Nama Jalan
    string data1;   // Field data tambahan 1
    string data2;   // Field data tambahan 2

    NodeLL* next;
    NodeLL* prev;

    NodeLL(int i = 0, string n = "", string d1 = "", string d2 = "") : id(i), name(n), data1(d1), data2(d2), next(NULL), prev(NULL) {}
};

class ManualLinkedList {
public:
    NodeLL* sentinel;
    int count;

    ManualLinkedList() {
        sentinel = new NodeLL(-1, "SENTINEL"); // Node sentinel
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
        return count == 0; // Atau sentinel->next == sentinel;
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
        return NULL; // Tidak ditemukan
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
        return false; // Tidak ditemukan
    }

    void displayList() const {
        if (isEmpty()) {
            cout << "List kosong." << endl;
            return;
        }
        NodeLL* current = sentinel->next;
        while (current != sentinel) {
            cout << "ID: " << current->id << ", Nama: " << current->name;
            if (!current->data1.empty()) cout << ", Data1: " << current->data1;
            if (!current->data2.empty()) cout << ", Data2: " << current->data2;
            cout << endl;
            current = current->next;
        }
    }

    // FITUR AMBISIUS: Manual Insertion Sort (untuk linked list, berguna untuk PQ di Dijkstra)
    // Sortir spesifik ini untuk DijkstraNode, bukan NodeLL generik.
    // Sortir yang lebih generik memerlukan pointer fungsi atau template (tidak menggunakan template di sini demi kesederhanaan)
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

// --- IMPLEMENTASI INTI: Manual Queue (menggunakan prinsip ManualLinkedList) ---
// Untuk kesederhanaan, Queue akan menggunakan struktur singly linked list secara internal,
// daripada langsung menggunakan ulang ManualLinkedList untuk menghindari konflik tipe untuk item antrean.
struct QueueNode {
    // Untuk simulasi lalu lintas, kendaraan mungkin memiliki plat nomor dan tipe
    string licensePlate;
    string vehicleType; // contoh: MOBIL, BUS, DARURAT
    int priority; // Untuk priority queue, 0 adalah prioritas tertinggi

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

    // Enqueue Dasar
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

    // FITUR AMBISIUS: Priority Enqueue (versi sederhana, menyisipkan berdasarkan prioritas)
    // Angka lebih kecil berarti prioritas lebih tinggi. Mengasumsikan antrean dijaga terurut berdasarkan prioritas.
    // Ini menjadikannya priority queue sederhana. Untuk PQ sejati, heap lebih baik.
    void priorityEnqueue(string lp, string vt, int prio) {
        QueueNode* newNode = new QueueNode(lp, vt, prio);
        count++;

        if (isEmpty() || prio < front->priority) {
            newNode->next = front;
            front = newNode;
            if (rear == NULL) rear = front; // jika sebelumnya kosong
            return;
        }

        QueueNode* current = front;
        while (current->next != NULL && current->next->priority <= prio) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
        if(newNode->next == NULL) rear = newNode; // jika disisipkan di akhir
    }


    QueueNode* dequeue() {
        if (isEmpty()) {
            return NULL;
        }
        QueueNode* temp = front;
        front = front->next;
        if (front == NULL) { // Antrean menjadi kosong
            rear = NULL;
        }
        temp->next = NULL; // Isolasi node
        count--;
        return temp; // Pemanggil harus menghapus node ini
    }

    QueueNode* peek() const {
        return front;
    }

    void displayQueue() const {
        if (isEmpty()) {
            cout << "Antrean kosong." << endl;
            return;
        }
        QueueNode* current = front;
        cout << "Kendaraan dalam Antrean (Depan ke Belakang):" << endl;
        while (current != NULL) {
            cout << "  Plat: " << current->licensePlate << ", Tipe: " << current->vehicleType << ", Prio: " << current->priority << endl;
            current = current->next;
        }
    }
};

// --- IMPLEMENTASI INTI: Manual Stack (menggunakan prinsip ManualLinkedList) ---
// Juga menggunakan singly linked list sederhana untuk item stack.
struct StackNode {
    string data; // Contoh: Plat Nomor Mobil Terparkir, Perintah Undo
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
            return "KESALAHAN_STACK_KOSONG";
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
            return "KESALAHAN_STACK_KOSONG";
        }
        return top->data;
    }

    void displayStack() const {
        if (isEmpty()) {
            cout << "Stack kosong." << endl;
            return;
        }
        StackNode* current = top;
        cout << "Item dalam Stack (Atas ke Bawah):" << endl;
        while (current != NULL) {
            cout << "  " << current->data << endl;
            current = current->next;
        }
    }
};

// --- IMPLEMENTASI INTI: N-Ary Tree (Left-Child, Right-Sibling) ---
struct NaryTreeNode {
    string name; // Nama departemen, area, dll.
    NaryTreeNode* firstChild;
    NaryTreeNode* nextSibling;

    NaryTreeNode(string n) : name(n), firstChild(NULL), nextSibling(NULL) {}

    ~NaryTreeNode() { // Destruktor rekursif untuk anak dan saudara
        delete firstChild;  // Ini akan memicu rantai destruktor anak
        delete nextSibling; // Ini akan memicu rantai destruktor saudara
    }
};

class NaryTree {
public:
    NaryTreeNode* root;

    NaryTree(string rootName) {
        root = new NaryTreeNode(rootName);
    }

    ~NaryTree() {
        delete root; // Ini akan memulai penghapusan rekursif
    }

    // Helper untuk mencari node (bisa ditingkatkan, contoh: pencarian BFS/DFS)
    NaryTreeNode* findNodeRecursive(NaryTreeNode* current, const string& name) {
        if (!current) return NULL;
        if (current->name == name) return current;

        NaryTreeNode* foundNode = findNodeRecursive(current->firstChild, name);
        if (foundNode) return foundNode;

        return findNodeRecursive(current->nextSibling, name);
    }
    
    NaryTreeNode* findNode(const string& name) {
        if (root && root->name == name) return root; // Periksa root dulu
        return root ? findNodeRecursive(root->firstChild, name) : NULL; // Cari anak jika root ada
    }


    void addChild(const string& parentName, const string& childName) {
        NaryTreeNode* parentNode = findNode(parentName);
        if (!parentNode) {
            cout << "Kesalahan: Node induk '" << parentName << "' tidak ditemukan untuk N-Ary Tree." << endl;
            return;
        }

        NaryTreeNode* newNode = new NaryTreeNode(childName);
        if (!parentNode->firstChild) { // Belum ada anak
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
        for (int i = 0; i < depth; ++i) cout << "  "; // Indentasi
        cout << "- " << node->name << endl;

        displayTreeRecursive(node->firstChild, depth + 1);
        displayTreeRecursive(node->nextSibling, depth); // Saudara berada pada kedalaman yang sama relatif terhadap pemanggilan induk
    }

    void displayTree() {
        cout << "Struktur N-Ary Tree:" << endl;
        if(root) {
             cout << root->name << endl;
             displayTreeRecursive(root->firstChild, 1);
        } else {
            cout << "Tree kosong." << endl;
        }
    }
};

// --- FITUR AMBISIUS: Trie (Prefix Tree) ---
struct TrieNode {
    TrieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;
    string wordStored; // Simpan kata lengkap untuk kemudahan pengambilan saran

    TrieNode() {
        isEndOfWord = false;
        wordStored = "";
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            children[i] = NULL;
        }
    }

    // Destruktor untuk TrieNode untuk menghapus anak-anak
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
            // Konversi dasar ke huruf kecil dan pemeriksaan
            char ch = word[i];
            if (ch >= 'A' && ch <= 'Z') ch = ch - 'A' + 'a';
            if (ch < 'a' || ch > 'z') continue; // Lewati non-alfabet

            int index = ch - 'a';
            if (!current->children[index]) {
                current->children[index] = new TrieNode();
            }
            current = current->children[index];
        }
        current->isEndOfWord = true;
        current->wordStored = word; // Simpan kata asli (atau yang telah diproses)
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

    // Helper untuk saran
    void getSuggestionsRecursive(TrieNode* node, string currentPrefix, ManualLinkedList* suggestionsList, int& count) {
        if (!node || count >= MAX_SUGGESTIONS) {
            return;
        }

        if (node->isEndOfWord && !node->wordStored.empty()) { // Pastikan wordStored tidak kosong
            suggestionsList->insertAtTail(count + 1, node->wordStored); // Gunakan ID untuk nomor saran
            count++;
        }

        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            if (node->children[i]) {
                // char nextChar = 'a' + i; // Tidak terlalu dibutuhkan untuk struktur list ini
                getSuggestionsRecursive(node->children[i], currentPrefix /* + nextChar */, suggestionsList, count);
            }
        }
    }

    ManualLinkedList* getSuggestions(string prefix) {
        TrieNode* current = root;
        for (int i = 0; i < prefix.length(); ++i) {
            char ch = prefix[i];
            if (ch >= 'A' && ch <= 'Z') ch = ch - 'A' + 'a';
            if (ch < 'a' || ch > 'z') return new ManualLinkedList(); // List kosong untuk prefix tidak valid

            int index = ch - 'a';
            if (!current->children[index]) {
                return new ManualLinkedList(); // Prefix tidak ditemukan
            }
            current = current->children[index];
        }

        ManualLinkedList* suggestionsList = new ManualLinkedList();
        int suggestionCount = 0;
        getSuggestionsRecursive(current, prefix, suggestionsList, suggestionCount);
        return suggestionsList;
    }
};


// --- IMPLEMENTASI INTI: Graph (Adjacency List menggunakan ManualLinkedList) ---
// Node edge untuk adjacency list
struct GraphEdgeNode {
    int toVertexID;
    int weight;
    GraphEdgeNode* next;

    GraphEdgeNode(int to, int w) : toVertexID(to), weight(w), next(NULL) {}
};

// Node vertex untuk graph
struct GraphVertexNode {
    int id;
    string name; // contoh: Nama Persimpangan, Nama Lokasi
    GraphEdgeNode* adjListHead; // Head dari adjacency list (linked list dari edge)
    GraphVertexNode* nextVertex; // Untuk menghubungkan vertex dalam list utama vertex graph

    // Untuk Dijkstra
    int distFromSource;
    bool visited;


    GraphVertexNode(int i, string n) : id(i), name(n), adjListHead(NULL), nextVertex(NULL), distFromSource(2147483647), visited(false) {} // 2147483647 ~ tak hingga

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
    GraphVertexNode* vertexListHead; // Head dari linked list semua vertex dalam graph
    int numVertices;

    Graph() : vertexListHead(NULL), numVertices(0) {}

    ~Graph() {
        GraphVertexNode* currentVertex = vertexListHead;
        while (currentVertex) {
            GraphVertexNode* tempVertex = currentVertex;
            currentVertex = currentVertex->nextVertex;
            delete tempVertex; // Ini akan memanggil ~GraphVertexNode() yang menghapus adj list-nya
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
            cout << "Kesalahan: Vertex dengan ID " << id << " sudah ada." << endl;
            return;
        }
        GraphVertexNode* newNode = new GraphVertexNode(id, name);
        newNode->nextVertex = vertexListHead; // Tambah ke head dari list vertex
        vertexListHead = newNode;
        numVertices++;
    }

    void addEdge(int fromID, int toID, int weight) {
        GraphVertexNode* fromVertex = findVertex(fromID);
        GraphVertexNode* toVertex = findVertex(toID);

        if (!fromVertex || !toVertex) {
            cout << "Kesalahan: Satu atau kedua vertex tidak ditemukan untuk edge (" << fromID << " -> " << toID << ")." << endl;
            return;
        }

        // Tambah edge dari fromID ke toID
        GraphEdgeNode* newEdge = new GraphEdgeNode(toID, weight);
        newEdge->next = fromVertex->adjListHead;
        fromVertex->adjListHead = newEdge;

        // Untuk graph tidak berarah, tambahkan juga edge sebaliknya
        // GraphEdgeNode* reverseEdge = new GraphEdgeNode(fromID, weight);
        // reverseEdge->next = toVertex->adjListHead;
        // toVertex->adjListHead = reverseEdge;
    }

    void displayGraph() {
        cout << "Adjacency List Graph:" << endl;
        GraphVertexNode* currentVertex = vertexListHead;
        if (!currentVertex) {
            cout << "Graph kosong." << endl;
            return;
        }
        while (currentVertex) {
            cout << "Vertex " << currentVertex->id << " (" << currentVertex->name << "): ";
            GraphEdgeNode* currentEdge = currentVertex->adjListHead;
            if (!currentEdge) {
                cout << "-> Tidak ada edge keluar";
            }
            while (currentEdge) {
                cout << "-> " << currentEdge->toVertexID << "(bobot:" << currentEdge->weight << ") ";
                currentEdge = currentEdge->next;
            }
            cout << endl;
            currentVertex = currentVertex->nextVertex;
        }
    }

    // --- FITUR AMBISIUS: Breadth-First Search (BFS) ---
    void BFS(int startVertexID) {
        GraphVertexNode* startNode = findVertex(startVertexID);
        if (!startNode) {
            cout << "Kesalahan BFS: Vertex awal " << startVertexID << " tidak ditemukan." << endl;
            return;
        }

        // Pelacakan visited manual (bisa array bool jika ID dari 0 hingga N-1, atau map/set jika diizinkan)
        // Untuk versi C++ murni ini, kita akan menambahkan flag 'visited' ke GraphVertexNode
        // Reset flag visited untuk semua node
        GraphVertexNode* tempV = vertexListHead;
        while(tempV) { tempV->visited = false; tempV = tempV->nextVertex; }


        ManualQueue q; // Antrean kita sendiri
        cout << "BFS dimulai dari vertex " << startVertexID << ": ";

        startNode->visited = true;
        q.enqueue(to_string(startVertexID), "TIPE_VERTEX"); // Simpan ID sebagai string untuk data antrean

        while (!q.isEmpty()) {
            QueueNode* currentQNode = q.dequeue();
            int currentVertexID = stoi(currentQNode->licensePlate); // Gunakan ulang field, konversi kembali
            delete currentQNode;

            cout << currentVertexID << " ";

            GraphVertexNode* u = findVertex(currentVertexID);
            GraphEdgeNode* edge = u->adjListHead;
            while (edge) {
                GraphVertexNode* v = findVertex(edge->toVertexID);
                if (v && !v->visited) {
                    v->visited = true;
                    q.enqueue(to_string(v->id), "TIPE_VERTEX");
                }
                edge = edge->next;
            }
        }
        cout << endl;
    }

    // --- FITUR AMBISIUS: Depth-First Search (DFS) ---
    void DFSUtil(GraphVertexNode* v) { 
        // Untuk C++ murni, menggunakan flag visited milik node
        v->visited = true;
        cout << v->id << " ";

        GraphEdgeNode* edge = v->adjListHead;
        while(edge) {
            GraphVertexNode* adjV = findVertex(edge->toVertexID);
            if(adjV && !adjV->visited) {
                DFSUtil(adjV); // Pemanggilan rekursif
            }
            edge = edge->next;
        }
    }

    void DFS(int startVertexID) {
        GraphVertexNode* startNode = findVertex(startVertexID);
        if (!startNode) {
            cout << "Kesalahan DFS: Vertex awal " << startVertexID << " tidak ditemukan." << endl;
            return;
        }
        // Reset flag visited
        GraphVertexNode* tempV = vertexListHead;
        while(tempV) { tempV->visited = false; tempV = tempV->nextVertex; }

        cout << "DFS dimulai dari vertex " << startVertexID << ": ";
        DFSUtil(startNode);
        cout << endl;
    }

    // --- FITUR AMBISIUS: Algoritma Dijkstra (menggunakan sorted linked list sebagai PQ) ---
    void Dijkstra(int startVertexID) {
        GraphVertexNode* startNode = findVertex(startVertexID);
        if (!startNode) {
            cout << "Kesalahan Dijkstra: Vertex awal " << startVertexID << " tidak ditemukan." << endl;
            return;
        }

        // 1. Inisialisasi jarak dan status visited
        GraphVertexNode* currentV = vertexListHead;
        while (currentV) {
            currentV->distFromSource = (currentV->id == startVertexID) ? 0 : 2147483647; // tak hingga
            currentV->visited = false;
            currentV = currentV->nextVertex;
        }

        // 2. "Priority Queue" - Diimplementasikan sebagai sorted linked list dari DijkstraNode
        DijkstraNode* pqHead = NULL; // Head dari sorted linked list PQ kita
        ManualLinkedList dijkstraPQHelper; // Hanya untuk menggunakan metode sortedInsertDijkstra-nya

        dijkstraPQHelper.sortedInsertDijkstra(&pqHead, new DijkstraNode(startVertexID, 0));

        cout << "Jalur Terpendek Dijkstra dari Vertex " << startVertexID << ":" << endl;

        while (pqHead != NULL) {
            // Ekstrak min dari "PQ" kita
            DijkstraNode* minNode = pqHead;
            pqHead = pqHead->next; // Dequeue

            GraphVertexNode* u = findVertex(minNode->vertexID);

            if (!u || u->visited) { // Sudah diproses atau tidak valid
                delete minNode;
                continue;
            }
            u->visited = true;

            // Iterasi atas tetangga
            GraphEdgeNode* edge = u->adjListHead;
            while (edge) {
                GraphVertexNode* v = findVertex(edge->toVertexID);
                if (v && !v->visited) {
                    int newDist = u->distFromSource + edge->weight;
                    if (newDist < v->distFromSource) {
                        v->distFromSource = newDist;
                        // Tambah/Update v di PQ
                        // Untuk sorted list, ini berarti menghapus yang lama jika ada dan menyisipkan yang baru.
                        // Lebih sederhana: cukup sisipkan. Duplikat akan ditangani oleh flag visited atau ekstraksi nanti.
                        dijkstraPQHelper.sortedInsertDijkstra(&pqHead, new DijkstraNode(v->id, newDist));
                    }
                }
                edge = edge->next;
            }
            delete minNode; // Bersihkan node yang di-dequeue dari PQ
        }

        // Cetak hasil
        cout << "Jarak vertex dari " << startVertexID << ":" << endl;
        currentV = vertexListHead;
        while (currentV) {
            cout << "  Ke " << currentV->id << " (" << currentV->name << "): ";
            if (currentV->distFromSource == 2147483647) {
                cout << "Tidak Terjangkau" << endl;
            } else {
                cout << currentV->distFromSource << endl;
            }
            currentV = currentV->nextVertex;
        }
    }
};


// --- Definisi Entitas Smart City ---
struct Citizen {
    int id;
    string name;
    string address;
    // Data relevan lainnya, contoh: ID utilitas, bisa dihubungkan via ManualLinkedList jika perlu
    Citizen(int i=0, string n="", string addr=""): id(i), name(n), address(addr){}
};

struct VehicleEntity { // Untuk membedakan dari QueueNode untuk lalu lintas
    string licensePlate;
    string type; // MOBIL, BUS, KENDARAAN_DARURAT
    string ownerCitizenName; // Hubungkan ke penduduk

    VehicleEntity(string lp = "", string t = "", string owner = "") : licensePlate(lp), type(t), ownerCitizenName(owner) {}
};


// --- Implementasi Modul Smart City ---
class CitizenManagement {
public:
    ManualLinkedList citizenList; // Menyimpan objek Citizen (konseptualnya, NodeLL akan menyimpan data penduduk)
    Trie citizenNameTrie; // Untuk pencarian/autocomplete nama cepat

    CitizenManagement() {}

    void addCitizen(int id, string name, string address) {
        if (citizenList.findNode(id)) {
            cout << "Penduduk dengan ID " << id << " sudah ada." << endl;
            return;
        }
        citizenList.insertAtTail(id, name, address); // Menggunakan field NodeLL untuk data penduduk
        citizenNameTrie.insert(name);
        cout << "Penduduk " << name << " ditambahkan." << endl;
    }

    void findCitizen(int id) {
        NodeLL* citizenNode = citizenList.findNode(id);
        if (citizenNode) {
            cout << "Penduduk Ditemukan: ID=" << citizenNode->id << ", Nama=" << citizenNode->name << ", Alamat=" << citizenNode->data1 << endl;
        } else {
            cout << "Penduduk dengan ID " << id << " tidak ditemukan." << endl;
        }
    }

    void displayAllCitizens() {
        cout << "\n--- Semua Penduduk ---" << endl;
        citizenList.displayList();
    }

    void suggestCitizenNames(string prefix) {
        cout << "\n--- Saran Nama Penduduk untuk prefix '" << prefix << "' ---" << endl;
        ManualLinkedList* suggestions = citizenNameTrie.getSuggestions(prefix);
        if (suggestions->isEmpty()) {
            cout << "Tidak ada saran ditemukan." << endl;
        } else {
            suggestions->displayList();
        }
        delete suggestions; // Bersihkan list saran
    }
};

class TrafficManagement {
public:
    ManualQueue intersectionQueueA; // Contoh: Sebuah persimpangan dengan priority queue
    // Graph* cityRoadNetwork; // Pointer ke graph utama kota

    TrafficManagement(/*Graph* roadNetwork*/) {
        // cityRoadNetwork = roadNetwork;
    }

    void vehicleArrives(string plate, string type, int priority = 1) { // prioritas default 1
        if (type == "DARURAT") { // Aturan sederhana untuk darurat
            priority = 0;
        }
        intersectionQueueA.priorityEnqueue(plate, type, priority);
        cout << "Kendaraan " << plate << " (" << type << ", Prio " << priority <<") tiba di persimpangan A." << endl;
    }

    void vehicleDeparts() {
        if (intersectionQueueA.isEmpty()) {
            cout << "Tidak ada kendaraan untuk berangkat dari persimpangan A." << endl;
            return;
        }
        QueueNode* departed = intersectionQueueA.dequeue();
        cout << "Kendaraan " << departed->licensePlate << " (" << departed->vehicleType << ") berangkat dari persimpangan A." << endl;
        delete departed;
    }

    void displayIntersectionQueue() {
        cout << "\n--- Status Antrean Persimpangan A ---" << endl;
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
            cout << "Tempat parkir Menara 1 penuh. Tidak bisa parkir " << licensePlate << "." << endl;
            return;
        }
        parkingLotTower1.push(licensePlate);
        cout << "Kendaraan " << licensePlate << " parkir di Menara 1. Sisa tempat: " << capacity - parkingLotTower1.getSize() << endl;
    }

    void unparkVehicle() {
        if (parkingLotTower1.isEmpty()) {
            cout << "Tempat parkir Menara 1 kosong. Tidak ada kendaraan untuk keluar parkir." << endl;
            return;
        }
        string unparkedPlate = parkingLotTower1.pop();
        cout << "Kendaraan " << unparkedPlate << " keluar parkir dari Menara 1. Sisa tempat: " << capacity - parkingLotTower1.getSize() << endl;
    }

    void displayParkingLotStatus() {
        cout << "\n--- Status Tempat Parkir Menara 1 (Kapasitas: " << capacity << ") ---" << endl;
        parkingLotTower1.displayStack();
        cout << "Kendaraan Terparkir: " << parkingLotTower1.getSize() << endl;
        cout << "Tempat Tersedia: " << capacity - parkingLotTower1.getSize() << endl;
    }
};


class UrbanStructure {
public:
    NaryTree cityHierarchy;

    UrbanStructure(string cityName) : cityHierarchy(cityName) {}

    void addDepartment(string parentDept, string newDept) {
        cityHierarchy.addChild(parentDept, newDept);
        cout << "Departemen '" << newDept << "' ditambahkan di bawah '" << parentDept << "'." << endl;
    }

    void displayCityHierarchy() {
        cout << "\n--- Hierarki Kota ---" << endl;
        cityHierarchy.displayTree();
    }
};

class NetworkService {
public:
    Graph* cityGraph; // Referensi ke graph utama kota

    NetworkService(Graph* graph) : cityGraph(graph) {}

    void findShortestPath(int startID, int endID) { // endID tidak digunakan dalam output Dijkstra saat ini
        if (!cityGraph) {
            cout << "Kesalahan: Graph kota belum diinisialisasi untuk Network Service." << endl;
            return;
        }
        cout << "\n--- Mencari Jalur Terpendek (Dijkstra) ---" << endl;
        cityGraph->Dijkstra(startID);
    }

    void performBFS(int startID) {
        if (!cityGraph) {
            cout << "Kesalahan: Graph kota belum diinisialisasi untuk Network Service." << endl;
            return;
        }
        cout << "\n--- Melakukan Traversal BFS ---" << endl;
        cityGraph->BFS(startID);
    }
    
    void performDFS(int startID) {
        if (!cityGraph) {
            cout << "Kesalahan: Graph kota belum diinisialisasi untuk Network Service." << endl;
            return;
        }
        cout << "\n--- Melakukan Traversal DFS ---" << endl;
        cityGraph->DFS(startID);
    }
};


// --- Kelas Utama Smart City Simulator ---
class SmartCitySimulator {
public:
    CitizenManagement citizenModule;
    TrafficManagement trafficModule;
    ParkingManagement parkingModule;
    UrbanStructure structureModule;
    NetworkService networkModule;
    Graph cityRoadNetwork; // Instansi graph yang sebenarnya

    SmartCitySimulator(string cityName) : structureModule(cityName), networkModule(&cityRoadNetwork) /*, trafficModule(&cityRoadNetwork)*/ {
        cout << "Simulator Smart City untuk '" << cityName << "' diinisialisasi." << endl;
        initializeDefaultCityData();
    }

    ~SmartCitySimulator() {
        cout << "Simulator Smart City dimatikan." << endl;
        // Destruktor dari objek member akan dipanggil secara otomatis.
        // cityRoadNetwork (instansi Graph) juga akan dibersihkan oleh destruktornya.
    }

    void initializeDefaultCityData() {
        cout << "\nMenginisialisasi data kota default..." << endl;
        // Penduduk
        citizenModule.addCitizen(101, "Adi Nugraha", "Jl. Merdeka 1");
        citizenModule.addCitizen(102, "Bunga Citra", "Jl. Sudirman 20");
        citizenModule.addCitizen(103, "Charlie Van Houten", "Jl. Diponegoro 5A");

        // Struktur Perkotaan
        structureModule.addDepartment("Kota Smartvana", "Dinas Kependudukan");
        structureModule.addDepartment("Kota Smartvana", "Dinas Perhubungan");
        structureModule.addDepartment("Dinas Perhubungan", "Seksi Lalu Lintas");
        structureModule.addDepartment("Dinas Perhubungan", "Seksi Parkir");

        // Jaringan Jalan (Vertex adalah lokasi/persimpangan)
        cityRoadNetwork.addVertex(1, "Simpang A");
        cityRoadNetwork.addVertex(2, "Simpang B");
        cityRoadNetwork.addVertex(3, "Kantor Walikota");
        cityRoadNetwork.addVertex(4, "Pasar Sentral");
        cityRoadNetwork.addVertex(5, "Rumah Sakit Umum");
        cityRoadNetwork.addVertex(6, "Area Parkir Pusat");


        // Jalan (Edge) - (dari, ke, bobot/jarak)
        cityRoadNetwork.addEdge(1, 2, 5); // A -> B, bobot 5
        cityRoadNetwork.addEdge(2, 1, 5); // B -> A (jika dua arah)
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


        cout << "Inisialisasi data default selesai." << endl;
    }


    void displayMainMenu() {
        cout << "\n===== Menu Simulator Smart City =====" << endl;
        cout << "1. Manajemen Penduduk" << endl;
        cout << "2. Manajemen Lalu Lintas" << endl;
        cout << "3. Manajemen Parkir" << endl;
        cout << "4. Struktur Perkotaan" << endl;
        cout << "5. Layanan Jaringan (Graph)" << endl;
        cout << "0. Keluar Simulator" << endl;
        cout << "===================================" << endl;
        cout << "Masukkan pilihan Anda: ";
    }

    void citizenMenu() {
        int choice;
        do {
            cout << "\n--- Menu Manajemen Penduduk ---" << endl;
            cout << "1. Tambah Penduduk" << endl;
            cout << "2. Cari Penduduk berdasarkan ID" << endl;
            cout << "3. Tampilkan Semua Penduduk" << endl;
            cout << "4. Sarankan Nama Penduduk (Trie)" << endl;
            cout << "0. Kembali ke Menu Utama" << endl;
            cout << "Masukkan pilihan: ";
            cin >> choice;
            cin.ignore(); // konsumsi newline

            int id;
            string name, address, prefix;

            switch (choice) {
                case 1:
                    cout << "Masukkan ID: "; cin >> id; cin.ignore();
                    cout << "Masukkan Nama: "; getline(cin, name);
                    cout << "Masukkan Alamat: "; getline(cin, address);
                    citizenModule.addCitizen(id, name, address);
                    break;
                case 2:
                    cout << "Masukkan ID untuk dicari: "; cin >> id; cin.ignore();
                    citizenModule.findCitizen(id);
                    break;
                case 3:
                    citizenModule.displayAllCitizens();
                    break;
                case 4:
                    cout << "Masukkan prefix nama untuk saran: "; getline(cin, prefix);
                    citizenModule.suggestCitizenNames(prefix);
                    break;
                case 0: break;
                default: cout << "Pilihan tidak valid. Coba lagi." << endl;
            }
        } while (choice != 0);
    }

    void trafficMenu() {
         int choice;
        do {
            cout << "\n--- Menu Manajemen Lalu Lintas ---" << endl;
            cout << "1. Kendaraan Tiba di Persimpangan A" << endl;
            cout << "2. Kendaraan Berangkat dari Persimpangan A" << endl;
            cout << "3. Tampilkan Antrean Persimpangan A" << endl;
            cout << "0. Kembali ke Menu Utama" << endl;
            cout << "Masukkan pilihan: ";
            cin >> choice;
            cin.ignore();

            string plate, type;
            int priority;

            switch (choice) {
                case 1:
                    cout << "Masukkan Plat Nomor: "; getline(cin, plate);
                    cout << "Masukkan Tipe Kendaraan (MOBIL, BUS, DARURAT): "; getline(cin, type);
                    // Penetapan prioritas sederhana berdasarkan tipe
                    if (type == "DARURAT") priority = 0;
                    else if (type == "BUS") priority = 1;
                    else priority = 2; // Asumsi MOBIL atau tipe lain
                    trafficModule.vehicleArrives(plate, type, priority);
                    break;
                case 2:
                    trafficModule.vehicleDeparts();
                    break;
                case 3:
                    trafficModule.displayIntersectionQueue();
                    break;
                case 0: break;
                default: cout << "Pilihan tidak valid. Coba lagi." << endl;
            }
        } while (choice != 0);
    }

    void parkingMenu() {
        int choice;
        do {
            cout << "\n--- Menu Manajemen Parkir ---" << endl;
            cout << "1. Parkir Kendaraan di Menara 1" << endl;
            cout << "2. Keluarkan Kendaraan Terakhir dari Menara 1" << endl;
            cout << "3. Tampilkan Status Menara 1" << endl;
            cout << "0. Kembali ke Menu Utama" << endl;
            cout << "Masukkan pilihan: ";
            cin >> choice;
            cin.ignore();
            string plate;
            switch (choice) {
                case 1:
                    cout << "Masukkan Plat Nomor untuk Parkir: "; getline(cin, plate);
                    parkingModule.parkVehicle(plate);
                    break;
                case 2:
                    parkingModule.unparkVehicle();
                    break;
                case 3:
                    parkingModule.displayParkingLotStatus();
                    break;
                case 0: break;
                default: cout << "Pilihan tidak valid. Coba lagi." << endl;
            }
        } while (choice != 0);
    }
    
    void structureMenu() {
        int choice;
        do {
            cout << "\n--- Menu Struktur Perkotaan ---" << endl;
            cout << "1. Tambah Departemen/Sub-unit" << endl;
            cout << "2. Tampilkan Hierarki Kota" << endl;
            cout << "0. Kembali ke Menu Utama" << endl;
            cout << "Masukkan pilihan: ";
            cin >> choice;
            cin.ignore();
            string parent, child;
            switch (choice) {
                case 1:
                    cout << "Masukkan Nama Departemen/Unit Induk: "; getline(cin, parent);
                    cout << "Masukkan Nama Departemen/Sub-unit Baru: "; getline(cin, child);
                    structureModule.addDepartment(parent, child);
                    break;
                case 2:
                    structureModule.displayCityHierarchy();
                    break;
                case 0: break;
                default: cout << "Pilihan tidak valid. Coba lagi." << endl;
            }
        } while (choice != 0);
    }

    void networkMenu() {
        int choice;
        do {
            cout << "\n--- Menu Layanan Jaringan ---" << endl;
            cout << "1. Tampilkan Jaringan Jalan (Graph)" << endl;
            cout << "2. Cari Jalur Terpendek dari Lokasi (Dijkstra)" << endl;
            cout << "3. Lakukan Traversal BFS dari Lokasi" << endl;
            cout << "4. Lakukan Traversal DFS dari Lokasi" << endl;
            cout << "0. Kembali ke Menu Utama" << endl;
            cout << "Masukkan pilihan: ";
            cin >> choice;
            cin.ignore();
            int startID;
            switch (choice) {
                case 1:
                    cityRoadNetwork.displayGraph();
                    break;
                case 2:
                    cout << "Masukkan ID Lokasi Awal untuk Dijkstra: "; cin >> startID; cin.ignore();
                    networkModule.findShortestPath(startID, -1); // endID tidak digunakan di sini
                    break;
                case 3:
                    cout << "Masukkan ID Lokasi Awal untuk BFS: "; cin >> startID; cin.ignore();
                    networkModule.performBFS(startID);
                    break;
                case 4:
                    cout << "Masukkan ID Lokasi Awal untuk DFS: "; cin >> startID; cin.ignore();
                    networkModule.performDFS(startID);
                    break;

                case 0: break;
                default: cout << "Pilihan tidak valid. Coba lagi." << endl;
            }
        } while (choice != 0);
    }


    void run() {
        int choice;
        do {
            displayMainMenu();
            cin >> choice;
            cin.ignore(); // konsumsi karakter newline setelah membaca integer

            switch (choice) {
                case 1: citizenMenu(); break;
                case 2: trafficMenu(); break;
                case 3: parkingMenu(); break;
                case 4: structureMenu(); break;
                case 5: networkMenu(); break;
                case 0: cout << "Keluar dari Simulator Smart City. Sampai jumpa!" << endl; break;
                default: cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
            }
        } while (choice != 0);
    }
};


int main() {
    // Pemeriksaan dasar untuk kegagalan new (meskipun C++ modern melempar bad_alloc)
    SmartCitySimulator* myCity = NULL;
    try {
       myCity  = new SmartCitySimulator("Kota Smartvana"); // Nama kota tetap sama
    } catch(...) { // Menangkap potensi error dari new, meskipun jarang hanya dengan iostream/string
        cout << "KESALAHAN FATAL: Tidak dapat mengalokasikan memori untuk SmartCitySimulator." << endl;
        return 1;
    }
    
    if (myCity) {
        myCity->run();
        delete myCity; // Pastikan destruktor dipanggil
    } else {
         cout << "KESALAHAN FATAL: Pointer SmartCitySimulator null setelah upaya alokasi." << endl;
        return 1;
    }

    return 0;
}
