#include <stdio.h>
#include <stdlib.h>
#include "Animal.h"

#define START_ID 1

int main(int argc, char *argv[]){
    
    struct Animal rec;
    char c='Y';
    int id;
    
    FILE* fp;
    
	fp = fopen(argv[1],"rb");

    while(c =='Y'){
        printf("INPUT FINDING ID: ");
        if(scanf("%d", &id) == 1){
            fseek(fp, (id-START_ID)*sizeof(rec), SEEK_SET);
            if((fread(&rec, sizeof(rec),1,fp) > 0) && (rec.id != 0))
                printf("ID: %d NAME: %s SPECIES: %s WEIGHT: %d HEIGHT: %d\n",
                       rec.id,rec.name, rec.species, rec.weight, rec.height);
            else printf("UNKNOWN\n");
        } else printf("INPUT ERROR\n");
        
        printf("CONTINUE? Y/N ");
        scanf(" %c", &c);
    }
    
    fclose(fp);
    exit(0);

}
