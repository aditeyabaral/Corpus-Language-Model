#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
int main()
{
    FILE *f = fopen("BrownCorpus.txt", "r");                            //Change training corpus here
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *s = malloc(fsize + 1);
    fread(s, 1, fsize, f);
    fclose(f);
    s[fsize] = 0;

    MARKOV *model = (MARKOV*)malloc(sizeof(MARKOV));
    model->head = NULL;
    printf("Creating Markov Chains...\n");
    create_model(s,2,model);                                            //Edit 2 for ngram
    printf("\nMarkov Model created.\n\nEnter initial text: ");
    //display(model);                                                   //Uncomment to display model
    
    char query[1000000], ch;
    int pos = 0;
    while((ch = getchar())!='\n')
        query[pos++] = ch;
    char* text = predict(query,2,model);                                //Edit 2 for ngram
    printf("\n%s\n",text);

    free(model);
    return 0;
}

void create_model(char* s, int n, MARKOV* model)
{
    char words[10000];
    int beg = 0;
    int len = strlen(s);
    s[len] = ' ';
    for(int i=0;i<len;i++)
    {
        if (s[i]==' ')
        {
            for(int j = 0;j<10000;j++)
                words[j] = '\0';
            int pos = 0;
            for(int j=beg;j<i;j++)
                words[pos++] = s[j];
            int ctr = 0;
            beg = i+1;
            words[pos++] = ' ';
            for(int j = i+1;j<len;j++)
            {
                if(ctr==(n-1))
                {
                    words[pos-1] = '\0';
                    break;
                }
                if (s[j]==' ')
                    ctr++;
                words[pos++] = s[j];
            }
            if (ctr==n-1)
                insert(words,model);
        }
    }
    s[len] = '\0';

    NODE *pres1, *pres2, *prev;
    pres1 = model->head;
    pres2 = NULL;
    int m,j;
    while(pres1!=NULL)
    {
        m = len;
        n = strlen(pres1->str);
        for(int i=0;i<m-n;i++)
        {
            j = 0;
            while(j<n && s[i+j]==pres1->str[j])
                j++;
            if (j==n)
            {
                int k = i+j+1, pos = 0;
                char temp[20];
                for(int t=0;t<20;t++)
                    temp[t] = '\0';
                while(k<m && s[k]!=' ')
                    temp[pos++] = s[k++];
                NODE* node = (NODE*)malloc(sizeof(NODE));
                node->next = NULL;
                node->link = NULL;
                strcpy(node->str,temp);
                node->count = 1;
                if(pres1->link==NULL)
                    pres1->link = node;
                else
                {
                    pres2 = pres1->link;
                    prev = NULL;
                    while(pres2!=NULL && strcmp(pres2->str,temp))
                    {
                        prev = pres2;
                        pres2 = pres2->next;
                    }
                    if(pres2==NULL)
                    {
                        prev->next = node;
                        node->next = NULL;
                    }
                    else
                        pres2->count+=1;
                }
            }
        }
        pres1 = pres1->next;
    }
}

void insert(char*s, MARKOV* model)
{
    NODE* node = (NODE*)malloc(sizeof(NODE));
    node->count = 1;
    node->link = NULL;
    node->next = NULL;
    strcpy(node->str,s);
    if (model->head==NULL)
        model->head = node;
    else
    {
        NODE* pres = (NODE*)malloc(sizeof(NODE));
        NODE* prev = (NODE*)malloc(sizeof(NODE));
        pres = model->head;
        prev = NULL;
        while(pres!=NULL && strcmp(pres->str,s))
        {
            prev = pres;
            pres = pres->next;
        }
        if(pres==NULL)
        {
            prev->next = node;
            node->next = NULL;
        }
        else
            pres->count+=1;
    }
}

char* predict(char*s,int n,MARKOV* model)
{
    int max_count = 50, count = 0;                                      //Change max_count for word limit 
    char* gen = (char*)malloc(sizeof(char)*100000);
    strcpy(gen,s);
    while(1 && count < max_count)
    {
        int len = strlen(gen);
        char temp[len+1];
        for(int j=0;j<len+1;j++)
            temp[j] = '\0';
        temp[0] = ' ';
        strcat(temp,gen);
        int i = len;
        int ctr = 0;
        while(i>=0 && ctr!=n)
        {
            if(temp[i]==' ')
                ctr++;
            i--;
        }
        if (ctr!=n)
            break;
        else
        {
            i+=2;
            int pos = 0;
            for(int j=i-1;j<len;j++)
                temp[pos++] = gen[j];
            for (int j=pos;j<len+1;j++)
                temp[j] = '\0';
            NODE* node = model->head;
            while(node!=NULL)
            {
                if (!strcmp(node->str,temp))
                    break;
                node = node->next;
            }
            if (node==NULL)
                break;
            else
            {
                NODE* branch = node->link;
                NODE* max_s = branch; int max_c = 0;
                while(branch!=NULL)
                {
                    if (branch->count>max_c)
                    {
                        max_s = branch;
                        max_c = branch->count;
                    }
                    branch = branch->next;
                }
                strcat(gen," ");
                strcat(gen,max_s->str);

            }
        }
        count++;
    }
    return gen;
}


void display(MARKOV* model)
{
    NODE* node1 = model->head;
    NODE* node2;
    while(node1!=NULL)
    {
        printf("%s %d\n",node1->str,node1->count);
        node2 = node1->link;
        while(node2!=NULL)
        {
            printf("%s %d\n",node2->str,node2->count);
            node2 = node2->next;
        }
        printf("\n");
        node1 = node1->next;
    }
    printf("\n");
}