#include "node.h"

Node *NodeNew(NodePool *pool)
{
    Node *result;
    SLLPop(pool->first, result);
    
    if (!result) {
        for (int i = 0; i < 64; ++i) {
            Node *node = ArenaPush(pool->arena, sizeof(Node));
            SLLPush(pool->first, node);
        }
        SLLPop(pool->first, result);
    }
    *result = (Node) {0};

    return result;
}

void NodeFree(NodePool *pool, Node *node)
{
    SLLPush(pool->first, node);
}

Node *NodesFromJson(NodePool *pool, struct json_value_s *json)
{
    Node *first = 0;

    struct json_object_s *root = json_value_as_object(json);
    // if not?
    for (struct json_object_element_s *elem = root->start; elem; elem = elem->next) {
        Node *node = NodeNew(pool);
        SLLPush(first, node);
        node->name = StringC(pool->arena, elem->name->string);

        struct json_array_s *tag = json_value_as_array(elem->value);
        // if not?
        for (struct json_array_element_s *member = tag->start; member; member = member->next) {
            Node *path = NodeNew(pool);
            SLLPush(node->child, path);
            struct json_string_s *str = json_value_as_string(member->value);
            // if not?
            path->name = StringC(pool->arena, str->string);
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
