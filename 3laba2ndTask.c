// Структура содержит информацию о сотрудниках фирмы: шифр отдела (число), фамилию (указатель), вложенную структуру – дату приёма на работу (строка фиксированной длины) и сумму оклада. Найти сотрудников с заданной фамилией. Удалить сотрудников с окладом ниже заданного.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_NAME_LENGTH 50
#define DATE_LENGTH 11 // Формат даты

enum {
    max_limit = 2147483647, 
    min_limit = 0          
};

int execute_verification(int min_limit, int max_limit) {
    char input[100];
    int number;
    char symbol;

    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Ошибка ввода! Попробуйте еще раз!\n");
            continue;
        }

        if (sscanf_s(input, "%d %c", &number, &symbol) != 1) { 
            printf("Ошибка ввода! Введите целое число:\n");
            continue;
        }

        if (number > max_limit || number < min_limit) {
            printf("Ошибка ввода! Введите число из диапазона [%d, %d]:\n", min_limit, max_limit);
            continue;
        }

        return number;
    }
}

float execute_verification_float(float min_limit, float max_limit) {
    char input[100];
    float number;
    char symbol;

    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Ошибка ввода! Попробуйте еще раз!\n");
            continue;
        }

        if (sscanf_s(input, "%f %c", &number, &symbol) != 1) { 
            printf("Ошибка ввода! Введите число с плавающей запятой:\n");
            continue;
        }

        if (number > max_limit || number < min_limit) {
            printf("Ошибка ввода! Введите число из диапазона [%.2f, %.2f]:\n", min_limit, max_limit);
            continue;
        }

        return number;
    }
}

// Структура для даты
typedef struct {
    char date[DATE_LENGTH]; // Дата приёма на работу
} HireDate;

// Структура для сотрудника
typedef struct {
    int dept_code;         
    char* surname;         
    HireDate hire_date;    
    double salary;         
} Employee;

// Функция ввода базовой информации о сотруднике (шифр отдела и фамилия)
void input_employee_base(Employee* emp) {
    printf("Введите шифр отдела (целое число):\n");
    emp->dept_code = execute_verification(min_limit, max_limit);

    char temp_name[MAX_NAME_LENGTH];
    int valid_input = 0;
    do {
        printf("Введите фамилию сотрудника (с большой буквы):\n");
        fgets(temp_name, MAX_NAME_LENGTH, stdin);
        temp_name[strcspn(temp_name, "\n")] = 0;

        if (temp_name[0] >= 'А' && temp_name[0] <= 'Я') {
            valid_input = 1; 
        }
        else {
            printf("Ошибка! Фамилия должна начинаться с большой русской буквы (А-Я).\n");
        }
    } while (!valid_input);

    emp->surname = (char*)malloc(strlen(temp_name) + 1);

    if (!emp->surname) {
        printf("Ошибка выделения памяти!\n");
        exit(1);
    }
    // Копируем введённую фамилию из временного массива temp_name в выделенную память surname
    strcpy_s(emp->surname, strlen(temp_name) + 1, temp_name);
}

// Функция вывода информации о сотруднике 
void print_employee_base(Employee emp) {
    printf("Шифр отдела: %d, Фамилия: %s\n", emp.dept_code, emp.surname);
}

// Функция ввода полной информации о сотруднике
void input_employee(Employee* emp) {
    input_employee_base(emp);

    int valid_date = 0;
    do {
        printf("Введите дату приёма на работу (DD.MM.YYYY): ");
        fgets(emp->hire_date.date, DATE_LENGTH, stdin);
        emp->hire_date.date[strcspn(emp->hire_date.date, "\n")] = 0;

        // Проверка длины и формата введённой даты
        if (strlen(emp->hire_date.date) != 10 ||
            emp->hire_date.date[2] != '.' ||
            emp->hire_date.date[5] != '.') {
            printf("Ошибка! Дата должна быть в формате DD.MM.YYYY (например, 15.03.2025).\n");
            continue;
        }

        for (int i = 0; i < 10; i++) {
            if (i != 2 && i != 5 && (emp->hire_date.date[i] < '0' || emp->hire_date.date[i] > '9')) {

                printf("Ошибка! Дата должна содержать только цифры и точки в формате DD.MM.YYYY.\n");
                valid_date = 0;
                break;
            }
            if (i == 9) valid_date = 1; 
        }

        if (!valid_date) continue;

        int day, month, year;
        if (sscanf_s(emp->hire_date.date, "%2d.%2d.%4d", &day, &month, &year) != 3) {

            printf("Ошибка! Неверный формат даты.\n");
            valid_date = 0;
            continue;
        }

        if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2100) {
           
            printf("Ошибка! Некорректные значения: день (1-31), месяц (1-12), год (1900-2100).\n");
            valid_date = 0;
            continue;
        }

        int days_in_month;
        // Проверка февраля с учётом високосного года
        if (month == 2) {

            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {

                days_in_month = 29;
            }
            else {

                days_in_month = 28;
            }
        }
        
        else if (month == 4 || month == 6 || month == 9 || month == 11) {
            days_in_month = 30;
        }
        
        else {
            days_in_month = 31;
        }

        if (day > days_in_month) {

            printf("Ошибка! В месяце %d года %d не может быть %d дней.\n", month, year, day);
            valid_date = 0;
            continue;
        }

        valid_date = 1; 
    } while (!valid_date);

    while (getchar() != '\n' && getchar() != EOF);

    printf("Введите оклад сотрудника:\n");
    emp->salary = execute_verification_float(0.0f, 1000000.0f);
}

// Функция вывода полной информации о сотруднике
void print_employee(Employee emp) {
    // Вызываем функцию для вывода базовой информации (шифр отдела и фамилия)
    // print_employee_base печатает эти данные в консоль
    print_employee_base(emp);

    // Выводим дату приёма на работу и оклад сотрудника в консоль в отформатированном виде
    // %s — формат для строки (hire_date.date), %.2f — формат для числа с плавающей запятой с двумя знаками после точки (salary)
    // emp.hire_date.date — строка с датой в формате DD.MM.YYYY
    // emp.salary — оклад сотрудника в виде числа с плавающей запятой
    // \n добавляет перевод строки после вывода
    printf("Дата приёма: %s, Оклад: %.2f\n", emp.hire_date.date, emp.salary);
}

// Функция поиска сотрудников по заданной фамилии 
void find_by_surname(Employee* employees, int size, const char* search_surname) {

    int found = 0;

    for (int i = 0; i < size; i++) {
        // strcmp возвращает 0, если строки идентичны
        if (strcmp(employees[i].surname, search_surname) == 0) {

            print_employee(employees[i]);
            found++;
        }
    }

    if (!found) {

        printf("Сотрудники с фамилией '%s' не найдены.\n", search_surname);
    }
}

// Функция удаления сотрудников с окладом ниже заданного
Employee* remove_by_salary(Employee* employees, int* size, double min_salary) {
    int new_size = 0; // сотрудники которые останутся

    for (int i = 0; i < *size; i++) {
        if (employees[i].salary >= min_salary) {
            new_size++;
        }
    }

    Employee* new_employees = (Employee*)malloc(new_size * sizeof(Employee));
    if (!new_employees) {
        printf("Ошибка выделения памяти!\n");
        exit(1);
    }

    int j = 0;
    for (int i = 0; i < *size; i++) {

        if (employees[i].salary >= min_salary) {
            // Копируем шифр отдела из старого массива в новый
            new_employees[j].dept_code = employees[i].dept_code;
            // Выделяем память для фамилии в новом массиве
            new_employees[j].surname = (char*)malloc(strlen(employees[i].surname) + 1);
            if (!new_employees[j].surname) {
                printf("Ошибка выделения памяти!\n");
                exit(1);
            }
            // Копируем фамилию из старого массива в новый
            strcpy_s(new_employees[j].surname, strlen(employees[i].surname) + 1, employees[i].surname);
            strcpy_s(new_employees[j].hire_date.date, DATE_LENGTH, employees[i].hire_date.date);
            new_employees[j].salary = employees[i].salary;
            j++;
        }
        free(employees[i].surname);
    }

    free(employees);
    *size = new_size;
    return new_employees;
}

// Функция освобождения памяти для массива сотрудников
void free_employees(Employee* employees, int size) {
    for (int i = 0; i < size; i++) {
        free(employees[i].surname);
    }
    free(employees);
}


int main() {

    int size = 0;
    Employee* employees = NULL;
    int choice;
    setlocale(LC_ALL, "RUS");
    system("chcp 1251");

    do {
        printf("\nМеню:\n");
        printf("1. Добавить сотрудника\n");
        printf("2. Найти сотрудников по фамилии\n");
        printf("3. Удалить сотрудников с окладом ниже заданного\n");
        printf("4. Вывести всех сотрудников\n");
        printf("5. Выход\n");
        printf("Выберите действие:\n");

        choice = execute_verification(1, 5);

        switch (choice) {
        case 1: { 

            int count;
            printf("Введите количество сотрудников для добавления:\n");
            count = execute_verification(1, max_limit);

            Employee* temp = (Employee*)realloc(employees, (size + count) * sizeof(Employee));
            if (!temp) {

                printf("Ошибка выделения памяти!\n");
                exit(1);
            }

            employees = temp;

            for (int i = size; i < size + count; i++) {

                input_employee(&employees[i]);
            }
            size += count;

            break;
        }

        case 2: { 

            char search_surname[MAX_NAME_LENGTH];

            int valid_input = 0;

            do {

                printf("Введите фамилию для поиска (с большой буквы): ");
                fgets(search_surname, MAX_NAME_LENGTH, stdin);
                search_surname[strcspn(search_surname, "\n")] = 0;
                if (search_surname[0] >= 'А' && search_surname[0] <= 'Я') {

                    valid_input = 1;
                }
                else {

                    printf("Ошибка! Фамилия должна начинаться с большой русской буквы (А-Я).\n");
                }
            } while (!valid_input);
            find_by_surname(employees, size, search_surname);
            break;
        }
        case 3: { 
          double min_salary;
            printf("Введите минимальный оклад для удаления:\n");
            min_salary = execute_verification_float(0.0f, 1000000.0f);
            employees = remove_by_salary(employees, &size, min_salary);
            printf("Сотрудники с окладом ниже %.2f удалены.\n", min_salary);
            break;
        }
        case 4: { 
            if (size == 0) {

                printf("Список сотрудников пуст.\n");
            }
            else {
                for (int i = 0; i < size; i++) {
                    printf("Сотрудник %d:\n", i + 1);
                    print_employee(employees[i]);
                }
            }
            break;
        }
        case 5: { 
            free_employees(employees, size);
            employees = NULL;
            size = 0;
            printf("Программа завершена.\n");
            break;
        }
        default: {
            printf("Неверный выбор, попробуйте снова.\n");
           break;
        }
        }
    } while (choice != 5);
    return 0;
}
