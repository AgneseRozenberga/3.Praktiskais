#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Product {
    string name;
    int quantity;
    double price;
    double earnings; // Earnings = Quantity * Price

};

// Comparison function to sort the product by price
bool priceCompare(Product a, Product b) {
    return a.price < b.price;
}

void top3Cheapest() {
    vector<Product> products;
    Product temp;
    fstream file("product.bin", ios::in | ios::binary);
    while (file.read((char *) &temp, sizeof(Product))) {
        products.push_back(temp);
    }
    file.close();

    sort(products.begin(), products.end(), priceCompare);

    cout << "Top 3 Cheapest product:" << endl;
    for (int i = 0; i < min((int) products.size(), 3); i++) {
        cout << products[i].name << " - Price: $" << products[i].price << endl;
    }
}
bool quantityCompare(Product a, Product b) {
    return a.quantity > b.quantity;
}
void top3MostSold() {
    vector<Product> products;

    fstream file("product.bin", ios::in | ios::binary);
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

void insertProduct() {
    Product newProduct;
    // Prompt user for the product details

    cout << "Enter the name of the new product: ";
    cin.ignore();
    getline(cin, newProduct.name);
    cout << "Enter the quantity of the new product: ";
    cin >> newProduct.quantity;
    cout << "Enter the price of the new product: ";
    cin >> newProduct.price;
    newProduct.earnings = newProduct.quantity * newProduct.price;
    bool idExist = false;
    // Open the binary file and check if the id exist or not 
    fstream file("product.bin", ios::in | ios::binary);
    Product product;
    while (file.read((char* ) &product, sizeof(Product))) {
        if (product.name == newProduct.name) {
            idExist = true;
            break;
        }
    }
    file.close();
    // if id not exist, insert the new product in the binary file
    if (!idExist) {
        file.open("product.bin", ios::out | ios::app | ios::binary);
        file.write((char*) &newProduct, sizeof(Product));
        file.close();
        cout<< "product added successfully"<<endl;
    } else {
        cout << "product with name = " << newProduct.name << " already exist" << endl;
    }
}

bool earningsCompare(Product a, Product b) {
    return a.earnings < b.earnings;

}
void top3LeastEarned() {
    vector<Product> products;
    Product temp;
    fstream file("product.bin", ios::in | ios::binary);

    while (file.read((char *) &temp, sizeof(Product))) {
        products.push_back(temp);
    }

    file.close();

    sort(products.begin(), products.end(), earningsCompare);

    cout << "Top 3 Least Earned product:" << endl;
    for (int i = 0; i < min((int) products.size(), 3); i++) {
        cout << products[i].name << " - Earnings: $" << products[i].earnings << endl;
    }
}
void outputAllData() {
    fstream file("product.bin", ios::in | ios::binary);
    Product product;
    cout<< "All Products:"<<endl;
    while (file.read((char *) &product, sizeof(Product))) {
        cout << " Name: " << product.name << " Quantity: " << product.quantity << " Price: $" << product.price << " Earnings: $" << product.earnings << endl;
    }
    file.close();
}

int main() {
cout << "Welcome to Product Management System" << endl;
// Creating a variable to store the user's choice
int choice;

// Looping until the user exits
do {
    cout << "1. Insert Product" << endl;
    cout << "2. Output All Data" << endl;
    cout << "3. Top 3 Cheapest Products" << endl;
    cout << "4. Top 3 Most Sold Products" << endl;
    cout << "5. Top 3 Least Earned Products" << endl;
    cout << "6. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            insertProduct();
            break;
        case 2:
            outputAllData();
            break;
        case 3:
            top3Cheapest();
            break;
        case 4:
            top3MostSold();
            break;
        case 5:
            top3LeastEarned();
            break;
        case 6:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
    }
} while (choice != 6);

return 0;
}
