// 2022MCB1311
// Prem Kumar
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
void insert(BTree *tree, int key);
bool search(btNode *node, int key);
void inOrderTraversal(btNode *node);
void deleteKey(btNode *node, int key);
int findSuccessor(btNode *node, int index);
void deleteFromNonLeaf(btNode *node, int index);
void deleteFromLeaf(btNode *node, int index);
void borrowFromPrevious(btNode *node, int index);
void borrowFromNext(btNode *node, int index);
void mergeNodes(btNode *node, int index);
void deleteKey(btNode *node, int key);
void fillChild(btNode *node, int index);
void bTreeDelete(BTree *tree, int key);
int findMinimum(btNode *node);
int t;
typedef struct btNode
{
    int numKeys;
    int isLeaf;
    int *keys;
    struct btNode **children;
} btNode;

typedef struct BTree
{
    btNode *root;
} BTree;

void createBTree(BTree *tree)
{
    btNode *rootNode = (btNode *)malloc(sizeof(btNode));
    rootNode->keys = (int *)malloc((2 * t - 1) * sizeof(int));
    rootNode->children = (btNode **)malloc((2 * t) * sizeof(btNode *));
    rootNode->numKeys = 0;
    rootNode->isLeaf = 1;
    tree->root = rootNode;
}
void splitChild(btNode *parent, int index);
void insertNonFull(btNode *node, int key);
void splitChild(btNode *parent, int index)
{
    btNode *newChild = (btNode *)malloc(sizeof(btNode));
    newChild->keys = (int *)malloc((2 * t - 1) * sizeof(int));
    newChild->children = (btNode **)malloc((2 * t) * sizeof(btNode *));
    btNode *oldChild = parent->children[index];

    newChild->isLeaf = oldChild->isLeaf;
    newChild->numKeys = t - 1;

    for (int i = 0; i < t - 1; i++)
    {
        newChild->keys[i] = oldChild->keys[i + t];
    }

    if (!oldChild->isLeaf)
    {
        for (int i = 0; i < t; i++)
        {
            newChild->children[i] = oldChild->children[i + t];
        }
    }

    oldChild->numKeys = t - 1;

    for (int i = parent->numKeys; i > index; i--)
    {
        parent->children[i + 1] = parent->children[i];
    }

    parent->children[index + 1] = newChild;

    for (int i = parent->numKeys - 1; i >= index; i--)
    {
        parent->keys[i + 1] = parent->keys[i];
    }

    parent->keys[index] = oldChild->keys[t - 1];
    parent->numKeys++;
}
void insertNonFull(btNode *node, int key)
{
    int i = node->numKeys - 1;

    if (node->isLeaf)
    {
        while (i >= 0 && key < node->keys[i])
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->numKeys++;
    }
    else
    {
        while (i >= 0 && key < node->keys[i])
        {
            i--;
        }
        i++;

        if (node->children[i]->numKeys == (2 * t - 1))
        {
            splitChild(node, i);
            if (key > node->keys[i])
            {
                i++;
            }
        }

        insertNonFull(node->children[i], key);
    }
}
void insert(BTree *tree, int key)
{
    btNode *root = tree->root;

    if (root->numKeys == (2 * t - 1))
    {
        btNode *newRoot = (btNode *)malloc(sizeof(btNode));
        newRoot->keys = (int *)malloc((2 * t - 1) * sizeof(int));
        newRoot->children = (btNode **)malloc((2 * t) * sizeof(btNode *));
        newRoot->isLeaf = 0;
        newRoot->numKeys = 0;
        newRoot->children[0] = root;
        tree->root = newRoot;
        splitChild(newRoot, 0);
        insertNonFull(newRoot, key);
    }
    else
    {
        insertNonFull(root, key);
    }
}

// Function to search for an element in the B-tree
bool search(btNode *node, int key)
{
    int i = 0;
    while (i < node->numKeys && key > node->keys[i])
    {
        i++;
    }

    if (i < node->numKeys && key == node->keys[i])
    {
        return true;
    }

    if (node->isLeaf)
    {
        return false;
    }

    return search(node->children[i], key);
}

// Function to perform an in-order traversal of the B-tree
void inOrderTraversal(btNode *node)
{
    int i;
    for (i = 0; i < node->numKeys; i++)
    {
        if (!node->isLeaf)
        {
            inOrderTraversal(node->children[i]);
        }
        printf("%d ", node->keys[i]);
    }

    if (!node->isLeaf)
    {
        inOrderTraversal(node->children[i]);
    }
}
int findPredecessor(btNode *node, int index)
{
    btNode *current = node->children[index];
    while (!current->isLeaf)
    {
        current = current->children[current->numKeys];
    }
    return current->keys[current->numKeys - 1];
}
int findSuccessor(btNode *node, int index)
{
    btNode *current = node->children[index + 1];
    while (!current->isLeaf)
    {
        current = current->children[0];
    }
    return current->keys[0];
}
void deleteFromNonLeaf(btNode *node, int index)
{
    int key = node->keys[index];

    if (node->children[index]->numKeys >= t)
    {
        int predecessor = findPredecessor(node, index);
        node->keys[index] = predecessor;
        deleteKey(node->children[index], predecessor);
    }
    else if (node->children[index + 1]->numKeys >= t)
    {
        int successor = findSuccessor(node, index);
        node->keys[index] = successor;
        deleteKey(node->children[index + 1], successor);
    }
    else
    {
        mergeNodes(node, index);
        deleteKey(node->children[index], key);
    }
}
void deleteFromLeaf(btNode *node, int index)
{
    for (int i = index + 1; i < node->numKeys; i++)
    {
        node->keys[i - 1] = node->keys[i];
    }
    node->numKeys--;
}
void borrowFromPrevious(btNode *node, int index)
{
    btNode *child = node->children[index];
    btNode *sibling = node->children[index - 1];

    for (int i = child->numKeys - 1; i >= 0; i--)
    {
        child->keys[i + 1] = child->keys[i];
    }

    if (!child->isLeaf)
    {
        for (int i = child->numKeys; i >= 0; i--)
        {
            child->children[i + 1] = child->children[i];
        }
    }

    child->keys[0] = node->keys[index - 1];

    if (!child->isLeaf)
    {
        child->children[0] = sibling->children[sibling->numKeys];
    }

    node->keys[index - 1] = sibling->keys[sibling->numKeys - 1];
    child->numKeys++;
    sibling->numKeys--;
}
void borrowFromNext(btNode *node, int index)
{
    btNode *child = node->children[index];
    btNode *sibling = node->children[index + 1];

    child->keys[child->numKeys] = node->keys[index];

    if (!child->isLeaf)
    {
        child->children[child->numKeys + 1] = sibling->children[0];
    }

    node->keys[index] = sibling->keys[0];

    for (int i = 1; i < sibling->numKeys; i++)
    {
        sibling->keys[i - 1] = sibling->keys[i];
    }

    if (!sibling->isLeaf)
    {
        for (int i = 1; i <= sibling->numKeys; i++)
        {
            sibling->children[i - 1] = sibling->children[i];
        }
    }

    child->numKeys++;
    sibling->numKeys--;
}
void mergeNodes(btNode *node, int index)
{
    btNode *child = node->children[index];
    btNode *sibling = node->children[index + 1];

    child->keys[t - 1] = node->keys[index];

    for (int i = 0; i < sibling->numKeys; i++)
    {
        child->keys[i + t] = sibling->keys[i];
    }

    if (!child->isLeaf)
    {
        for (int i = 0; i <= sibling->numKeys; i++)
        {
            child->children[i + t] = sibling->children[i];
        }
    }

    for (int i = index + 1; i < node->numKeys; i++)
    {
        node->keys[i - 1] = node->keys[i];
    }

    for (int i = index + 2; i <= node->numKeys; i++)
    {
        node->children[i - 1] = node->children[i];
    }

    child->numKeys += sibling->numKeys + 1;
    node->numKeys--;

    free(sibling);
}
void deleteKey(btNode *node, int key)
{
    int index = 0;

    while (index < node->numKeys && key > node->keys[index])
    {
        index++;
    }

    if (index < node->numKeys && key == node->keys[index])
    {
        if (node->isLeaf)
        {
            deleteFromLeaf(node, index);
        }
        else
        {
            deleteFromNonLeaf(node, index);
        }
    }
    else
    {
        if (node->isLeaf)
        {
            printf("The key %d is not present in the tree.\n", key);
            return;
        }

        bool flag = (index == node->numKeys);

        if (node->children[index]->numKeys < t)
        {
            fillChild(node, index);
        }

        if (flag && index > node->numKeys)
        {
            deleteKey(node->children[index - 1], key);
        }
        else
        {
            deleteKey(node->children[index], key);
        }
    }
}
void fillChild(btNode *node, int index)
{
    if (index != 0 && node->children[index - 1]->numKeys >= t)
    {
        borrowFromPrevious(node, index);
    }
    else if (index != node->numKeys && node->children[index + 1]->numKeys >= t)
    {
        borrowFromNext(node, index);
    }
    else
    {
        if (index != node->numKeys)
        {
            mergeNodes(node, index);
        }
        else
        {
            mergeNodes(node, index - 1);
        }
    }
}
void bTreeDelete(BTree *tree, int key)
{
    btNode *root = tree->root;

    if (root->numKeys == 1)
    {
        btNode *child = root->children[0];
        btNode *sibling = root->children[1];

        if (child->numKeys == t - 1 && sibling->numKeys == t - 1)
        {
            mergeNodes(root, 0);
            free(root);
            tree->root = child;
        }
    }

    deleteKey(root, key);
}
int findMinimum(btNode *node)
{
    if (node->isLeaf)
    {
        return node->keys[0];
    }
    else
    {
        return findMinimum(node->children[0]);
    }
}

int main()
{
    printf("Enter the minimum degree of the B-tree 't': ");
    scanf("%d", &t);
    int app;
    int n, ele;
    printf("Write '1' for First approach and '2' for Second approach : ");
    scanf("%d", &app);
    int choice, key;
    int rt;
    BTree tree;
    createBTree(&tree);
    if (app == 2)
    {
        printf("Choose an option:\n");
        printf("1. Insert\n");
        printf("2. Search\n");
        printf("3. Delete\n");
        printf("4. Print Inorder\n");
        printf("5. Find Minimum\n");
        printf("6. Exit\n");

        while (1)
        {
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                printf("Enter the number of elements you wanted to enter : ");
                scanf("%d", &rt);
                for (int i = 0; i < rt; i++)
                {
                    scanf("%d", &key);
                    insert(&tree, key);
                }
                break;
            case 2:
                printf("Enter the key to search: ");
                scanf("%d", &key);
                if (search(tree.root, key))
                {
                    printf("Key %d is present in the tree.\n", key);
                }
                else
                {
                    printf("Key %d is not present in the tree.\n", key);
                }
                break;
            case 3:
                printf("Enter the key to delete: ");
                scanf("%d", &key);
                bTreeDelete(&tree, key);
                break;
            case 4:
                printf("Inorder Traversal: ");
                inOrderTraversal(tree.root);
                printf("\n");
                break;
            case 5:
                printf("Minimum Key: %d\n", findMinimum(tree.root));
                break;
            case 6:
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
            }
        }
    }
    if (app == 1)
    {
        printf("Enter 'n' : ");
        scanf("%d", &n);

        for (int i = n; i > 0; i--)
            insert(&tree, i);

        printf("Inoder traversal is: ");
        inOrderTraversal(tree.root);
        printf("\nEnter an element to search: ");
        scanf("%d", &ele);
        if (search((&tree)->root, ele))
            printf("ELement found\n");
        else
            printf("No element exist\n");
    }
    else
        printf("Invalid input");
    return 0;
}
