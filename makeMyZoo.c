#include <stdlib.h>
#include <stdio.h>
#include "Animal.h"

int main(int argc, const char * argv[]) {
    
    FILE* fp;
    struct Animal rec;
    
    fp = fopen(argv[1],"wb");
    
    printf("%s %s %s  %s %s\n", "ID","Name", "Species", "Weight", "Height");
    
    while(scanf("%d %s %s %d %d",&rec.id,rec.name, rec.species, &rec.weight,&rec.height) == 5){
        fseek(fp, (rec.id-START_ID)*sizeof(rec), SEEK_SET);
        fwrite(&rec, sizeof(rec), 1, fp);
    }
    
    fclose(fp);
    exit(0);
    
}
