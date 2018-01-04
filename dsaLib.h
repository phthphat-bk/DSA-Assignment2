/*
 * =========================================================================================
 * Name        : dsaLib.h
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 2 - Data structures and Algorithms - Fall 2017
 * =========================================================================================
 */

#ifndef A02_DSALIB_H
#define A02_DSALIB_H

#include <string>
#include <math.h>
#include <vector>

using namespace std;


class DSAException {
    int     _error;
    string  _text;
public:

    DSAException() : _error(0), _text("Success") {}
    DSAException(int err) : _error(err), _text("Unknown Error") {}
    DSAException(int err, const char* text) : _error(err), _text(text) {}

    int getError() { return _error; }
    string& getErrorText() { return _text; }
};

template <class T>
struct L1Item {
    T data;
    L1Item<T> *pNext;
    L1Item() : pNext(NULL) {}
    L1Item(T &a) : data(a), pNext(NULL) {}
};

template <class T>
class L1List {
    L1Item<T>   *_pHead;// The head pointer of linked list
    size_t      _size;// number of elements in this list
public:
    L1List() : _pHead(NULL), _size(0) {}
    //~L1List();

    void    clean();
    bool    isEmpty() {
        return _pHead == NULL;
    }
    size_t  getSize() {
        return _size;
    }

    T&      at(int i);
    T&      operator[](int i);

    bool    find(T& a, int& idx);
    T*      find(T& a);
    T*      findLast(T& a);
    int     insert(int i, T& a);
    int     remove(int i);
    int     removeAll(T& a);

    int     push_back(T& a);
    int     insertHead(T& a);

    int     removeHead();
    int     removeLast();

    void    reverse();

    void    traverse(void (*op)(T&)) {
        L1Item<T>   *p = _pHead;
        while (p) {
            op(p->data);
            p = p->pNext;
        }
    }
    void    traverse(void (*op)(T&, void*), void* pParam) {
        L1Item<T>   *p = _pHead;
        while (p) {
            op(p->data, pParam);
            p = p->pNext;
        }
    }
};

template<class T>
T& L1List<T>::operator[](int i){
    L1Item<T> *p = _pHead;
    if (i <= _size && i >= 0) {
        for (int index = 0; index < i; index++) {
            p = p->pNext;
        }
        return p->data;
    }
}
/// Insert item to the end of the list
/// Return 0 if success
template <class T>
int L1List<T>::push_back(T &a) {
    if (_pHead == NULL) {
        _pHead = new L1Item<T>(a);
    }
    else {
        L1Item<T>   *p = _pHead;
        while (p->pNext) p = p->pNext;
        p->pNext = new L1Item<T>(a);
    }

    _size++;
    return 0;
}

/// Insert item to the front of the list
/// Return 0 if success
template <class T>
int L1List<T>::insertHead(T &a) {
    L1Item<T>   *p = new L1Item<T>(a);
    p->pNext = _pHead;
    _pHead = p;
    _size++;
    return 0;
}

/// Remove the first item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeHead() {
    if(_pHead) {
        L1Item<T>* p = _pHead;
        _pHead = p->pNext;
        delete p;
        _size--;
        return 0;
    }
    return -1;
}

/// Remove the last item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeLast() {
    if(_pHead) {
        if(_pHead->pNext) {
            L1Item<T>* prev = _pHead;
            L1Item<T>* pcur = prev->pNext;
            while(pcur->pNext) {
                prev = pcur;
                pcur = pcur->pNext;
            }
            delete pcur;
            prev->pNext = NULL;
        }
        else {
            delete _pHead;
            _pHead = NULL;
        }
        _size--;
        return 0;
    }
    return -1;
}

/************************************************************************
 * This section is for AVL tree
 ************************************************************************/
int max(int a, int b){
    return a>b?a:b;
}
enum banlance{ L = -1, E = 0, R = 1};
template <class T>
struct AVLNode {
    T           _data;
    AVLNode<T>   *_left, *_right;
// #ifdef AVL_USE_HEIGHT
    int         _height;
//     AVLNode(T &a) : _data(a), _left(NULL), _right(NULL), _height(1) {}
// #else
    int         _bFactor;
    AVLNode(T &a) : _data(a), _left(NULL), _right(NULL), _bFactor(0), _height(1) {}
};

template <class T>
class AVLTree {
    AVLNode<T> *_root;
public:
    AVLTree() : _root(NULL) {}
    ~AVLTree() { destroy(_root); }

    bool find(T& key, T* &ret) { return find(_root, key, ret); }
    bool insert(T& key) { return insert(_root, key); }
    bool remove(T& key) { return remove(_root, key); }
    void traverseNLR(void (*op)(T&)) { traverseNLR(_root, op); }
    void traverseLNR(void (*op)(T&)) { traverseLNR(_root, op); }
    void traverseLRN(void (*op)(T&)) { traverseLRN(_root, op); }

protected:
    AVLNode<T> *findMin(AVLNode<T> *pR);
    void destroy(AVLNode<T>* &pR);
    bool find(AVLNode<T> *pR, T& key, T* &ret);
    bool insert(AVLNode<T>* &pR, T& a);
    bool remove(AVLNode<T>* &pR, T& a);
    void traverseNLR(AVLNode<T> *pR, void (*op)(T&));
    void traverseLNR(AVLNode<T> *pR, void (*op)(T&));
    void traverseLRN(AVLNode<T> *pR, void (*op)(T&));

    int getBalance(AVLNode<T> *pR);
    void rotLeft(AVLNode<T>* &pR);
    void rotRight(AVLNode<T>* &pR);
    void rotLR(AVLNode<T>* &pR);
    void rotRL(AVLNode<T>* &pR);

    bool balanceLeft(AVLNode<T>* &pR);
    bool balanceRight(AVLNode<T>* &pR);
};
template<class T>
int AVLTree<T>::getBalance(AVLNode<T> *pR){
    return pR->_left->_height - pR->_right->_height;
}
template<class T>
void AVLTree<T>::rotLeft(AVLNode<T> *&pR){
    if(!pR) return;
    AVLNode<T> *temp = pR;
    pR = pR->_right;
    temp->_right = pR->_left;
    pR->_left = temp;

    temp->_height = 1+max(temp->_left->_height, temp->_right->_height);    
    pR->_height = 1+max(pR->_left->_height, pR->_right->_height);
}
template<class T>
void AVLTree<T>::rotRight(AVLNode<T> *&pR){
    if(!pR) return;
    AVLNode<T> *temp = pR;
    pR = pR->_left;
    temp->_left = pR->_right;
    pR->_right = temp;

    temp->_height = 1+max(temp->_left->_height, temp->_right->_height);
    pR->_height = 1+max(pR->_left->_height, pR->_right->_height);
}
template<class T>
bool AVLTree<T>::insert(AVLNode<T>* &pR, T& a){
    if (!pR) pR = new AVLNode<T>(a);
    if(a < pR->_data)
        insert(pR->_left, a);
    else if (a > pR->_data) {
        insert(pR->_right, a);
    }
    else return true;
    pR->_height = 1+max(pR->_left->_height, pR->_right->_height);
    int balance = getBalance(pR);
    if(balance > 1 && a < pR->_left->_data){ //L-L
        rotRight(pR);
    }
    if(balance < -1 && a > pR->_right->_data){ //R-R
        rotLeft(pR);
    }
    if(balance > 1 && a > pR->_left->data){ //L-R
        rotLeft(pR->_left);
        rotRight(pR);
    }
    if(balance < -1 && a < pR->_right->_data){ //R-L
        rotRight(pR->_right);
        rotLeft(pR);
    }
    return true;
}
template<class T>
bool AVLTree<T>::remove(AVLNode<T>* &pR, T& a){
    if(!pR) return false;
    if(a < pR->_data){
        remove(pR->_left, a);
    }
    else if (a > pR->_data) {
        remove(pR->_right, a);
    }
    else {
        if(!pR->_left || !pR->_right){
            AVLNode<T> *temp = pR->_left ? pR->_left : pR->_right;
            if(!temp){
                temp = pR;
                pR = NULL;
            }
            else
                *pR = *temp;
            delete temp;
        }
        else {
            //2 children
            AVLNode<T> *temp = findMin(pR->_right);
            pR->_data = temp->_data;
            remove(pR, temp->_data);
        }
    }
    //if(!pR) return false;
    pR->_height = 1+max(pR->_left->_height - pR->_right->_height);
    int balance = getBalance(pR);
    if(balance > 1 && getBalance(pR->_left) >= 0){
        rotRight(pR);
        return true;
    }
    if(balance < -1 && getBalance(pR->_right) <= 0){
        rotLeft(pR);
        return true;
    }
    if(balance > 1 && getBalance(pR->_left) < 0){
        rotLeft(pR->_left);
        rotRight(pR);
        return true;
    }
    if(balance < -1 && getBalance(pR->_right)>0){
        rotRight(pR->_right);
        rotLeft(pR);
        return true;
    }
}
#endif //A02_DSALIB_H
