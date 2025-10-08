// problem 1 -- -- -- Calculator 
//
//
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

typedef struct {
    int data[MAX];
    int top;
} Stack;

void init(Stack *s) {
    s->top = -1;
}

int empty(Stack *s) {
    return s->top == -1;
}

int full(Stack *s) {
    return s->top == MAX - 1;
}

void push(Stack *s, int x) {
    if (full(s)) {
        printf("Stack Overflow! Cannot push %d\n", x);
        return;
    }
    s->data[++s->top] = x;
}

int pop(Stack *s) {
    if (empty(s)) {
        printf("Stack Underflow! Cannot pop\n");
        return 0;
    }
    return s->data[s->top--];
}

int peek(Stack *s) {
    if (empty(s)) {
        printf("Stack Empty! No element to peek\n");
        return 0;
    }
    return s->data[s->top];
}

int eval(const char *expr, int *error) {
    Stack s; init(&s);
    char lastOp = '+';
    int i = 0, n = strlen(expr), curr = 0, hasNum = 0;
    while (i <= n) {
        char ch = expr[i];
        if (isdigit(ch)) {
            curr = curr * 10 + (ch - '0');
            hasNum = 1;
        }
        if ((!isdigit(ch) && ch != ' ') || i == n) {
            if (hasNum == 0 && ch != ' ' && i != n) { *error = 1; return 0; }
            if (lastOp == '+') push(&s, curr);
            else if (lastOp == '-') push(&s, -curr);
            else if (lastOp == '*') { int t = pop(&s); push(&s, t * curr); }
            else if (lastOp == '/') {
                if (curr == 0) { *error = 2; return 0; }
                int t = pop(&s); push(&s, t / curr);
            }
            if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
                lastOp = ch;
            else if (ch != ' ' && i != n) { *error = 1; return 0; }
            curr = 0;
            hasNum = 0;
        }
        ++i;
    }
    int res = 0;
    while (!empty(&s)) res += pop(&s);
    return res;
}

int main() {
    char expr[256];
    printf("Enter expression: ");
    scanf("%[^\n]", expr);
    int error = 0;
    int ans = eval(expr, &error);
    if (error == 1) printf("Error: Invalid expression.\n");
    else if (error == 2) printf("Error: Division by zero.\n");
    else printf("%d\n", ans);
    return 0;
}




// problem 2 --- CRUD ops in file

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILENAME "users.txt"

typedef struct {
    int id;
    char name[50];
    int age;
} User;

void trimNewline(char *str) {
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

void createUser() {
    User u;
    FILE *f = fopen(FILENAME, "a");
    if (!f) {
        printf("Error opening file!\n");
        return;
    }
    printf("Enter ID: ");
    scanf("%d", &u.id);
    getchar();

    printf("Enter Name: ");
    scanf("%[^\n]", u.name);
    getchar();

    printf("Enter Age: ");
    scanf("%d", &u.age);
    getchar();

    fprintf(f, "%d %s %d\n", u.id, u.name, u.age);
    fclose(f);
    printf("User added.\n");
}

void readUsers() {
    User u;
    FILE *f = fopen(FILENAME, "r");
    if (!f) {
        printf("No users found.\n");
        return;
    }
    printf("ID\tName\tAge\n");
    while (fscanf(f, "%d", &u.id) == 1) {
        char line[100];
        if (!fgets(line, sizeof(line), f)) break;

        trimNewline(line);

        char *lastSpace = strrchr(line, ' ');
        if (lastSpace) {
            u.age = atoi(lastSpace + 1);
            *lastSpace = '\0';
            strncpy(u.name, line, sizeof(u.name));
            u.name[sizeof(u.name) - 1] = '\0';
        } else {
            strcpy(u.name, "");
            u.age = 0;
        }

        printf("%d\t%s\t%d\n", u.id, u.name, u.age);
    }
    fclose(f);
}

int userExists(int id, User *user) {
    FILE *f = fopen(FILENAME, "r");
    if (!f) return 0;

    User u;
    while (fscanf(f, "%d", &u.id) == 1) {
        char line[100];
        if (!fgets(line, sizeof(line), f)) break;

        trimNewline(line);

        char *lastSpace = strrchr(line, ' ');
        if (lastSpace) {
            u.age = atoi(lastSpace + 1);
            *lastSpace = '\0';
            strncpy(u.name, line, sizeof(u.name));
            u.name[sizeof(u.name) - 1] = '\0';
        } else {
            strcpy(u.name, "");
            u.age = 0;
        }

        if (u.id == id) {
            if (user) *user = u;
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void updateUser() {
    int id;
    printf("Enter ID to update: ");
    scanf("%d", &id);
    getchar();

    User u;
    if (!userExists(id, &u)) {
        printf("User with ID %d not found.\n", id);
        return;
    }

    printf("Enter new name: ");
    scanf("%[^\n]", u.name);
    getchar();

    printf("Enter new age: ");
    scanf("%d", &u.age);
    getchar();

    FILE *f = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!f || !temp) {
        printf("File error.\n");
        if (f) fclose(f);
        if (temp) fclose(temp);
        return;
    }

    User tempUser;
    while (fscanf(f, "%d", &tempUser.id) == 1) {
        char line[100];
        if (!fgets(line, sizeof(line), f)) break;

        trimNewline(line);

        char *lastSpace = strrchr(line, ' ');
        if (lastSpace) {
            tempUser.age = atoi(lastSpace + 1);
            *lastSpace = '\0';
            strncpy(tempUser.name, line, sizeof(tempUser.name));
            tempUser.name[sizeof(tempUser.name) - 1] = '\0';
        } else {
            strcpy(tempUser.name, "");
            tempUser.age = 0;
        }

        if (tempUser.id == id)
            fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
        else
            fprintf(temp, "%d %s %d\n", tempUser.id, tempUser.name, tempUser.age);
    }

    fclose(f);
    fclose(temp);

    remove(FILENAME);
    rename("temp.txt", FILENAME);
    printf("User updated.\n");
}

void deleteUser() {
    int id;
    printf("Enter ID to delete: ");
    scanf("%d", &id);
    getchar();

    User dummy;
    if (!userExists(id, NULL)) {
        printf("User with ID %d not found.\n", id);
        return;
    }

    FILE *f = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!f || !temp) {
        printf("File error.\n");
        if (f) fclose(f);
        if (temp) fclose(temp);
        return;
    }

    User u;
    while (fscanf(f, "%d", &u.id) == 1) {
        char line[100];
        if (!fgets(line, sizeof(line), f)) break;

        trimNewline(line);

        char *lastSpace = strrchr(line, ' ');
        if (lastSpace) {
            u.age = atoi(lastSpace + 1);
            *lastSpace = '\0';
            strncpy(u.name, line, sizeof(u.name));
            u.name[sizeof(u.name) - 1] = '\0';
        } else {
            strcpy(u.name, "");
            u.age = 0;
        }

        if (u.id != id)
            fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
    }

    fclose(f);
    fclose(temp);

    remove(FILENAME);
    rename("temp.txt", FILENAME);
    printf("User removed.\n");
}

int main() {
    int choice;
    while (1) {
        printf("\nUser CRUD Menu:\n");
        printf("1. Add User\n");
        printf("2. List Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: createUser(); break;
            case 2: readUsers(); break;
            case 3: updateUser(); break;
            case 4: deleteUser(); break;
            case 5: return 0;
            default: printf("Invalid choice.\n");
        }
    }
}