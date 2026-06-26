#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>

class Product {
private:
    int id;
    char* name;
    double price;
    
public:
    Product();
    Product(int id, const char* name, double price);
    Product(const Product& other);
    ~Product();
    
    int getId() const;
    const char* getName() const;
    double getPrice() const;
    void setName(const char* name);
    void setPrice(double price);
    
    // Перегрузка операторов
    friend std::ostream& operator<<(std::ostream& os, const Product& p);
    friend std::istream& operator>>(std::istream& is, Product& p);
    Product& operator=(const Product& other);
    bool operator==(const Product& other) const;
    const char* operator[](int index) const;
};

class ProductList {
private:
    Product* arr;
    int size;
    int capacity;
    
    void resize();
    
public:
    ProductList();
    ProductList(const ProductList& other);
    ~ProductList();
    
    void add(const Product& p);
    void remove(int index);
    void clear();
    int getSize() const;
    bool isEmpty() const;
    
    Product& operator[](int index);
    const Product& operator[](int index) const;
    ProductList& operator=(const ProductList& other);
    
    int findById(int id) const;
    ProductList findByName(const char* name) const;
    ProductList findByPrice(double min, double max) const;
};

// Глобальные функции для работы с файлом
bool loadFromFile(ProductList& list, const char* filename);
bool saveToFile(const ProductList& list, const char* filename);

#endif