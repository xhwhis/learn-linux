#include "linklist.h"

int insert(LinkedList head, Node *node) {
    Node *p = head;
    p->len++;
    while (p->next != NULL) p = p->next;
    p->next = node;
    node->next = NULL;
    return 0;
}

int delete(LinkedList head, Node *node) {
    Node *p = head;
    Node *q = head;
    p->len--;
    while (p->next != NULL && p->next != node) {
        q = p;
        p = p->next;
    }
    q->next = p->next;
    free(p);
    return 0;
}
