/*---------------------------*/
/* -->>     Grupo I     <<-- */
/*        Thiago Haab        */
/*      Henrique Vazatta     */
/*---------------------------*/

#include "iloc_generator.h"

// returns unique code label
int get_label()
{
  static int label_count = 1;
  return label_count++;
}

// returns unique register identifier
int get_register()
{
  static int register_count = 1;
  return register_count++;
}

// creates operation with default values based on type
Operation create_operation_from_type(OperationsEnum type)
{
  Operation operation;
  operation.type = type;
  operation.label = -1;
  operation.op1 = -1;
  operation.op2 = -1;
  operation.out1 = -1;
  operation.out2 = -1;
  return operation;
}

// creates operation with default values based on type and label
Operation create_operation_from_type_and_label(OperationsEnum type, int label)
{
  Operation operation = create_operation_from_type(type);
  operation.label = label;
  return operation;
}

// creates operation with specified values
Operation create_operation(OperationsEnum type, int op1, int op2, int out1, int out2)
{
  Operation operation = create_operation_from_type(type);
  operation.op1 = op1;
  operation.op2 = op2;
  operation.out1 = out1;
  operation.out2 = out2;
  return operation;
}

// creates operation with specified values and label
Operation create_operation_from_label(OperationsEnum type, int label, int op1, int op2, int out1, int out2)
{
  Operation operation = create_operation(type, op1, op2, out1, out2);
  operation.label = label;
  return operation;
}

// creates empty operation list
OperationList *create_operation_list()
{
  OperationList *operation_list = malloc(sizeof(OperationList));

  if (!operation_list)
  {
    printf("failed to create operation list\n");
    return NULL;
  }

  operation_list->operation = create_operation_from_type(OPERATION_UNDEFINED);
  operation_list->next_operation_list = NULL;
  return operation_list;
}

// generates code for given operation via printf
void map_operation_to_instruction(Operation operation)
{
  if (operation.type != OPERATION_UNDEFINED && operation.label != -1)
  {
    printf("l%d: ", operation.label);
  }

  switch (operation.type)
  {
  // does nothing
  case OPERATION_NOP:
    printf("nop \n");
    break;
  // r3 = r1 * r2
  case OPERATION_MULT:
    printf("mult r%d, r%d => r%d \n", operation.op1, operation.op2, operation.out1);
    break;
  // r3 = r1 / r2
  case OPERATION_DIV:
    printf("div r%d, r%d => r%d \n", operation.op1, operation.op2, operation.out1);
    break;
  // r3 = 0 - r1 (negate)
  case OPERATION_NEG:
    printf("rsubI r%d, 0 => r%d \n", operation.op1, operation.out1);
    break;
  // r3 = r1 xor -1 (logical negation)
  case OPERATION_NOT:
    printf("xorI r%d, -1 => r%d \n", operation.op1, operation.out1);
    break;
  // r3 = r1 - r2
  case OPERATION_SUB:
    printf("sub r%d, r%d => r%d \n", operation.op1, operation.op2, operation.out1);
    break;
  // r3 = r1 + r2
  case OPERATION_ADD:
    printf("add r%d, r%d => r%d \n", operation.op1, operation.op2, operation.out1);
    break;
  // r3 = r1 && r2
  case OPERATION_AND:
    printf("and r%d, r%d => r%d \n", operation.op1, operation.op2, operation.out1);
    break;
  // r3 = r1 || r2
  case OPERATION_OR:
    printf("or r%d, r%d => r%d \n", operation.op1, operation.op2, operation.out1);
    break;
  // r3 = true if r1 >= r2, else false
  case OPERATION_CMP_GE:
    printf("cmp_GE r%d, r%d -> r%d \n", operation.op1, operation.op2, operation.out1);
    break;
  // r3 = true if r1 <= r2, else false
  case OPERATION_CMP_LE:
    printf("cmp_LE r%d, r%d -> r%d \n", operation.op1, operation.op2, operation.out1);
    break;
  // r3 = true if r1 > r2, else false
  case OPERATION_CMP_GT:
    printf("cmp_GT r%d, r%d -> r%d \n", operation.op1, operation.op2, operation.out1);
    break;
  // r3 = true if r1 < r2, else false
  case OPERATION_CMP_LT:
    printf("cmp_LT r%d, r%d -> r%d \n", operation.op1, operation.op2, operation.out1);
    break;
  // r3 = true if r1 != r2, else false
  case OPERATION_CMP_NE:
    printf("cmp_NE r%d, r%d -> r%d \n", operation.op1, operation.op2, operation.out1);
    break;
  // r3 = true if r1 == r2, else false
  case OPERATION_CMP_EQ:
    printf("cmp_EQ r%d, r%d -> r%d \n", operation.op1, operation.op2, operation.out1);
    break;
  // PC = address(l2) if r1 is true, else PC = address(l3)
  case OPERATION_CBR:
    printf("cbr r%d -> l%d, l%d \n", operation.op1, operation.out1, operation.out2);
    break;
  // PC = address(l1)
  case OPERATION_JUMPI:
    printf("jumpI -> l%d \n", operation.op1);
    break;
  // r2 = c1
  case OPERATION_LOADI:
    printf("loadI %d => r%d \n", operation.op1, operation.out1);
    break;
  // r2 = Memory(rbss + c1)
  case OPERATION_LOADAI_GLOBAL:
    printf("loadAI rbss, %d => r%d \n", operation.op1, operation.out1);
    break;
  // r2 = Memory(rfp + c1)
  case OPERATION_LOADAI_LOCAL:
    printf("loadAI rfp, %d => r%d \n", operation.op1, operation.out1);
    break;
  // Memory(rbss + c1) = r1
  case OPERATION_STOREAI_GLOBAL:
    printf("storeAI r%d => rbss, %d \n", operation.op1, operation.out1);
    break;
  // Memory(rfp + c1) = r1
  case OPERATION_STOREAI_LOCAL:
    printf("storeAI r%d => rfp, %d \n", operation.op1, operation.out1);
    break;
  default:
    break;
  }
}

// generates code for list of operations
void generate_code(OperationList *operation_list)
{
  OperationList *next_operation_list = operation_list;
  while (next_operation_list != NULL)
  {
    Operation operation = next_operation_list->operation;
    map_operation_to_instruction(operation);
    next_operation_list = next_operation_list->next_operation_list;
  }
}

// generate code for if statement
OperationList *generate_if_statement_code(struct Node *expression_node, struct Node *block_node)
{
  int expression_register = expression_node->output_register;
  int if_false_register = get_register();
  int cmp_register = get_register();

  int if_true_label = get_label();
  int if_false_label = get_label();

  OperationList *operation_list = insert_new_operation_list(expression_node->operation_list);

  Operation if_false_operation = create_operation(OPERATION_LOADI, 0, -1, if_false_register, -1);
  insert_operation_into_list(operation_list, if_false_operation);

  Operation cmp_operation = create_operation(OPERATION_CMP_NE, expression_register, if_false_register, cmp_register, -1);
  insert_operation_into_list(operation_list, cmp_operation);

  Operation branch_operation = create_operation(OPERATION_CBR, cmp_register, -1, if_true_label, if_false_label);
  insert_operation_into_list(operation_list, branch_operation);

  Operation if_true_label_operation = create_operation_from_type_and_label(OPERATION_NOP, if_true_label);
  insert_operation_into_list(operation_list, if_true_label_operation);

  if (block_node)
  {
    append_operation_list(operation_list, block_node->operation_list);
  }

  Operation if_false_label_operation = create_operation_from_type_and_label(OPERATION_NOP, if_false_label);
  insert_operation_into_list(operation_list, if_false_label_operation);

  return operation_list;
}

// generate code for if with mandatory else statement
OperationList *generate_if_else_statement_code(struct Node *expression_node, struct Node *if_block_node, struct Node *else_block_node)
{
  int expression_register = expression_node->output_register;
  int if_false_register = get_register();
  int cmp_register = get_register();

  int if_true_label = get_label();
  int else_label = get_label();
  int end_if_label = get_label();

  OperationList *operation_list = insert_new_operation_list(expression_node->operation_list);

  Operation if_false_operation = create_operation(OPERATION_LOADI, 0, -1, if_false_register, -1);
  insert_operation_into_list(operation_list, if_false_operation);

  Operation cmp_operation = create_operation(OPERATION_CMP_NE, expression_register, if_false_register, cmp_register, -1);
  insert_operation_into_list(operation_list, cmp_operation);

  Operation branch_operation = create_operation(OPERATION_CBR, cmp_register, -1, if_true_label, else_label);
  insert_operation_into_list(operation_list, branch_operation);

  Operation if_true_label_operation = create_operation_from_type_and_label(OPERATION_NOP, if_true_label);
  insert_operation_into_list(operation_list, if_true_label_operation);

  if (if_block_node)
  {
    append_operation_list(operation_list, if_block_node->operation_list);
  }

  Operation end_if_operation = create_operation(OPERATION_JUMPI, end_if_label, -1, -1, -1);
  insert_operation_into_list(operation_list, end_if_operation);

  Operation else_label_operation = create_operation_from_type_and_label(OPERATION_NOP, else_label);
  insert_operation_into_list(operation_list, else_label_operation);

  if (else_block_node)
  {
    append_operation_list(operation_list, else_block_node->operation_list);
  }

  Operation end_if_label_operation = create_operation_from_type_and_label(OPERATION_NOP, end_if_label);
  insert_operation_into_list(operation_list, end_if_label_operation);

  return operation_list;
}

// generate code for while statement
OperationList *generate_while_statement_code(struct Node *expression_node, struct Node *block_node)
{
  int expression_register = expression_node->output_register;
  int if_false_register = get_register();
  int cmp_register = get_register();

  int loop_comparison_label = get_label();
  int if_true_label = get_label();
  int if_false_label = get_label();

  OperationList *operation_list = create_operation_list();

  Operation if_false_operation = create_operation(OPERATION_LOADI, 0, -1, if_false_register, -1);
  insert_operation_into_list(operation_list, if_false_operation);

  Operation end_loop_comparison_operation = create_operation_from_type_and_label(OPERATION_NOP, loop_comparison_label);
  insert_operation_into_list(operation_list, end_loop_comparison_operation);

  append_operation_list(operation_list, expression_node->operation_list);

  Operation cmp_operation = create_operation(OPERATION_CMP_NE, expression_register, if_false_register, cmp_register, -1);
  insert_operation_into_list(operation_list, cmp_operation);

  Operation branch_operation = create_operation(OPERATION_CBR, cmp_register, -1, if_true_label, if_false_label);
  insert_operation_into_list(operation_list, branch_operation);

  Operation if_true_label_operation = create_operation_from_type_and_label(OPERATION_NOP, if_true_label);
  insert_operation_into_list(operation_list, if_true_label_operation);

  if (block_node)
  {
    append_operation_list(operation_list, block_node->operation_list);
  }

  Operation end_loop_operation = create_operation(OPERATION_JUMPI, loop_comparison_label, -1, -1, -1);
  insert_operation_into_list(operation_list, end_loop_operation);

  Operation if_false_label_operation = create_operation_from_type_and_label(OPERATION_NOP, if_false_label);
  insert_operation_into_list(operation_list, if_false_label_operation);

  return operation_list;
}

// generate code for unary logic expression (-, !)
OperationList *generate_unary_expression_code(OperationsEnum operation_type, struct Node *node, int *output_register)
{
  OperationList *operation_list = insert_new_operation_list(node->operation_list);

  int r1 = node->output_register;
  int r2 = get_register();

  Operation operation = create_operation(operation_type, r1, -1, r2, -1);
  insert_operation_into_list(operation_list, operation);

  *output_register = r2;
  return operation_list;
}

// generate code for binary logic expression (&, |)
OperationList *generate_binary_expression_code(OperationsEnum operation_type, struct Node *left_node, struct Node *right_node, int *output_register)
{
  OperationList *operation_list = merge_operation_list(left_node->operation_list, right_node->operation_list);

  int r1 = left_node->output_register;
  int r2 = right_node->output_register;
  int r3 = get_register();

  Operation operation = create_operation(operation_type, r1, r2, r3, -1);
  insert_operation_into_list(operation_list, operation);

  *output_register = r3;
  return operation_list;
}

// generate code for comparison expression (>=, <=, >, <, !=, ==)
OperationList *generate_comparison_expression_code(OperationsEnum operation_type, struct Node *left_node, struct Node *right_node, int *output_register)
{
  OperationList *operation_list = merge_operation_list(left_node->operation_list, right_node->operation_list);

  int r1 = left_node->output_register;
  int r2 = right_node->output_register;
  int r3 = get_register();
  int r4 = get_register();
  int if_true_label = get_label();
  int if_false_label = get_label();
  int end_if_label = get_label();

  Operation cmp_operation = create_operation(operation_type, r1, r2, r3, -1);
  insert_operation_into_list(operation_list, cmp_operation);

  Operation branch_operation = create_operation(OPERATION_CBR, r3, -1, if_true_label, if_false_label);
  insert_operation_into_list(operation_list, branch_operation);

  Operation if_true_operation = create_operation_from_label(OPERATION_LOADI, if_true_label, 1, -1, r4, -1);
  insert_operation_into_list(operation_list, if_true_operation);

  Operation end_if_operation = create_operation(OPERATION_JUMPI, end_if_label, -1, -1, -1);
  insert_operation_into_list(operation_list, end_if_operation);

  Operation if_false_operation = create_operation_from_label(OPERATION_LOADI, if_false_label, 0, -1, r4, -1);
  insert_operation_into_list(operation_list, if_false_operation);

  Operation end_if_label_operation = create_operation_from_type_and_label(OPERATION_NOP, end_if_label);
  insert_operation_into_list(operation_list, end_if_label_operation);

  *output_register = r4;
  return operation_list;
}

// generate code for arithmetic expression (+, -, *, /)
OperationList *generate_arithmetic_expression_code(OperationsEnum operation_type, struct Node *left_node, struct Node *right_node, int *output_register)
{
  OperationList *operation_list = merge_operation_list(left_node->operation_list, right_node->operation_list);

  int r1 = left_node->output_register;
  int r2 = right_node->output_register;
  int r3 = get_register();

  Operation operation = create_operation(operation_type, r1, r2, r3, -1);
  insert_operation_into_list(operation_list, operation);

  *output_register = r3;
  return operation_list;
}

// generate code for load identifier (global/local)
OperationList *generate_load_identifier_code(SymbolTableItemValue value, int *output_register)
{
  OperationList *operation_list = create_operation_list();

  int address = value.position;
  int r1 = get_register();

  Operation operation;

  if (value.is_global)
  {
    operation = create_operation(OPERATION_LOADAI_GLOBAL, address, -1, r1, -1);
  }
  else
  {
    operation = create_operation(OPERATION_LOADAI_LOCAL, address, -1, r1, -1);
  }

  insert_operation_into_list(operation_list, operation);

  *output_register = r1;
  return operation_list;
}

// generate code for store identifier (global/local)
OperationList *generate_store_identifier_code(SymbolTableItemValue value, struct Node *expression_node)
{
  OperationList *operation_list = insert_new_operation_list(expression_node->operation_list);

  int address = value.position;
  int r1 = expression_node->output_register;

  Operation operation;

  if (value.is_global)
  {
    operation = create_operation(OPERATION_STOREAI_GLOBAL, r1, -1, address, -1);
  }
  else
  {
    operation = create_operation(OPERATION_STOREAI_LOCAL, r1, -1, address, -1);
  }

  insert_operation_into_list(operation_list, operation);

  return operation_list;
}

// inserts new operation list into existing one
OperationList *insert_new_operation_list(OperationList *operation_list)
{
  OperationList *new_operation_list = create_operation_list();

  if (!new_operation_list)
    return NULL;

  OperationList *current_list = operation_list;
  while (current_list != NULL)
  {
    insert_operation_into_list(new_operation_list, current_list->operation);
    current_list = current_list->next_operation_list;
  }
  return new_operation_list;
}

// inserts operation into operation list
void insert_operation_into_list(OperationList *operation_list, Operation operation)
{
  if (operation_list == NULL)
  {
    printf("failed to insert because operation list is empty\n");
    return;
  }

  OperationList *last_operation_list = operation_list;
  while (last_operation_list->next_operation_list != NULL)
  {
    last_operation_list = last_operation_list->next_operation_list;
  }

  OperationList *new_operation_list = create_operation_list();
  new_operation_list->operation = operation;
  last_operation_list->next_operation_list = new_operation_list;
}

// appends operation list to another
void append_operation_list(OperationList *current_list, OperationList *new_list)
{
  if (current_list == NULL || new_list == NULL)
  {
    printf("failed to append because operation list is empty\n");
    return;
  }

  OperationList *temp_operation_list = new_list;
  while (temp_operation_list != NULL)
  {
    insert_operation_into_list(current_list, temp_operation_list->operation);
    temp_operation_list = temp_operation_list->next_operation_list;
  }
}

// merges two operation lists into new operation list
OperationList *merge_operation_list(OperationList *current_list, OperationList *new_list)
{
  if (current_list == NULL || new_list == NULL)
  {
    printf("failed to merge because operation list is empty\n");
    return NULL;
  }

  OperationList *new_operation_list = create_operation_list();

  append_operation_list(new_operation_list, current_list);
  append_operation_list(new_operation_list, new_list);

  return new_operation_list;
}

// free operation list
void free_operation_list(OperationList *operation_list)
{
  OperationList *current_list = operation_list;
  while (current_list != NULL)
  {
    OperationList *next_list = current_list->next_operation_list;
    free(current_list);
    current_list = next_list;
  }
}