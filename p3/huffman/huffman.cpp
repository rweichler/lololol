#include <stdio.h>
#include <stdlib.h>
#include "BinaryTree.h"

typedef struct char_node {
    char character;
    int count;
    struct char_node *next;
    BinaryTree<char> *tree;
} char_node;

char *readFile(char *filename)
{
    size_t size;
    FILE *f = fopen(filename, "r");
    if(!f) return NULL;
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    rewind(f);

    char *contents = (char *)malloc((size + 1)*sizeof(char));
    fread(contents, sizeof(char), size, f);
    fclose(f);
    contents[size] = '\0'; //just in case

    return contents;
}

char_node *new_node(char c, char_node *next)
{
    char_node *node = (char_node *)malloc(sizeof(char_node));
    node->character = c;
    node->next = next;
    node->count = 1;
    node->tree = NULL;
    return node;
}

char_node *make_nodes(char *contents)
{
    char_node *root = NULL;

    int i;
    for(i = 0; contents[i] != '\0'; i++)
    {
        char c = contents[i];
        //printf("Inserting %c\n", c);
        if(!root)
        {
            root = new_node(c, NULL);
        }
        else
        {
            char_node *node, *prev = NULL;
            for(node = root; node != NULL; node = node->next)
            {
                if(node->character == c)
                {
                    node->count = node->count + 1;
                    while(node->next && node->next->count < node->count)
                    {
                        char_node *second = node->next;
                        char_node *third = node;

                        third->next = second->next;
                        second->next = third;
                        if(prev)
                            prev->next = second;
                        else
                            root = second;

                        prev = second;
                    }
                    break;
                }
                prev = node;
            }
            if(node == NULL) //didnt find anything
            {
                root = new_node(c, root); //put it at front
            }
        }
    }

    return root;
}

void print_node(char_node *node)
{
    printf("NODES: \n    ");
    for(; node != NULL; node = node->next)
    {
        if(node->tree)
            printf("TREE %d <%d>", node->count, (int)(node->tree));
        else if(node->character == '\n')
            printf("\\n %d", node->count);
        else
            printf("%c %d", node->character, node->count);

        printf("\n    ");
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("Usage: %s file\n", argv[0]);
        return 1;
    }

    char *contents = readFile(argv[1]);
    if(!contents)
    {
        printf("File '%s' doesn't exist\n", argv[1]);
        return 1;
    }

    char_node *root = make_nodes(contents);
    //print_node(root);

    while(root->next != NULL)
    {
        BinaryTree<char> *left;
        BinaryTree<char> *right;


        if(root->tree)
            left = root->tree;
        else
        {
            left = new BinaryTree<char>(root->character, NULL, NULL, root->count);
        }

        if(root->next->tree)
            right = root->next->tree;
        else
        {
            right = new BinaryTree<char>(root->next->character, NULL, NULL, root->next->count);
        }

        BinaryTree<char> *tree = new BinaryTree<char>('\0', left, right, 0);
        char_node *newnode = new_node('\0', root->next->next);
        newnode->tree = tree;
        newnode->count = root->count + root->next->count;


        root = newnode;

        char_node *node = root;
        char_node *prev = NULL;
        while(node->next && (node->next->count < node->count || node->next->count == node->count && node->tree && node->next->tree == NULL))
        {
            char_node *second = node->next;
            char_node *third = node;

            third->next = second->next;
            second->next = third;
            if(prev)
                prev->next = second;
            else
                root = second;

            prev = second;
        }
    }

    char encoding[2000];
    root->tree->printTree(encoding, 0);

    return 0;
}
