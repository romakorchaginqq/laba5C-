#include "Product.h"
#include <limits>

using namespace std;

void printMenu() {
    cout << "\n╔══════════════════════════════════════════╗\n";
    cout << "║         УПРАВЛЕНИЕ ТОВАРАМИ             ║\n";
    cout << "╠══════════════════════════════════════════╣\n";
    cout << "║  1. Показать все товары                  ║\n";
    cout << "║  2. Добавить товар                       ║\n";
    cout << "║  3. Удалить товар                       ║\n";
    cout << "║  4. Редактировать товар                 ║\n";
    cout << "║  5. Поиск по ID                         ║\n";
    cout << "║  6. Поиск по названию                   ║\n";
    cout << "║  7. Поиск по диапазону цен              ║\n";
    cout << "║  8. Сохранить в файл                    ║\n";
    cout << "║  9. Загрузить из файла                  ║\n";
    cout << "║  0. Выход                               ║\n";
    cout << "╚══════════════════════════════════════════╝\n";
    cout << "Выбор: ";
}

void showAll(const ProductList& list) {
    if (list.isEmpty()) {
        cout << "\nСписок пуст\n";
        return;
    }
    
    cout << "\n=== ТОВАРЫ ===\n";
    for (int i = 0; i < list.getSize(); i++) {
        cout << list[i] << "\n";
    }
    cout << "Всего: " << list.getSize() << "\n";
}

void showSearchResults(const ProductList& results) {
    if (results.isEmpty()) {
        cout << "\nНичего не найдено\n";
        return;
    }
    
    cout << "\n=== РЕЗУЛЬТАТЫ ПОИСКА ===\n";
    for (int i = 0; i < results.getSize(); i++) {
        cout << results[i] << "\n";
    }
    cout << "Найдено: " << results.getSize() << "\n";
}

int main(int argc, char* argv[]) {
    ProductList list;
    char filename[256] = "data.txt";
    
    if (argc >= 2) {
        strcpy(filename, argv[1]);
    } else {
        cout << "Введите имя файла (по умолчанию data.txt): ";
        cin.getline(filename, 256);
        if (strlen(filename) == 0) {
            strcpy(filename, "data.txt");
        }
    }
    
    cout << "Файл: " << filename << "\n";
    loadFromFile(list, filename);
    
    int choice;
    do {
        printMenu();
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Введите число!\n";
            continue;
        }
        
        switch(choice) {
            case 0:
                saveToFile(list, filename);
                cout << "До свидания!\n";
                break;
                
            case 1:
                showAll(list);
                break;
                
            case 2: {
                Product p;
                cin >> p;
                int newId = 1;
                for (int i = 0; i < list.getSize(); i++) {
                    if (list[i].getId() >= newId) newId = list[i].getId() + 1;
                }
                p = Product(newId, p.getName(), p.getPrice());
                list.add(p);
                cout << "Добавлен ID: " << newId << "\n";
                saveToFile(list, filename);
                break;
            }
                
            case 3: {
                int id;
                showAll(list);
                cout << "Введите ID для удаления: ";
                cin >> id;
                int idx = list.findById(id);
                if (idx == -1) {
                    cout << "Не найден\n";
                } else {
                    list.remove(idx);
                    cout << "Удален\n";
                    saveToFile(list, filename);
                }
                break;
            }
                
            case 4: {
                int id;
                showAll(list);
                cout << "Введите ID для редактирования: ";
                cin >> id;
                int idx = list.findById(id);
                if (idx == -1) {
                    cout << "Не найден\n";
                    break;
                }
                char name[100];
                double price;
                cout << "Новое название (Enter - пропустить): ";
                cin.ignore();
                cin.getline(name, 100);
                if (strlen(name) > 0) {
                    list[idx].setName(name);
                }
                cout << "Новая цена (0 - пропустить): ";
                cin >> price;
                if (price > 0) {
                    list[idx].setPrice(price);
                }
                cout << "Изменено\n";
                saveToFile(list, filename);
                break;
            }
                
            case 5: {
                int id;
                cout << "Введите ID: ";
                cin >> id;
                ProductList result;
                int idx = list.findById(id);
                if (idx != -1) result.add(list[idx]);
                showSearchResults(result);
                break;
            }
                
            case 6: {
                char name[100];
                cout << "Введите название: ";
                cin.ignore();
                cin.getline(name, 100);
                showSearchResults(list.findByName(name));
                break;
            }
                
            case 7: {
                double min, max;
                cout << "Мин. цена: ";
                cin >> min;
                cout << "Макс. цена: ";
                cin >> max;
                showSearchResults(list.findByPrice(min, max));
                break;
            }
                
            case 8:
                saveToFile(list, filename);
                break;
                
            case 9:
                loadFromFile(list, filename);
                break;
                
            default:
                cout << "Неверный выбор\n";
        }
    } while (choice != 0);
    
    return 0;
}