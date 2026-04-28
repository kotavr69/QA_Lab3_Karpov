/**
 * В программе реализована иерархия изданий:
 * Publication -> Book, Magazine, Newspaper.
 *
 * \dot
 * digraph hierarchy {
 *     rankdir=TB;
 *     node [shape=box, style=filled, fillcolor=lightyellow];
 *
 *     Publication [label="Publication\n(базовый класс)", fillcolor=lightblue];
 *     Book [label="Book\n(книга)"];
 *     Magazine [label="Magazine\n(журнал)"];
 *     Newspaper [label="Newspaper\n(газета)"];
 *     Library [label="Library\n(библиотека)"];
 *
 *     Publication -> Book [arrowhead=onormal];
 *     Publication -> Magazine [arrowhead=onormal];
 *     Publication -> Newspaper [arrowhead=onormal];
 *     Library -> Publication [arrowhead=diamond];
 * }
 * \enddot
 *
 * \image html library_diagram.png "Схема иерархии классов"
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#define NOMINMAX
#include <windows.h>


using namespace std;

/**
 * @class Publication
 * @brief Базовый класс для всех изданий в библиотеке.
 *
 * Хранит общие для всех изданий поля: название, автор, год.
 * Содержит чисто виртуальные методы, которые обязательно
 * должны переопределить наследники.
 *
 * Возраст издания вычисляется по формуле:
 * \f$ age = current\_year - publication\_year \f$
 */
class Publication {
protected:
    string title;
    string author;
    int year;  // Год издания должен быть в диапазоне от 1000 до 2026

public:
    Publication(const string& t, const string& a, int y)
        : title(t), author(a), year(y) {}

    // Виртуальный деструктор необходим для корректного удаления объектов наследников
    virtual ~Publication() {}

    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    int getYear() const { return year; }

    // Чисто виртуальные методы — каждый наследник обязан их реализовать по-своему
    virtual string getDescription() const = 0;
    virtual string getType() const = 0;
    virtual string getAdditionalInfo() const = 0;
};

/**
 * @class Book
 * @brief Класс книги. Наследует всё от Publication и добавляет ISBN и кол-во страниц.
 */
class Book : public Publication {
private:
    string isbn;
    int pages;

public:
    Book(const string& t, const string& a, int y, const string& isbn, int p)
        : Publication(t, a, y), isbn(isbn), pages(p) {}

    string getISBN() const { return isbn; }
    int getPages() const { return pages; }

    // Формируем полное описание книги для вывода в консоль
    string getDescription() const override {
        return "Книга: \"" + title + "\", " + author + " (" + to_string(year) + ")";
    }

    string getType() const override {
        return "Книга";
    }

    string getAdditionalInfo() const override {
        return "ISBN: " + isbn + ", Страниц: " + to_string(pages);
    }
};

/**
 * @class Magazine
 * @brief Класс журнала. Наследует Publication, добавляет номер выпуска и месяц.
 */
class Magazine : public Publication {
private:
    int issueNumber;
    string month;

public:
    Magazine(const string& t, const string& a, int y, int issue, const string& m)
        : Publication(t, a, y), issueNumber(issue), month(m) {}

    int getIssueNumber() const { return issueNumber; }
    string getMonth() const { return month; }

    string getDescription() const override {
        return "Журнал: \"" + title + "\", " + month + " (" + to_string(year) + ")";
    }

    string getType() const override {
        return "Журнал";
    }

    string getAdditionalInfo() const override {
        return "Выпуск №" + to_string(issueNumber) + ", " + month + " " + to_string(year);
    }
};

/**
 * @class Newspaper
 * @brief Класс газеты. Наследует Publication, добавляет день, месяц и номер выпуска.
 */
class Newspaper : public Publication {
private:
    int day;
    int month_num;
    int editionNumber;

public:
    Newspaper(const string& t, const string& a, int y, int d, int m, int edition)
        : Publication(t, a, y), day(d), month_num(m), editionNumber(edition) {}

    int getDay() const { return day; }
    int getMonthNum() const { return month_num; }
    int getEditionNumber() const { return editionNumber; }

    string getDescription() const override {
        return "Газета: \"" + title + "\" от " + to_string(day) + "." + to_string(month_num) + "." + to_string(year);
    }

    string getType() const override {
        return "Газета";
    }

    string getAdditionalInfo() const override {
        return "Выпуск №" + to_string(editionNumber) + " от " + to_string(day) + "." + to_string(month_num) + "." + to_string(year);
    }
};

/**
 * @class Library
 * @brief Хранит все издания и умеет их выводить, искать и считать статистику.
 *
 * Все издания хранятся в векторе из shared_ptr — это гарантирует,
 * что память освободится автоматически при удалении библиотеки.
 */
class Library {
private:
    string name;
    // Используем shared_ptr, чтобы не следить за удалением объектов вручную
    vector<shared_ptr<Publication>> publications;

public:
    Library(const string& n) : name(n) {}

    // Добавляет издание в конец списка
    void addPublication(shared_ptr<Publication> pub) {
        publications.push_back(pub);
    }

    size_t getPublicationCount() const {
        return publications.size();
    }

    string getName() const { return name; }

    // Выводит все издания с полной информацией о каждом
    void printAllPublications() const {
        cout << "\n========== " << name << " ==========" << endl;
        cout << "Всего изданий: " << publications.size() << endl;
        cout << "=========================================" << endl;

        for (size_t i = 0; i < publications.size(); i++) {
            cout << "\n" << (i + 1) << ". " << publications[i]->getDescription() << endl;
            cout << "   " << publications[i]->getAdditionalInfo() << endl;
        }
    }

    // Ищет все издания конкретного автора (точное совпадение)
    vector<shared_ptr<Publication>> findByAuthor(const string& author) const {
        vector<shared_ptr<Publication>> result;
        for (const auto& pub : publications) {
            if (pub->getAuthor() == author) {
                result.push_back(pub);
            }
        }
        return result;
    }

    // Подсчитывает количество изданий каждого типа
    void printStatistics() const {
        int books = 0, magazines = 0, newspapers = 0;
        for (const auto& pub : publications) {
            if (pub->getType() == "Книга") books++;
            else if (pub->getType() == "Журнал") magazines++;
            else if (pub->getType() == "Газета") newspapers++;
        }

        cout << "\n=== СТАТИСТИКА БИБЛИОТЕКИ ===" << endl;
        cout << "Книг: " << books << endl;
        cout << "Журналов: " << magazines << endl;
        cout << "Газет: " << newspapers << endl;
        cout << "Всего: " << publications.size() << endl;
    }
};

/**
 * @brief Безопасный ввод целого числа — защита от случайного ввода букв.
 *
 * Если пользователь введёт не число, функция очистит буфер и попросит повторить ввод.
 * @param prompt Текст-подсказка перед вводом
 * @return Корректное целое число
 */
int inputInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            // Очищаем флаг ошибки и сбрасываем некорректный ввод из буфера
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка! Введите целое число." << endl;
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

/**
 * @brief Безопасный ввод строки.
 * @param prompt Текст-подсказка перед вводом
 * @return Введённая строка
 */
string inputString(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

/**
 * @brief Точка входа в программу. Показывает меню и обрабатывает выбор пользователя.
 *
 * Пользователь может добавлять книги/журналы/газеты,
 * смотреть весь каталог, искать по автору и смотреть статистику.
 */
int main() {
    // Настройка консоли для корректного отображения русского языка
    setlocale(LC_ALL, "ru");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Library library("Центральная городская библиотека");

    bool running = true;
    while (running) {
        cout << "\n========================================" << endl;
        cout << "    СИСТЕМА УПРАВЛЕНИЯ БИБЛИОТЕКОЙ      " << endl;
        cout << "========================================" << endl;
        cout << "1. Добавить книгу" << endl;
        cout << "2. Добавить журнал" << endl;
        cout << "3. Добавить газету" << endl;
        cout << "4. Показать все издания" << endl;
        cout << "5. Поиск по автору" << endl;
        cout << "6. Показать статистику" << endl;
        cout << "7. Выход" << endl;
        cout << "----------------------------------------" << endl;

        int choice = inputInt("Выберите действие (1-7): ");

        switch (choice) {
        case 1: {
            cout << "\n--- ДОБАВЛЕНИЕ КНИГИ ---" << endl;
            string title = inputString("Название: ");
            string author = inputString("Автор: ");
            int year = inputInt("Год издания: ");
            string isbn = inputString("ISBN: ");
            int pages = inputInt("Количество страниц: ");

            // make_shared автоматически выделяет память и возвращает умный указатель
            library.addPublication(make_shared<Book>(title, author, year, isbn, pages));
            cout << "Книга добавлена!" << endl;
            break;
        }
        case 2: {
            cout << "\n--- ДОБАВЛЕНИЕ ЖУРНАЛА ---" << endl;
            string title = inputString("Название: ");
            string author = inputString("Редакция/автор: ");
            int year = inputInt("Год издания: ");
            int issue = inputInt("Номер выпуска: ");
            string month = inputString("Месяц: ");

            library.addPublication(make_shared<Magazine>(title, author, year, issue, month));
            cout << "Журнал добавлен!" << endl;
            break;
        }
        case 3: {
            cout << "\n--- ДОБАВЛЕНИЕ ГАЗЕТЫ ---" << endl;
            string title = inputString("Название: ");
            string author = inputString("Издатель: ");
            int year = inputInt("Год издания: ");
            int day = inputInt("День выпуска: ");
            int month = inputInt("Месяц выпуска (числом): ");
            int edition = inputInt("Номер выпуска: ");

            library.addPublication(make_shared<Newspaper>(title, author, year, day, month, edition));
            cout << "Газета добавлена!" << endl;
            break;
        }
        case 4: {
            library.printAllPublications();
            break;
        }
        case 5: {
            cout << "\n--- ПОИСК ПО АВТОРУ ---" << endl;
            string author = inputString("Введите автора: ");
            auto results = library.findByAuthor(author);
            if (results.empty()) {
                cout << "Изданий автора \"" << author << "\" не найдено." << endl;
            }
            else {
                cout << "Найдено " << results.size() << " изданий:" << endl;
                for (size_t i = 0; i < results.size(); i++) {
                    cout << "  " << (i + 1) << ". " << results[i]->getDescription() << endl;
                }
            }
            break;
        }
        case 6: {
            library.printStatistics();
            break;
        }
        case 7: {
            running = false;
            cout << "Программа завершена." << endl;
            break;
        }
        default: {
            cout << "Неверный выбор! Попробуйте снова." << endl;
            break;
        }
        }
    }

    return 0;
}