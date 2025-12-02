#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cctype>
using namespace std;

struct Item {
    string name;
    string stat;
    int price;
    int type;
};

int coins = 9000;

vector<Item> allItems = {
    {"Dagger", "+15 ATK", 250, 0}, {"Legion Sword", "+60 ATK", 910, 0}, {"Blade of Despair", "+160 ATK", 3010, 0},
    {"Mystery Codex", "+15 HP", 300, 1}, {"Magic Wand", "+45 MGK", 820, 1}, {"Holy Crystal", "+185 MGK", 3000, 1},
    {"Vitality Crystal", "+230 HP", 300, 2}, {"Ares Belt", "+600 HP", 700, 2}, {"Antique Cuirass", "+920 HP", 2170, 2}
};

vector<int> inventoryPrice;
vector<int> stock(9, 3);

void clearScreen() {
    system("cls");
}

void Continue() {
    int x;
    cout << "\nEnter 1 to continue: ";
    cin >> x;
    while (x != 1) {
        cout << "Invalid. Enter 1 to continue: ";
        cin >> x;
    }
    clearScreen();
}

int pendingTotalPrice() {
    int total = 0;
    for (int shopItem : inventoryPrice)
        total += allItems[shopItem].price;
    return total;
}

void addItem(int shopItem) {
    if (stock[shopItem] <= 0)
        cout << "This item is out of stock.";
    else if (pendingTotalPrice() + allItems[shopItem].price > coins)
        cout << "You don't have enough coins to buy this Item.";
    else {
        inventoryPrice.push_back(shopItem);
        stock[shopItem]--;
        cout << "Successfully added " << allItems[shopItem].name << " to your Inventory.";
    }
    Continue();
}

int extractValue(const string& stat) {
    int value = stat.find(" ");
    return stoi(stat.substr(1, value - 1));
}

void displayItems(const vector<int>& items, const string title) {
    while (true) {
        cout << "--" << title << "--  Coins -> $" << coins;
        if (!inventoryPrice.empty()) cout << " ->" << pendingTotalPrice();
        cout << "\n\n";

        cout << left << setw(4) << "#" << setw(20) << "Item Name"
             << setw(12) << "Stat" << setw(8) << "Stock" << "Price\n";
        cout << "----------------------------------------------------------\n";

        for (size_t i = 0; i < items.size(); ++i) {
            const Item& it = allItems[items[i]];
            cout << left << setw(4) << "(" + to_string(i+1) + ")"
                 << setw(20) << it.name << setw(12) << it.stat
                 << setw(8) << stock[items[i]]
                 << "$" << it.price << "\n";
        }

        cout << "(B) Back\nChoose: ";
        string choice;
        cin >> choice;
        clearScreen();

        if (choice == "B" || choice == "b") break;

        if (isdigit(choice[0])) {
            int c = stoi(choice) - 1;
            if (c >= 0 && c < (int)items.size())
                addItem(items[c]);
        }
    }
}

void shopMenu() {
    while (true) {
        cout << "---Shop---  Coins -> $" << coins;
        if (!inventoryPrice.empty()) cout << " ->" << pendingTotalPrice();
        cout << "\n(1) Weapons\n(2) Armors\n(B) Back\n\nChoose: ";

        string choice;
        cin >> choice;
        clearScreen();

        if (choice == "1") {
            while (true) {
                cout << "--Weapons--  Coins -> $" << coins;
                if (!inventoryPrice.empty()) cout << " ->" << pendingTotalPrice();
                cout << "\n(1) Physical\n(2) Magic\n(B) Back\nChoose: ";
                cin >> choice;
                clearScreen();

                if (choice == "1") displayItems({0,1,2}, "Physical");
                else if (choice == "2") displayItems({3,4,5}, "Magic");
                else if (choice == "B" || choice == "b") break;
            }
        }
        else if (choice == "2") displayItems({6,7,8}, "Armors");
        else if (choice == "B" || choice == "b") break;
    }
}

void inventoryMenu() {
    cout << "--Inventory--  Coins -> $" << coins;
    if (!inventoryPrice.empty()) cout << " ->" << pendingTotalPrice();
    cout << "\n\n";

    if (inventoryPrice.empty())
        cout << "(empty)\n";
    else
        for (int shopItem : inventoryPrice)
            cout << allItems[shopItem].name << "\n";

    cout << "\n(B) Back\n";
    string x; cin >> x;
    clearScreen();
}

void checkoutMenu() {
    if (inventoryPrice.empty()) {
        cout << "No items in inventory to checkout.\n";
        Continue();
        return;
    }

    int atk=0, mgk=0, hp=0, totalPrice=0;

    cout << "--Checkout--\n\n";
    cout << left << setw(20) << "Item Name"
         << setw(12) << "Stat" << "Price\n";
    cout << "--------------------------------------------------\n";

    for (int shopItem : inventoryPrice) {
        const Item& it = allItems[shopItem];
        cout << left << setw(20) << it.name 
             << setw(12) << it.stat 
             << "$" << it.price << "\n";

        totalPrice += it.price;
        if (it.stat.find("ATK") != string::npos) atk += extractValue(it.stat);
        if (it.stat.find("MGK") != string::npos) mgk += extractValue(it.stat);
        if (it.stat.find("HP")  != string::npos) hp  += extractValue(it.stat);
    }

    cout << "--------------------------------------------------\n";

    cout << left << setw(22) << "Total Stats"
         << "| Total: $" << totalPrice << "\n";

    cout << left << setw(22) << ("ATK +" + to_string(atk))
         << "| Coins -$" << totalPrice << "\n";

    cout << left << setw(22) << ("MGK +" + to_string(mgk))
         << "|        -------" << "\n";

    cout << left << setw(22) << ("HP +" + to_string(hp))
         << "| Coins ->$" << (coins - totalPrice) << "\n\n";

    cout << "(F) Finalize Purchase\n(1) Start New Terminal\n(B) Back\nChoose: ";
    string choice;
    cin >> choice;
    clearScreen();

    if (choice=="F" || choice=="f") {
        coins -= totalPrice;
        inventoryPrice.clear();
        cout << "Purchase finalized.\n";
        Continue();
    }
    else if (choice=="1") {
        coins = 9000;
        inventoryPrice.clear();
        stock.assign(9, 3);
        cout << "Terminal restarted.\n";
        Continue();
    }
}

int main() {
    clearScreen();
    cout << "Welcome to Terminal Legends: Shop Shop!\nYou have 6000 coins to spend.\n";
    Continue();

    while (true) {
        cout << "--Main Menu--  Coins -> $" << coins;
        if (!inventoryPrice.empty()) cout << " ->" << pendingTotalPrice();

        cout << "\n(1) Tutorial\n(2) Shop\n(3) Inventory\n(4) Checkout\n\nChoose: ";
        string choice;
        cin >> choice;
        clearScreen();

        if (choice == "1") {
            cout << "---Tutorial---\nPick items to add to your inventory.\nCoins are deducted after checkout.\n";
            Continue();
        }
        else if (choice == "2") shopMenu();
        else if (choice == "3") inventoryMenu();
        else if (choice == "4") checkoutMenu();
    }

    return 0;
}
