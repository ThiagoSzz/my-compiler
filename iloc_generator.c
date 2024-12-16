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