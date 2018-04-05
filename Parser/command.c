#include"util.h"

//command entry functions
struct command_entry *create_cmd_entry(char *cmd_name){
    if(cmd_name == NULL){
        fprintf(stderr,"invalid argument in %s at %d\n",__FILE__,__LINE__);
        return NULL;
    }
    struct command_entry *this = (struct command_entry *)malloc(sizeof(struct command_entry));
    if(this == NULL){
        fprintf(stderr,"Malloc failed in %s at line %d\n",__FILE__,__LINE__);
        return NULL;
    }
    this->availableArgs = DEFAULT_ARGS; //setting a default of 5 args
    this->numArgs = 0;
    this->arglist = (char **)malloc(this->availableArgs * sizeof(char *));
    if(this->arglist == NULL){
        fprintf(stderr,"Malloc failed in %s at line %d\n",__FILE__,__LINE__);
        return NULL;
    }
    this->command_name = (char *)malloc(sizeof(cmd_name));
    if(this->command_name == NULL){
        fprintf(stderr,"Malloc failed in %s at line %d\n",__FILE__,__LINE__);
        return NULL;
    }
    strcpy(this->command_name,cmd_name);
    return this;
}

int add_argument(struct command_entry *this,char *arg){
    if(this == NULL){
        fprintf(stderr,"invalid argument in %s at %d\n",__FILE__,__LINE__);
        return 0;
    }
    if(arg == NULL){
        fprintf(stderr,"invalid argument in %s at %d\n",__FILE__,__LINE__);
        return 0;
    }
    if(this->command_name == NULL){
        fprintf(stderr,"Initialize before use error: %s at %d\n",__FILE__,__LINE__);
        return 0;
    }
    if(this->arglist == NULL){
        fprintf(stderr,"Initialize before use error: %s at %d\n",__FILE__,__LINE__);
        return 0;
    }
    if(this->numArgs == this->availableArgs){
        this->availableArgs *= 2;
        this->arglist = realloc(this->arglist,this->availableArgs * sizeof(char *));
    }
    if((this->arglist[this->numArgs] = (char *)malloc(sizeof(arg))) == NULL){
        fprintf(stderr,"Malloc failed in %s at line %d\n",__FILE__,__LINE__);
        return 0;
    }
    strcpy(this->arglist[this->numArgs],arg);
    this->numArgs++;
    return 1;
}

int delete_cmd_entry(struct command_entry *this){
    free(this->command_name);
    for(int i=0;i<this->numArgs;i++)
        free(this->arglist[i]);
    free(this->arglist);
    free(this);
    return 1;
}

//command line functions
struct command_line *create_cmd_line(){
    struct command_line *this;
    if((this = (struct command_line *)malloc(sizeof(struct command_line ))) == NULL){
        fprintf(stderr,"Malloc failed in %s at line %d\n",__FILE__,__LINE__);
        return NULL;
    }
    this->available_command_entries = DEFAULT_ENTRIES;
    this->num_command_entries = 0;
    this->infile = NULL;
    this->outfile = NULL;
    this->appendMode = FALSE;
    this->isBackground = FALSE;
    if((this->command_entry_list = (struct command_entry **)malloc(this->available_command_entries * sizeof(struct command_entry *))) == NULL){
        fprintf(stderr,"Malloc failed in %s at line %d\n",__FILE__,__LINE__);
        return NULL;
    }
    return this;
}

int add_cmd_entry(struct command_line *this,struct command_entry *arg){
    if(this == NULL){
        fprintf(stderr,"Failed to initialize cmd_line in %s at line %d\n",__FILE__,__LINE__);
        return 0;
    }
    if(arg == NULL){
        fprintf(stderr,"Invalid argument in %s at line %d\n",__FILE__,__LINE__);
        return 0;
    }
    if(this->command_entry_list == NULL){
        fprintf(stderr,"Initialize before use error: %s at %d\n",__FILE__,__LINE__);
        return 0;
    }
    if(arg->command_name == NULL){
        fprintf(stderr,"Initialize before use error: %s at %d\n",__FILE__,__LINE__);
        return 0;
    }
    if(arg->arglist == NULL){
        fprintf(stderr,"Initialize before use error: %s at %d\n",__FILE__,__LINE__);
        return 0;
    }
    if(this->num_command_entries == this->available_command_entries){
        this->available_command_entries *= 2;
        this->command_entry_list = realloc(this->command_entry_list,this->available_command_entries * sizeof(struct command_entry));
        if(this->command_entry_list == NULL){
            fprintf(stderr,"Realloc failed in %s at line %d\n",__FILE__,__LINE__);
            return 0; 
        }
    }
    this->command_entry_list[this->num_command_entries] = arg;
    this->num_command_entries++;
    return 1;    
}

int set_params(struct command_line *this,char *infile,char *outfile,bool appendMode,bool isBackground){
    if(this == NULL){
        fprintf(stderr,"Initialize before use error: %s at %d\n",__FILE__,__LINE__);
        return 0;
    }
    if(infile != NULL){
        if((this->infile = (char *)malloc(sizeof(infile))) == NULL){
            fprintf(stderr,"Malloc failed in %s at line %d\n",__FILE__,__LINE__);
            return 0;
        }
        strcpy(this->infile,infile);
    }
    if(outfile != NULL){
        if((this->outfile = (char *)malloc(sizeof(outfile))) == NULL){
            fprintf(stderr,"Malloc failed in %s at line %d\n",__FILE__,__LINE__);
            return 0;
        }
        strcpy(this->outfile,outfile);
    }
    this->appendMode = appendMode;
    this->isBackground = isBackground;
    return 1;

}

int delete_cmd_line(struct command_line *this){
    for(int i=0;i<this->num_command_entries;i++){
        delete_cmd_entry(this->command_entry_list[i]);
    }
    if(this->infile != NULL)
        free(this->infile);
    if(this->outfile != NULL)
        free(this->outfile);
    free(this);
    return 1;
}

//command table functions
struct command_table *initialize_command_table(){
    struct command_table *this;
    if((this = (struct command_table *)malloc(sizeof(struct command_table))) == NULL){
        fprintf(stderr,"Malloc failed in %s at line %d\n",__FILE__,__LINE__);
        return NULL;
    }
    this->available_args = DEFAULT_LINES;
    this->num_args = 0;
    if((this->cmd_line_list = (struct command_line *)malloc(this->available_args * sizeof(struct command_line *))) == NULL){
        fprintf(stderr,"Malloc failed in %s at line %d\n",__FILE__,__LINE__);
        return NULL;
    }
    return this;
}

int add_cmd_line(struct command_table *table,struct command_line *line){
    if(table == NULL){
        fprintf(stderr,"Initialize before use error: %s at %d\n",__FILE__,__LINE__);
        return 0;
    }
    if(line == NULL){
        fprintf(stderr,"Initialize before use error: %s at %d\n",__FILE__,__LINE__);
        return 0;
    }
    if(table->cmd_line_list == NULL){
        fprintf(stderr,"Initialize before use error: %s at %d\n",__FILE__,__LINE__);
        return 0;
    }
    if(table->num_args == table->available_args){
        table->available_args *= 2;
        table->cmd_line_list = realloc(table->cmd_line_list,table->available_args * sizeof(struct command_line *));
        if(table->cmd_line_list == NULL){
            fprintf(stderr,"Malloc failed in %s at %d\n",__FILE__,__LINE__);
            return 0;
        }
    }
    table->cmd_line_list[table->num_args] = line;
    table->num_args++;
    return 1;
}

int delete_cmd_table(struct command_table *this){
    for(int i=0;i<this->num_args;i++){
        delete_cmd_line(this->cmd_line_list[i]);
    }
    free(this);
    return 1;
}