#define Each(T,c,f) T *(c) = (f); (c) != 0; (c)=(c)->next
#define SLLPush(f,n) ((f)==0? \
        ((f)=(n), (n)->next = 0): \
        ((n)->next = (f), (f)=(n)))

typedef struct Node Node;
struct Node {
    String name;

    Node *next;
    Node *child;
};

Node *NodesFromJson(Arena *arena, struct json_value_s *json)
{
    Node *first = 0;

    struct json_object_s *root = json_value_as_object(json);
    // if not?
    for (struct json_object_element_s *elem = root->start; elem; elem = elem->next) {
        Node *node = ArenaPush(arena, sizeof(Node));
        SLLPush(first, node);
        node->name = StringC(arena, elem->name->string);

        struct json_array_s *tag = json_value_as_array(elem->value);
        // if not?
        for (struct json_array_element_s *member = tag->start; member; member = member->next) {
            Node *path = ArenaPush(arena, sizeof(Node));
            SLLPush(node->child, path);
            struct json_string_s *str = json_value_as_string(member->value);
            // if not?
            path->name = StringC(arena, str->string);
        }
    }

    return first;
}
