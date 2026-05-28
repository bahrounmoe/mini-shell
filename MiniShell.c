#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

char** parseCommand(char* command){
    char* part;
    char** tab=NULL;
    int taille=0;

    part = strtok(command," ");
    while (part != NULL){
        tab=realloc(tab,(taille+1)*sizeof(char*));
        tab[taille]=malloc(strlen(part)+1);
        strcpy(tab[taille],part);
        taille++;
        part = strtok(NULL," ");
    }
    tab=realloc(tab,(taille+1)*sizeof(char*));
    tab[taille]=malloc(strlen(part)+1);
    tab[taille]=NULL;
    return tab;
}

void executeExternalCommand(char** tab){
    pid_t pid = fork();
    if (pid==0){
        int erreur=execvp(tab[0],tab);
        if (erreur == -1){
            exit(1);
        }
    }
    else if (pid>0){
        int status;
        wait(&status);
        if (WIFEXITED(status)){
            printf("programme terminé avec le status %d",WEXITSTATUS(status));
        }
    }
    else {
        perror("fork echoué");
        exit(1);
    }
}

int main (){
    char commande[100];
    while (1){ 
        printf("minishell>");
        fgets(commande,sizeof(commande),stdin);
        commande[strcspn(commande, "\n")] = '\0';
        char** tab=parseCommand(commande);


        if (strcasecmp(tab[0],"cd")==0 || strcasecmp(tab[0],"exit")==0 || strcasecmp(tab[0],"help")==0){
            if (strcasecmp(tab[0],"cd")==0){
                printf("on se deplace de cette repertoire");

            }
            if (strcasecmp(tab[0],"exit")==0){
                printf("le programme s'arrete");
                exit(0);
            }
            if (strcasecmp(tab[0],"help")==0){
                printf("aide");
            }
        }
        executeExternalCommand(tab);
    }
    return 0;
        
}





