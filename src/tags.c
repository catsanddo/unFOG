int AddTag(NodePool *pool, Node **first, String tag)
{
    for (Each(Node, t, *first)) {
        if (strcmp(t->name.str, tag.str) == 0) {
            return 1;
        }
    }
    Node *tag_node = NodeNew(pool);
    tag_node->name = tag;
    tag_node->child = 0;
    SLLPush(*first, tag_node);

    return 0;
}

void RemoveTag(Node **first, String tag)
{
}

// TODO: check for duplicates
void TagFile(NodePool *pool, Node **first, String tag, String file)
{
    Node *file_node = NodeNew(pool);

    Node *iter = *first;

    file_node->name = file;
    file_node->child = 0;
    while (iter && strcmp(iter->name.str, tag.str) != 0) {
        iter = iter->next;
    }

    if (!iter) {
        AddTag(pool, first, tag);
        iter = *first;
    }

    SLLPush(iter->child, file_node);
}

Node *QueryTag(Node *first, String tag)
{
    for (Each(Node, tag_node, first)) {
        if (strcmp(tag.str, tag_node->name.str) == 0) {
            return tag_node;
        }
    }

    return NULL;
}
