#include <unistd.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

// Вспомогательная функция - ищем файл с именем file в путях, содержащихся
// в переменной PATH
char *findfile(const char *file)
{
    char *pathvar;
    pathvar = getenv("PATH");
    char *fname = (char*)malloc(strlen(pathvar) + strlen(file) + 2);
    fname[0] = '\0';
    // разбиваем на токены используя двоеточие как разделитель
    char *pch = strtok(pathvar, ":");
    while (pch != NULL) {
        // в fname объединим путь и имя файла, разделив их слэшем
        strcpy(fname, pch);
        strcat(fname, "/");
        strcat(fname, file);
        // если файл существует - то используем его для запуска
        if (access(fname, F_OK) == 0) {
            break;
        }
        pch = strtok (NULL, ":");
        fname[0] = '\0';
    }

    return fname;
}

// Используем вариативный шаблон, чтобы передать переменное количество аргументов
// в другую функцию
template <class ... Args>
int execlp(const char *file, const char *arg, Args ... args)
{
    int res = 0;

    char *fname = findfile(file);
    res = execl(fname, arg, args...);
    free(fname);

    return res;
}

int main()
{
    execlp("ls", "/bin/ls", "/tmp", "/home", NULL);

    return 0;
}
