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

struct json_value_s *JsonFromNodes(Arena *arena, Node *node)
{
    struct json_value_s *result = ArenaPush(arena, sizeof(struct json_value_s));
    struct json_object_s *root = ArenaPush(arena, sizeof(struct json_object_s));

    result->payload = root;
    result->type = json_type_object;

    root->start = 0;
    for (Each(Node, tag, node)) {
        struct json_object_element_s *elem = ArenaPush(arena, sizeof(struct json_object_element_s));
        elem->name = ArenaPush(arena, sizeof(struct json_string_s));
        elem->name->string = tag->name.str;
        elem->name->string_size = tag->name.length;
        elem->value = ArenaPush(arena, sizeof(struct json_value_s));
        elem->value->payload = ArenaPush(arena, sizeof(struct json_array_s));
        elem->value->type = json_type_array;

        root->length += 1;
        SLLPush(root->start, elem);
        struct json_array_s *array = json_value_as_array(elem->value);
        array->start = 0;
        for (Each(Node, file, tag->child)) {
            struct json_array_element_s *member = ArenaPush(arena, sizeof(struct json_array_element_s));
            member->value = ArenaPush(arena, sizeof(struct json_value_s));
            member->value->payload = ArenaPush(arena, sizeof(struct json_string_s));
            member->value->type = json_type_string;
            struct json_string_s *path = json_value_as_string(member->value);
            path->string = file->name.str;
            path->string_size = file->name.length;

            SLLPush(array->start, member);
            array->length += 1; 
        }
    }

    return result;
}
