// Найти вершины, для которых высота левого поддерева не равна высоте правого поддерева.

#include <stdio.h> 
#include <stdlib.h> 
#include <conio.h> 
#include <limits.h> 
#include <locale.h> 

int getValidIntegerInput() {
    int num;
    char buffer[100], symbol;
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) continue;
        if (sscanf_s(buffer, "%d %c", &num, &symbol) == 1) return num;
        printf("Ошибка! Введите корректное целое число: ");
    }
}

// Структура узла дерева
typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

void createNode(TreeNode** nodePtr, int value) {
    *nodePtr = (TreeNode*)malloc(sizeof(TreeNode));
    if (!(*nodePtr)) {
        printf("Ошибка выделения памяти!\n");
        exit(1);
    }
    (*nodePtr)->data = value;
    (*nodePtr)->left = (*nodePtr)->right = NULL; // Инициализация указателей на поддеревья как NULL
}

// Функция для вставки нового узла в дерево
void insert(TreeNode** root, int value) {
    if (*root == NULL) {
        createNode(root, value);
        return;
    }

    TreeNode* current = *root; // Указатель на текущий узел, начиная с корня

    while (1) {
        system("cls"); // Очистка экрана консоли
        printf("---> ВСТАВКА ЭЛЕМЕНТА <---\n\n");
        printf("Текущий узел: %d\n", current->data);

        printf("\n");
        printf("      [ %d ]\n", current->data);
        printf("      /   \\\n");

        if (current->left)
            printf("  [L:%d]   ", current->left->data);
        else
            printf("  [L: ]   ");

        if (current->right)
            printf("[R:%d]\n", current->right->data);
        else
            printf("[R: ]\n");

        printf("\nНажмите <-- для левого перехода, --> для правого\n");

        int key = _getch(); // Получение нажатой клавиши
        if (key == 0 || key == 224) {
            key = _getch(); // Получение кода стрелки
            if (key == 75) {
                if (current->left == NULL) {
                    createNode(&current->left, value);
                    printf("Элемент добавлен слева от %d.\n", current->data);
                    return;
                }
                else {
                    current = current->left;
                }
            }
            else if (key == 77) {
                if (current->right == NULL) {
                    createNode(&current->right, value);
                    printf("Элемент добавлен справа от %d.\n", current->data);
                    return;
                }
                else {
                    current = current->right;
                }
            }
            else {
                printf("Нажмите только <-- или -->\n");
                _getch();
            }
        }
    }
}

void freeTree(TreeNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void printTree(TreeNode* root, int level) {
    if (root == NULL) return;
    printTree(root->right, level + 1);
    for (int i = 0; i < level; ++i)
        printf("   |");
    printf("-- %d\n", root->data);
    printTree(root->left, level + 1);
}

TreeNode* copyTree(TreeNode* root) {
    if (root == NULL)
        return NULL;

    TreeNode* newNode = NULL;
    createNode(&newNode, root->data);
    newNode->left = copyTree(root->left);
    newNode->right = copyTree(root->right);
    return newNode;
}

void deleteReplace(TreeNode** root, TreeNode* parent, TreeNode* current, int direction) {
    if (current->left && current->right) { // Если узел имеет оба поддерева
        int key; // Переменная для хранения выбора пользователя
        while (1) {
            system("cls"); // Очистка экрана консоли
            printf("Узел %d имеет два подузла.\n", current->data);
            printf("Кого поставить на его место?\n");
            printf("<--: левый | -->: правый\n");
            printf("Левый подузел(%d)   ", current->left->data);
            printf("Правый подузел (%d)\n", current->right->data);
            printf("Выбор: ");
            key = _getch(); // Получение нажатой клавиши
            if (key == 0 || key == 224) {
                key = _getch();
                if (key == 75 || key == 77)
                    break;
            }
            printf("Нет такого варианта. Повторите ввод!\n");
        }
        current->data = (key == 75) ? current->left->data : current->right->data;
        deleteReplace(root, current, (key == 75) ? current->left : current->right, (key == 75) ? 0 : 1);
    }
    else { // Если узел имеет не более одного поддерева
        TreeNode* replacement = current->left ? current->left : current->right; // Выбор заменяющего поддерева
        free(current);
        if (!parent) // Если удаляется корень
            *root = replacement; // Обновление корня
        else if (direction == 0) // Если узел был левым потомком
            parent->left = replacement; // Обновление левого указателя родителя
        else // Если узел был правым потомком
            parent->right = replacement; // Обновление правого указателя родителя
    }
    return;
}

void deleteNode(TreeNode** root) {
    if (*root == NULL) {
        printf("Дерево пусто.\n");
        return;
    }
    TreeNode* copy = copyTree(*root);

    TreeNode* current = *root;
    TreeNode* parent = NULL;
    int direction = -1; // Направление: 0 — слева, 1 — справа, -1 — корень

    while (1) {
        system("cls"); // Очистка экрана консоли
        printf("---> УДАЛЕНИЕ УЗЛА <---\n\n");
        printf("Текущий узел: %d\n", current->data);

        printf("\n");
        printf("      [ %d ]\n", current->data);
        printf("      /   \\\n");

        if (current->left)
            printf("  [L:%d]   ", current->left->data);
        else
            printf("  [L: ]   ");

        if (current->right)
            printf("[R:%d]\n", current->right->data);
        else
            printf("[R: ]\n");

        printf("\n<--: влево | -->: вправо | Enter: удалить этот узел | Esc: отмена\n");

        int key = _getch();
        if (key == 0 || key == 224) {
            key = _getch();
            if (key == 75 && current->left) {
                parent = current;
                current = current->left;
                direction = 0;
            }
            else if (key == 77 && current->right) {
                parent = current;
                current = current->right;
                direction = 1;
            }
        }
        else if (key == 13) { // Если нажата клавиша Enter
            deleteReplace(root, parent, current, direction);
            printf("До удаления:\n");
            printTree(copy, 0);
            free(copy);
            printf("После удаления:\n");
            printTree(*root, 0);
            return;
        }
        else if (key == 27) { // Если нажата клавиша Esc
            printf("Удаление отменено.\n");
            return;
        }
    }
}

// Функция для вычисления высоты поддерева
int getHeight(TreeNode* root) {
    if (!root) return -1; // Если узел пуст, вернуть высоту -1
    int leftHeight = getHeight(root->left); // Рекурсивное вычисление высоты левого поддерева
    int rightHeight = getHeight(root->right); // Рекурсивное вычисление высоты правого поддерева
    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1; // Возврат максимальной высоты плюс 1
}

// Условие задачи
void findUnbalancedNodes(TreeNode* root, int* nodes, int* count) {
    if (!root) return;

    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);

    if (leftHeight != rightHeight) {
        nodes[*count] = root->data;
        (*count)++;
    }

    findUnbalancedNodes(root->left, nodes, count);
    findUnbalancedNodes(root->right, nodes, count);
}

void printMenu() {
    printf("\n=== МЕНЮ ===\n");
    printf("1. Добавить элемент\n");
    printf("2. Удалить элемент и его поддерево\n");
    printf("3. Визуализировать дерево\n");
    printf("4. Найти вершины с неравными высотами поддеревьев\n");
    printf("5. Выход\n");
    printf("Выберите действие: ");
}

void menu() {
    TreeNode* root = NULL;
    int choice, value;
    int unbalancedNodes[100];
    int count = 0;

    while (1) {
        printMenu();
        choice = getValidIntegerInput();

        switch (choice) {
        case 1:
            printf("Введите значение для добавления: ");
            value = getValidIntegerInput();
            insert(&root, value);
            printTree(root, 0);
            break;
        case 2:
            deleteNode(&root);
            break;
        case 3:
            printf("\nВизуализация дерева:\n");
            printTree(root, 0);
            break;
        case 4:
            if (!root) {
                printf("Дерево пусто.\n");
                break;
            }
            count = 0;
            findUnbalancedNodes(root, unbalancedNodes, &count);
            printTree(root, 0);
            if (count == 0) {
                printf("Нет вершин, где высота левого поддерева не равна высоте правого.\n");
            }
            else {
                printf("Вершины с неравными высотами поддеревьев: ");
                for (int i = 0; i < count; i++) {
                    printf("%d", unbalancedNodes[i]);
                    if (i < count - 1) printf(", ");
                }
                printf("\n");
            }
            break;
        case 5:
            freeTree(root);
            return;
        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    }
}


int main() {
    setlocale(LC_ALL, "RUS");
    menu();
}
