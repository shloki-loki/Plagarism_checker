#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include "../lib/Node.h"
#include <regex.h>
#include <ctype.h>


char *strlwr(char *str)
{
  unsigned char *p = (unsigned char *)str;
    int i=0;
  while (*p) {
     *p = tolower((unsigned char)*p);
      p++;
  }

  return str;
}

map* parse(char *filename){
    FILE *in = fopen(filename, "r");
    char* str = malloc(sizeof(char) * 10000);
    char* word;
    map* node= NULL;
    char delimiters[] = " []<>\n\t\r()-,;.;:;!;?;�;�;�";
    regex_t reg;

    if (in == NULL) perror ("Error opening file");
    else {
        while ( fgets (str , 10000 , in) != NULL ){
            word = strtok (str, delimiters);
            while (word!= NULL){
                word = strlwr(word);
                if(strlen(word) != 0 && 97<=word[0] && word[0]<=122) {
                    push((&node), word, 1.0);
                }
                word = strtok (NULL, delimiters);
            }
        }
        fclose(in);
        //printList(node);
        float size = totalFreq(&node);
        //printf("%s  %0.01f\n", filename, size);
        map* temp= malloc(sizeof(map));
        temp = node;
        if(size!=0){
            while(temp!=NULL){
                temp->count /= size;
                temp = temp->next;
            }
        }
    }
    //printList(node);
    return node;
}

map *idf(map *corpus[25], map *file, int n){
    map* node= malloc(sizeof(map));
    node = NULL;
    for(int i=0; i<n; i++){
        add(&node, &corpus[i]);
    }
    add(&node, &file);
    
    map* temp= malloc(sizeof(map));
    temp = node;
    while(temp!=NULL){
        temp->count = log((n+1)/temp->count);
        temp = temp->next;
    }
    //printList(node);
    return node;
}

float cosim (map *a, map *b){
    return dotP(&a, &b)/(mod(&b)*mod(&a));
}

int main(int argc, char *argv[]){
    DIR* folder;
    struct dirent* in_file;
    char    buffer[BUFSIZ];

    map p;
    char *dir = argv[2];
    folder= opendir(dir);
    int files=0;
    if(folder == NULL)
    {
        perror("Unable to read directory");
        return(1);
    }

    map* corpus[25];
    char *filenames[25];

    while( (in_file=readdir(folder)) )
    {
        char* temp = ".txt";
        if(strcmp ((in_file -> d_name + strlen(in_file -> d_name) - 4), temp))
            continue;

        char* filename = calloc(0,sizeof(char) * (strlen(dir)+ strlen(in_file-> d_name)+1));
        strcat(filename, dir);
        strcat(filename, "/");
        strcat(filename, in_file->d_name);
        corpus[files] = parse(filename);
        filenames[files] = in_file->d_name;

        files++;
    }
    closedir(folder);

    map *toCheck = parse(argv[1]);
    map *idf_map = idf(corpus, toCheck, files);
    //printList(toCheck);

    for(int i=0; i<files; i++){
        tfidf(&corpus[i], &idf_map);
    }
    tfidf(&toCheck, &idf_map);
    
    map *plagiarised = NULL;
    
    printf("The similarity found with each of the corpus files is listed below:-\n");
    for(int i=0; i<files; i++){
        float similarity = cosim(corpus[i], toCheck) * 100;
        printf("File %d: %s, \tSimilarity: %f%% \n", i+1, filenames[i], similarity);
        if(similarity>15) push(&plagiarised, filenames[i], similarity);
    }
    printf("--------------------\n\n");
    if(plagiarised==NULL) printf("The file: \"%s\" is not plagiarised from any of the corpus files.", argv[1]);
    else{
        printf("File: %s is plagiarised from the following files:\n", argv[1]);
        printPlagiarism(plagiarised);
    }

    return 0;
}