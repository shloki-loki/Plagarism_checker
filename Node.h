#include<stdio.h>
#include<stdlib.h>
#include <string.h>

typedef struct pair map;

struct pair {
    char *word;
    float count;
    map *next;
};


void printList(map *node) 
{ 
    while (node != NULL) 
    { 
        printf("%s %f\n", node->word, node->count);
        node = node->next; 
    } 
}

void printPlagiarism(map *node) 
{ 
    while (node != NULL) 
    { 
        printf("File: \"%s\" \tSimilarity: %f%%\n", node->word, node->count);
        node = node->next; 
    } 
}

void push(map** head, char *val, float count){
    map* node= malloc(sizeof(map));
    map* temp= malloc(sizeof(map));
    node->next = NULL;
    temp=(*head);
    node->word = malloc(strlen(val));
    strcpy(node->word, val);
    node->count = count;

    if(*head==NULL){
        (*head) =(node);
        return;
    }
    if (strcmp(temp->word, val)>0){
        node->next = temp;
        (*head) = node;
        return;
    }
    if (strcmp(temp->word, val)==0){
        temp->count = temp->count + count;
        free(node);
        return;
    }
    while(temp->next!=NULL && strcmp(temp->next->word, val)<0){
        temp= (temp->next);
    }
    if(temp->next!=NULL && strcmp(temp->next->word, val)==0){
        temp->next->count = temp->next->count + count;
        free(node);
    }
    else{  
        node->next = (temp->next);
        temp->next = (node);
    }
}


void add(map** a, map**b){
    map*temp = malloc(sizeof(map));
    temp = *b;
    while(temp!=NULL){
        push(a, temp->word, 1.0);
        temp = temp->next;
    }
}


float totalFreq(map** head){
    map* temp= malloc(sizeof(map));
    temp=(*head);
    
    if(*head==NULL){
        return 0;
    }
    float count= (*head)->count;
    
    while(temp->next!=NULL){
        //printInt(temp->data);
        temp= (temp->next);
        count+=temp->count;
    }
    return count; 
}

void tfidf(map **doc, map **corpus){
    map *temp = malloc(sizeof(map));
    map *check = malloc(sizeof(map));
    temp = *doc;
    check = *corpus;
    while(temp!=NULL){
        while(strcmp(temp->word, check->word)>0){
            //printf("%s %s\n", temp->word, check->word);
            check = check->next;
            
        }
        temp->count *= check->count;
        temp = temp->next;
    }
    //printList(*doc);
    //printf("\n");
}

float mod(map **head){
    map *temp = malloc(sizeof(map));
    temp = *head;
    float val =0;

    while(temp != NULL){
        val += (temp->count * temp->count);
        //printf("%s %f\n", temp->word, val);
        temp=temp->next;
    }
    return sqrt(val);
}

float dotP(map **a, map **b){
    map *ta = malloc(sizeof(map));
    map *tb = malloc(sizeof(map));
    ta = *a;
    tb = *b;
    float val = 0;

    while(ta!=NULL && tb!=NULL){
        while(tb!=NULL && strcmp(ta->word, tb->word)>0){
            //printf("1. %s %s\n", ta->word, tb->word);
            tb=tb->next;
        }
        while(tb !=NULL && ta!=NULL && strcmp(ta->word, tb->word)<0){
            //printf("2. %s %s\n", ta->word, tb->word);
            ta=ta->next;
        }
        if(tb !=NULL && ta!=NULL && strcmp(ta->word, tb->word)==0){
            val += ta->count * tb->count;

            //printf("%s %s %f\n", ta->word, tb->word, val);
            ta=ta->next;
            tb=tb->next;
        }
    }
    return val;
}
