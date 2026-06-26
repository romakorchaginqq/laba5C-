#include "Product.h"

Product::Product() : id(0), name(nullptr), price(0) {
    name = new char[1];
    name[0] = '\0';
}

Product::Product(int id, const char* name, double price) : id(id), price(price) {
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name);
}

Product::Product(const Product& other) : id(other.id), price(other.price) {
    name = new char[strlen(other.name) + 1];
    strcpy(name, other.name);
}

Product::~Product() {
    delete[] name;
}

int Product::getId() const { return id; }
const char* Product::getName() const { return name; }
double Product::getPrice() const { return price; }

void Product::setName(const char* name) {
    delete[] this->name;
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name);
}

void Product::setPrice(double price) {
    this->price = price;
}

std::ostream& operator<<(std::ostream& os, const Product& p) {
    os << "ID: " << p.id << " | " << p.name << " | " << std::fixed << std::setprecision(2) << p.price << " руб.";
    return os;
}

std::istream& operator>>(std::istream& is, Product& p) {
    char buffer[100];
    std::cout << "Название: ";
    is.ignore();
    is.getline(buffer, 100);
    p.setName(buffer);
    std::cout << "Цена: ";
    is >> p.price;
    return is;
}

Product& Product::operator=(const Product& other) {
    if (this != &other) {
        id = other.id;
        price = other.price;
        delete[] name;
        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);
    }
    return *this;
}

bool Product::operator==(const Product& other) const {
    return id == other.id;
}

const char* Product::operator[](int index) const {
    static char buf[50];
    switch(index) {
        case 0: sprintf(buf, "%d", id); return buf;
        case 1: return name;
        case 2: sprintf(buf, "%.2f", price); return buf;
        default: return "";
    }
}

ProductList::ProductList() : arr(nullptr), size(0), capacity(0) {}

ProductList::ProductList(const ProductList& other) : arr(nullptr), size(0), capacity(0) {
    *this = other;
}

ProductList::~ProductList() {
    clear();
}

void ProductList::resize() {
    int newCap = capacity == 0 ? 1 : capacity * 2;
    Product* newArr = new Product[newCap];
    for (int i = 0; i < size; i++) {
        newArr[i] = arr[i];
    }
    delete[] arr;
    arr = newArr;
    capacity = newCap;
}

void ProductList::add(const Product& p) {
    if (size >= capacity) resize();
    arr[size++] = p;
}

void ProductList::remove(int index) {
    if (index < 0 || index >= size) return;
    for (int i = index; i < size - 1; i++) {
        arr[i] = arr[i + 1];
    }
    size--;
}

void ProductList::clear() {
    delete[] arr;
    arr = nullptr;
    size = 0;
    capacity = 0;
}

int ProductList::getSize() const { return size; }
bool ProductList::isEmpty() const { return size == 0; }

Product& ProductList::operator[](int index) { return arr[index]; }
const Product& ProductList::operator[](int index) const { return arr[index]; }

ProductList& ProductList::operator=(const ProductList& other) {
    if (this != &other) {
        clear();
        for (int i = 0; i < other.size; i++) {
            add(other.arr[i]);
        }
    }
    return *this;
}

int ProductList::findById(int id) const {
    for (int i = 0; i < size; i++) {
        if (arr[i].getId() == id) return i;
    }
    return -1;
}

ProductList ProductList::findByName(const char* name) const {
    ProductList result;
    for (int i = 0; i < size; i++) {
        if (strstr(arr[i].getName(), name) != nullptr) {
            result.add(arr[i]);
        }
    }
    return result;
}

ProductList ProductList::findByPrice(double min, double max) const {
    ProductList result;
    for (int i = 0; i < size; i++) {
        double p = arr[i].getPrice();
        if (p >= min && p <= max) {
            result.add(arr[i]);
        }
    }
    return result;
}

bool loadFromFile(ProductList& list, const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Файл не найден: " << filename << "\n";
        return false;
    }
    
    list.clear();
    char line[256];
    int loaded = 0;
    
    while (file.getline(line, 256)) {
        char* token = strtok(line, "|");
        if (!token) continue;
        int id = atoi(token);
        
        token = strtok(nullptr, "|");
        if (!token) continue;
        
        token = strtok(nullptr, "|");
        if (!token) continue;
        double price = atof(token);
        
        char* name = strtok(nullptr, "|");
        if (!name) {
            char temp[100];
            strcpy(temp, token);
            list.add(Product(id, temp, price));
        } else {
            list.add(Product(id, token, price));
        }
        loaded++;
    }
    
    file.close();
    std::cout << "Загружено " << loaded << " товаров\n";
    return true;
}

bool saveToFile(const ProductList& list, const char* filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Ошибка записи в файл\n";
        return false;
    }
    
    for (int i = 0; i < list.getSize(); i++) {
        const Product& p = list[i];
        file << p.getId() << "|" << p.getName() << "|" 
             << std::fixed << std::setprecision(2) << p.getPrice() << "\n";
    }
    
    file.close();
    std::cout << "Сохранено " << list.getSize() << " товаров\n";
    return true;
}