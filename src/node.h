#ifndef NODE_H
#define NODE_H

#include "string.h"
#include "arena.h"

#define Each(T,c,f) T *(c) = (f); (c) != 0; (c)=(c)->next
#define SLLPush(f,n) ((f)==0? \
        ((f)=(n), (n)->next = 0): \
        ((n)->next = (f), (f)=(n)))
#define SLLPop(f,n) ((f)==0? \
        ((n)=0): \
        ((n)=(f),(f)=(f)->next))

#define DLLPushBack(f,l,n) ((f)==0? \
        ((f)=(l)=(n), (n)->next = 0): \
        ((n)->next = (f), (f)=(n)))

typedef struct Node Node;
struct Node {
    String name;

    Node *next;
    Node *child;
};

typedef struct NodePool NodePool;
struct NodePool {
    Arena *arena;

    Node *first;
};

Node *NodeNew(NodePool *pool);
void NodeFree(NodePool *pool, Node *node);
Node *NodesFromJson(NodePool *pool, struct json_value_s *json);
struct json_value_s *JsonFromNodes(Arena *arena, Node *node);

#endif
