#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

struct Product {
    char name[50];
    int quantity;
    int sold;
    double price;
    double earnings;
}; 

bool priceCompare(const Product& a, const Product& b) {
    return a.price < b.price;
}

bool quantityCompare(Product a, Product b) {
    return a.quantity > b.quantity;
}

bool earningsCompare(Product a, Product b) {
    return a.earnings < b.earnings;
}

bool priceDescendingCompare(Product a, Product b) {
    return a.price > b.price;
}

void insertProduct(vector<Product>& products) {
    Product newProduct;
    cout <<"Enter the name of the new product: ";
    cin >> newProduct.name;
    cout << "Enter the quantity of the new product: ";
    cin >> newProduct.quantity;
    cout << "Enter the price of the new product: $";
    cin >> newProduct.price;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    products.push_back(newProduct);

    ofstream file("product.bin", ios::out | ios::binary);
    for (const auto& product : products) {
        file.write(reinterpret_cast<const char*>(&product), sizeof(Product));
    }
    file.close();
}

void outputAllData(vector<Product>& products) {
    ifstream file("product.bin", ios::binary);
    Product temp;

    if (file.is_open()) {
        while (file.read((char*)&temp, sizeof(Product))) {
            products.push_back(temp);
        }
        file.close();
    } 
    else {
        cout << "File not found" << endl;
        return;
    }

    cout << "All Product Data:" << endl;
    for (int i = 0; i < products.size(); i++) {
        cout << "Name: " << products[i].name << " - Quantity: " << products[i].quantity << " - Price: $" << products[i].price << endl;
    }
}

void sellProduct(vector<Product>& products) {
    string name;
    int quantity;
    cout << "Enter the name of the product you want to sell: ";
    cin >> name;
    cout << "Enter the quantity you want to sell: ";
    cin >> quantity;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    bool found = false;
    for (int i = 0; i < products.size(); i++) {
        if (products[i].name == name) {
            found = true;
            if (products[i].quantity >= quantity) {
                products[i].quantity -= quantity;
                products[i].sold += quantity;
                cout << "Successfully sold " << quantity << " units of " << name << endl;
                break;
            } 
            else {
                cout << "Cannot sell " << quantity << " units of " << name << ". Only " << products[i].quantity << " units available" << endl;
                break;
            }
        }
    }
    if (!found) {
        cout << "Product with name " << name << " not found" << endl;
    }
    else {
        ofstream file("product.bin", ios::out | ios::binary);
        if(file.is_open()){
            for (const auto& product : products) {
                if(product.quantity != 0){
                    file.write((char*)&product, sizeof(Product));
                }
            }
        }
        else{
            cout << "Error opening file" << endl;
        }
        file.close();
    }
}

void searchProductByName(string name) {
    ifstream file("product.bin", ios::binary);
    if (!file.is_open()) {
        cout << "Error opening file" << endl;
        return;
    }

    Product product;
    bool found = false;
    while (file.read((char*)&product, sizeof(Product))) {
        if (name == product.name) {
            found = true;
            cout << "Name: " << product.name << endl;
            cout << "Quantity: " << product.quantity << endl;
            cout << "Price: " << product.price << endl;
            break;
        }
    }
    if (!found) {
        cout << "product not found" << endl;
    }
    file.close();
}

void top3MostSold(vector<Product>& products) {
    Product temp;
    products.clear();
    ifstream file("product.bin", ios::in | ios::binary);
    if (file.is_open()) {
        while (file.read((char*)&temp, sizeof(temp))) {
            if (temp.sold > 0) {
                products.push_back(temp);
            }
        }
        file.close();
    } 
    else {
        cout << "File not found" << endl;
    }

    sort(products.begin(), products.end(), [](const Product &a, const Product &b){ return a.sold > b.sold; });

    cout << "Top 3 Most Sold Products:" << endl;
    for (int i = 0; i < min((int) products.size(), 3); i++) {
        cout << products[i].name << " - Quantity Sold: " << products[i].sold << endl;
    }
}

void top3LeastSold(vector<Product>& products) {
    Product temp;
    products.clear();
    ifstream file("product.bin", ios::in | ios::binary);
    if (file.is_open()) {
        while (file.read(reinterpret_cast<char*>(&temp), sizeof(Product))) {
            products.push_back(temp);
        }
        file.close();
    } 
    else {
        cout << "File not found" << endl;
    }

    sort(products.begin(), products.end(), [](const Product &a, const Product &b){ return a.sold < b.sold; });

    cout << "Top 3 Least Sold Products:" << endl;
    int limit = min((int) products.size(), 3);
    for (int i = 0; i < limit; i++) {
        cout << products[i].name << " - Quantity Sold: " << products[i].sold << endl;
    }
}

void top3MostEarned(vector<Product>& products) {
    Product temp;
    products.clear();
    ifstream file("product.bin", ios::in | ios::binary);
    if (file.is_open()) {
        while (file.read(reinterpret_cast<char*>(&temp), sizeof(Product))) {
            if (temp.quantity > 0) {
                products.push_back(temp);
            }
        }
        file.close();
    } 
    else {
        cout << "File not found" << endl;
    }

    sort(products.begin(), products.end(), earningsCompare);

    cout << "Top 3 Products for Which the Most Amount Has Been Earned:" << endl;
    for (int i = products.size() - 1; i >= max(0, (int) products.size() - 3); i--) {
        cout << products[i].name << " - Earnings: $" << products[i].earnings << endl;
    }
}

void top3LeastEarned(vector<Product> products) {
    Product temp;
    products.clear();
    ifstream file("product.bin", ios::in | ios::binary);
    if (file.is_open()) {
        while (file.read(reinterpret_cast<char*>(&temp), sizeof(Product))) {
            if (temp.quantity > 0) {
                products.push_back(temp);
            }
        }
        file.close();
    } 
    else {
        cout << "File not found" << endl;
    }

    sort(products.begin(), products.end(), earningsCompare);

    cout << "Top 3 Products for Which the Least Amount Has Been Earned:" << endl;
    for (int i = 0; i < min((int) products.size(), 3); i++) {
        cout << products[i].name << " - Earnings: $" << products[i].earnings << endl;
    }
}

void top3MostExpensive(vector<Product>& products) {
    Product temp;
    products.clear();
    ifstream file("product.bin", ios::in | ios::binary);
    if (file.is_open()) {
        while (file.read(reinterpret_cast<char*>(&temp), sizeof(Product))) {
            products.push_back(temp);
        }
    } 
    else {
        cout << "File not found" << endl;
    }

    sort(products.begin(), products.end(), priceCompare);

    try {
        cout << "Top 3 Expensive product:" << endl;
        for (int i = 0; i < min((int) products.size(), 3); i++) {
            if (i < 0 || i >= products.size()) {
                throw out_of_range("Index out of range");
            }
            cout << products[i].name << " - Price: $" << products[i].price << endl;
        }
    }

    catch (const exception& e) {
        cout << e.what() << endl;
    }
    file.close(); 
}

void top3Cheapest(vector<Product>& products) {
    Product temp;
    products.clear();
    ifstream file("product.bin", ios::in | ios::binary);
    if (file.is_open()) {
        while (file.read(reinterpret_cast<char*>(&temp), sizeof(Product))) {
            products.push_back(temp);
        }
    } 
    else {
        cout << "File not found" << endl;
    }

    sort(products.begin(), products.end(), priceCompare);

    try {
        cout << "Top 3 Cheapest product:" << endl;
        for (int i = 0; i < min((int) products.size(), 3); i++) {
            if(i < 0 || i >= products.size()) {
                throw out_of_range("Index out of range");
            }
            cout << products[i].name << " - Price: $" << products[i].price << endl;
        }
    }

    catch (const exception& e) {
        cout << e.what() << endl;
    }
    file.close(); 
}

int main() {
    vector<Product> products;
    int choice;
    string productName;

    while (true) {
        cout << "1. Insert a new product" << endl;
        cout << "2. Output all data" << endl;
        cout << "3. Sell a product" << endl;
        cout << "4. Info about specific product" << endl;
        cout << "5. Top 3 most sold products" << endl;
        cout << "6. Top 3 least sold products" << endl;
        cout << "7. Top 3 most earned products" << endl;
        cout << "8. Top 3 least earned products" << endl;
        cout << "9. Top 3 most expensive products" << endl;
        cout << "10. Top 3 cheapest products" << endl;
        cout << "11. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                insertProduct(products);
                break;
            case 2:
                outputAllData(products);
                break;
            case 3:
                sellProduct(products);
                break;
            case 4:
                cout << "Enter the product name: ";
                cin >> productName;
                searchProductByName(productName);
                break;
            case 5:
                top3MostSold(products);
                break;
            case 6:
                top3LeastSold(products);
                break;
            case 7:
                top3MostEarned(products);
                break;
            case 8:
                top3LeastEarned(products);
                break;
            case 9:
                top3MostExpensive(products);
                break;
            case 10:
                top3Cheapest(products);
                break;
            case 11:
                return 0;
                break;
            default:
                cout << "Invalid choice" << endl;
                break;
        }
    }
}
