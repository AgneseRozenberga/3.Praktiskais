#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Product {
    int id;
    string name;
    int quantity;
    double price;
    double earnings; // Earnings = Quantity * Price
};

// Comparison function to sort the products by price
bool priceCompare(Product a, Product b) {
    return a.price < b.price;
}

void top3Cheapest() {
    vector<Product> products;

    fstream file("products.bin", ios::in | ios::binary);
    Product product;

    while (file.read((char *) &product, sizeof(Product))) {
        products.push_back(product);
    }

    file.close();

    sort(products.begin(), products.end(), priceCompare);

    cout << "Top 3 Cheapest Products:" << endl;
    for (int i = 0; i < min((int) products.size(), 3); i++) {
        cout << products[i].name << " - Price: $" << products[i].price << endl;
    }
}

void insertProduct() {
    // Prompt user for the product details
    Product newProduct;
    cout << "Enter the ID of the new product: ";
    cin >> newProduct.id;
    cout << "Enter the name of the new product: ";
    cin >> newProduct.name;
    cout << "Enter the quantity of the new product: ";
    cin >> newProduct.quantity;
    cout << "Enter the price of the new product: ";
    cin >> newProduct.price;
    newProduct.earnings = newProduct.quantity * newProduct.price;
    bool idExist = false;
    // Open the binary file and check if the id exist or not 
    fstream file("products.bin", ios::in | ios::binary);
    Product p;
    while (file.read((char *) &p, sizeof(Product))) {
        if (p.id == newProduct.id) {
            idExist = true;
            break;
        }
    }
    file.close();
    // if id not exist, insert the new product in the binary file
    if (!idExist) {
        file.open("products.bin", ios::out | ios::app | ios::binary);
        file.write((char *) &newProduct, sizeof(Product));
        file.close();
        cout<< "product added successfully"<<endl;
    } else {
        cout << "product with id = " << newProduct.id << " already exist" << endl;
    }
}

void outputAllData() {
    fstream file("products.bin", ios::in | ios::binary);
    Product product;
    cout<< "All Products:"<<endl;
    while (file.read((char *) &product, sizeof(Product))) {
        cout << "ID: " << product.id << " Name: " << product.name << " Quantity: " << product.quantity << " Price: $" << product.price << " Earnings: $" << product.earnings << endl;
    }
    file.close();
}

bool quantityCompare(Product a, Product b) {
    return a.quantity > b.quantity;
}

void top3MostSold() {
    vector<Product> products;

    fstream file("products.bin", ios::in | ios::binary);
    Product product;

    while (file.read((char *) &product, sizeof(Product))) {
        products.push_back(product);
    }

    file.close();

    sort(products.begin(), products.end(), quantityCompare);

    cout << "Top 3 Most Sold Products:" << endl;
    for (int i = 0; i < min((int) products.size(), 3); i++) {
        cout << products[i].name << " - Quantity Sold: " << products[i].quantity << endl;
    }
}

bool earningsCompare(Product a, Product b) {
    return a.earnings < b.earnings;
}

void top3LeastEarned() {
    vector<Product> products;

    fstream file("products.bin", ios::in | ios::binary);
    Product product;

    while (file.read((char *) &product, sizeof(Product))) {
        products.push_back(product);
    }

    file.close();

    sort(products.begin(), products.end(), earningsCompare);

    cout << "Top 3 Products for Which the Least Amount Has Been Earned:" << endl;
    for (int i = 0; i < min((int) products.size(), 3); i++) {
        cout << products[i].name << " - Earnings: $" << products[i].earnings << endl;
    }
}

bool priceDescendingCompare(Product a, Product b) {
    return a.price > b.price;
}

void top3MostExpensive() {
    vector<Product> products;

    fstream file("products.bin", ios::in | ios::binary);
    Product product;

    while (file.read((char *) &product, sizeof(Product))) {
        products.push_back(product);
    }

    file.close();

    sort(products.begin(), products.end(), priceDescendingCompare);

    cout << "Top 3 Most Expensive Products:" << endl;
    for (int i = 0; i < min((int) products.size(), 3); i++) {
        cout << products[i].name << " - Price: $" << products[i].price << endl;
    }
}

int main() {
    fstream file("products.bin", ios::in | ios::out | ios::binary);
    Product product;
    // Prompt user for the ID of the product to be sold
    int productId;
    cout << "Enter the ID of the product to be sold: ";
    cin >> productId;

    // Search for the product in the binary file and update the quantity
    while (file.read((char*) &product, sizeof(Product))) {
        if (product.id == productId) {
            if (product.quantity > 0) {
                product.quantity--;
                file.seekp(file.tellp() - sizeof(Product));
                file.write((char *) &product, sizeof(Product));
                cout << "Product sold!" << endl;
            } else {
                cout << "There are no products to sell." << endl;
            }
            break;
            }
        }
        if (file.eof()) {
        cout << "Product ID not found. Would you like to add it? (y/n)" << endl;
        char ans;
        cin >> ans;
        
        if (ans == 'y') {
        insertProduct();
        }
    }
    file.close();
    top3Cheapest();
    top3MostSold();
    outputAllData();
    top3LeastEarned();
    top3MostExpensive();
    return 0;
}
