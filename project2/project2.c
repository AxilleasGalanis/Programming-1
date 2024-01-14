#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct std{
    unsigned long int aem;
    char name[64];
    unsigned short int courses;
    struct list *array;
    struct std *nxt;
    struct std *prv;
};

struct list{
    unsigned short int lessons;
    struct list *nxt;
};

unsigned long hash(char name[]);

int find(struct std ***table,unsigned long int aem,int counter,int res_srt,int k);

int modify(struct std **hash_table,struct std ***table,unsigned long int aem,unsigned short int ncourses,int counter,int res_srt,int *len3);

int clear(struct std ***table,struct std ***hash_table,struct list***array,int*counter,int **elements,int*len1,int*len3,char *argv[]);

int add(struct std ***table,struct std ***hash_table,struct list ***array,unsigned long int aem,char name[],unsigned short int courses,int *len1,int len2,int *len3,int*counter,int res_srt,int **elements,double load_factor);

int rmv(struct std ***table,struct std ***hash_table,struct list ***array,unsigned long int aem,int*counter,int*len1,int len2,int *len3,int res_srt,int **elements,double load_factor,char *argv[]);

int sort(struct std **table,int counter,int k);

void print(struct std ***hash_table,int **elements,int *len3,double sum,double load_factor,int *len1);

int linear_search(struct std ***table,unsigned long int aem,int counter,int k);

int binary_search(struct std ***table,unsigned long int aem,int counter,int k);

void hash_insert(struct std ***hash_table,struct std **table,int counter,int *len3,char *name,int **elements,double load_factor);

void hash_remove(struct std ***hash_table,struct std *table,int counter,int *len3,char *name,int **elements,double load_factor,char *argv[]);

int hash_find(struct std **hash_table,struct std *table,int *len3,char *name);

void sort_hash(struct std **hash_table,int *len3,char *name);

void increase_size(struct std ***table,int *len1,int len2){
    *table=(struct std**)realloc(*table,(*len1+len2)*sizeof(struct std*));
    (*len1)=(*len1)+len2;
}

void capitals(char name[]){
    int i=0;
    while(name[i]!='\0'){
        if(name[i]=='\0'){
                    break;
        }
        if(name[i]>='a'&&name[i]<='z'){
            name[i]-=32;
        }
        i++;
    }
}

int clear(struct std ***table,struct std ***hash_table,struct list***array,int*counter,int **elements,int*len1,int*len3,char *argv[]){
    struct std *temp=NULL,*next;
    struct list *curr;
    int i=0;

    if(*counter>0){
        for(i=0;i<*len3;i++){
            temp=(*hash_table)[i];
            while(temp!=NULL){
                next=temp->nxt;
                free(temp->array);
                free(temp);
                temp=next;
            }
            (*elements)[i]=0;
            (*hash_table)[i]=NULL;
        }
        for(i=0;i<*counter;i++){
            while((*table)[i]->array!=NULL){
                curr=(*table)[i]->array;
                (*table)[i]->array=(*table)[i]->array->nxt;
                free(curr);
            }
            free((*table)[i]);
            (*table)[i]=NULL;
        }
    }

    (*counter)=0;
    (*len1)=0;
    (*len3)=atoi(argv[3]);
   return 1;
}

int add(struct std ***table,struct std ***hash_table,struct list ***array,unsigned long int aem,char name[],unsigned short int courses,int* len1,int len2,int *len3,int* counter,int res_srt,int **elements,double load_factor){
    int res;
    struct std *p;

    res=find(table,aem,*counter,res_srt,0);
    if(res!=-1){
        return -1;
    }

    p=malloc(sizeof(struct std));
    p->aem=aem;
    strcpy(p->name,name);
    p->courses=courses;
    p->array=(**array);

    if(*counter<*len1){
        (*table)[*counter]=p;
        hash_insert(hash_table,*table,*counter,len3,name,elements,load_factor);
        (*counter)++;
        return 1;
    }
    if(*counter==*len1){
        increase_size(table,len1,len2);
        (*table)[*counter]=p;
        hash_insert(hash_table,*table,*counter,len3,name,elements,load_factor);
        (*counter)++;
    }
    return 0;
}

int rmv(struct std ***table,struct std ***hash_table,struct list ***array,unsigned long int aem,int* counter,int* len1,int len2,int *len3,int res_srt,int **elements,double load_factor,char *argv[]){
    struct list *curr;
    int res;

    res=find(table,aem,*counter,res_srt,0);
    if(res==-1){
        return -1;
    }
    else{
        hash_remove(hash_table,(*table)[res],*counter,len3,(*table)[res]->name,elements,load_factor,argv);
        while((*table)[res]->array!=NULL){
            curr=(*table)[res]->array;
            (*table)[res]->array=(*table)[res]->array->nxt;
            free(curr);
        }
        free((*table)[res]);
        for(;res<*counter-1;res++){
            (*table)[res]=(*table)[res+1];
        }
        (*counter)--;
        (*table)[*counter]=NULL;
        if(*counter<*len1-len2){
            (*table)=(struct std**)realloc((*table),(*len1-len2)*sizeof(struct std *));
            (*array)=(struct list**)realloc((*array),(*len1-len2)*sizeof(struct list *));
            (*len1)=(*len1)-len2;
        }
    }

    return *counter;
}

int linear_search(struct std ***table,unsigned long int aem,int counter,int k){
    int i=0,comp=0;

    for(i=0;i<counter;i++){
        comp++;
        if((*table)[i]->aem==aem){
            if(k==1){
                fprintf(stderr, "\n$%d\n", comp);
            }
            return i;
        }
    }
    if(k==1){
        fprintf(stderr, "\n$%d\n", comp);
    }
    return -1;
}

int binary_search(struct std ***table,unsigned long int aem,int counter,int k){
    int first=0,last=counter-1,mid,comp=0;

    while(first<=last){
        mid=(first+last)/2;
        comp++;
        if((*table)[mid]->aem==aem){
            if(k==1){
                fprintf(stderr, "\n$%d\n", comp);
            }
            return mid;
        }
        else if((*table)[mid]->aem<aem){
            first=mid+1;
            comp++;
        }
        else{
            last=mid-1;
            comp++;
        }
    }
    if(k==1){
        fprintf(stderr, "\n$%d\n", comp);
    }
    return -1;
}

int find(struct std ***table,unsigned long int aem,int counter,int res_srt,int k){
    int res;
    if(res_srt==1){
        if(k==0){
            res=binary_search(table,aem,counter,0);
        }
        if(k==1){
            res=binary_search(table,aem,counter,1);
        }
        if(res!=-1){
            return res;
        }
    }
    else{
        if(k==0){
            res=linear_search(table,aem,counter,0);
        }
        if(k==1){
             res=linear_search(table,aem,counter,1);
        }
        if(res!=-1){
            return res;
        }
    }
    return -1;
}

int sort(struct std **table,int counter,int k){
    int i,j,comp=0;
    struct std *temp;

    for(i=1;i<counter;i++){
        j=i;
        if(table[j-1]->aem<table[j]->aem){
            comp++;
        }
        while ((j>0)&&(table[j-1]->aem>table[j]->aem)){
            comp++;
            temp=table[j-1];
            table[j-1]=table[j];
            table[j]=temp;
            j--;
        }
    }
    if(k){
        fprintf(stderr, "\n$%d\n", comp);
    }
    return 1;
}

void print(struct std ***hash_table,int **elements,int *len3,double sum,double load_factor,int *len1){
    int i=0,max=0;
    struct std *temp;

    max=(*elements[0]);
    for(i=1;i<(*len3);i++){
        if((*elements)[i]>max){
            max=(*elements)[i];
        }
    }

    printf("\n#");
    printf("\n%d %0.lf %.2lf %d",(*len3),sum,load_factor,max);
    if(max==0&&(*len1)==0){
        printf("\n%d %d\n",0,(*elements)[i]);
        return;
    }
    for(i=0;i<(*len3);i++){
        printf("\n%d %d",i,(*elements)[i]);
        temp=NULL;
        temp=(*hash_table)[i];
        while(temp!=NULL){
            if((*elements)[i]>0){
                printf(" [%lu %s %hu]",temp->aem,temp->name,temp->courses);
                if(temp->nxt==NULL){
                    break;
                }
                temp=temp->nxt;
            }
        }
        printf("\n");
    }
}

int modify(struct std **hash_table,struct std ***table,unsigned long int aem,unsigned short int ncourses,int counter,int res_srt,int *len3){
    int res1,value=0;
    struct std *temp;

    res1=find(table,aem,counter,res_srt,0);
    if(res1!=-1){
        value=hash((*table)[res1]->name)%(*len3);
        temp=hash_table[value];
        while(temp!=NULL){
            if(temp->aem==(*table)[res1]->aem){
                temp->courses=ncourses;
            }
            if(temp->nxt==NULL){
                break;
            }
            temp=temp->nxt;
        }
        (*table)[res1]->courses=ncourses;
        return 1;
    }
    return 0;
}



void list_insert(struct list **array,unsigned short int course_nmr){
    struct list *curr;

    curr=(struct list*)malloc(sizeof(struct list));
    curr->lessons=course_nmr;
    curr->nxt=(*array);
    (*array)=curr;
}

int list_find(struct list **array,unsigned short int course_nmr){
    struct list *curr;

    curr=(*array);
    while(curr!=NULL){
        if(curr->lessons==course_nmr){
            return 1;
        }
        curr=curr->nxt;
    }
    return 0;
}

void list_remove(struct list **array,unsigned short int course_nmr){
    struct list *prev,*curr;

    for(curr=*array;(curr!=NULL)&&(curr->lessons!=course_nmr);prev=curr,curr=curr->nxt);
    if(curr!=NULL){
        if(curr==*array){
            *array=curr->nxt;
        }
        else{
            prev->nxt=curr->nxt;
        }
        free(curr);
    }
}

void list_sort(struct list *array){
    int temp;
    struct list *curr,*index=NULL;

    curr=array;
    while(curr!=NULL){
        index=curr->nxt;
        while(index!=NULL){
            if(curr->lessons>index->lessons){
                temp=curr->lessons;
                curr->lessons=index->lessons;
                index->lessons=temp;
            }
            index=index->nxt;
        }
        curr=curr->nxt;
    }
}

int isreg(struct std **table,unsigned long int aem,int counter,struct list **array,int res_srt,int course_nmr){
    int res1,res2;

    res1=find(&table,aem,counter,res_srt,0);
    if(res1!=-1){
        res2=list_find(&(table[res1]->array),course_nmr);
        if(res2==1){
            return 1;
        }
        else{
            return 0;
        }
    }
    return -1;
}

int reg(struct std **table,unsigned long int aem,int counter,int res_srt,int course_nmr){
    int res1,res2;

    res1=find(&table,aem,counter,res_srt,0);
    if(res1!=-1){
        res2=isreg(table,aem,counter,(&table[res1]->array),res_srt,course_nmr);
        if(res2==1){
            return -1;
        }
        list_insert((&table[res1]->array),course_nmr);
        return 1;
    }
    return -2;
}

int unreg(struct std **table,unsigned long int aem,int counter,int res_srt,int course_nmr){
    int res1,res2;

    res1=find(&table,aem,counter,res_srt,0);
    if(res1!=-1){
        res2=isreg(table,aem,counter,(&table[res1]->array),res_srt,course_nmr);
        if(res2==0){
            return -1;
        }
        list_remove((&table[res1]->array),course_nmr);
        return 1;
    }
    return -2;
}

int list_courses(struct std **table,unsigned long int aem,int counter,int res_srt,struct list **array){
    int res1;
    struct list *curr;

    res1=find(&table,aem,counter,res_srt,0);
    if(res1==-1){
        return 0;
    }

    curr=table[res1]->array;
    list_sort(curr);
    while(curr!=NULL){
        printf("%d\n",curr->lessons);
        curr=curr->nxt;
    }
        return 1;
}

unsigned long hash(char name[]){
    unsigned long hash=5381;
    int c;


    while((c=*name++))
        hash=((hash<<5)+hash)+c;

    return hash;
}

int list2_find(struct std **hash_table,struct std *table,int *len3,char name[]){
    struct std *curr;
    int value=0;

    value=hash(name)%(*len3);
    curr=hash_table[value];
    while(curr!=NULL){
        if(strcmp(curr->name,table->name)==0){
            return 1;
        }
        if(curr->nxt!=NULL){
            curr=curr->nxt;
        }
        else
            break;
    }
    return -1;
}

void list2_insert(struct std **hash_table,struct std *table,int *len3,char *name){
    struct std *curr;
    int value=0;

    curr=(struct std *)malloc(sizeof(struct std));

    value=hash(name)%(*len3);
    curr->aem=table->aem;
    strcpy(curr->name,name);
    curr->courses=table->courses;
    curr->array=table->array;

    if(hash_table[value]==NULL){
        curr->nxt=NULL;
        curr->prv=NULL;
        hash_table[value]=curr;
    }
    else{
        curr->prv=NULL;
        curr->nxt=hash_table[value];
        hash_table[value]->prv=curr;
        hash_table[value]=curr;
    }
}

void list2_remove(struct std **hash_table,struct std *table,int *len3,char *name,int *elements){
    struct std *curr,*temp=NULL;
    int value,i;

    value=hash(name)%(*len3);
    curr=hash_table[value];
    temp=table;
    for(i=1;i<elements[value]&&curr!=NULL;){
        if(strcmp(curr->name,table->name)==0){
            break;
        }
        i++;
        curr=curr->nxt;
    }
    if(i==1){
        if(hash_table[value]==NULL){
            hash_table[value]=NULL;
            return;
        }
        temp=hash_table[value];
        hash_table[value]=hash_table[value]->nxt;
        if(hash_table[value]!=NULL){
            hash_table[value]->prv=NULL;
        }
        free(temp);
        return;
    }
    else if(i==elements[value]){
        temp=curr;
        curr=curr->prv;
        curr->nxt=NULL;
        free(temp);
        return;
    }
    else if(curr!=NULL){
        if(curr->prv!=NULL){
            curr->prv->nxt=curr->nxt;
        }
        if(curr->nxt!=NULL){
            curr->nxt->prv=curr->prv;
        }
        free(curr);
        return;
    }
}

int find_name(struct std **hash_table,char *name,int *len3,int k){
    int value=0,comp=0,flag=0;
    struct std *temp;

    value=hash(name)%(*len3);
    temp=hash_table[value];
    while(temp!=NULL){
        if(strcmp(temp->name,name)==0){
            comp++;
            if(k==1){
                printf("%lu %hu\n",temp->aem,temp->courses);
            }
            flag=1;
        }
        temp=temp->nxt;
    }

    //fprintf(stderr, "\n$%d\n", comp);
    if(flag){
        return 1;
    }
    return 0;
}

int ideal_buckets(int load_factor,int *len3){
    if(load_factor>=4){
        (*len3)=(*len3)*2;
        return (*len3);
    }
    if(load_factor==1){
        (*len3)=(*len3)/2;
    }
    return (*len3);
}

void rehash(struct std ***hash_table,int *len3,int load_factor,int **elements){
    struct std **curr;
    struct std *temp,*next;
    int res,i=0,value,prev_len3;

    prev_len3=(*len3);
    res=ideal_buckets(load_factor,len3);
    curr=calloc(res,sizeof(struct std *));
    *elements=realloc(*elements,res*sizeof(int));

    for(i=0;i<res;i++){
        (*elements)[i]=0;
    }


    for(i=0;i<prev_len3;i++){
        temp=(*hash_table)[i];
        while(temp!=NULL){
            next=temp->nxt;
            value=hash(temp->name)%res;
            list2_insert(curr,temp,&res,temp->name);
            sort_hash(curr,len3,temp->name);
            (*elements)[value]++;
            free(temp);
            temp=next;
        }
    }


    *hash_table=realloc(*hash_table,res*sizeof(struct std *));
    for(i=0;i<res;i++){
        (*hash_table)[i]=curr[i];
    }
    free(temp);
    free(curr);
    prev_len3=res;
}

void sort_hash(struct std **hash_table,int *len3,char *name){
    int swapped,value=0;
    struct std *temp1,*temp2=NULL;
    int aem,courses;
    char temp_name[64];
    struct list *array;

    if(hash_table==NULL){
        return;
    }


    value=hash(name)%(*len3);
    do{
        swapped=0;
        temp1=hash_table[value];
        while(temp1->nxt!=temp2){
            if(temp1->aem>temp1->nxt->aem&&strcmp(temp1->name,temp1->nxt->name)==0){
                aem=temp1->aem;
                strcpy(temp_name,temp1->name);
                courses=temp1->courses;
                array=temp1->array;
                temp1->aem=temp1->nxt->aem;
                strcpy(temp1->name,temp1->nxt->name);
                temp1->courses=temp1->nxt->courses;
                array=temp1->nxt->array;
                temp1->nxt->aem=aem;
                strcpy(temp1->nxt->name,temp_name);
                temp1->nxt->courses=courses;
                temp1->nxt->array=array;
                swapped=1;
            }
            if(strcmp(temp1->name,temp1->nxt->name)>0){
                aem=temp1->aem;
                strcpy(temp_name,temp1->name);
                courses=temp1->courses;
                array=temp1->array;
                temp1->aem=temp1->nxt->aem;
                strcpy(temp1->name,temp1->nxt->name);
                temp1->courses=temp1->nxt->courses;
                array=temp1->nxt->array;
                temp1->nxt->aem=aem;
                strcpy(temp1->nxt->name,temp_name);
                temp1->nxt->courses=courses;
                temp1->nxt->array=array;
                swapped=1;
            }
            temp1=temp1->nxt;
        }
        temp2=temp1;
    } while(swapped!=0);
    return;
}

void hash_insert(struct std ***hash_table,struct std **table,int counter,int *len3,char *name,int **elements,double load_factor){
    int value=0,i,sum=0;

    value=hash(name)%(*len3);
    list2_insert(*hash_table,table[counter],len3,name);
    (*elements)[value]++;
    sort_hash(*hash_table,len3,name);
    for(i=0;i<(*len3);i++){
        sum=sum+(*elements)[i];
    }
    load_factor=sum/(*len3);
    if(load_factor>=4){
        rehash(hash_table,len3,load_factor,elements);
    }
    return;
}

void hash_remove(struct std ***hash_table,struct std *table,int counter,int *len3,char *name,int **elements,double load_factor,char *argv[]){
    int value=0,res,i;
    double len3_temp=(*len3),sum=0;

    value=hash(table->name)%(*len3);
    if((*hash_table)[value]!=NULL){
        res=hash_find(*hash_table,table,len3,name);
        if(res==1){
            list2_remove(*hash_table,table,len3,name,*elements);
            (*elements)[value]--;
            if((*hash_table)[value]!=NULL){
                sort_hash(*hash_table,len3,name);
            }
            for(i=0;i<(*len3);i++){
                sum=sum+(*elements)[i];
            }
            load_factor=sum/len3_temp;
            if(load_factor==1 && (*len3)>atoi(argv[3])){
                rehash(hash_table,len3,load_factor,elements);
            }
        }
        else{
            return;
        }
    }
}

int hash_find(struct std **hash_table,struct std *table,int *len3,char *name){
    int value=0,res;

    value=hash(name)%(*len3);
    while(hash_table[value]!=NULL){
        res=list2_find(hash_table,table,len3,name);
        return res;
    }
    return -1;
}

int main(int argc, char* argv[]){
    int i,len1,len2,len3,res_add,res_srt=0,res_fnd,res_clr=0,res_mdf,res_rmv,res_reg,res_isreg,res_unreg,res_name,counter=0;
    double load_factor=0,sum=0;
    unsigned long int gaem,naem;
    char gname[64],choice;
    unsigned short int gcourses,ncourses,course_nmr;
    struct std **table;
    struct list **array;
    struct std **hash_table;

    len1=atoi(argv[1]);
    len2=atoi(argv[2]);
    len3=atoi(argv[3]);
    int *elements=NULL;
    array=NULL;

    if(len1!=0){
        table=(struct std**)calloc(len1,sizeof(struct std*));
        array=(struct list**)calloc(len1,sizeof(struct list*));
        hash_table=(struct std**)calloc(len3,sizeof(struct std*));
    }
    else{
        table=(struct std**)calloc(len2,sizeof(struct std*));
        array=(struct list**)calloc(len2,sizeof(struct list*));
        hash_table=(struct std**)calloc(len3,sizeof(struct std*));
        len1=len2;
    }

    elements=(int*)malloc(len3*sizeof(int));
    for(i=0;i<len3;i++){
        elements[i]=0;
    }

    do{
        sum=counter;
        load_factor=sum/len3;
        scanf(" %c",&choice);
        switch(choice){
            case 'a':
                scanf(" %lu %s %hu",&gaem,gname,&gcourses);
                capitals(gname);
                res_add=add(&table,&hash_table,&array,gaem,gname,gcourses,&len1,len2,&len3,&counter,res_srt,&elements,load_factor);
                if(res_add==1){
                    res_srt=0;
                    printf("\nA-OK %lu, %d %d\n",gaem,counter,len1);
                }
                else if(res_add==0){
                    res_srt=0;
                    printf("\nA-OK %lu, %d %d\n",gaem,counter,len1);
                }
                else if(res_add==-1){
                    res_srt=0;
                    printf("\nA-NOK %lu, %d %d\n",gaem,counter,len1);
                }
                res_clr=0;
                sum=0;
                break;
            case 'r':
                scanf("%lu",&naem);
                res_rmv=rmv(&table,&hash_table,&array,naem,&counter,&len1,len2,&len3,res_srt,&elements,load_factor,argv);
                if(res_rmv==-1){
                    printf("\nR-NOK %lu, %d %d\n",naem,counter,len1);
                }
                else{
                    res_srt=0;
                    printf("\nR-OK %lu, %d %d\n",naem,counter,len1);
                }
                res_clr=0;
                sum=0;
                break;
            case 'f':
                scanf("%lu",&gaem);
                res_fnd=find(&table,gaem,counter,res_srt,1);
                if(res_fnd!=-1){
                    printf("\nF-OK %s %hu\n",table[res_fnd]->name,table[res_fnd]->courses);
                }
                else{
                   printf("\nF-NOK %lu\n",gaem);
                }
                res_clr=0;
                sum=0;
                break;
            case 'p':
                print(&hash_table,&elements,&len3,sum,load_factor,&len1);
                printf("\n");
                res_clr=0;
                sum=0;
                break;
            case 's':
                res_srt=sort(table,counter,0);
                printf("\nS-OK\n");
                res_clr=0;
                sum=0;
                break;
            case 'c':
                res_clr=clear(&table,&hash_table,&array,&counter,&elements,&len1,&len3,argv);
                printf("\nC-OK\n");
                sum=0;
                break;
            case 'm':
                scanf("%lu %hu",&gaem,&ncourses);
                res_mdf=modify(hash_table,&table,gaem,ncourses,counter,res_srt,&len3);
                if(res_mdf){
                    printf("\nM-OK %lu\n",gaem);
                }
                else{
                    printf("\nM-NOK %lu\n",gaem);
                }
                res_clr=0;
                sum=0;
                break;
            case 'q':
                if(res_clr){
                    free(hash_table);
                    free(table);
                    free(array);
                    free(elements);
                    table=NULL;
 		            hash_table=NULL;
		            array=NULL;
		            elements=NULL;
                    return 0;
                }
                else{
                    clear(&table,&hash_table,&array,&counter,&elements,&len1,&len3,argv);
                    free(hash_table);
                    free(table);
                    free(array);
                    free(elements);
                    table=NULL;
 		            hash_table=NULL;
		            array=NULL;
		            elements=NULL;
                    return 0;
                }
                break;
            case 'g':
                scanf(" %lu %hu",&gaem,&course_nmr);
                res_reg=reg(table,gaem,counter,res_srt,course_nmr);
                if(res_reg==1){
                    printf("\nG-OK %lu %hu\n",gaem,course_nmr);
                    res_srt=0;
                }
                else if(res_reg==-2){
                    printf("\nG-NOK %lu\n",gaem);
                    res_srt=0;
                }
                else if(res_reg==-1){
                    printf("\nG-NOK %hu\n",course_nmr);
                    res_srt=0;
                }
                res_clr=0;
                sum=0;
                break;
            case 'u':
                scanf(" %lu %hu",&gaem,&course_nmr);
                res_unreg=unreg(table,gaem,counter,res_srt,course_nmr);
                if(res_unreg==1){
                    printf("\nU-OK %lu %hu\n",gaem,course_nmr);
                    res_srt=0;
                }
                else if(res_unreg==-2){
                    printf("\nU-NOK %lu\n",gaem);
                    res_srt=0;
                }
                else if(res_unreg==-1){
                    printf("\nU-NOK %hu\n",course_nmr);
                    res_srt=0;
                }
                res_clr=0;
                sum=0;
                break;
            case 'i':
                 scanf(" %lu %hu",&gaem,&course_nmr);
                 res_isreg=isreg(table,gaem,counter,array,res_srt,course_nmr);
                 if(res_isreg==1){
                    printf("\nYES\n");
                    res_srt=0;
                 }
                 if(res_isreg==0){
                    printf("\nNO\n");
                    res_srt=0;
                 }
                 if(res_isreg==-1){
                    printf("\nI-NOK %lu\n",gaem);
                    res_srt=0;
                 }
                 res_clr=0;
                 sum=0;
                break;
            case 'l':
                scanf(" %lu",&gaem);
                res_fnd=find(&table,gaem,counter,res_srt,0);
                if(res_fnd!=-1){
                    printf("\nL-OK %s\n",table[res_fnd]->name);
                    list_courses(table,gaem,counter,res_srt,array);
                    res_srt=0;
                }
                else{
                    printf("\nL-NOK %lu\n",gaem);
                    res_srt=0;
                }
                res_clr=0;
                sum=0;
                break;
            case 'n':
                scanf(" %s",gname);
                capitals(gname);
                res_name=find_name(hash_table,gname,&len3,0);
                if(res_name==1){
                    printf("\nN-OK %s\n",gname);
                    find_name(hash_table,gname,&len3,1);
                    res_srt=0;
                }
                else{
                    printf("\nN-NOK %s\n",gname);
                    res_srt=0;
                }
                res_clr=0;
                sum=0;
                break;
        }

    }while (choice!='q');
    return 0;
}