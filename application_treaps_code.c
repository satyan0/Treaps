/*
    NOTES
    1) the first value of txt file describes the number of entries
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

struct Node
{
    struct Node *lchild, *rchild, *parent;
    char name[100];
    int priority;
} typedef node;

void insert(node *, int);
void min_heapify(node *);
node *search(node *);
void inorder(node *);
void ltor_rotate(node *);
void rtol_rotate(node *);
void delete (node *);
void meld(node *, node *, node *, char);
void clrtarr(void);

char tarr[100];

int main()
{
    node *temp;
    clrtarr();

    FILE *file;
    file = fopen("data1.txt", "r");

    int n;
    fscanf(file, "%d", &n);

    int prior;

    fscanf(file, "%s", tarr);
    fgetc(file);
    fscanf(file, "%d", &prior);

    // manually inserting first element
    node *root = (node *)malloc(sizeof(node));
    root->parent = NULL;
    root->lchild = NULL;
    root->rchild = NULL;
    root->priority = prior; // its just random for testing IGNORE
    strcpy(root->name, tarr);

    for (int i = 1; i < n; ++i)
    {
        clrtarr();

        fscanf(file, "%s", tarr);
        fgetc(file);
        fscanf(file, "%d", &prior);

        insert(root, prior);
    }

    int choice;
    while (1)
    {

        printf("\n\n1. insert \n2. search\n3. inorder\n4. quit\n\n");
        printf("\nEnter the number corresponding your choice:\n");
        scanf("%d", &choice);

        clrtarr();

        switch (choice)
        {

        case 1:

            printf("Enter element name: ");
            scanf("%s", tarr);
            getchar();
            printf("Enter element priority: ");
            scanf("%d", &prior);
            insert(root, prior);
            break;

        case 2:
            temp = NULL;
            printf("Enter element name: ");
            scanf("%s", tarr);
            getchar();

            temp = search(root);
            if (!temp)
                printf("Required element is absent\n");
            else
                printf("Present with priority=%d", temp->priority);
            break;

        case 3:
            inorder(root);
            break;

        case 4:
            return 0;
        }
    }

}

void insert(node *NODE, int prior)
{

    node *new = (node *)malloc(sizeof(node));

    while (1) // iterate till desired node (below which key is to be inserted) is reached
    {
        if (strcmp((NODE->name), tarr) > 0)
        {
            // if key is smaller go left

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
        else if (strcmp((NODE->name), tarr) < 0)
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
    strcpy(new->name, tarr);
    new->priority = prior; // previously generated random number

    min_heapify(new); // only the new node could cause problems

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

    if (new->priority < lpriority || new->priority < rpriority)
    // if new is not highest priority
    {
        // then swap with the highest priority
        if (lpriority > rpriority)
        {
            rtol_rotate(new);
        }
        else
        {
            ltor_rotate(new);
        }
    }
    return;
}

void ltor_rotate(node *new)
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

    min_heapify(new); // again call min_heapify on new
    return;
}

void rtol_rotate(node *new)
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

    min_heapify(new); // again call min_heapify on new
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
    printf("%s   priority--%d\n", NODE->name, NODE->priority);
    inorder(NODE->rchild);
}

node *search(node *NODE) // returns node so that it can be used in delete()
{                        // normal binary tree search function ---- returns node containing desired key
    if (NODE == NULL)
    {
        return NULL;
    }
    else if (strcmp((NODE->name), tarr) == 0)
    {
        return NODE;
    }
    else if (strcmp((NODE->name), tarr) > 0)
    {
        return search(NODE->lchild);
    }
    else
    {
        return search(NODE->rchild);
    }
}

void delete (node *NODE)
{
    NODE = search(NODE);
    if (NODE == NULL)
    {
        return;
    }

    node *parent = NODE->parent;
    char direction; // lchild or rchild of parent
    if (parent->lchild == NODE)
    {
        direction = 'l';
    }
    else
    {
        direction = 'r';
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
        return;
    }

    if (leftpriority > rightpriority)
    {
        if (direction == 'l')
        {
            prnt->lchild = left;
        }
        else
        {
            prnt->rchild = left;
        }
        left->parent = prnt;
        prnt = left;
        left = left->rchild;

        prnt->rchild = NULL;
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
        else
        {
            prnt->rchild = right;
        }
        right->parent = prnt;
        prnt = right;
        right = right->lchild;

        prnt->lchild = NULL;
        if (right != NULL)
        {
            right->parent = NULL;
        }
        meld(left, right, prnt, 'l');
    }
    return;
}

void clrtarr(void)
{
    for (int i = 0; i < 100; ++i)
        tarr[i] = 0;
}