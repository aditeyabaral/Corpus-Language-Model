struct node
{
    char str[50];
    int count;
    struct node* link;
    struct node* next;
};
typedef struct node NODE;

struct list
{
    NODE* head;
};
typedef struct list MARKOV;

char* word_ngram(char*, int);
void create_model(char*,int,MARKOV*);
void insert(char*, MARKOV*);
void display(MARKOV*);
char* predict(char*,int,MARKOV*);