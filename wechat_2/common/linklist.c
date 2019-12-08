#include "linklist.h"

int insert(LinkedList head, Node *node) {
    Node *p = head;
    while (p->next != NULL) p = p->next;
    p->next = node;
    return 0;
}
