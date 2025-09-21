#include "113511172_hw2.h"
#include <ctime>
#include <cstdint>
#include <random>
#include <chrono>

ios_base::sync_with_stdio(false);
cin.tie(NULL);

const int test_count = 200000;
const int max_int = 1000000;
bool used_int_xlist[max_int] = {false};
bool used_int_slist[max_int] = {false};
void slist_test( single_list& test_s_list );
void xlist_test( xor_list& test_x_list );

inline Node* XOR(Node* a, Node* b) {
    return (Node*)((uintptr_t)(a) ^ (uintptr_t)(b));
}
int main() {
    auto start = chrono::high_resolution_clock::now();
    single_list s_list;
    xor_list x_list;
    slist_test( s_list );
    auto s_list_end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_s_list = s_list_end - start;

    xlist_test( x_list );
    auto x_list_end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_x_list = x_list_end - start;

    cout << "Elapsed time for singly linked list: " << elapsed_s_list.count() << " seconds" << endl;
    cout << "Elapsed time for XOR linked list: " << elapsed_x_list.count() << " seconds" << endl;
    cout<< "Total elapsed time: " << (elapsed_s_list + elapsed_x_list).count() << " seconds" << endl;
    return 0;
}

void slist_test( single_list& test_s_list ){
    srand( time(NULL) );
    int added_int = rand() % max_int ;
    used_int_slist[ added_int ] = true;
    Node* head_node = new Node( added_int + 1 );
    test_s_list. list_insert(head_node, nullptr);

    Node* prev_node = head_node;
    for (int i = 0; i < test_count; i++) {
        int added_int = rand() % max_int ;

        if ( !used_int_slist[ added_int ]){
        used_int_slist[ added_int ] = true;
        Node* new_node = new Node( added_int + 1 );
        test_s_list.list_insert(new_node, prev_node);
        prev_node = new_node;
        }
        else i--;
    }

    cout<< "Insertion done." << endl;

    for(int i = 0; i < test_count; i++){
        int key = rand() % max_int + 1;
        test_s_list.list_ins_del(key);
    }
    cout<< "Insertion and Deletion done." << endl;

    test_s_list.list_walk();
    cout<< "Walk done." << endl;
    test_s_list.list_reverse();
    cout<< "Reverse done." << endl;
    test_s_list.list_walk();
    cout<< "Second Walk done." << endl;
}

void xlist_test(xor_list& test_x_list) {
    srand(time(NULL) + 1);
    int added_int = rand() % max_int;
    used_int_xlist[added_int] = true;

    Node* head_node = new Node(added_int + 1);
    test_x_list.list_insert(head_node, nullptr);

    Node* current_prev = head_node;
    for (int i = 0; i < test_count; i++) {
        int added_int = rand() % max_int;
        if (!used_int_xlist[added_int]) {
            used_int_xlist[added_int] = true;
            Node* new_node = new Node(added_int + 1);
            test_x_list.list_insert(new_node, current_prev);
            current_prev = new_node;
        } else {
            i--;
        }
    }
    cout << "XOR Insertion done." << endl;
    for (int i = 0; i < test_count; i++) {
        int key = rand() % max_int + 1;
        test_x_list.list_ins_del(key);
    }
    cout << "XOR Insertion and Deletion done." << endl;
    test_x_list.list_walk();
    cout << "XOR Walk done." << endl;
    test_x_list.list_reverse();
    cout << "XOR Reverse done." << endl;
    test_x_list.list_walk();
    cout << "XOR Second Walk done." << endl;
}

void single_list:: list_insert(Node* new_node, Node* prev_node){
    if (prev_node == nullptr) {
        new_node->set_stored_pointer(head);
        head = new_node;
    } else {
        new_node->set_stored_pointer(prev_node->get_stored_pointer());
        prev_node->set_stored_pointer(new_node);
    }
    //cout<< "Inserted " << new_node->get_data() << endl;
}

void xor_list::list_insert(Node* new_node, Node* prev_node){
    if (prev_node == nullptr) {
        new_node->set_stored_pointer(head);

        if (head != nullptr) {
            head->set_stored_pointer(XOR(head->get_stored_pointer(), new_node));
        }
        head = new_node;

    } else {
        Node* curr_walk = head;
        Node* prev_walk = nullptr; 
        Node* prev_prev_node = nullptr;

        if (curr_walk == prev_node) { 
            prev_prev_node = nullptr;
        } else {
            while (curr_walk != nullptr && curr_walk != prev_node) {
                Node* next_walk = XOR(prev_walk, curr_walk->get_stored_pointer());
                prev_walk = curr_walk;
                curr_walk = next_walk;
            }
            if (curr_walk == prev_node) { 
                prev_prev_node = prev_walk; 
            } else {
                // This scenario means prev_node was not found in the list.
                // This is an error condition for list_insert. For now, we'll
                // assume prev_node is always valid.
                // A robust implementation might throw an exception or return an error.
                return;
            }
        }

        Node* next_node_of_prev = XOR(prev_prev_node, prev_node->get_stored_pointer());

        new_node->set_stored_pointer(XOR(prev_node, next_node_of_prev));

        prev_node->set_stored_pointer(XOR(prev_node->get_stored_pointer(), next_node_of_prev)); // Remove old next
        prev_node->set_stored_pointer(XOR(prev_node->get_stored_pointer(), new_node));         // Add new node

        if (next_node_of_prev != nullptr) {
            next_node_of_prev->set_stored_pointer(XOR(next_node_of_prev->get_stored_pointer(), prev_node)); // Remove old prev
            next_node_of_prev->set_stored_pointer(XOR(next_node_of_prev->get_stored_pointer(), new_node)); // Add new node
        }
    }
    //cout<< "Inserted " << new_node->get_data() << endl;
}

void single_list:: list_delete(Node* del_node)
{
    int data = del_node->get_data();
     used_int_slist[data - 1] = false;
    if (del_node == head) {
        head = del_node->get_stored_pointer();
        delete del_node;
    } else {
        Node* current = head;
        while (current != nullptr && current->get_stored_pointer() != del_node) {
            current = current->get_stored_pointer();
        }

        if (current != nullptr) {
            current->set_stored_pointer(del_node->get_stored_pointer());
            delete del_node;
        }
    }
    //cout<< "Deleted " << data << endl;
}

void xor_list::list_delete(Node* del_node) {
    if (del_node == nullptr) {
        return; // Safety check
    }

    int data = del_node->get_data();
    used_int_xlist[data - 1] = false;

    // Traverse to find the node *before* del_node.
    // This is necessary to update the links of both neighbors.
    Node* current = head;
    Node* prev = nullptr;
    while (current != nullptr && current != del_node) {
        Node* next = XOR(prev, current->get_stored_pointer());
        prev = current;
        current = next;
    }

    // If current is null, the node wasn't in the list (should not happen if logic is correct).
    if (current == nullptr) return;

    // We have found the node to delete (`current` or `del_node`) and the node before it (`prev`).
    
    // 1. Find the node *after* del_node.
    Node* next_node = XOR(prev, del_node->get_stored_pointer());

    // 2. Update the link of the previous node (if it exists).
    if (prev != nullptr) {
        // Find the node before prev.
        Node* prev_prev_node = XOR(prev->get_stored_pointer(), del_node);
        // Link prev to next_node.
        prev->set_stored_pointer(XOR(prev_prev_node, next_node));
    } else {
        // We are deleting the head, so the new head is the next node.
        head = next_node;
    }

    // 3. *** THE FIX: Update the link of the next node (if it exists). ***
    if (next_node != nullptr) {
        // Find the node after next_node.
        Node* next_next_node = XOR(next_node->get_stored_pointer(), del_node);
        // Link next_node back to prev.
        next_node->set_stored_pointer(XOR(prev, next_next_node));
    }

    //cout<< "Deleted " << data << endl;
    delete del_node;
}

// Your list_ins_del function is logically fine now, as it correctly calls the fixed list_delete.
// It is still inefficient because of the double traversal, but it will be CORRECT.

Node* xor_list::get_next_node_pointer(Node* current, Node* prev) {
    if(current == this->head) return current->get_stored_pointer();
    else return (Node*)((uintptr_t)(current->get_stored_pointer()) ^ (uintptr_t)prev);
}

void single_list:: list_walk()
{
    //cout << "List: ";
    Node* current = head;
    while (current != nullptr) {
        //cout << current->get_data() ;
        current = current->get_stored_pointer();
        if(current == nullptr) break;
        else {}//cout << ", ";
    }
    //cout << endl;
}

void xor_list::list_walk()
{
    //cout << "List: ";
    Node* current = head;
    Node* prev = nullptr;
    Node* next;

    int count = 0;
    while (current != nullptr) {
        //cout << current->get_data() ;
        count++;

        next = XOR(prev, current->get_stored_pointer());
        prev = current;
        current = next;
        if(current == nullptr) break;
        else{} 
            //cout << ", ";
    }
    //cout << endl;
}


void xor_list::list_prepend(Node* new_node) {
    new_node->set_stored_pointer(this->head);
    this->head->set_stored_pointer(XOR(this->head->get_stored_pointer(), new_node));
    this->head = new_node;
}

void single_list::list_prepend(Node* new_node) {
    new_node->set_stored_pointer(this->head);
    this->head = new_node;
}

void single_list:: list_ins_del(int key){
    if(used_int_slist[key - 1]){
        Node* current = head;
        Node* prev = nullptr;

        while (current != nullptr && current->get_data() != key) {
            prev = current;
            current = current->get_stored_pointer();
        }

        if (current != nullptr) {
            list_delete(current);
        }
    }
    else{
        Node* new_node = new Node(key);
        used_int_slist[key - 1] = true;
        list_prepend(new_node);
    }
}

void xor_list::list_ins_del(int key) {
    if(used_int_xlist[key - 1]) {
        Node* current = head;
        Node* prev = nullptr;

        while (current != nullptr && current->get_data() != key) {
            Node* next = XOR(prev, current->get_stored_pointer());
            prev = current;
            current = next;
        }

        if (current != nullptr) {
            list_delete(current); // This will now work correctly.
        }

    } else {
        Node* new_node = new Node(key);
        used_int_xlist[key - 1] = true;
        list_prepend(new_node);
    }
}

void single_list:: list_reverse(){
    Node* prev = nullptr;
    Node* current = head;
    Node* next = nullptr;

    while (current != nullptr) {
        next = current->get_stored_pointer();
        current->set_stored_pointer(prev);
        prev = current;
        current = next;
    }
    head = prev;
}

void xor_list:: list_reverse(){
    Node* prev = nullptr;
    Node* current = head;
    Node* next = nullptr;

    while (current != nullptr) {
        next = (Node*)((uintptr_t)(current->get_stored_pointer()) ^ (uintptr_t)prev);
        prev = current;
        current = next;
    }
    head = prev;
}

list::list(){
    head = nullptr;
}

list::~list(){
    // Base class destructor does nothing
}

single_list::single_list(){
    head = nullptr;
}

single_list::~single_list(){
    Node* current = head;
    while (current != nullptr)
    {
        Node* next = current->get_stored_pointer();
        delete current;
        current = next;
    }
}

xor_list::xor_list(){
    head = nullptr;
}

xor_list::~xor_list() {
    if (!head) return;
    Node* prev = nullptr;
    Node* current = head;
    while (current != nullptr) {
        Node* next = (Node*)((uintptr_t)(current->get_stored_pointer()) ^ (uintptr_t)prev);
        prev = current;
        delete current;
        current = next;
    }
}

void xor_list::list_search(int key){
    if (used_int_xlist[key - 1]) {
        //cout << "Found " << key << endl;
        return;
    } else {
        //cout << key << " not found" << endl;
        return;
    }
}

void single_list::list_search(int key){
    if (used_int_slist[key - 1]) {
        //cout << "Found " << key << endl;
        return;
    } else {
        //cout << key << " not found" << endl;
        return;
    }
}