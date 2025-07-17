// С клавиатуры заполнить файл числами. Вывести содержимое файла на экран.
// Определить максимальное число, записанное в файле (если таких чисел несколько, вывести все). 
// Перевернуть в файле число, номер которого задан с клавиатуры

#include <stdio.h>
#include <locale.h>
#include <string.h>

void fillFile(const char* filename);
void printFile(const char* filename);
void findMaxNumbers(const char* filename);
void reverseNumberByIndex(const char* filename, int index);
int isTextFile(const char* filename);
int checkout(int min, int max);


int checkout(int min, int max) {
    int num;
    char symbol;
    while (1) {
        char input[100];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Ошибка ввода! Повторите ввод: \n");
            continue;
        }
        if (sscanf_s(input, "%d %c", &num, &symbol) != 1) {
            printf("Ошибка ввода! Введите целое число: \n");
            continue;
        }
        if (num < min || num > max) {
            printf("Ошибка ввода! Введите число от %d до %d: ", min, max);
            continue;
        }
        return num;
    }
}

int isTextFile(const char* filename) {
    const char* ext = strrchr(filename, '.'); 

    if (!ext || strcmp(ext, ".txt") != 0) { 
        return 0;
    }
    return strchr(filename, '.') == ext; 
}

void fillFile(const char* filename) {
    FILE* file;
    if (fopen_s(&file, filename, "w") != 0) {
        perror("Ошибка при открытии файла");
        return;
    }
    printf("Введите количество элементов: ");
    int count = checkout(1, 999); 
    printf("Введите %d целых чисел:\n", count);
    for (int i = 0; i < count; i++) {
        printf("Элемент [%d]: ", i + 1);
        int num = checkout(-999, 999);  
        fprintf(file, "%d ", num);
    }
    fclose(file);  
    printFile(filename);  
}

void printFile(const char* filename) {
    FILE* file;
    if (fopen_s(&file, filename, "r") != 0) {
        perror("Ошибка при открытии файла");
        return;
    }
    printf("Содержимое файла %s:\n", filename);
    int num;
    while (fscanf_s(file, "%d", &num) == 1) {
        printf("%d ", num);
    }
    printf("\n");
    fclose(file);  
}

void findMaxNumbers(const char* filename) {
    FILE* file;
    if (fopen_s(&file, filename, "r") != 0) {
        perror("Ошибка при открытии файла");
        return;
    }
    int max = 0, num, maxCount = 0;
    while (fscanf_s(file, "%d", &num) == 1) {
        if (num > max) {
            max = num;
            maxCount = 1; // Счётчик максимальных чисел
        }
        else if (num == max) {
            maxCount++;
        }
    }
    fclose(file);  
    if (maxCount == 0) {
        printf("Файл пуст или содержит некорректные данные.\n");
        return;
    }
    printf("Максимальное число: %d (встречается %d раз(а))\n", max, maxCount);
    for (int i = 0; i < maxCount; i++) {
        printf("%d ", max); 
    }
    printf("\n");
}

void reverseNumberByIndex(const char* filename, int index) {
    FILE* file;
    if (fopen_s(&file, filename, "r+") != 0) { // чтение и запись
        perror("Ошибка при открытии файла");
        return;
    }
    int num, count = 0; // count = порядковый номер = index 
    long pos = 0; // если число большое 
    while (fscanf_s(file, "%d", &num) == 1) {
        count++;
        if (count == index) {
            long currentPos = ftell(file);
            int reversed = 0, temp = num, sign = 1;
            if (temp < 0) {
                sign = -1;
                temp = -temp; // отрицательное -> положительное 
            }
            while (temp > 0) {
                reversed = reversed * 10 + temp % 10;
                temp /= 10;
            }
            reversed *= sign; // возвращаем знак
            printf("Полученное число: %d\n", reversed);
            int len = snprintf(NULL, 0, "%d", num); // узнаем сколько символов было в числе 
            fseek(file, currentPos - len, SEEK_SET); 
            fprintf(file, "%*d", len, reversed); // %*d: len = 3, если было -34, reversed = число 
            break; 
        }
    fclose(file);
    printFile(filename); 
}


int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "RUS");  
    if (argc < 2) {
        printf("Использование: %s <файл>\n", argv[0]);
        return 1;
    }    if (!isTextFile(argv[1])) {
        printf("Ошибка: файл должен иметь расширение .txt\n");
        return 1;
    }
    int choice;
    while (1) {
        printf("\nМеню:\n");
        printf("1. Заполнить файл числами\n");
        printf("2. Вывести содержимое файла\n");
        printf("3. Найти максимальное число(а)\n");
        printf("4. Перевернуть число по номеру\n");
        printf("9. Выход\n");
        printf("Выберите действие: ");
        choice = checkout(1, 9);
        switch (choice) {
        case 1:
            fillFile(argv[1]); 
            break;
        case 2:
            printFile(argv[1]);  
            break;
        case 3:
            findMaxNumbers(argv[1]);  
            break;
        case 4:
            printf("Введите номер числа для реверса: ");
            reverseNumberByIndex(argv[1], checkout(1, 999)); 
            break;
        case 9:
            return 0;  
        default:
            printf("Некорректный выбор. Попробуйте снова.\n"); } }
    return 0;
}
