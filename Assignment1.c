/*
 * Question 1: Basic Arithmetic Calculator
 * Description: This program evaluates a simple arithmetic expression
 * using a stack.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 100

typedef struct {
    int data[MAX_SIZE];
    int top;
} Stack;

void init_stack(Stack *s) {
    s->top = -1;
}

int is_stack_empty(Stack *s) {
    return s->top == -1;
}

int is_stack_full(Stack *s) {
    return s->top == MAX_SIZE - 1;
}

void push(Stack *s, int value) {
    if (is_stack_full(s)) {
        printf("Stack Overflow! Cannot push %d\n", value);
        return;
    }
    s->data[++s->top] = value;
}

int pop(Stack *s) {
    if (is_stack_empty(s)) {
        printf("Stack Underflow! Cannot pop\n");
        return 0;
    }
    return s->data[s->top--];
}

int evaluate_expression(const char *expression, int *error_code) {
    Stack operand_stack;
    init_stack(&operand_stack);

    char last_operator = '+';
    int i = 0;
    int expression_length = strlen(expression);
    int current_number = 0;
    int is_number_being_parsed = 0;

    while (i <= expression_length) {
        char current_char = expression[i];

        if (isdigit(current_char)) {
            current_number = current_number * 10 + (current_char - '0');
            is_number_being_parsed = 1;
        }

        if ((!isdigit(current_char) && current_char != ' ') || i == expression_length) {
            if (is_number_being_parsed == 0 && current_char != ' ' && i != expression_length) {
                *error_code = 1; // Invalid expression
                return 0;
            }

            if (last_operator == '+') {
                push(&operand_stack, current_number);
            } else if (last_operator == '-') {
                push(&operand_stack, -current_number);
            } else if (last_operator == '*') {
                int top_operand = pop(&operand_stack);
                push(&operand_stack, top_operand * current_number);
            } else if (last_operator == '/') {
                if (current_number == 0) {
                    *error_code = 2; // Division by zero
                    return 0;
                }
                int top_operand = pop(&operand_stack);
                push(&operand_stack, top_operand / current_number);
            }

            if (current_char == '+' || current_char == '-' || current_char == '*' || current_char == '/') {
                last_operator = current_char;
            } else if (current_char != ' ' && i != expression_length) {
                *error_code = 1; // Invalid character
                return 0;
            }

            current_number = 0;
            is_number_being_parsed = 0;
        }
        i++;
    }

    int final_result = 0;
    while (!is_stack_empty(&operand_stack)) {
        final_result += pop(&operand_stack);
    }
    return final_result;
}

int main() {
    char input_expression[256];
    printf("Enter expression: ");
    scanf("%[^\n]", input_expression);
    getchar();

    int error_code = 0;
    int result = evaluate_expression(input_expression, &error_code);

    if (error_code == 1) {
        printf("Error: Invalid expression.\n");
    } else if (error_code == 2) {
        printf("Error: Division by zero.\n");
    } else {
        printf("Result: %d\n", result);
    }
    return 0;
}


/*
 * Question 2: User CRUD Operations in a File
 *
 * Description: This program performs Create,Read,Update & Delete
 * operations for user records stored in a text file.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILENAME "users.txt"

typedef struct {
    int id;
    char name[50];
    int age;
} User;

void create_user() {
    User new_user;
    FILE *file = fopen(FILENAME, "a");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter ID: ");
    scanf("%d", &new_user.id);
    getchar(); // Consume the newline character

    printf("Enter Name: ");
    scanf("%[^\n]", new_user.name);
    getchar();

    printf("Enter Age: ");
    scanf("%d", &new_user.age);
    getchar();

    fprintf(file, "%d %s %d\n", new_user.id, new_user.name, new_user.age);
    fclose(file);
    printf("User added.\n");
}

void read_users() {
    User current_user;
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        printf("No users found or file cannot be opened.\n");
        return;
    }

    printf("\n--- User List ---\n");
    printf("ID\tName\t\tAge\n");
    printf("---------------------------\n");
    char line_buffer[100];
    
    while (fgets(line_buffer, sizeof(line_buffer), file) != NULL) {
        if (sscanf(line_buffer, "%d %[a-zA-Z ] %d", &current_user.id, current_user.name, &current_user.age) == 3) {
            printf("%d\t%-15s\t%d\n", current_user.id, current_user.name, current_user.age);
        }
    }
    fclose(file);
}

int find_user_by_id(int id, User *found_user) {
    FILE *file = fopen(FILENAME, "r");
    if (!file) return 0;

    User current_user;
    char line_buffer[100];
    int user_found = 0;

    while (fgets(line_buffer, sizeof(line_buffer), file) != NULL) {
        if (sscanf(line_buffer, "%d", &current_user.id) == 1) {
            if (current_user.id == id) {
                 if (sscanf(line_buffer, "%d %[a-zA-Z ] %d", &current_user.id, current_user.name, &current_user.age) == 3) {
                    if (found_user) {
                        *found_user = current_user;
                    }
                    user_found = 1;
                    break;
                 }
            }
        }
    }

    fclose(file);
    return user_found;
}

void update_user() {
    int id_to_update;
    printf("Enter ID to update: ");
    scanf("%d", &id_to_update);
    getchar();

    User user_to_update;
    if (!find_user_by_id(id_to_update, &user_to_update)) {
        printf("User with ID %d not found.\n", id_to_update);
        return;
    }

    printf("Enter new name (current: %s): ", user_to_update.name);
    scanf("%[^\n]", user_to_update.name);
    getchar();

    printf("Enter new age (current: %d): ", user_to_update.age);
    scanf("%d", &user_to_update.age);
    getchar();

    FILE *original_file = fopen(FILENAME, "r");
    FILE *temp_file = fopen("temp.txt", "w");
    if (!original_file || !temp_file) {
        printf("File error.\n");
        if (original_file) fclose(original_file);
        if (temp_file) fclose(temp_file);
        return;
    }

    User user_from_file;
    char line_buffer[100];
    while (fgets(line_buffer, sizeof(line_buffer), original_file) != NULL) {
         if (sscanf(line_buffer, "%d", &user_from_file.id) == 1) {
            if (user_from_file.id == id_to_update) {
                fprintf(temp_file, "%d %s %d\n", user_to_update.id, user_to_update.name, user_to_update.age);
            } else {
                fputs(line_buffer, temp_file);
            }
         }
    }

    fclose(original_file);
    fclose(temp_file);

    remove(FILENAME);
    rename("temp.txt", FILENAME);
    printf("User updated.\n");
}

void delete_user() {
    int id_to_delete;
    printf("Enter ID to delete: ");
    scanf("%d", &id_to_delete);
    getchar();

    if (!find_user_by_id(id_to_delete, NULL)) {
        printf("User with ID %d not found.\n", id_to_delete);
        return;
    }

    FILE *original_file = fopen(FILENAME, "r");
    FILE *temp_file = fopen("temp.txt", "w");
    if (!original_file || !temp_file) {
        printf("File error.\n");
        if (original_file) fclose(original_file);
        if (temp_file) fclose(temp_file);
        return;
    }

    User user_from_file;
    char line_buffer[100];
    while (fgets(line_buffer, sizeof(line_buffer), original_file) != NULL) {
        if (sscanf(line_buffer, "%d", &user_from_file.id) == 1) {
            if (user_from_file.id != id_to_delete) {
                fputs(line_buffer, temp_file);
            }
        }
    }

    fclose(original_file);
    fclose(temp_file);

    remove(FILENAME);
    rename("temp.txt", FILENAME);
    printf("User removed.\n");
}

int main() {
    int menu_choice;
    while (1) {
        printf("\nUser CRUD Menu:\n");
        printf("1. Add User\n");
        printf("2. List Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &menu_choice);
        getchar();

        switch (menu_choice) {
            case 1: create_user(); break;
            case 2: read_users(); break;
            case 3: update_user(); break;
            case 4: delete_user(); break;
            case 5: return 0;
            default: printf("Invalid choice.\n");
        }
    }
}