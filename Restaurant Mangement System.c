#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50

// Structure for Menu Item (BST Node)
typedef struct MenuItem {
    int id;
    char name[MAX_NAME];
    float price;
    struct MenuItem* left;
    struct MenuItem* right;
} MenuItem;

// Structure for Order (Queue Node)
typedef struct Order {
    int orderId;
    int itemId;
    int quantity;
    struct Order* next;
} Order;

// Structure for Undo Stack Node
typedef struct StackNode {
    int orderId;
    struct StackNode* next;
} StackNode;

// Global Variables
MenuItem* menuRoot = NULL; // BST Root for Menu
Order* front = NULL;       // Front of the Queue (Orders)
Order* rear = NULL;        // Rear of the Queue (Orders)
StackNode* undoStack = NULL; // Stack for Undo functionality
int orderCounter = 1;

// Function Prototypes
void addMenuItem();
void displayMenu(MenuItem* root);
MenuItem* insertMenuItem(MenuItem* root, int id, char name[], float price);
MenuItem* searchMenuItem(MenuItem* root, int id);
MenuItem* deleteMenuItem(MenuItem* root, int id);
MenuItem* findMin(MenuItem* root);
void sortMenu(MenuItem* root);
void takeOrder();
void displayOrders();
void cancelOrder();
void generateBill();
void enqueue(int orderId, int itemId, int quantity);
Order* dequeue();
void push(int orderId);
int pop();
void displayWelcomeScreen();
void displayMainMenu();

// Main Function
int main() {
    int choice;

    displayWelcomeScreen();

    do {
        displayMainMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            addMenuItem();
            break;
        case 2:
            printf("\nMenu:\n");
            displayMenu(menuRoot);
            break;
        case 3:
            printf("Enter Item ID to delete: ");
            int id;
            scanf("%d", &id);
            menuRoot = deleteMenuItem(menuRoot, id);
            printf("Menu item deleted successfully!\n");
            break;
        case 4:
            takeOrder();
            break;
        case 5:
            displayOrders();
            break;
        case 6:
            cancelOrder();
            break;
        case 7:
            generateBill();
            break;
        case 8:
            printf("\nSorted Menu:\n");
            sortMenu(menuRoot);
            break;
        case 9:
            printf("Exiting... Goodbye!\n");
            break;
        default:
            printf("Invalid choice! Try again.\n");
        }
    } while (choice != 9);

    return 0;
}

// Function Definitions

void displayWelcomeScreen() {
    Sleep(1000);  // Pause for 1000 milliseconds (1 second)
    printf("                        ***********************************************\n");
    printf("                        *                                             *\n");
    printf("                        *       WELCOME TO THE RESTAURANT             *\n");
    printf("                        *          MANAGEMENT SYSTEM                  *\n");
    printf("                        *                                             *\n");
    printf("                        ***********************************************\n");
    printf("\n");
    Sleep(2000);  // Pause for 2000 milliseconds (2 seconds)
    printf("\n                         Developed By: [ MUNNA, OYSHE, AKIB, SANCHARI, LABIB ]\n");
    Sleep(2000);  // Pause for 2000 milliseconds (2 seconds)
    printf("\n                         Press Enter to continue...");
    getchar();
}

void displayMainMenu() {
    Sleep(1000);  // Pause for 1000 milliseconds (1 seconds)
    printf("\n");
    printf("                        ***********************************************\n");
    printf("                        *               MAIN MENU                     *\n");
    printf("                        ***********************************************\n");
    Sleep(2000);  // Pause for 2000 milliseconds (2 seconds)
    printf("                        1. Add Menu Item\n");
    printf("                        2. Display Menu\n");
    printf("                        3. Delete Menu Item\n");
    printf("                        4. Take Order\n");
    printf("                        5. Display Orders\n");
    printf("                        6. Cancel Order\n");
    printf("                        7. Generate Bill\n");
    printf("                        8. Sort Menu\n");
    printf("                        9. Exit\n");
    printf("                        ***********************************************\n");
}

void addMenuItem() {
    int id;
    char name[MAX_NAME];
    float price;

    printf("Enter Item ID: ");
    scanf("%d", &id);
    printf("Enter Item Name: ");
    scanf(" %[^\n]", name);
    printf("Enter Item Price: ");
    scanf("%f", &price);

    menuRoot = insertMenuItem(menuRoot, id, name, price);
    printf("Menu item added successfully!\n");
}

MenuItem* insertMenuItem(MenuItem* root, int id, char name[], float price) {
    if (root == NULL) {
        MenuItem* newItem = (MenuItem*)malloc(sizeof(MenuItem));
        newItem->id = id;
        strcpy(newItem->name, name);
        newItem->price = price;
        newItem->left = newItem->right = NULL;
        return newItem;
    }
    if (id < root->id)
        root->left = insertMenuItem(root->left, id, name, price);
    else if (id > root->id)
        root->right = insertMenuItem(root->right, id, name, price);
    return root;
}

void displayMenu(MenuItem* root) {
    if (root != NULL) {
        displayMenu(root->left);
        printf("%d\t%s\t%.2f\n", root->id, root->name, root->price);
        displayMenu(root->right);
    }
}

MenuItem* searchMenuItem(MenuItem* root, int id) {
    if (root == NULL || root->id == id)
        return root;
    if (id < root->id)
        return searchMenuItem(root->left, id);
    return searchMenuItem(root->right, id);
}

MenuItem* deleteMenuItem(MenuItem* root, int id) {
    if (root == NULL) return root;
    if (id < root->id)
        root->left = deleteMenuItem(root->left, id);
    else if (id > root->id)
        root->right = deleteMenuItem(root->right, id);
    else {
        if (root->left == NULL) {
            MenuItem* temp = root->right;
            free(root);
            return temp;
        }
        if (root->right == NULL) {
            MenuItem* temp = root->left;
            free(root);
            return temp;
        }
        MenuItem* temp = findMin(root->right);
        root->id = temp->id;
        strcpy(root->name, temp->name);
        root->price = temp->price;
        root->right = deleteMenuItem(root->right, temp->id);
    }
    return root;
}

MenuItem* findMin(MenuItem* root) {
    while (root->left != NULL) root = root->left;
    return root;
}

void sortMenu(MenuItem* root) {
    displayMenu(root); // BST in-order traversal gives sorted order
}

void takeOrder() {
    int itemId, quantity;
    printf("Enter Item ID to order: ");
    scanf("%d", &itemId);
    printf("Enter Quantity: ");
    scanf("%d", &quantity);

    enqueue(orderCounter++, itemId, quantity);
    printf("Order placed successfully!\n");
}

void enqueue(int orderId, int itemId, int quantity) {
    Order* newOrder = (Order*)malloc(sizeof(Order));
    newOrder->orderId = orderId;
    newOrder->itemId = itemId;
    newOrder->quantity = quantity;
    newOrder->next = NULL;

    if (rear == NULL) {
        front = rear = newOrder;
        return;
    }
    rear->next = newOrder;
    rear = newOrder;
}

Order* dequeue() {
    if (front == NULL) return NULL;
    Order* temp = front;
    front = front->next;
    if (front == NULL) rear = NULL;
    return temp;
}

void displayOrders() {
    if (front == NULL) {
        printf("No orders placed yet!\n");
        return;
    }
    printf("Orders:\n");
    printf("OrderID\tItemID\tQuantity\n");
    Order* temp = front;
    while (temp) {
        printf("%d\t%d\t%d\n", temp->orderId, temp->itemId, temp->quantity);
        temp = temp->next;
    }
}

void cancelOrder() {
    int orderId;
    printf("Enter Order ID to cancel: ");
    scanf("%d", &orderId);

    // Push to undo stack
    push(orderId);

    // Simulate order cancellation
    printf("Order cancelled successfully!\n");
}

void generateBill() {
    Order* temp = NULL;
    float total = 0;

    while ((temp = dequeue()) != NULL) {
        MenuItem* item = searchMenuItem(menuRoot, temp->itemId);
        if (item) {
            printf("OrderID: %d\t%s\tQuantity: %d\tPrice: %.2f\n",
                   temp->orderId, item->name, temp->quantity, item->price * temp->quantity);
            total += item->price * temp->quantity;
        }
        free(temp);
    }
    printf("\nTotal Amount: %.2f\n", total);
    printf("Orders cleared after bill generation.\n");
}

void push(int orderId) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    newNode->orderId = orderId;
    newNode->next = undoStack;
    undoStack = newNode;
}

int pop() {
    if (undoStack == NULL) return -1;
    StackNode* temp = undoStack;
    undoStack = undoStack->next;
    int orderId = temp->orderId;
    free(temp);
    return orderId;
}

