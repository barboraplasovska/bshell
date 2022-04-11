#ifndef CD_H
#define CD_H

#include "builtin.h"

#define   TRUE	1
#define   FALSE	0

struct t_node
{
	char* name;
	int  ptd;
	struct t_node *next_dfile;
	struct t_node *next_file;
};

static struct t_node* create_tree(char *);
void print_tree(struct t_node *);
static struct t_node* create_tnode(char* );
int count;

#endif
