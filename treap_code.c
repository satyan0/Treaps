/*
    NOTES
    1) the first value of txt file describes the number of entries
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

struct Node
{
    struct Node *lchild, *rchild, *parent;
    int val, priority;
} typedef node;

void insert(node *, int);
void max_heapify(node *);
void min_heapify(node *);
node *search(node *, int);
void inorder(node *);
void ltor_rotate(node *);
void rtol_rotate(node *);
void delete(node *, int);
void meld(node *, node *, node *, char);

int main()
{
    FILE *file;
    file = fopen("data.txt", "r");

    int i;
    fscanf(file, "%d", &i);

    // manually inserting first element
    node *root = (node *)malloc(sizeof(node));
    root->parent = NULL;
    root->lchild = NULL;
    root->rchild = NULL;
    root->priority = 781; // its just random for testing IGNORE

    { // IGNORE these parenthesis{}
        // used parnthesis just to save memory as int v gets dumped
        int v;
        fscanf(file, "%d", &v);
        root->val = v;
    }

    // run insert() function in loop to insert 2nd 3rd.... elements
    for (int j = 0; j < i - 1; j++)
    {
        int v;
        fscanf(file, "%d", &v);
        insert(root, v);
    }

    // the tree is binary tree only if inorder() gives output in ascending order [KINDLY CHECK OUTPUT]
    inorder(root);

    int choice;
    while (1)
    {
        printf("\n\n1.Insert \n2.Search\n3.Inorder print\n4.Delete\n5.quit\n\n");
        printf("\nEnter the number corresponding your choice:\n");
        scanf("%d", &choice);

        switch (choice)
        {

        case 1:
            printf("Enter element :");
            scanf("%d", &choice);
            insert(root, choice);
            break;

        case 2:
            printf("Enter element to search:");
            scanf("%d", &choice);
            node *temp = search(root, choice);
            if (temp == NULL)
            {
                printf("Element is not present\n");
            }
            else
            {
                printf("Element is present with priority %d\n", temp->priority);
            }
            break;

        case 3:
            inorder(root);
            break;

        case 4:
            printf("Enter element to delete:\n");
            scanf("%d", &choice);
            delete(root, choice);
            break;

        case 5:
            return 0;
        }
    }

    int j;
    printf("\nEnter integer to delete\n");
    scanf("%d", &j);
    delete (root, j);

    inorder(root);
}

void insert(node *NODE, int key)
{
    int random = (unsigned)(rand() % 1000); // priority randomly generate between 0 -> 999 [ALTER AS NEEDED]

    node *new = (node *)malloc(sizeof(node));

    while (1) // iterate till desired node (below which key is to be inserted) is reached
    {
        if ((NODE->val) > key)
        // if key is smaller go left
        {
            if (NODE->lchild != NULL)
            {
                NODE = NODE->lchild;
            }
            else
            {
                // reached leaf
                NODE->lchild = new; // attach to leaf
                break;
            }
        }
        else if ((NODE->val) < key)
        // if key is smaller go left
        {
            if (NODE->rchild != NULL)
            {
                NODE = NODE->rchild;
            }
            else
            {
                // reached leaf
                NODE->rchild = new; // attach to leaf
                break;
            }
        }
    }

    // fill values in new node
    new->parent = NODE;
    new->lchild = NULL;
    new->rchild = NULL;
    new->val = key;
    new->priority = random; // previously generated random number

    max_heapify(new); // only the new node could cause problems

    return;
}

void min_heapify(node *new)
{
    int lpriority, rpriority; // priority of left and right children of new

    if (new->lchild != NULL)
    {
        lpriority = new->lchild->priority;
    }
    else
    {
        lpriority = 0;
    }

    if (new->rchild != NULL)
    {
        rpriority = new->rchild->priority;
    }
    else
    {
        rpriority = 0;
    }

    if (new->priority > lpriority || new->priority > rpriority)
    // if new is not lowest priority
    {
        // then swap with the lowest priority
        if (lpriority > rpriority)
        {
            ltor_rotate(new);
        }
        else
        {
            rtol_rotate(new);
        }
    }
    return;
}

void max_heapify(node *new)
{
    int lpriority, rpriority; // priority of left and right children of new

    if (new->lchild != NULL)
    {
        lpriority = new->lchild->priority;
    }
    else
    {
        lpriority = 0;
    }

    if (new->rchild != NULL)
    {
        rpriority = new->rchild->priority;
    }
    else
    {
        rpriority = 0;
    }

    if (new->priority < lpriority || new->priority < rpriority)
    // if new is not highest priority
    {
        // then swap with the highest priority
        if (lpriority > rpriority)
        {
            ltor_rotate(new);
        }
        else
        {
            rtol_rotate(new);
        }
    }
    return;
}

void rtol_rotate(node *new)
{
    node *l, *r, *rl, *p;
    p = new->parent;
    l = new->lchild;
    r = new->rchild;
    rl = r->lchild;

    // NOTE: a total of 6 connections are to be altered
    if (new == p->lchild)
    {
        p->lchild = r;
    }
    else
    {
        p->rchild = r;
    }
    r->parent = p;
    r->lchild = new;
    new->parent = r;
    new->rchild = rl;
    rl->parent = new;

    max_heapify(new); // again call max_heapify on new
    return;
}

void ltor_rotate(node *new)
{
    node *l, *r, *lr, *p;
    p = new->parent;
    l = new->lchild;
    r = new->rchild;
    lr = l->rchild;

    // NOTE: a total of 6 connections are to be altered
    if (new == p->lchild)
    {
        p->lchild = l;
    }
    else
    {
        p->rchild = l;
    }
    l->parent = p;
    l->rchild = new;
    new->parent = l;
    new->lchild = lr;
    lr->parent = new;

    max_heapify(new); // again call max_heapify on new
    return;
}

void inorder(node *NODE)
// normal binary tree inorder traversal
{
    if (NODE == NULL)
    {
        return;
    }
    inorder(NODE->lchild);
    printf("%d   priority--%d\n", NODE->val, NODE->priority);
    inorder(NODE->rchild);
}

node *search(node *NODE, int key) // returns node so that it can be used in delete()
{                                 // normal binary tree search function ---- returns node containing desired key
    if (NODE == NULL)
    {
        return NULL;
    }
    else if ((NODE->val) == key)
    {
        return NODE;
    }
    else if ((NODE->val) > key)
    {
        return search(NODE->lchild, key);
    }
    else
    {
        return search(NODE->rchild, key);
    }
}

void delete(node *NODE, int key)
{
    NODE = search(NODE, key);
    if (NODE == NULL)
    {
        return;
    }

    node *parent = NODE->parent;
    char direction; // lchild or rchild of parent
    if (parent != NULL)
    {
        if (parent->lchild == NODE)
        {
            direction = 'l';
        }
        else
        {
            direction = 'r';
        }
    }
    else
    {
        direction = 'N';
    }
    meld(NODE->lchild, NODE->rchild, parent, direction);
    return;
}

void meld(node *left, node *right, node *prnt, char direction)
{
    int leftpriority, rightpriority;
    if (left != NULL)
    {
        leftpriority = left->priority;
    }
    else
    {
        leftpriority = 0;
    }
    if (right != NULL)
    {
        rightpriority = right->priority;
    }
    else
    {
        rightpriority = 0;
    }

    if (rightpriority == 0 && leftpriority == 0)
    {
        if (direction == 'l')
        {
            prnt->lchild = NULL;
        }
        else if (direction == 'r')
        {
            prnt->rchild = NULL;
        }
        return;
    }

    if (leftpriority > rightpriority)
    {
        if (direction == 'l')
        {
            prnt->lchild = left;
        }
        else if (direction == 'r')
        {
            prnt->rchild = left;
        }
        left->parent = prnt;
        if (prnt != NULL)
        {
            prnt = left;
        }
        left = left->rchild;
        if (prnt != NULL)
        {
            prnt->rchild = NULL;
        }
        if (left != NULL)
        {
            left->parent = NULL;
        }
        meld(left, right, prnt, 'r');
    }
    else if (leftpriority < rightpriority)
    {
        if (direction == 'l')
        {
            prnt->lchild = right;
        }
        else if (direction == 'r')
        {
            prnt->rchild = right;
        }
        right->parent = prnt;
        if (prnt != NULL)
        {
            prnt = right;
        }
        right = right->lchild;

        if (prnt != NULL)
        {
            prnt->lchild = NULL;
        }
        if (right != NULL)
        {
            right->parent = NULL;
        }
        meld(left, right, prnt, 'l');
    }
    return;
}
