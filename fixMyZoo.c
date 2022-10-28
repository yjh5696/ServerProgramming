#include <stdio.h>
#include <stdlib.h>
#include "Animal.h"

int main(int argc, char* argv[]){
    
    struct Animal rec;
    char c;
    int id;
    FILE* fp;

	fp = fopen(argv[1],"rb+");
    
    do{
        printf("INPUT FINDING ID: ");
        if(scanf("%d",&id)==1){
            fseek(fp,(id-START_ID)*sizeof(rec), SEEK_SET);
            if((fread(&rec,sizeof(rec),1,fp)>0) && (rec.id!=0)){
                printf("ID: %d NAME: %s  SPECIES: %s WEIGHT: %d HEIGHT: %d",
                     rec.id,rec.name, rec.species, rec.weight, rec.height);
                printf("\nCHANGING INFORMATION(S,W,H): ");
				char t;
			    scanf(" %c", &t);
				fseek(fp,-sizeof(rec),SEEK_CUR);
                if(t=='S'){
					printf("%s is %s\n",rec.name,rec.species);
                    scanf("%s", &rec.species);
                    fwrite(&rec, sizeof(rec), 1, fp);
                }else if(t=='W'){
					printf("current weight is %d\n",rec.weight);
                    scanf("%d", &rec.weight);
                    fwrite(&rec, sizeof(rec), 1, fp);
                }else if(t=='H'){
					printf("current height is %d\n", rec.height);
                    scanf("%d", &rec.height);
                    fwrite(&rec, sizeof(rec), 1, fp);
                }else printf("WRONG ACCESS\n");   
            }
       }
        
        printf("CONTINUE? (Y/N) ");
        scanf(" %c", &c);
    }while(c=='Y');

    
    fclose(fp);
    exit(0);
}
