#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm> 
using namespace std;

struct Item {
    string name;
    string stat;
    int price;
    int type; // 0=Physical,1=Magic,2=Armor
};

int coins = 6000;
vector<Item> allItems = {
    {"Dagger", "+15 ATK", 250, 0}, {"Legion Sword", "+60 ATK", 910, 0}, {"Blade of Despair", "+160 ATK", 3010, 0},
    {"Mystery Codex", "+15 HP", 300, 1}, {"Magic Wand", "+45 MGK", 820, 1}, {"Holy Crystal", "+185 MGK", 3000, 1},
    {"Vitality Crystal", "+230 HP", 300, 2}, {"Ares Belt", "+600 HP", 700, 2}, {"Antique Cuirass", "+920 HP", 2170, 2}
};

vector<int> inventoryPrice; 

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
    for (int shopItem : inventoryPrice) total += allItems[shopItem].price;
    return total;
}

void addItem(int shopItem) {
    if (find(inventoryPrice.begin(), inventoryPrice.end(), shopItem) != inventoryPrice.end())
        cout << "You already have this item in your Inventory.";
    else if (pendingTotalPrice() + allItems[shopItem].price > coins)
        cout << "You don't have enough coins to buy this Item.";
    else {
        inventoryPrice.push_back(shopItem);
        cout << "Successfully added " << allItems[shopItem].name << " to your Inventory.";
    }
    Continue();
}

void displayItems(const vector<int> items, const string title) {
    while (true) {
        cout << "--" << title << "--  Coins -> $" << coins;
        int total = pendingTotalPrice();
        if (total > 0) cout << " ->" << total;
        cout << "\n\n";
        cout << left << setw(20) << "Item Name" << setw(12) << "Stat" << setw(8) << "Price\n";
        cout << "--------------------------------------\n";
        for (size_t i = 0; i < items.size(); ++i) {
            const Item& it = allItems[items[i]];
            cout << "(" << i+1 << ") " << setw(18) << it.name << setw(12) << it.stat << "$" << it.price << "\n";
        }
        cout << "(B) Back\nChoose: ";
        string choice; cin >> choice;
        clearScreen();
        if (choice == "B" || choice == "b") break;
        int c = stoi(choice) - 1;
        if (c >= 0 && c < (int)items.size()) addItem(items[c]);
    }
}

void shopMenu() {
    while (true) {
        cout << "---Shop---  Coins -> $" << coins;
        if (!inventoryPrice.empty()) cout << " ->" << pendingTotalPrice();
        cout << "\n(1) Weapons\n(2) Armors\n(B) Back\n\nChoose: ";
        string choice; cin >> choice; clearScreen();
        if (choice=="1") {
            while (true) {
                cout << "--Weapons--  Coins -> $" << coins;
                if (inventoryPrice.empty()) cout << " ->" << pendingTotalPrice();
                cout << "\n(1) Physical\n(2) Magic\n(B) Back\nChoose: ";
                cin >> choice; clearScreen();
                if (choice=="1") displayItems({0,1,2}, "Physical");
                else if (choice=="2") displayItems({3,4,5}, "Magic");
                else if (choice=="B"||choice=="b") break;
            }
        } else if (choice=="2") displayItems({6,7,8}, "Armors");
        else if (choice=="B"||choice=="b") break;
    }
}

void inventoryMenu() {
    cout << "--Inventory--  Coins -> $" << coins;
    if (!inventoryPrice.empty()) cout << " ->" << pendingTotalPrice();
    cout << "\n\n";
    if (inventoryPrice.empty()) cout << "(empty)\n";
    else for (int shopItem : inventoryPrice) cout << allItems[shopItem].name << "\n";
    cout << "\n(B) Back\n"; string tmp; cin >> tmp; clearScreen();
}

void checkoutMenu() {
    if (inventoryPrice.empty()) {
        cout << "No items in inventory to checkout.\n";
        Continue();
        return;
    }

    int atk=0, mgk=0, hp=0, totalPrice=0;
    cout << "--Checkout--\n\n";
    cout << left << setw(18) << "Item Name" << setw(12) << "Stat" << setw(8) << "Price\n";
    cout << "--------------------------------------\n";
    for (int shopItem : inventoryPrice) {
        const Item& it = allItems[shopItem];
        cout << setw(18) << it.name << setw(12) << it.stat << "$" << it.price << "\n";
        totalPrice += it.price;
        if (it.stat.find("ATK")!=string::npos) atk += stoi(it.stat.substr(1));
        if (it.stat.find("MGK")!=string::npos) mgk += stoi(it.stat.substr(1));
        if (it.stat.find("HP") !=string::npos) hp  += stoi(it.stat.substr(1));
    }
    cout << "--------------------------------------\n";
    cout << "Total Stats              |Total: $" << totalPrice << "\n";
    cout << "ATK +" << atk << "                 |Coins -$" << coins << "\n";
    cout << "MGK +" << mgk << "                 |      -------\n";
    cout << "HP +" << hp << "                  |Coins ->$" << (coins-totalPrice) << "\n\n";

    cout << "(F) Finalize Purchase\n(1) Start New Game\n(B) Back\nChoose: ";
 
       string choice; cin >> choice; clearScreen();
     if (choice=="1") { coins=6000; inventoryPrice.clear(); cout << "Game restarted.\n"; Continue(); }
}

int main() {
    clearScreen();
    cout << "Welcome to Terminal Legends: Shop Shop!\nYou have 6000 coins to spend.\n";
    Continue();

    while (true) {
        cout << "--Main Menu--  Coins -> $" << coins;
        if (inventoryPrice.empty()) cout << " ->" << pendingTotalPrice();
        
        cout << "\n(1) Tutorial\n(2) Shop\n(3) Inventory\n(4) Checkout\n\nChoose: ";
        string choice; cin >> choice; 
        clearScreen();

        if (choice=="1") {
            cout << "---Tutorial---\nPick items to add to your inventory.\nCoins are deducted after checkout.\n";
            Continue();
        }
        else if (choice=="2") shopMenu();
        else if (choice=="3") inventoryMenu();
        else if (choice=="4") checkoutMenu();
    }

    return 0;
}
