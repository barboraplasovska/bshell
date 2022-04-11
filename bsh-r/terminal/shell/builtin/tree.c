#include "tree.h"

static struct t_node* create_tnode(char* n)
{
	struct t_node *temp=(struct t_node * )malloc(sizeof(struct t_node ));
	temp->name=n;
	temp->next_dfile=NULL;
	temp->next_file=NULL;
return temp;
}

void print_tree(struct t_node *start)
{
    char *s="|-----",*s1="|     ";
	struct t_node *temp=start;
	int i;
    printf("\033[0;33m");
	if(start==NULL)
		return;

	printf("\n");
	for(i=0;i<(count-1);i++){
		printf("%s",s1);   
    }
	if(count>0)
	    printf("%s",s);
    printf("\033[0m");
	printf("%s",temp->name);
        
	if(temp->ptd==TRUE){
		count++;
		print_tree(temp->next_dfile);
		count--;
	}
	print_tree(temp->next_file);

}

static struct t_node* create_tree(char *root_name)
{
	DIR *dir=opendir(root_name);
	//printf("\nrootname : %s",root_name);
	struct dirent *dr={NULL};
	struct t_node *ptr_tstart=NULL,*temp=NULL,*temp1=NULL;
	char *name=(char *)calloc(2000,sizeof(char));


	if(dir==NULL){
		printf("\nFailed to open ..!!");
		printf(" : %s",root_name);
		return NULL;
	}

	while((dr=readdir(dir))!= NULL){
			if(strcmp((dr->d_name),".")!=0 && strcmp((dr->d_name),"..")!=0) 
				temp=create_tnode(dr->d_name);
			else{
				temp=NULL;
				continue;
			}
			
			if(temp1!=NULL){
				temp1->next_file=temp;
			
			}
			else{
				(ptr_tstart)=temp;
			}

			if((dr->d_type) == DT_DIR) {
				temp->ptd=TRUE;
				strcpy(name,root_name);
				(temp->next_dfile)=create_tree((strcat((strcat(name,"/")),dr->d_name)));
				strcpy(name,root_name);
			}
			else{
				(temp)->ptd=FALSE;
				(temp)->next_dfile=NULL;		
			}

		temp1=temp;
	}

    return (ptr_tstart);
}

int main(int args, char** argv)
{
	struct t_node *tstart=create_tnode(".");

	if(args>2){
		exit(EXIT_FAILURE);
        return -1;
	}

	tstart->ptd=TRUE;
	(tstart->next_dfile)=create_tree(((args==2)? argv[1]:getcwd(0,0))); //??
	printf("\033[0;33m");
    printf("[ʕ•́ᴥ•̀ʔっdont fall] :");
    printf("\033[0m"); 
    print_tree(tstart);

return 0;
}
