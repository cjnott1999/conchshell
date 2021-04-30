#ifndef __AST_H__
#define __AST_H__

typedef enum node_type {
    NODE_INVALID,
    NODE_COMMAND,
    NODE_FILE_OUTPUT,
    NODE_FILE_INPUT,
    NODE_BANG,
    NODE_LASTs
} NodeType;

typedef struct t_ast {
    NodeType type;
    char * value;
    struct t_ast *left;
    struct t_ast *right;
    
} AST;

#endif // __AST_H__
