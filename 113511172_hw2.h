#include <iostream>

using namespace std;

class Node{
private:
    int data;
    Node* stored_pointer;
public:
    Node(int val);
    Node* get_stored_pointer() { return stored_pointer; }
    void set_stored_pointer(Node* next) { stored_pointer = next; }
    int get_data() { return data; }
    void set_data(int new_data) { data = new_data; }
};

Node::Node(int val) : data(val), stored_pointer(nullptr){}

class list{
protected:
    Node* head;
public:
    list();
    ~list();
    void list_walk();
    void list_insert(Node* new_node, Node* prev_node);
    void list_delete(Node* del_node);
};

class single_list : public list{
private:
    
public:
    single_list();
    ~single_list();
    void list_walk();
    void list_insert(Node* new_node, Node* prev_node);
    void list_delete(Node* del_node);
    Node* get_headpointer() { return head; }
    void list_ins_del(int key);
    void list_reverse();
    void list_search(int key);
    void list_prepend(Node* new_node);
};

class xor_list : public list{
private:
    Node* get_pointer(int index);
public:
    xor_list();
    ~xor_list();
    void list_walk();
    void list_insert(Node* new_node, Node* prev_node);
    void list_delete(Node* del_node);
    Node* get_headpointer() { return head; }
    Node* get_next_node_pointer(Node* current, Node* prev);
    void list_ins_del(int key);
    void list_reverse();
    void list_search(int key);
    void list_prepend(Node* new_node);
};