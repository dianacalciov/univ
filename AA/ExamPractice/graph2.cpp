// 4,5 2.Distanța dintre două cuvinte este dată de numărul de litere care trebuie modificate pentru a transforma un cuvânt în altul.
// Operațiile permise sunt permutarea, inserția de literă, ștergerea de literă, respectiv copierea pe aceiași poziție dintr-un cuvânt în altul (include ștergerea / includerea literei destinație), translatare (shiftare).
// ex: PISICA
//     PIERSICA
// o soluție: ștergem e și r și translatăm (=> distanța între string1 și string2 este 4)
// a) Se va genera un graf ponderat și orientat în care nodurile sunt cuvinte introduse de la tastatura, arcele fiind automat orientate 1.5p
// automat dinspre cuvântul care conține mai multe vocale către cel care conține mai puține iar ponderea fiind dată de dinstanța dintre cele două cuvinte. 1.5p
// b) Să se afișeze graful prin cuprindere/adâncime. 2.5p
// Dacă două cuvinte au același număr de vocale arcul e indrptat arbitrar. (Distanța levanstein)

// 1 1 1 2 1 3 1 4 1 5 1 6 3 1 2 3 1 3 3 2 4 3 2 5 3 4 5 3 4 6 3 3 5 3 5 6

#include <iostream>
using namespace std;
#define MAX 30
typedef struct Subnode
{
    int cost;
    string key;
    struct Subnode *next;
} Subnode;

typedef struct Node
{
    unsigned short int visited : 1;
    string key;
    struct Node *next;
    struct Subnode *sublist;
} Node;

void menu(int *option)
{
    cout << "\n0. Exit";
    cout << "\n1. Add node";
    cout << "\n2. Display list";
    cout << "\n3. Add edge";
    cout << "\n4. Depth traversal";
    cout << "\n5. Breadth traversal";
    cout << "\n6. Prim traversal";
    cout << "\n7. Delete an edge";
    cout << "\n8. Delete a node";
    cout << "\n9. Get path between two vertices";
    cout << "\nOption: ";
    cin >> *option;
}
Node *searchNode(Node *head, string key);
void displayList(Node *head);
Node *cleanUp(Node *head);
Node *readNode(Node *head);
Node *addNode(Node *head, string key);
void deleteNode(Node *head);
Subnode *deleteSubnode(Subnode *head, string key);
Subnode *addSubnode(Subnode *head, string key, int cost);
void depthTraversal(Node *head, Node *current);
void breadthTraversal(Node *head, string key);
void BFT(Node *head, string Queue[MAX]);
void PrimTraversal(Node *head, Subnode *shead);
Node *addEdge(Node *head);
void deleteEdge(Node *head);
int getDistance(string str1, string str2);
int getNrOfVowels(string str);
void getPath(Node *head);
string toUppercase(string str);

int main()
{
    Node *list = NULL;

    int option;
    while (1)
    {
        menu(&option);
        switch (option)
        {
        case 0:
            return 0;
        case 1:
            list = readNode(list);
            break;
        case 2:
            displayList(list);
            break;
        case 3:
            addEdge(list);
            break;
        case 4:
            depthTraversal(list, list);
            list = cleanUp(list);
            break;
        case 5:
            breadthTraversal(list, list->key);
            list = cleanUp(list);
            break;
        case 6:
            PrimTraversal(list, list->sublist);
            list = cleanUp(list);
            break;
        case 7:
            deleteEdge(list);
            break;
        case 8:
            deleteNode(list);
            break;
        case 9:
            getPath(list);
            list = cleanUp(list);
            break;
        default:
            break;
        }
    }
}
Node *searchNode(Node *head, string key)
{
    Node *node = head;
    for (node = head; node && node->key != key; node = node->next)
        ;
    return node;
}
Subnode *searchSubnode(Subnode *head, string key)
{
    Subnode *subnode = head;
    for (subnode = head; subnode && subnode->key != key; subnode = subnode->next)
        ;
    return subnode;
}

Node *readNode(Node *head)
{
    string key;
    cout << "\nInsert key: ";
    cin >> key;
    if (searchNode(head, key) == NULL)
        head = addNode(head, key);
    return head;
}

int getDistance(string str1, string str2)
{
    long int diff = 0;
    unsigned short int distance = 0,
                       strlen = 0;
    string shortest = str1;
    string longest = str2;
    if (str1.length() > str2.length())
    {
        longest = str1;
        shortest = str2;
    }
    diff += longest.length() - shortest.length();
    distance = diff;
    strlen = shortest.length();
    int gap = diff;
    for (size_t i = 0; i < strlen; i++)
        if (shortest[i] != longest[i])
        {
            if (gap <= 0)
                distance++;
            else
                while (shortest[i] != longest[i] && gap--)
                {
                    for (size_t j = i; j < strlen + diff - 1; j++)
                        longest[j] = longest[j + 1];
                    distance++;
                }
        }
    return distance;
}

string toUppercase(string str)
{
    for (size_t i = 0; str[i] != '\0'; i++)
        if (str[i] > 96)
            str[i] -= 32;
    return str;
}

int getNrOfVowels(string str)
{
    str = toUppercase(str);
    char vowels[] = {'A', 'E', 'I', 'O', 'U'};
    int nr = 0;
    for (size_t i = 0; str[i] != '\0'; i++)
        for (size_t j = 0; j < 5; j++)
            if (str[i] == vowels[j])
            {
                nr++;
                break;
            }
    return nr;
}

Node *addEdge(Node *head)
{
    Subnode *subnode;
    Node *nodeA, *nodeB;
    string keyA, keyB;
    unsigned short int distance = 0;
    cout << "\nInsert first vertex: ";
    cin >> keyA;
    while ((nodeA = searchNode(head, keyA)) == NULL)
    {
        cout << "\nInsert an existing vertex: ";
        cin >> keyA;
    }

    cout << "\nInsert second vertex: ";
    cin >> keyB;
    while ((nodeB = searchNode(head, keyB)) == NULL || keyA == keyB)
    {
        cout << "\nInsert an existing vertex different from first one: ";
        cin >> keyB;
    }

    distance = getDistance(nodeA->key, nodeB->key);
    if (getNrOfVowels(nodeA->key) >= getNrOfVowels(nodeB->key))
    {
        if (searchSubnode(nodeA->sublist, nodeB->key) == NULL)
            nodeA->sublist = addSubnode(nodeA->sublist, nodeB->key, distance);
    }
    else if (searchSubnode(nodeB->sublist, nodeA->key) == NULL)
        nodeB->sublist = addSubnode(nodeB->sublist, nodeA->key, distance);
    return head;
}

Node *addNode(Node *head, string key)
{
    Node *node,
        *q1, *q2;
    node = (Node *)malloc(sizeof(Node));
    node->next = NULL;
    node->sublist = NULL;
    node->visited = 0;

    node->key = key;
    for (q1 = q2 = head; q1 != NULL && q1->key < key; q2 = q1, q1 = q1->next)
        ;
    if (q1 != q2)
    {
        q2->next = node;
        node->next = q1;
    }
    else
    {
        node->next = head;
        head = node;
    }
    return head;
}

Subnode *addSubnode(Subnode *head, string key, int cost)
{
    Subnode *subnode,
        *q1, *q2;
    subnode = (Subnode *)malloc(sizeof(Subnode));
    subnode->cost = cost;
    subnode->next = NULL;
    subnode->key = key;
    for (q1 = q2 = head; q1 != NULL && q1->key < key; q2 = q1, q1 = q1->next)
        ;
    if (q1 == q2)
    {
        subnode->next = head;
        head = subnode;
    }
    else
    {
        q2->next = subnode;
        subnode->next = q1;
    }
    return head;
}

void displayList(Node *head)
{
    Node *node = head;
    Subnode *subnode;
    for (node = head; node; node = node->next)
    {
        cout << "\nnode: " << node->key;
        if (node->sublist != NULL)
            for (subnode = node->sublist; subnode; subnode = subnode->next)
                cout << "\n\tVertex: " << subnode->key << ", cost: " << subnode->cost;
    }
}
Node *cleanUp(Node *head)
{
    Node *node = head;
    for (node = head; node; node = node->next)
        node->visited = 0;
    return head;
}

string popQueue(string Queue[MAX])
{
    string aux = Queue[0];
    int i, k = 0;
    for (i = 0; i < MAX - 1; i++)
    {
        if (Queue[i] != "")
            k++;
        Queue[i] = Queue[i + 1];
    }
    Queue[k] = "";
    return aux;
}

void pushQueue(string Queue[MAX], string str)
{
    int i;
    for (i = MAX - 1; Queue[i].length() == 0; i--)
        ;
    Queue[i + 1] = str;
}

void breadthTraversal(Node *head, string key)
{
    string Queue[MAX];
    Queue[0] = key;
    cout << "\nnode (b): " << key;
    BFT(head, Queue);
}
void BFT(Node *head, string Queue[MAX])
{
    string current = popQueue(Queue);
    Node *node;
    node = searchNode(head, current);
    if (node && node->visited == 0)
    {
        node->visited = 1;
        Subnode *subnode = node->sublist;
        while (subnode != NULL)
        {
            if (searchNode(head, subnode->key)->visited == 0)
            {
                cout << "\nnode (b): " << subnode->key;
                pushQueue(Queue, subnode->key);
            }
            subnode = subnode->next;
        }
        BFT(head, Queue);
    }
}

void depthTraversal(Node *head, Node *current)
{
    if (current && current->visited != 1)
    {
        current->visited = 1;
        cout << "\nnode (d): " << current->key;
        Subnode *subnode = current->sublist;
        for (subnode = current->sublist; subnode; subnode = subnode->next)
            depthTraversal(head, searchNode(head, subnode->key));
    }
}

void PrimTraversal(Node *head, Subnode *shead)
{
    Node *node;
    Subnode *subnode = shead;

    while (subnode)
    {
        if ((node = searchNode(head, subnode->key))->visited == 0)
        {
            cout << "\nnode (p): " << subnode->key;
            node->visited = 1;
            PrimTraversal(head, node->sublist);
        }
        subnode = subnode->next;
    }
}

void deleteNode(Node *head)
{
    string key;
    cout << "\nkey: ";
    cin >> key;
    Node *q1, *q2;
    for (q1 = q2 = head; q1 != NULL && q1->key != key; q2 = q1, q1 = q1->next)
        ;
    if (q1 != q2)
    {
        q2->next = q1->next;
        for (q2 = head; q2 != NULL; q2 = q2->next)
            if (q2->sublist)
                q2->sublist = deleteSubnode(q2->sublist, q1->key);
        free(q1);
    }
    else
        head = head->next;
}

Subnode *deleteSubnode(Subnode *head, string key)
{
    Subnode *q1, *q2;
    for (q1 = q2 = head; q1 != NULL && q1->key != key; q2 = q1, q1 = q1->next)
        ;

    if (q1 != NULL && q1->key == key)
    {
        if (q1 == q2)
            head = head->next;
        else
        {
            q2->next = q1->next;
            free(q1);
        }
    }
    return head;
}

void deleteEdge(Node *head)
{
    Subnode *subnode;
    Node *nodeA, *nodeB;
    string keyA, keyB;
    cout << "\nInsert first vertex: ";
    cin >> keyA;
    while ((nodeA = searchNode(head, keyA)) == NULL)
    {
        cout << "\nInsert an existing vertex: ";
        cin >> keyA;
    }
    cout << "\nInsert second vertex: ";
    cin >> keyB;
    while ((nodeB = searchNode(head, keyB)) == NULL || keyA == keyB)
    {
        cout << "\nInsert an existing vertex different from first one: ";
        cin >> keyB;
    }

    nodeA->sublist = deleteSubnode(nodeA->sublist, nodeB->key);
    nodeB->sublist = deleteSubnode(nodeB->sublist, nodeA->key);
}
void searchPath(Node *head, Node *current, Node *end)
{
    if (current && current->visited != 1 && end->key != current->key)
    {
        // current->visited = 1; // uncomment if you wanna see only one path
        cout << current->key << " -> ";
        Subnode *subnode = current->sublist;
        for (subnode = current->sublist; subnode; subnode = subnode->next)
            searchPath(head, searchNode(head, subnode->key), end);
        cout << "\n"; // comment if you wanna see only one path
    }
}
void getPath(Node *head)
{
    Node *nodeA, *nodeB;
    string keyA, keyB;
    cout << "\nInsert first vertex: ";
    cin >> keyA;
    while ((nodeA = searchNode(head, keyA)) == NULL)
    {
        cout << "\nInsert an existing vertex: ";
        cin >> keyA;
    }
    cout << "\nInsert second vertex: ";
    cin >> keyB;
    while ((nodeB = searchNode(head, keyB)) == NULL || keyA == keyB)
    {
        cout << "\nInsert an existing vertex different from first one: ";
        cin >> keyB;
    }
    cout << "\npath: ";
    searchPath(head, nodeA, nodeB);
    cout << nodeB->key;
}