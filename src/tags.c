int AddTag(Arena *arena, Node **first, String tag)
{
    for (Each(Node, t, *first)) {
        if (strcmp(t->name.str, tag.str) == 0) {
            return 1;
        }
    }
    Node *tag_node = ArenaPush(arena, sizeof(Node));
    tag_node->name = tag;
    tag_node->child = 0;
    SLLPush(*first, tag_node);

    return 0;
}

void RemoveTag(Node **first, String tag)
{
}

// TODO: check for duplicates
void TagFile(Arena *arena, Node **first, String tag, String file)
{
    Node *file_node = ArenaPush(arena, sizeof(Node));
    Node *iter = *first;

    file_node->name = file;
    file_node->child = 0;
    while (iter && strcmp(iter->name.str, tag.str) != 0) {
        iter = iter->next;
    }

    if (!iter) {
        AddTag(arena, first, tag);
        iter = *first;
    }

    SLLPush(iter->child, file_node);
}
