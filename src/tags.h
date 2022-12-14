#ifndef TAGS_H
#define TAGS_H

#include "node.h"

int AddTag(NodePool *pool, Node **first, String tag);
void RemoveTag(Node **first, String tag);
void TagFile(NodePool *pool, Node **first, String tag, String file);
Node *QueryTag(Node *first, String tag);
Node *QueryFile(NodePool *pool, Node *first, String file);

#endif
