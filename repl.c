#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	char* buffer;
	size_t buffer_length;
	ssize_t input_length;
}InputBuffer;

typedef enum{
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum{
  PREPARE_SUCCESS,
  PREPARE_UNRECOGNIZED_STATEMENT
}PrepareResult;

typedef enum{
  STATEMENT_INSERT,
  STATEMENT_SELECT
}StatementType;

typedef struct{
  StatementType type;
}Statement;

InputBuffer* new_input_buffer(){
	InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
	input_buffer->buffer = NULL;
	input_buffer->buffer_length = 0;
	input_buffer->input_length = 0;

	return input_buffer;
}

void close_input_buffer(InputBuffer* input_buffer){
	free(input_buffer->buffer);
	free(input_buffer);
}

MetaCommandResult do_meta_command(InputBuffer* input_buffer){
  if(strcmp(input_buffer->buffer,".exit")==0){
    close_input_buffer(input_buffer);
    printf("Have a good day. Goodbye.\n");
    exit(EXIT_SUCCESS);
  }
  else{
    return META_COMMAND_UNRECOGNIZED_COMMAND;
  }
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement){
  if(strncmp(input_buffer->buffer,"insert",6)==0){
    statement->type  = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }
  if(strncmp(input_buffer->buffer,"select",6)==0){ //strncmp compares the first n characters of the input string which is the first parameter to the give const char * viz the second parameter
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }
  return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement){ //going to act like our VM
  switch(statement->type){
    case(STATEMENT_INSERT):
      printf("This is where we would do an insert.\n");
      break;
    case(STATEMENT_SELECT):
      printf("This is where we would do an select.\n");
      break;
  }
}

void print_prompt(){
	printf("karkidb >> ");
}

void read_input(InputBuffer* input_buffer){
	ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

	if(bytes_read<=0){
		printf("Error reading input.\n");
		exit(EXIT_FAILURE);
	}
	input_buffer->input_length = bytes_read - 1;
	input_buffer->buffer[bytes_read - 1] = 0;

}


//*************************** START OF MAIN FUNCTION, REPL ******************************//

int main(){
	InputBuffer* input_buffer = new_input_buffer();
	while(1){
		print_prompt();
		read_input(input_buffer);

    if(input_buffer->buffer[0]=='.'){
      switch(do_meta_command(input_buffer)){
        case(META_COMMAND_SUCCESS):
          continue;
        case(META_COMMAND_UNRECOGNIZED_COMMAND):
          printf("Unrecognized command '%s'\n", input_buffer->buffer);
          continue;
      }
    }

    Statement statement;
    switch(prepare_statement(input_buffer,&statement)){
      case(PREPARE_SUCCESS):
        break;
      case(PREPARE_UNRECOGNIZED_STATEMENT):
        printf("Unrecognized keyword at start of '%s'.\n",input_buffer->buffer);
        continue;
    }
    execute_statement(&statement);
    printf("One statement executed.\n");
	}
}
