#ifndef CETECH_YAML_H
#define CETECH_YAML_H


//==============================================================================
// Includes
//==============================================================================

#include <stdbool.h>

//==============================================================================
// Defines
//==============================================================================

#define YAML_NODE_SCOPE(var_name, h, n, k, body)\
        do {\
            yaml_node_t var_name = yaml_get_node(h, n, k);\
            body \
            yaml_node_free(h, var_name);\
        } while(0)

#define YAML_NODE_SCOPE_IDX(var_name, h, n, i, body)\
        do {\
            yaml_node_t var_name = yaml_get_seq_node(h, n, i);\
            body \
            yaml_node_free(h, var_name);\
        } while(0)

//==============================================================================
// Typedefs
//==============================================================================

typedef void *yaml_handler_t;
typedef size_t yaml_node_t;

typedef void(*yaml_foreach_map_clb_t)(yaml_handler_t handler, yaml_node_t key, yaml_node_t value, void *data);

//==============================================================================
// Enums
//==============================================================================

enum yaml_node_type {
    YAML_TYPE_UNDEF,
    YAML_TYPE_NULL,
    YAML_TYPE_SCALAR,
    YAML_TYPE_SEQ,
    YAML_TYPE_MAP
};


//==============================================================================
// Interface
//==============================================================================

yaml_node_t yaml_load_str(const char *str, yaml_handler_t *handler);

yaml_node_t yaml_get_node(yaml_handler_t handler,
                          yaml_node_t node_idx,
                          const char *key);

yaml_node_t yaml_get_seq_node(yaml_handler_t handler,
                              yaml_node_t seq_node_idx,
                              size_t idx);

void yaml_node_free(yaml_handler_t handler,
                    yaml_node_t node_idx);

enum yaml_node_type yaml_node_type(yaml_handler_t handler,
                                   yaml_node_t node_idx);

size_t yaml_node_size(yaml_handler_t handler, yaml_node_t node_idx);

void
yaml_node_foreach_dict(yaml_handler_t handler, yaml_node_t node_idx, yaml_foreach_map_clb_t foreach_clb, void *data);

//==============================================================================
// Define nodes
//==============================================================================

#define YAML_NODE_AS_DEF(type) type yaml_node_as_##type(yaml_handler_t handler, yaml_node_t node_idx)

YAML_NODE_AS_DEF(int);

YAML_NODE_AS_DEF(bool);

YAML_NODE_AS_DEF(float);

int yaml_node_as_string(yaml_handler_t handler, yaml_node_t node_idx, char *output, size_t max_len);

#undef YAML_NODE_AS_DEF

#endif //CETECH_YAML_H
