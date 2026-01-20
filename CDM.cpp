#include <conio.h>		 // For console input/output functions like _getch()
#include <ctime>		 // For time-related functions
#include <fstream>		 // For file input/output operations
#include <iostream>		 // For standard input/output operations
#include <istream>		 // For input stream operations
#include <stdio.h>		 // For standard input/output functions
#include <stdlib.h>		 // For general utility functions
#include <string.h>		 // For string manipulation functions
#include <string>		 // For C++ string class
#include <vector>		 // For dynamic arrays (vectors)
#include <Windows.h>	 // For Windows-specific functions like console manipulation
#include <time.h>		 // For time functions
#include <iomanip>		 // For input/output manipulators
#include <sstream>		 // For string stream operations
#include <locale>		 // For localization support
#include <fcntl.h>		 // For file control operations
#include <io.h>			 // For input/output operations
#include <limits>		 // For numeric limits
#include <ios>			 // For input/output stream base classes
#include <unordered_map> // For hash map data structure
#include <queue>		 // For queue data structure
#include <stack>		 // For stack data structure
#include <algorithm>	 // For algorithms like sort, find, etc.
#include <map>			 // For map data structure

using namespace std; // Use standard namespace to avoid writing std:: repeatedly

// Data Structures for Car Dealership Management System
// This section defines various data structures used throughout the program

// 1. Car Node for Linked List (Car Inventory Management)
struct CarNode
{
	int carID;		  // Unique identifier for each car
	string brand;	  // Car manufacturer (e.g., Toyota, Honda)
	string model;	  // Car model name (e.g., Camry, Civic)
	double price;	  // Price of the car in currency units
	bool isAvailable; // Availability status (true = available, false = sold)
	int popularity;	  // Popularity score for recommendation system
	CarNode *next;	  // Pointer to next car in the linked list

	// Constructor to initialize a new car node with given parameters
	CarNode(int id, string b, string m, double p, bool avail = true, int pop = 0)
		: carID(id), brand(b), model(m), price(p), isAvailable(avail), popularity(pop), next(nullptr) {}
};

// 2. Customer Record for Hash Map (Customer Database)
struct Customer
{
	int customerID; // Unique customer identification number
	string name;	// Customer's full name
	string phone;	// Customer's phone number
	string email;	// Customer's email address
	double budget;	// Customer's budget for car purchase

	// Default constructor
	Customer() : customerID(0), budget(0.0) {}
	// Parameterized constructor
	Customer(int id, string n, string p, string e, double b)
		: customerID(id), name(n), phone(p), email(e), budget(b) {}
};

// 3. BST Node for Car Search & Sorting (Binary Search Tree)
struct BSTNode
{
	CarNode *car;	// Pointer to car data
	BSTNode *left;	// Pointer to left child node
	BSTNode *right; // Pointer to right child node

	// Constructor to create BST node with car data
	BSTNode(CarNode *c) : car(c), left(nullptr), right(nullptr) {}
};

// 4. Test Drive Request for Queue (FIFO - First In, First Out)
struct TestDriveRequest
{
	int customerID;		// ID of customer requesting test drive
	int carID;			// ID of car for test drive
	string requestTime; // Timestamp of the request

	// Constructor to create test drive request
	TestDriveRequest(int cid, int carid, string time)
		: customerID(cid), carID(carid), requestTime(time) {}
};

// 5. Sale Transaction for Stack (LIFO - Last In, First Out for Undo functionality)
struct SaleTransaction
{
	int saleID;		 // Unique sale transaction ID
	int customerID;	 // ID of customer who made purchase
	int carID;		 // ID of car that was sold
	double amount;	 // Sale amount
	string saleTime; // Timestamp of the sale

	// Constructor to create sale transaction record
	SaleTransaction(int sid, int cid, int carid, double amt, string time)
		: saleID(sid), customerID(cid), carID(carid), amount(amt), saleTime(time) {}
};

// 6. Graph Node for Showroom Workflow (Department Management)
struct WorkflowNode
{
	string department;		 // Department name (Sales, Finance, Service)
	vector<int> connections; // List of connected departments (workflow path)

	WorkflowNode() : department("") {}				// Default constructor
	WorkflowNode(string dept) : department(dept) {} // Parameterized constructor
};

// 7. Car Recommendation for Priority Queue (Max Heap for best recommendations)
struct CarRecommendation
{
	CarNode *car; // Pointer to recommended car
	double score; // Recommendation score (higher = better match)

	// Constructor to create car recommendation
	CarRecommendation(CarNode *c, double s) : car(c), score(s) {}

	// Operator overload for priority queue (max heap based on score)
	bool operator<(const CarRecommendation &other) const
	{
		return score < other.score; // Lower scores have lower priority
	}
};

// Car Dealership Management System Class
class CarDealershipSystem
{
private:
	// 1. Linked List for Car Inventory
	CarNode *carInventoryHead;

	// 2. Hash Map for Customer Records
	unordered_map<int, Customer> customerRecords;

	// 3. BST for Car Search & Sorting
	BSTNode *priceSearchRoot;

	// 4. Queue for Test Drive Requests
	queue<TestDriveRequest> testDriveQueue;

	// 5. Stack for Undo Last Sale
	stack<SaleTransaction> salesHistory;

	// 6. Graph for Showroom Workflow
	map<string, WorkflowNode> workflowGraph;

	// 7. Priority Queue for Car Recommendations
	priority_queue<CarRecommendation> recommendationQueue;

	// Counters
	int nextCarID;
	int nextCustomerID;
	int nextSaleID;

public:
	CarDealershipSystem() : carInventoryHead(nullptr), priceSearchRoot(nullptr),
							nextCarID(1), nextCustomerID(1), nextSaleID(1)
	{
		initializeWorkflowGraph();
	}

	// Destructor to clean up memory
	~CarDealershipSystem()
	{
		// Clean up linked list
		while (carInventoryHead != nullptr)
		{
			CarNode *temp = carInventoryHead;
			carInventoryHead = carInventoryHead->next;
			delete temp;
		}

		// Clean up BST
		deleteBST(priceSearchRoot);
	}

	// Helper function to delete BST
	void deleteBST(BSTNode *root)
	{
		if (root != nullptr)
		{
			deleteBST(root->left);
			deleteBST(root->right);
			delete root;
		}
	}

	// Initialize the workflow graph
	void initializeWorkflowGraph()
	{
		workflowGraph["Sales"] = WorkflowNode("Sales");
		workflowGraph["Finance"] = WorkflowNode("Finance");
		workflowGraph["Service"] = WorkflowNode("Service");

		// Define workflow connections
		workflowGraph["Sales"].connections.push_back(1);   // Sales -> Finance
		workflowGraph["Finance"].connections.push_back(2); // Finance -> Service
		workflowGraph["Service"].connections.push_back(0); // Service -> Sales (cycle)
	}

	// 1. Linked List Operations - Car Inventory
	void addCarToInventory(string brand, string model, double price, int popularity = 0)
	{
		CarNode *newCar = new CarNode(nextCarID++, brand, model, price, true, popularity);
		newCar->next = carInventoryHead;
		carInventoryHead = newCar;

		// Also add to BST for searching
		insertIntoBST(priceSearchRoot, newCar);
	}

	void removeCarFromInventory(int carID)
	{
		CarNode *current = carInventoryHead;
		CarNode *prev = nullptr;

		while (current != nullptr && current->carID != carID)
		{
			prev = current;
			current = current->next;
		}

		if (current != nullptr)
		{
			if (prev == nullptr)
			{
				carInventoryHead = current->next;
			}
			else
			{
				prev->next = current->next;
			}
			delete current;
		}
	}

	void displayCarInventory()
	{
		CarNode *current = carInventoryHead;
		cout << "\n=== Car Inventory (Linked List) ===" << endl;
		while (current != nullptr)
		{
			cout << "ID: " << current->carID
				 << " | Brand: " << current->brand
				 << " | Model: " << current->model
				 << " | Price: $" << current->price
				 << " | Available: " << (current->isAvailable ? "Yes" : "No") << endl;
			current = current->next;
		}
	}

	// 2. Hash Map Operations - Customer Records
	void registerCustomer(string name, string phone, string email, double budget)
	{
		Customer newCustomer(nextCustomerID, name, phone, email, budget);
		customerRecords[nextCustomerID] = newCustomer;
		nextCustomerID++;
	}

	Customer *findCustomer(int customerID)
	{
		auto it = customerRecords.find(customerID);
		if (it != customerRecords.end())
		{
			return &(it->second);
		}
		return nullptr;
	}

	void displayCustomers()
	{
		cout << "\n=== Customer Records (Hash Map) ===" << endl;
		for (auto &pair : customerRecords)
		{
			Customer &c = pair.second;
			cout << "ID: " << c.customerID
				 << " | Name: " << c.name
				 << " | Phone: " << c.phone
				 << " | Budget: $" << c.budget << endl;
		}
	}

	// 3. BST Operations - Car Search & Sorting
	void insertIntoBST(BSTNode *&root, CarNode *car)
	{
		if (root == nullptr)
		{
			root = new BSTNode(car);
			return;
		}

		if (car->price < root->car->price)
		{
			insertIntoBST(root->left, car);
		}
		else
		{
			insertIntoBST(root->right, car);
		}
	}

	void inOrderTraversal(BSTNode *root)
	{
		if (root != nullptr)
		{
			inOrderTraversal(root->left);
			cout << "ID: " << root->car->carID
				 << " | " << root->car->brand << " " << root->car->model
				 << " | Price: $" << root->car->price << endl;
			inOrderTraversal(root->right);
		}
	}

	void displayCarsSortedByPrice()
	{
		cout << "\n=== Cars Sorted by Price (BST In-Order) ===" << endl;
		inOrderTraversal(priceSearchRoot);
	}

	// 4. Queue Operations - Test Drive Requests
	void requestTestDrive(int customerID, int carID)
	{
		time_t now = time(0);
		string timeStr = ctime(&now);
		testDriveQueue.push(TestDriveRequest(customerID, carID, timeStr));
		cout << "Test drive request added to queue!" << endl;
	}

	void processNextTestDrive()
	{
		if (!testDriveQueue.empty())
		{
			TestDriveRequest request = testDriveQueue.front();
			testDriveQueue.pop();
			cout << "Processing test drive for Customer ID: " << request.customerID
				 << " Car ID: " << request.carID << endl;
		}
		else
		{
			cout << "No test drive requests in queue." << endl;
		}
	}

	void displayTestDriveQueue()
	{
		cout << "\n=== Test Drive Queue ===" << endl;
		cout << "Queue size: " << testDriveQueue.size() << endl;
	}

	// 5. Stack Operations - Undo Last Sale
	void recordSale(int customerID, int carID, double amount)
	{
		time_t now = time(0);
		string timeStr = ctime(&now);
		salesHistory.push(SaleTransaction(nextSaleID++, customerID, carID, amount, timeStr));

		// Mark car as sold
		CarNode *current = carInventoryHead;
		while (current != nullptr)
		{
			if (current->carID == carID)
			{
				current->isAvailable = false;
				break;
			}
			current = current->next;
		}

		cout << "Sale recorded successfully!" << endl;
	}

	void undoLastSale()
	{
		if (!salesHistory.empty())
		{
			SaleTransaction lastSale = salesHistory.top();
			salesHistory.pop();

			// Mark car as available again
			CarNode *current = carInventoryHead;
			while (current != nullptr)
			{
				if (current->carID == lastSale.carID)
				{
					current->isAvailable = true;
					break;
				}
				current = current->next;
			}

			cout << "Last sale undone! Car ID " << lastSale.carID << " is now available." << endl;
		}
		else
		{
			cout << "No sales to undo." << endl;
		}
	}

	// 6. Graph Operations - Workflow Management
	void displayWorkflow()
	{
		cout << "\n=== Showroom Workflow (Graph) ===" << endl;
		vector<string> depts = {"Sales", "Finance", "Service"};
		for (int i = 0; i < depts.size(); i++)
		{
			cout << i << ": " << depts[i] << " -> ";
			for (int conn : workflowGraph[depts[i]].connections)
			{
				cout << depts[conn] << " ";
			}
			cout << endl;
		}
	}

	// BFS for workflow process
	void bfsWorkflow(int startDept)
	{
		vector<string> depts = {"Sales", "Finance", "Service"};
		vector<bool> visited(3, false);
		queue<int> q;

		q.push(startDept);
		visited[startDept] = true;

		cout << "\n=== BFS Workflow Process ===" << endl;
		while (!q.empty())
		{
			int current = q.front();
			q.pop();
			cout << "Processing: " << depts[current] << endl;

			for (int conn : workflowGraph[depts[current]].connections)
			{
				if (!visited[conn])
				{
					visited[conn] = true;
					q.push(conn);
				}
			}
		}
	}

	// 7. Priority Queue Operations - Car Recommendations
	void generateRecommendations(double customerBudget)
	{
		// Clear previous recommendations
		while (!recommendationQueue.empty())
		{
			recommendationQueue.pop();
		}

		CarNode *current = carInventoryHead;
		while (current != nullptr)
		{
			if (current->isAvailable && current->price <= customerBudget)
			{
				double score = calculateRecommendationScore(current, customerBudget);
				recommendationQueue.push(CarRecommendation(current, score));
			}
			current = current->next;
		}
	}

	double calculateRecommendationScore(CarNode *car, double budget)
	{
		double budgetScore = (budget - car->price) / budget; // Higher score for lower price
		double popularityScore = car->popularity / 10.0;	 // Normalize popularity
		return budgetScore * 0.7 + popularityScore * 0.3;	 // Weighted score
	}

	void displayRecommendations()
	{
		cout << "\n=== Car Recommendations (Priority Queue) ===" << endl;
		priority_queue<CarRecommendation> tempQueue = recommendationQueue;

		int count = 0;
		while (!tempQueue.empty() && count < 5)
		{ // Show top 5 recommendations
			CarRecommendation rec = tempQueue.top();
			tempQueue.pop();
			cout << "Recommended: " << rec.car->brand << " " << rec.car->model
				 << " | Price: $" << rec.car->price
				 << " | Score: " << rec.score << endl;
			count++;
		}
	}

	void addSampleData()
	{
		// Add sample cars
		addCarToInventory("Toyota", "Camry", 25000, 8);
		addCarToInventory("Honda", "Civic", 22000, 9);
		addCarToInventory("BMW", "X5", 55000, 6);
		addCarToInventory("Mercedes", "C-Class", 45000, 7);
		addCarToInventory("Ford", "Focus", 18000, 5);

		// Add sample customers
		registerCustomer("John Doe", "123-456-7890", "john@email.com", 30000);
		registerCustomer("Jane Smith", "098-765-4321", "jane@email.com", 50000);
		registerCustomer("Bob Johnson", "555-123-4567", "bob@email.com", 20000);

		// Add sample test drive requests
		requestTestDrive(1, 1);
		requestTestDrive(2, 3);

		cout << "Sample data added successfully!" << endl;
	}

	// Load existing car data from files
	void loadExistingCarData()
	{
		ifstream carsNameFile("Text Files/Cars_Name.txt");
		ifstream carsPriceFile("Text Files/Cars_Price.txt");

		string carName;
		int carPrice;

		// Read cars from existing files and add to our data structures
		while (carsNameFile >> carName && carsPriceFile >> carPrice)
		{
			// Extract brand and model (assuming format like "Toyota_Camry")
			size_t underscorePos = carName.find('_');
			string brand = "Unknown";
			string model = carName;

			if (underscorePos != string::npos)
			{
				brand = carName.substr(0, underscorePos);
				model = carName.substr(underscorePos + 1);
			}

			addCarToInventory(brand, model, carPrice, rand() % 10);
		}

		carsNameFile.close();
		carsPriceFile.close();

		cout << "Existing car data loaded into data structures!" << endl;
	}
};

// Global instance of the dealership system
CarDealershipSystem *dealershipSystem = nullptr;

// Loading function with proper console encoding and characters
void loading()
{
	printf("\e[?25l");										 // Hide cursor
	SetConsoleCP(437);										 // Set console input code page to CP437
	SetConsoleOutputCP(437);								 // Set console output code page to CP437
	int bar1 = 177, bar2 = 219;								 // ASCII characters for loading bar
	cout << "                           Loading..." << endl; // Centered loading text
	cout << "                    ";							 // Center alignment (same as other text)
	for (int i = 0; i < 25; i++)
	{ // Draw empty loading bar background
		cout << (char)bar1;
	}
	cout << "\r";					// Return to beginning of line
	cout << "                    "; // Center alignment (same as other text)
	for (int i = 0; i < 25; i++)
	{						// Fill loading bar progressively
		cout << (char)bar2; // Print filled character
		Sleep(150);			// Wait 150ms between each character
	}
	cout << endl
		 << endl; // Add spacing after loading
}

class HeaderClass
{
public:
	void titleText(string TitleText)
	{
		// Text COLOR settings
		HANDLE titleTextColor = GetStdHandle(STD_OUTPUT_HANDLE);

		system("COLOR A");
		system("CLS");

		cout << endl;
		SetConsoleTextAttribute(titleTextColor, 9);
		cout << "   _.-._.-._.-._.-_.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._   ";
		cout << endl;
		SetConsoleTextAttribute(titleTextColor, 9);
		cout << " ,'";
		SetConsoleTextAttribute(titleTextColor, 11);
		cout << "_.-._.-._.-._.-._.-._.-_.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._";
		SetConsoleTextAttribute(titleTextColor, 9);
		cout << "`. ";
		cout << endl;
		SetConsoleTextAttribute(titleTextColor, 9);
		cout << "( ";
		SetConsoleTextAttribute(titleTextColor, 11);
		cout << "(                                                                )";
		SetConsoleTextAttribute(titleTextColor, 9);
		cout << " )";
		cout << endl;
		SetConsoleTextAttribute(titleTextColor, 9);
		cout << " )";
		SetConsoleTextAttribute(titleTextColor, 11);
		cout << " )";
		SetConsoleTextAttribute(titleTextColor, 10);
		cout << "                Car Dealership Management (CDM)               ";
		SetConsoleTextAttribute(titleTextColor, 11);
		cout << "( ";
		SetConsoleTextAttribute(titleTextColor, 9);
		cout << "(";
		cout << endl;
		SetConsoleTextAttribute(titleTextColor, 9);
		cout << "( ";
		SetConsoleTextAttribute(titleTextColor, 11);
		cout << "(";
		SetConsoleTextAttribute(titleTextColor, 8);
		cout << TitleText;
		SetConsoleTextAttribute(titleTextColor, 11);
		cout << ")";
		SetConsoleTextAttribute(titleTextColor, 9);
		cout << " )";
		cout << endl;
		SetConsoleTextAttribute(titleTextColor, 9);
		cout << " )";
		SetConsoleTextAttribute(titleTextColor, 11);
		cout << " )                                                              ( ";
		SetConsoleTextAttribute(titleTextColor, 9);
		cout << "(";
		cout << endl;
		SetConsoleTextAttribute(titleTextColor, 9);
		cout << "( ";
		SetConsoleTextAttribute(titleTextColor, 11);
		cout << "(_.-._.-._.-._.-._.-._.-_.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._)";
		SetConsoleTextAttribute(titleTextColor, 9);
		cout << " )";
		cout << endl;
		cout << " `._.-._.-._.-._.-._.-._.-_.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.' ";
		cout << endl;
	}

	void userTitleText(string UserTitleText, string UserTitle, string UserTitleSeparatorLeft, string UserTitleSeparatorRight)
	{
		// Text COLOR settings
		HANDLE userTitleTextColor = GetStdHandle(STD_OUTPUT_HANDLE);

		system("COLOR A");
		system("CLS");

		cout << endl;
		SetConsoleTextAttribute(userTitleTextColor, 9);
		cout << "   _.-._.-._.-._.-_.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._   ";
		cout << endl;
		SetConsoleTextAttribute(userTitleTextColor, 9);
		cout << " ,'";
		SetConsoleTextAttribute(userTitleTextColor, 11);
		cout << "_.-._.-._.-._.-._.-._.-_.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._";
		SetConsoleTextAttribute(userTitleTextColor, 9);
		cout << "`. ";
		cout << endl;
		SetConsoleTextAttribute(userTitleTextColor, 9);
		cout << "( ";
		SetConsoleTextAttribute(userTitleTextColor, 11);
		cout << "(                                                                )";
		SetConsoleTextAttribute(userTitleTextColor, 9);
		cout << " )";
		cout << endl;
		SetConsoleTextAttribute(userTitleTextColor, 9);
		cout << " )";
		SetConsoleTextAttribute(userTitleTextColor, 11);
		cout << " )                ";
		SetConsoleTextAttribute(userTitleTextColor, 10);
		cout << "Car Dealership Management (CDM)";
		SetConsoleTextAttribute(userTitleTextColor, 11);
		cout << "               ( ";
		SetConsoleTextAttribute(userTitleTextColor, 9);
		cout << "(";
		cout << endl;
		SetConsoleTextAttribute(userTitleTextColor, 9);
		cout << "( ";
		SetConsoleTextAttribute(userTitleTextColor, 11);
		cout << "(                           ";
		SetConsoleTextAttribute(userTitleTextColor, 0);
		cout << "          ";
		SetConsoleTextAttribute(userTitleTextColor, 11);
		cout << "                           )";
		SetConsoleTextAttribute(userTitleTextColor, 9);
		cout << " )";
		cout << endl;
		SetConsoleTextAttribute(userTitleTextColor, 9);
		cout << " )";
		SetConsoleTextAttribute(userTitleTextColor, 11);
		cout << " )";
		cout << UserTitleText;
		cout << "( ";
		SetConsoleTextAttribute(userTitleTextColor, 9);
		cout << "(";
		cout << endl;
		cout << "( ";
		SetConsoleTextAttribute(userTitleTextColor, 11);
		cout << "(";
		cout << UserTitleSeparatorLeft;
		SetConsoleTextAttribute(userTitleTextColor, 8);
		cout << UserTitle;
		SetConsoleTextAttribute(userTitleTextColor, 11);
		cout << UserTitleSeparatorRight;
		cout << ")";
		SetConsoleTextAttribute(userTitleTextColor, 9);
		cout << " )";
		cout << endl;
		SetConsoleTextAttribute(userTitleTextColor, 9);
		cout << " )";
		SetConsoleTextAttribute(userTitleTextColor, 11);
		cout << " )                                                              ( ";
		SetConsoleTextAttribute(userTitleTextColor, 9);
		cout << "(";
		cout << endl;
		SetConsoleTextAttribute(userTitleTextColor, 9);
		cout << "( ";
		SetConsoleTextAttribute(userTitleTextColor, 11);
		cout << "(_.-._.-._.-._.-._.-._.-_.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._)";
		SetConsoleTextAttribute(userTitleTextColor, 9);
		cout << " )";
		cout << endl;
		cout << " `._.-._.-._.-._.-._.-._.-_.-._.-._.-._.-._.-._.-._.-._.-._.-._.-._.' ";
		cout << endl;
	}

	void choiceBlue(string choiceNumber, string choice)
	{
		// Text COLOR settings
		HANDLE Choice = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(Choice, 10);
		cout << "    [";
		SetConsoleTextAttribute(Choice, 8);
		cout << choiceNumber;
		SetConsoleTextAttribute(Choice, 10);
		cout << "] ";
		SetConsoleTextAttribute(Choice, 1);
		cout << choice << endl;
	}

	void choiceGreen(string choiceNumber, string choice)
	{
		// Text COLOR settings
		HANDLE Choice = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(Choice, 10);
		cout << "    [";
		SetConsoleTextAttribute(Choice, 8);
		cout << choiceNumber;
		SetConsoleTextAttribute(Choice, 10);
		cout << "] ";
		SetConsoleTextAttribute(Choice, 2);
		cout << choice << endl;
	}

	void choiceRed(string choiceNumber, string choice)
	{
		// Text COLOR settings
		HANDLE Choice = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(Choice, 10);
		cout << "    [";
		SetConsoleTextAttribute(Choice, 4);
		cout << choiceNumber;
		SetConsoleTextAttribute(Choice, 10);
		cout << "] ";
		SetConsoleTextAttribute(Choice, 4);
		cout << choice << endl;
	}

	void choiceBack(string choiceNumber, string choice)
	{
		// Text COLOR settings
		HANDLE Choice = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(Choice, 10);
		cout << endl;
		cout << "   [";
		SetConsoleTextAttribute(Choice, 8);
		cout << choiceNumber;
		SetConsoleTextAttribute(Choice, 10);
		cout << "] ";
		SetConsoleTextAttribute(Choice, 2);
		cout << choice;
		cout << endl;
	}
};

class DetailClass
{
public:
	void AboutUs()
	{
		system("CLS");		// Clear the screen
		system("COLOR 09"); // Set console color scheme

		// Text COLOR settings - get handle for console text attributes
		HANDLE aboutUsColor = GetStdHandle(STD_OUTPUT_HANDLE);

		// Display programmer information with colored text
		SetConsoleTextAttribute(aboutUsColor, 9); // Set text color to blue
		cout << endl;
		cout << endl;
		cout << "   Programmer: ";
		SetConsoleTextAttribute(aboutUsColor, 11);		  // Set text color to cyan
		cout << "Uneeb Khan, Hamza Khan, Madiha" << endl; // Display programmer name
		cout << endl;
		SetConsoleTextAttribute(aboutUsColor, 9); // Set text color back to blue
		cout << "   Course/Section: ";
		SetConsoleTextAttribute(aboutUsColor, 11); // Set text color to cyan
		cout << "SE Fall 24" << endl;			   // Display course information
		SetConsoleTextAttribute(aboutUsColor, 9);  // Set text color to blue
		cout << "   Date: ";
		SetConsoleTextAttribute(aboutUsColor, 11); // Set text color to cyan
		cout << "January 11, 2026" << endl;		   // Display current date
		SetConsoleTextAttribute(aboutUsColor, 9);  // Set text color to blue
		cout << "   Professor: ";
		SetConsoleTextAttribute(aboutUsColor, 11); // Set text color to cyan
		cout << "Sir Syed Burhan" << endl;		   // Display professor name
		cout << endl;
		SetConsoleTextAttribute(aboutUsColor, 9); // Set text color to blue
		cout << "   Info:" << endl;				  // Display project information header
		SetConsoleTextAttribute(aboutUsColor, 0); // Set text color to black
		// Display detailed project information
		cout << "              This project [Car Dealership Management Program],    " << endl;
		cout << "     which is a topic of the [DSA] course, " << endl;
		cout << "     involves creating a car dealership management entirely in C++." << endl;
		cout << "     We are pleased to announce the release of a convenient console" << endl;
		cout << "     application for performing management system tasks like adding" << endl;
		cout << "     new car(s) to a data set, modifying and deleting car(s) from a" << endl;
		cout << "     data set, exporting order logs, etc.                          " << endl;
		cout << "     Involves creating a car dealership management entirely in C++." << endl;
		cout << endl;
		cout << endl;
		SetConsoleTextAttribute(aboutUsColor, 10); // Set text color to green
	}

	void titleScreen()
	{
		// Text COLOR settings
		HANDLE TitleScreenColor = GetStdHandle(STD_OUTPUT_HANDLE);

		system("COLOR A");
		system("CLS");

		// Title design XD
		cout << endl;
		cout << "     .----------------.   .----------------.   .----------------. " << endl;

		cout << "    | ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << ".--------------. ";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << "| | ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << ".--------------.";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << " | | ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << ".--------------.";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << " |" << endl;

		cout << "    | ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "|     ";
		SetConsoleTextAttribute(TitleScreenColor, 1);
		cout << "______";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "   |";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << " | | ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "|  ";
		SetConsoleTextAttribute(TitleScreenColor, 4);
		cout << "________";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "    | ";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << "| |";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << " | ";
		SetConsoleTextAttribute(TitleScreenColor, 5);
		cout << "____    ____";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << " |";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << " | " << endl;

		cout << "    | ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "|";
		SetConsoleTextAttribute(TitleScreenColor, 1);
		cout << "   .' ___  |  ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "| ";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << "| |";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << " |";
		SetConsoleTextAttribute(TitleScreenColor, 4);
		cout << " |_   ___ `.  ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "| ";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << "| |";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << " |";
		SetConsoleTextAttribute(TitleScreenColor, 5);
		cout << "|_   \\  /   _|";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "|";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << " |" << endl;

		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << "    | ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "|";
		SetConsoleTextAttribute(TitleScreenColor, 1);
		cout << "  / .'   \\_|  ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "| ";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << "| |";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << " |";
		SetConsoleTextAttribute(TitleScreenColor, 4);
		cout << "   | |   `. \\ ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "| ";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << "| |";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << " |";
		SetConsoleTextAttribute(TitleScreenColor, 5);
		cout << "  |   \\/   |  ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "|";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << " |" << endl;

		cout << "    | ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "|";
		SetConsoleTextAttribute(TitleScreenColor, 1);
		cout << "  | |         ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "| ";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << "| |";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << " |";
		SetConsoleTextAttribute(TitleScreenColor, 4);
		cout << "   | |    | | ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "| ";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << "| |";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << " |";
		SetConsoleTextAttribute(TitleScreenColor, 5);
		cout << "  | |\\  /| |  ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "|";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << " |" << endl;

		cout << "    | ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "|";
		SetConsoleTextAttribute(TitleScreenColor, 1);
		cout << "  \\ `.___.'\\  ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "|";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << " | | ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "|";
		SetConsoleTextAttribute(TitleScreenColor, 4);
		cout << "  _| |___.' / ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "|";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << " | | ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "|";
		SetConsoleTextAttribute(TitleScreenColor, 5);
		cout << " _| |_\\/_| |_ ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "|";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << " |" << endl;

		cout << "    | ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "|";
		SetConsoleTextAttribute(TitleScreenColor, 1);
		cout << "   `._____.'  ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "| ";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << "| |";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << " |";
		SetConsoleTextAttribute(TitleScreenColor, 4);
		cout << " |________.'  ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "| ";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << "| |";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << " |";
		SetConsoleTextAttribute(TitleScreenColor, 5);
		cout << "|_____||_____|";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "|";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << " |" << endl;

		cout << "    | ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "|              |";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << " | | ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "|              |";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << " | | ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "|              |";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << " |" << endl;

		cout << "    | ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "'--------------'";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << " | | ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "'--------------'";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << " | | ";
		SetConsoleTextAttribute(TitleScreenColor, 8);
		cout << "'--------------'";
		SetConsoleTextAttribute(TitleScreenColor, 10);
		cout << " |" << endl;

		cout << "     '----------------'   '----------------'   '----------------' " << endl;

		cout << endl;
		cout << "                  ";
		SetConsoleTextAttribute(TitleScreenColor, 1);
		cout << "Car ";
		SetConsoleTextAttribute(TitleScreenColor, 4);
		cout << "Dealership ";
		SetConsoleTextAttribute(TitleScreenColor, 5);
		cout << "Management ";
		SetConsoleTextAttribute(TitleScreenColor, 11);
		cout << "Program               " << endl;
		SetConsoleTextAttribute(TitleScreenColor, 1);
		cout << "             _____";
		SetConsoleTextAttribute(TitleScreenColor, 2);
		cout << "  ___  ";
		SetConsoleTextAttribute(TitleScreenColor, 3);
		cout << "  ____    ";
		SetConsoleTextAttribute(TitleScreenColor, 4);
		cout << "__  __ ";
		SetConsoleTextAttribute(TitleScreenColor, 5);
		cout << " ___      ";
		SetConsoleTextAttribute(TitleScreenColor, 6);
		cout << "  ____" << endl;

		SetConsoleTextAttribute(TitleScreenColor, 1);
		cout << "            / ___/";
		SetConsoleTextAttribute(TitleScreenColor, 2);
		cout << " / _ \\";
		SetConsoleTextAttribute(TitleScreenColor, 3);
		cout << "  / __ \\ ";
		SetConsoleTextAttribute(TitleScreenColor, 4);
		cout << " / / / /";
		SetConsoleTextAttribute(TitleScreenColor, 5);
		cout << " / _ \\  ";
		SetConsoleTextAttribute(TitleScreenColor, 6);
		cout << "    / / /" << endl;

		SetConsoleTextAttribute(TitleScreenColor, 1);
		cout << "           / (_ / ";
		SetConsoleTextAttribute(TitleScreenColor, 2);
		cout << "/ , _/ ";
		SetConsoleTextAttribute(TitleScreenColor, 3);
		cout << "/ /_/ / ";
		SetConsoleTextAttribute(TitleScreenColor, 4);
		cout << "/ /_/ / ";
		SetConsoleTextAttribute(TitleScreenColor, 5);
		cout << "/ ___/     ";
		SetConsoleTextAttribute(TitleScreenColor, 6);
		cout << "/_  _/" << endl;

		SetConsoleTextAttribute(TitleScreenColor, 1);
		cout << "           \\___/ ";
		SetConsoleTextAttribute(TitleScreenColor, 2);
		cout << "/_/|_| ";
		SetConsoleTextAttribute(TitleScreenColor, 3);
		cout << " \\____/  ";
		SetConsoleTextAttribute(TitleScreenColor, 4);
		cout << "\\____/ ";
		SetConsoleTextAttribute(TitleScreenColor, 5);
		cout << "/_/      ";
		SetConsoleTextAttribute(TitleScreenColor, 6);
		cout << "    /_/  " << endl;
		SetConsoleTextAttribute(TitleScreenColor, 10);

		cout << endl;
		cout << endl;
		cout << "   ";
		system("PAUSE");
	}
};

class BackEnd
{
public:
	void display_menu(const string &manufacturer, const string names[], const int prices[], const size_t numCars)
	{
		// Text COLOR settings
		HANDLE Display_Menu_Color = GetStdHandle(STD_OUTPUT_HANDLE);

		cout << manufacturer << " Cars:" << endl;

		for (size_t i = 0; i < numCars; i++)
		{
			SetConsoleTextAttribute(Display_Menu_Color, 10);
			cout << "   [";
			SetConsoleTextAttribute(Display_Menu_Color, 8);
			cout << i + 1;
			SetConsoleTextAttribute(Display_Menu_Color, 10);
			cout << "]\t";
			SetConsoleTextAttribute(Display_Menu_Color, 9);
			cout << names[i];
			SetConsoleTextAttribute(Display_Menu_Color, 11);
			cout << "\tfor ";
			SetConsoleTextAttribute(Display_Menu_Color, 9);
			// structuring currency
			struct group_facet : public numpunct<char>
			{
			protected:
				string do_grouping() const { return "\003"; }
			};
			cout.imbue(locale(cout.getloc(), new group_facet));
			cout << fixed << prices[i];
			SetConsoleTextAttribute(Display_Menu_Color, 11);
			cout << " pesos";
			cout << endl;
		}
	}

	void display_color(const string color_index[], const size_t numColors)
	{
		// Text COLOR settings
		HANDLE Display_Color_Color = GetStdHandle(STD_OUTPUT_HANDLE);

		for (size_t a = 0; a < numColors; a++)
		{
			SetConsoleTextAttribute(Display_Color_Color, 10);
			cout << "    [";
			SetConsoleTextAttribute(Display_Color_Color, 8);
			cout << a + 1;
			SetConsoleTextAttribute(Display_Color_Color, 10);
			cout << "] ";
			SetConsoleTextAttribute(Display_Color_Color, 11);
			cout << color_index[a];
			cout << endl;
		}
	}

	void Reciept(string DateTime, string CarNames, string CarColors, int Price, int Payment, int Change)
	{
		// Text COLOR settings
		HANDLE RecieptColor = GetStdHandle(STD_OUTPUT_HANDLE);

		system("COLOR A");
		system("CLS");

		SetConsoleTextAttribute(RecieptColor, 8);
		cout << "\n\t+----------------------------------------------------+" << endl;
		cout << "\t|";
		SetConsoleTextAttribute(RecieptColor, 9);
		cout << "            ___ ___ ___ ___ ___ ___ _____           ";
		SetConsoleTextAttribute(RecieptColor, 8);
		cout << "|" << endl;
		cout << "\t|";
		SetConsoleTextAttribute(RecieptColor, 9);
		cout << "           | _ \\ __/ __| __|_ _| _ \\_   _|          ";
		SetConsoleTextAttribute(RecieptColor, 8);
		cout << "|" << endl;
		cout << "\t|";
		SetConsoleTextAttribute(RecieptColor, 9);
		cout << "           |   / _| (__| _| | ||  _/ | |            ";
		SetConsoleTextAttribute(RecieptColor, 8);
		cout << "|" << endl;
		cout << "\t|";
		SetConsoleTextAttribute(RecieptColor, 9);
		cout << "           |_|_\\___\\___|___|___|_|   |_|            ";
		SetConsoleTextAttribute(RecieptColor, 8);
		cout << "|" << endl;
		cout << "\t|                                                    |" << endl;
		cout << "\t|";
		SetConsoleTextAttribute(RecieptColor, 9);
		cout << " Store: ";
		SetConsoleTextAttribute(RecieptColor, 11);
		cout << "Car Dealership Management (CDM)             ";
		SetConsoleTextAttribute(RecieptColor, 8);
		cout << "|" << endl;
		cout << "\t|";
		SetConsoleTextAttribute(RecieptColor, 9);
		cout << " Store Number: ";
		SetConsoleTextAttribute(RecieptColor, 11);
		cout << "#1              ";
		SetConsoleTextAttribute(RecieptColor, 9);
		cout << "Manager: ";
		SetConsoleTextAttribute(RecieptColor, 11);
		cout << "Uneeb Khan, Hamza Khan, Madiha "; // Updated manager name
		SetConsoleTextAttribute(RecieptColor, 8);
		cout << "|" << endl;
		cout << "\t|";
		SetConsoleTextAttribute(RecieptColor, 9);
		cout << " Address: ";
		SetConsoleTextAttribute(RecieptColor, 11);
		cout << "Air University, Multan, Pakistan         ";
		SetConsoleTextAttribute(RecieptColor, 8);
		cout << "|" << endl;
		cout << "\t|                                                    |" << endl;
		cout << "\t| ";
		SetConsoleTextAttribute(RecieptColor, 9);
		cout << DateTime;
		SetConsoleTextAttribute(RecieptColor, 8);
		cout << "                             |" << endl;
		cout << "\t|                                                    |" << endl;
		cout << "\t|";
		SetConsoleTextAttribute(RecieptColor, 9);
		cout << " ================================================== ";
		SetConsoleTextAttribute(RecieptColor, 8);
		cout << "|" << endl;
		cout << "\t|";
		cout << "     Menu	   Price      Quant.  Total Price    ";
		SetConsoleTextAttribute(RecieptColor, 8);
		cout << "|" << endl;
		cout << "\t|";
		SetConsoleTextAttribute(RecieptColor, 9);
		cout << " ================================================== ";
		SetConsoleTextAttribute(RecieptColor, 8);
		cout << "|" << endl;
		cout << "\t|";
		SetConsoleTextAttribute(RecieptColor, 0);
		cout << " [";
		SetConsoleTextAttribute(RecieptColor, 11);
		cout << CarColors;
		SetConsoleTextAttribute(RecieptColor, 0);
		cout << "] ";
		SetConsoleTextAttribute(RecieptColor, 9);
		cout << CarNames;
		cout << "\t ";
		SetConsoleTextAttribute(RecieptColor, 11);
		cout << Price;
		SetConsoleTextAttribute(RecieptColor, 9);
		cout << "\t1      ";
		SetConsoleTextAttribute(RecieptColor, 11);
		cout << Price;
		SetConsoleTextAttribute(RecieptColor, 8);
		cout << "     |" << endl;
		cout << "\t|";
		SetConsoleTextAttribute(RecieptColor, 9);
		cout << " -------------------------------------------------- ";
		SetConsoleTextAttribute(RecieptColor, 8);
		cout << "|" << endl;
		cout << "\t|";
		SetConsoleTextAttribute(RecieptColor, 9);
		cout << " Total Price                          ";
		SetConsoleTextAttribute(RecieptColor, 11);
		cout << Price;
		SetConsoleTextAttribute(RecieptColor, 8);
		cout << "     |" << endl;
		cout << "\t|                                                    |" << endl;
		cout << "\t|";
		SetConsoleTextAttribute(RecieptColor, 9);
		cout << " Cash                                 ";
		SetConsoleTextAttribute(RecieptColor, 11);
		cout << Payment;
		SetConsoleTextAttribute(RecieptColor, 8);
		cout << "     |" << endl;
		cout << "\t|";
		SetConsoleTextAttribute(RecieptColor, 9);
		cout << " Change                               ";
		SetConsoleTextAttribute(RecieptColor, 11);
		cout << Change;
		SetConsoleTextAttribute(RecieptColor, 8);
		cout << "\t     |" << endl;
		cout << "\t|                                                    |" << endl;
		cout << "\t+----------------------------------------------------+" << endl;
	}

	void orderLogs()
	{
		// Text COLOR settings
		HANDLE OrderLogsColor = GetStdHandle(STD_OUTPUT_HANDLE);

		ifstream Order_Logs("Text Files/Order_Logs.txt");

		string date;
		string time;
		string car_name;
		string car_color;
		int car_price;

		SetConsoleTextAttribute(OrderLogsColor, 8);
		cout << "\n+------------+----------+--------------+------------+----------------+" << endl;
		cout << "|";
		SetConsoleTextAttribute(OrderLogsColor, 10);
		cout << setw(12) << left << "    Date";
		SetConsoleTextAttribute(OrderLogsColor, 8);
		cout << "|";
		SetConsoleTextAttribute(OrderLogsColor, 10);
		cout << setw(10) << left << "   Time";
		SetConsoleTextAttribute(OrderLogsColor, 8);
		cout << "|";
		SetConsoleTextAttribute(OrderLogsColor, 10);
		cout << setw(14) << left << "  Car Name";
		SetConsoleTextAttribute(OrderLogsColor, 8);
		cout << "|";
		SetConsoleTextAttribute(OrderLogsColor, 10);
		cout << setw(12) << left << " Car Color";
		SetConsoleTextAttribute(OrderLogsColor, 8);
		cout << "|";
		SetConsoleTextAttribute(OrderLogsColor, 10);
		cout << setw(16) << left << "   Car Price";
		SetConsoleTextAttribute(OrderLogsColor, 8);
		cout << "|" << endl;
		cout << "+------------+----------+--------------+------------+----------------+" << endl;

		while (Order_Logs >> date >> time >> car_name >> car_color >> car_price)
		{
			SetConsoleTextAttribute(OrderLogsColor, 8);
			cout << "|";
			SetConsoleTextAttribute(OrderLogsColor, 9);

			// Ensure date fits exactly in 12 characters
			string dateStr = "    " + date;
			if (dateStr.length() > 12)
				dateStr = dateStr.substr(0, 12);
			cout << setw(12) << left << dateStr;

			SetConsoleTextAttribute(OrderLogsColor, 8);
			cout << "|";
			SetConsoleTextAttribute(OrderLogsColor, 11);

			// Ensure time fits exactly in 10 characters
			string timeStr = "   " + time;
			if (timeStr.length() > 10)
				timeStr = timeStr.substr(0, 10);
			cout << setw(10) << left << timeStr;

			SetConsoleTextAttribute(OrderLogsColor, 8);
			cout << "|";
			SetConsoleTextAttribute(OrderLogsColor, 9);

			// Ensure car name fits exactly in 14 characters
			string nameStr = "  " + car_name;
			if (nameStr.length() > 14)
				nameStr = nameStr.substr(0, 14);
			cout << setw(14) << left << nameStr;

			SetConsoleTextAttribute(OrderLogsColor, 8);
			cout << "|";
			SetConsoleTextAttribute(OrderLogsColor, 11);

			// Ensure car color fits exactly in 12 characters
			string colorStr = " " + car_color;
			if (colorStr.length() > 12)
				colorStr = colorStr.substr(0, 12);
			cout << setw(12) << left << colorStr;

			SetConsoleTextAttribute(OrderLogsColor, 8);
			cout << "|";
			SetConsoleTextAttribute(OrderLogsColor, 9);

			// Format price with commas and ensure it fits exactly in 16 characters
			struct group_facet : public numpunct<char>
			{
			protected:
				string do_grouping() const { return "\003"; }
			};
			ostringstream priceStr;
			priceStr.imbue(locale(cout.getloc(), new group_facet));
			priceStr << fixed << car_price;
			string priceDisplay = "   " + priceStr.str();
			if (priceDisplay.length() > 16)
				priceDisplay = priceDisplay.substr(0, 16);
			cout << setw(16) << left << priceDisplay;

			SetConsoleTextAttribute(OrderLogsColor, 8);
			cout << "|" << endl;
			cout << "+------------+----------+--------------+------------+----------------+" << endl;
		}
		SetConsoleTextAttribute(OrderLogsColor, 9);
	}

	void loginLogs()
	{
		// Text COLOR settings
		HANDLE LoginLogsColor = GetStdHandle(STD_OUTPUT_HANDLE);

		ifstream Login_Logs("Text Files/Login_Logs.txt");

		string date;
		string time;
		string username;
		string access;

		SetConsoleTextAttribute(LoginLogsColor, 8);
		cout << "\n+----------------+------------+--------------+------------+" << endl;
		cout << "|";
		SetConsoleTextAttribute(LoginLogsColor, 10);
		cout << setw(16) << left << "    Username";
		SetConsoleTextAttribute(LoginLogsColor, 8);
		cout << "|";
		SetConsoleTextAttribute(LoginLogsColor, 10);
		cout << setw(12) << left << "   Access";
		SetConsoleTextAttribute(LoginLogsColor, 8);
		cout << "|";
		SetConsoleTextAttribute(LoginLogsColor, 10);
		cout << setw(14) << left << "     Date";
		SetConsoleTextAttribute(LoginLogsColor, 8);
		cout << "|";
		SetConsoleTextAttribute(LoginLogsColor, 10);
		cout << setw(12) << left << "    Time";
		SetConsoleTextAttribute(LoginLogsColor, 8);
		cout << "|" << endl;
		cout << "+----------------+------------+--------------+------------+" << endl;

		while (Login_Logs >> username >> access >> date >> time)
		{
			SetConsoleTextAttribute(LoginLogsColor, 8);
			cout << "|";
			SetConsoleTextAttribute(LoginLogsColor, 9);

			// Ensure username fits exactly in 16 characters
			string userStr = "    " + username;
			if (userStr.length() > 16)
				userStr = userStr.substr(0, 16);
			cout << setw(16) << left << userStr;

			SetConsoleTextAttribute(LoginLogsColor, 8);
			cout << "|";
			SetConsoleTextAttribute(LoginLogsColor, 11);

			// Ensure access fits exactly in 12 characters
			string accessStr = "   " + access;
			if (accessStr.length() > 12)
				accessStr = accessStr.substr(0, 12);
			cout << setw(12) << left << accessStr;

			SetConsoleTextAttribute(LoginLogsColor, 8);
			cout << "|";
			SetConsoleTextAttribute(LoginLogsColor, 9);

			// Ensure date fits exactly in 14 characters
			string dateStr = "     " + date;
			if (dateStr.length() > 14)
				dateStr = dateStr.substr(0, 14);
			cout << setw(14) << left << dateStr;

			SetConsoleTextAttribute(LoginLogsColor, 8);
			cout << "|";
			SetConsoleTextAttribute(LoginLogsColor, 11);

			// Ensure time fits exactly in 12 characters
			string timeStr = "    " + time;
			if (timeStr.length() > 12)
				timeStr = timeStr.substr(0, 12);
			cout << setw(12) << left << timeStr;

			SetConsoleTextAttribute(LoginLogsColor, 8);
			cout << "|" << endl;
			cout << "+----------------+------------+--------------+------------+" << endl;
		}
		SetConsoleTextAttribute(LoginLogsColor, 9);
	}

	void viewEmployeeData()
	{
		// Text COLOR settings
		HANDLE ViewEmployeeData = GetStdHandle(STD_OUTPUT_HANDLE);

		ifstream EmployeeFile("Text Files/Employee.txt");

		string name;
		string pass;

		SetConsoleTextAttribute(ViewEmployeeData, 8);
		cout << "\n+----------+----------------+----------------------+" << endl;
		cout << "|";
		SetConsoleTextAttribute(ViewEmployeeData, 10);
		cout << setw(10) << left << "   Line";
		SetConsoleTextAttribute(ViewEmployeeData, 8);
		cout << "|";
		SetConsoleTextAttribute(ViewEmployeeData, 10);
		cout << setw(16) << left << "    Username";
		SetConsoleTextAttribute(ViewEmployeeData, 8);
		cout << "|";
		SetConsoleTextAttribute(ViewEmployeeData, 10);
		cout << setw(22) << left << "      Password";
		SetConsoleTextAttribute(ViewEmployeeData, 8);
		cout << "|" << endl;
		cout << "+----------+----------------+----------------------+" << endl;

		int j = 0;
		while (EmployeeFile >> name >> pass)
		{
			j++;
			SetConsoleTextAttribute(ViewEmployeeData, 8);
			cout << "|";
			SetConsoleTextAttribute(ViewEmployeeData, 10);

			// Ensure line number fits exactly in 10 characters
			ostringstream lineStr;
			lineStr << "   [" << j << "]";
			string lineDisplay = lineStr.str();
			if (lineDisplay.length() > 10)
				lineDisplay = lineDisplay.substr(0, 10);
			cout << setw(10) << left << lineDisplay;

			SetConsoleTextAttribute(ViewEmployeeData, 8);
			cout << "|";
			SetConsoleTextAttribute(ViewEmployeeData, 9);

			// Ensure username fits exactly in 16 characters
			string nameStr = "    " + name;
			if (nameStr.length() > 16)
				nameStr = nameStr.substr(0, 16);
			cout << setw(16) << left << nameStr;

			SetConsoleTextAttribute(ViewEmployeeData, 8);
			cout << "|";
			SetConsoleTextAttribute(ViewEmployeeData, 11);

			// Ensure password fits exactly in 22 characters
			string passStr = "      " + pass;
			if (passStr.length() > 22)
				passStr = passStr.substr(0, 22);
			cout << setw(22) << left << passStr;

			SetConsoleTextAttribute(ViewEmployeeData, 8);
			cout << "|" << endl;
			cout << "+----------+----------------+----------------------+" << endl;
		}
	}

	void viewCarNamePrice()
	{
		// Text COLOR settings
		HANDLE ViewCarData = GetStdHandle(STD_OUTPUT_HANDLE);

		ifstream Cars_Name("Text Files/Cars_Name.txt");
		ifstream Cars_Price("Text Files/Cars_Price.txt");

		string CName;
		int CPrice;

		SetConsoleTextAttribute(ViewCarData, 8);
		cout << "\n+----------+----------------+----------------------+" << endl;
		cout << "|";
		SetConsoleTextAttribute(ViewCarData, 10);
		cout << setw(10) << left << "   Line";
		SetConsoleTextAttribute(ViewCarData, 8);
		cout << "|";
		SetConsoleTextAttribute(ViewCarData, 10);
		cout << setw(16) << left << "      Name";
		SetConsoleTextAttribute(ViewCarData, 8);
		cout << "|";
		SetConsoleTextAttribute(ViewCarData, 10);
		cout << setw(22) << left << "        Price";
		SetConsoleTextAttribute(ViewCarData, 8);
		cout << "|" << endl;
		cout << "+----------+----------------+----------------------+" << endl;

		int i = 0;
		while ((Cars_Name >> CName) && (Cars_Price >> CPrice))
		{
			i++;
			SetConsoleTextAttribute(ViewCarData, 8);
			cout << "|";
			SetConsoleTextAttribute(ViewCarData, 10);

			// Ensure line number fits exactly in 10 characters
			ostringstream lineStr;
			lineStr << "   [" << i << "]";
			string lineDisplay = lineStr.str();
			if (lineDisplay.length() > 10)
				lineDisplay = lineDisplay.substr(0, 10);
			cout << setw(10) << left << lineDisplay;

			SetConsoleTextAttribute(ViewCarData, 8);
			cout << "|";
			SetConsoleTextAttribute(ViewCarData, 9);

			// Ensure car name fits exactly in 16 characters
			string nameStr = "     " + CName;
			if (nameStr.length() > 16)
				nameStr = nameStr.substr(0, 16);
			cout << setw(16) << left << nameStr;

			SetConsoleTextAttribute(ViewCarData, 8);
			cout << "|";
			SetConsoleTextAttribute(ViewCarData, 11);

			// Format price with commas and ensure it fits exactly in 22 characters
			struct group_facet : public numpunct<char>
			{
			protected:
				string do_grouping() const { return "\003"; }
			};
			ostringstream priceStr;
			priceStr.imbue(locale(cout.getloc(), new group_facet));
			priceStr << fixed << CPrice;
			string priceDisplay = "  " + priceStr.str() + " pesos";
			if (priceDisplay.length() > 22)
				priceDisplay = priceDisplay.substr(0, 22);
			SetConsoleTextAttribute(ViewCarData, 9);
			cout << setw(22) << left << priceDisplay;

			SetConsoleTextAttribute(ViewCarData, 8);
			cout << "|" << endl;
			cout << "+----------+----------------+----------------------+" << endl;
		}
	}

	void viewCarColor()
	{
		// Text COLOR settings
		HANDLE ViewCarData = GetStdHandle(STD_OUTPUT_HANDLE);

		ifstream CarColorFile("Text Files/Cars_Color.txt");

		string Color;

		cout << endl;
		SetConsoleTextAttribute(ViewCarData, 8);
		cout << "+----------+----------------------------------+" << endl;
		cout << "|";
		SetConsoleTextAttribute(ViewCarData, 10);
		cout << setw(10) << left << "   Line";
		SetConsoleTextAttribute(ViewCarData, 8);
		cout << "|";
		SetConsoleTextAttribute(ViewCarData, 10);
		cout << setw(34) << left << "              Color";
		SetConsoleTextAttribute(ViewCarData, 8);
		cout << "|" << endl;
		cout << "+----------+----------------------------------+" << endl;

		int j = 0;
		while (CarColorFile >> Color)
		{
			j++;
			SetConsoleTextAttribute(ViewCarData, 8);
			cout << "|";
			SetConsoleTextAttribute(ViewCarData, 10);

			// Ensure line number fits exactly in 10 characters
			ostringstream lineStr;
			lineStr << "   [" << j << "]";
			string lineDisplay = lineStr.str();
			if (lineDisplay.length() > 10)
				lineDisplay = lineDisplay.substr(0, 10);
			cout << setw(10) << left << lineDisplay;

			SetConsoleTextAttribute(ViewCarData, 8);
			cout << "|";
			SetConsoleTextAttribute(ViewCarData, 9);

			// Ensure color fits exactly in 34 characters
			string colorStr = "              " + Color;
			if (colorStr.length() > 34)
				colorStr = colorStr.substr(0, 34);
			cout << setw(34) << left << colorStr;

			SetConsoleTextAttribute(ViewCarData, 8);
			cout << "|" << endl;
			cout << "+----------+----------------------------------+" << endl;
		}
	}

	void gotoxy(int x, int y)
	{
		COORD d;
		d.X = x;
		d.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), d);
	}

	void fontTextColor(int Color)
	{
		HANDLE FontColor = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(FontColor, Color);
	}
};

int main()
{
	// Initialize the new data structures system
	dealershipSystem = new CarDealershipSystem();

	// Add some sample data for demonstration
	dealershipSystem->addSampleData();

	// Create instances of classes for different functionalities
	DetailClass AboutUs;	 // For displaying about us information
	DetailClass TitleScreen; // For displaying title screen

	HeaderClass Choice; // For displaying menu choices

	HeaderClass TitleText;	   // For displaying title text
	HeaderClass UserTitleText; // For displaying user-specific title text

	// Create instances for backend operations
	BackEnd Display_Menu;	  // For displaying car menus
	BackEnd Display_Color;	  // For displaying color options
	BackEnd Reciept;		  // For generating receipts
	BackEnd OrderLogs;		  // For managing order logs
	BackEnd ViewCarData;	  // For viewing car data
	BackEnd ViewEmployeeData; // For viewing employee data
	BackEnd Coordinate;		  // For cursor positioning
	BackEnd FontTextColor;	  // For text color management
	BackEnd LoginLogs;		  // For managing login logs

	// Set system title in the console window
	system("TITLE Car Dealership Management Program By: Uneeb Khan, Hamza Khan, Madiha");

	// Set console window size (width: 70, height: 150)
	system("mode 70, 150");

	// Configure console font settings
	static CONSOLE_FONT_INFOEX fontex;			   // Font structure
	fontex.cbSize = sizeof(CONSOLE_FONT_INFOEX);   // Set structure size
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); // Get console handle
	GetCurrentConsoleFontEx(hOut, 1, &fontex);	   // Get current font
	fontex.FontWeight = 200;					   // Set font weight
	fontex.dwFontSize.X = 36;					   // Set font width
	fontex.dwFontSize.Y = 36;					   // Set font height
	SetCurrentConsoleFontEx(hOut, NULL, &fontex);  // Apply font settings

	// Display title screen once using flag system
	bool flag = false; // Flag to ensure title screen shows only once
	if (flag == false)
	{
		TitleScreen.titleScreen(); // Show the main title screen
	}
	flag = true; // Set flag to prevent showing title screen again

	string employeeLogin; // Store employee login name

	// Variables for order confirmation and payment processing
	char order_more = 'n',	// Variable to check if user wants to order more
		order_confirmation; // Variable for order confirmation

	int Payment, // Amount paid by customer
		Value,	 // Total value of the order
		Change,	 // Change to be returned
		Cars,	 // Selected car index
		Color;	 // Selected color index

	// Access level identifier (1=Admin, 2=Employee, 3=Guest)
	int AccessLevel = 0;

	// Menu choice variables
	int choice;			// Main menu choice
	int adminchoice;	// Admin menu choice
	int employeechoice; // Employee menu choice

	// Sub-menu choice variables
	int employeeData; // Employee data management choice
	int CarChoice;	  // Car data management choice

	// Show loading bar only before main menu
	system("CLS");					// Clear the screen
	system("COLOR 0A");				// Set console color scheme
	cout << "\n\n\n\n\n\n\n\n\n\n"; // Add vertical spacing
	cout << "                    Car Dealership Management System\n";
	cout << "                           Starting Program...\n\n";
	loading(); // Call the loading function
	cout << "                         Loading Complete!\n";
	Sleep(500); // Brief pause before continuing

	// Main program loop - keeps running until user chooses to exit
	while (true)
	{
	// Main Login Form - Entry point for user authentication
	LoginForm:

		// Set system title for login form
		system("TITLE Car Dealership Management Program [Login Form]");

		// Display main login menu with title and options
		TitleText.titleText("                    Welcome to the Login Menu                   ");

		cout << endl;
		// Display login options with different colors for different user types
		Choice.choiceBlue("1", "Admin");	// Admin login option in blue
		Choice.choiceBlue("2", "Employee"); // Employee login option in blue
		Choice.choiceBlue("3", "Guest");	// Guest access option in blue

		Choice.choiceGreen("4", "About Us"); // About us option in green

		Choice.choiceRed("5", "Exit"); // Exit option in red

		// Prompt user for their choice
		FontTextColor.fontTextColor(9); // Set text color to blue
		cout << "    Please insert your choice: ";
		FontTextColor.fontTextColor(11); // Set text color to cyan
		cin >> choice;					 // Get user input
		cin.ignore();					 // Clear input buffer

		// Admin Login Process
		if (choice == 1)
		{
		AdminLogin:

			// Set system title for admin login
			system("TITLE Car Dealership Management Program [Admin Login]");

			int adminlogin = 0;		 // Flag for successful login
			string user, pass, u, p; // Variables for username and password

			// Display admin login form
			TitleText.titleText("                           Admin Login                          ");

			Choice.choiceBack("ESC", "Back"); // Show back option

			FontTextColor.fontTextColor(1); // Set text color to dark blue
			cout << "\n   Please enter your login details";
			FontTextColor.fontTextColor(9); // Set text color to blue
			cout << "\n   Username: ";
			FontTextColor.fontTextColor(11); // Set text color to cyan

			// Username input with character-by-character processing
			string userHider; // String to store username
			char c_user;	  // Character input variable

			// Process username input character by character
			do
			{
				c_user = _getch(); // Get character without echo
				switch (c_user)
				{
				case 0:		  // Special key pressed
					_getch(); // Consume the next character
					break;
				case 13: // Enter key pressed
					cout << endl;
					break;
				case 27: // Escape key pressed
					system("CLS");
					goto LoginForm; // Go back to main login form
				case 8:				// Backspace key pressed
					if (userHider.length() > 0)
					{
						userHider.erase(userHider.end() - 1); // Remove last character
						cout << c_user << ' ' << c_user;	  // Visual backspace effect
					}
					break;
				default:				 // Regular character
					userHider += c_user; // Add character to username
					cout << c_user;		 // Display character
					break;
				}
			} while (c_user != 13); // Continue until Enter is pressed

			// Validate username input
			if (userHider.length() == 0)
			{
				system("CLS");
				system("COLOR 4F"); // Set error color scheme
				cout << "\n   Please enter Username!" << endl;
				cout << endl;
				cout << "   ";
				system("PAUSE");
				goto AdminLogin; // Return to admin login
			}

			else
			{
				user = userHider; // Store the entered username
			}

			// Password input section
			FontTextColor.fontTextColor(9); // Set text color to blue
			cout << "   Password: ";
			FontTextColor.fontTextColor(11); // Set text color to cyan

			int i = 0; // Counter variable (unused)

			string passHider; // String to store password
			char c_pass;	  // Character input variable

			// Process password input character by character (with masking)
			do
			{
				c_pass = _getch(); // Get character without echo
				switch (c_pass)
				{
				case 0:		  // Special key pressed
					_getch(); // Consume the next character
					break;
				case 13: // Enter key pressed
					cout << endl;
					break;
				case 27: // Escape key pressed
					system("CLS");
					goto LoginForm; // Go back to main login form
				case 8:				// Backspace key pressed
					if (passHider.length() > 0)
					{
						passHider.erase(passHider.end() - 1); // Remove last character
						cout << c_pass << ' ' << c_pass;	  // Visual backspace effect
					}
					break;
				default:				 // Regular character
					passHider += c_pass; // Add character to password
					cout << '*';		 // Display asterisk for security
					break;
				}
			} while (c_pass != 13); // Continue until Enter is pressed

			// Validate password input
			if (passHider.length() == 0)
			{
				system("CLS");
				system("COLOR 4F"); // Set error color scheme
				cout << "\n   Please enter Password!" << endl;
				cout << endl;
				cout << "   ";
				system("PAUSE");
				goto AdminLogin; // Return to admin login
			}

			else
			{
				pass = passHider; // Store the entered password
			}

			// Display verification message with animation
			FontTextColor.fontTextColor(9); // Set text color to blue
			cout << "\n   Verifying your login details please wait";
			for (int i = 0; i < 6; i++) // Animate dots for verification
			{
				cout << "."; // Print dot
				Sleep(500);	 // Wait 500ms between dots
			}

			// Read admin credentials from file and verify
			ifstream input("Text Files/Admin.txt"); // Open admin credentials file

			// Check each line in the admin file
			while (input >> u >> p)
			{
				// Check if username and password match
				if (u == user && p == pass)
				{
					adminlogin = 1; // Set login success flag
					cout << "\n   Admin login successful!";
					Sleep(2000);   // Wait 2 seconds
					system("CLS"); // Clear screen
				}

				// Handle incorrect username
				else if (u != user)
				{
					cout << "\n   Invalid Username please try again";
					Sleep(2000);	 // Wait 2 seconds
					goto AdminLogin; // Return to admin login
				}

				// Handle incorrect password
				else if (p != pass)
				{
					cout << "\n   Invalid Password please try again";
					Sleep(2000);	 // Wait 2 seconds
					goto AdminLogin; // Return to admin login
				}

				// Handle general login failure
				else
				{
					adminlogin = 0; // Set login failure flag
					system("CLS");
					system("COLOR 4F"); // Set error color scheme
					cout << "\n   Wrong Username or Password!"
						 << "\n   Try to login again\n"
						 << endl;
					Sleep(2000);	 // Wait 2 seconds
					goto AdminLogin; // Return to admin login
				}
			}
			input.close(); // Close the file

			// Process successful admin login
			if (adminlogin == 1)
			{
				// Record login activity in log file
				auto t = time(nullptr);	  // Get current time
				auto tm = *localtime(&t); // Convert to local time

				ostringstream oss;							// String stream for formatting
				oss << put_time(&tm, "%b/%d/%Y\t%H:%M:%S"); // Format date and time
				auto str = oss.str();						// Convert to string

				// Write login log entry
				ofstream Login_Logs("Text Files/Login_Logs.txt", ios::app);
				Login_Logs << endl
						   << user << "\t" << "Admin" << "\t" << str;
				Login_Logs.close();

				// Proceed to Admin Menu
			AdminMenu:

				// Set system title for admin menu
				system("TITLE Car Dealership Management Program [Admin Menu]");

				// Display admin menu with user-specific title
				UserTitleText.userTitleText("                           Admin Menu                         ", user, "                             [", "]                           ");

				cout << endl;
				// Display admin menu options with different colors
				Choice.choiceBlue("1", "Employee Data"); // Employee management in blue
				Choice.choiceBlue("2", "Cars Data");	 // Car data management in blue
				Choice.choiceBlue("3", "Order Logs");	 // View order logs in blue
				Choice.choiceBlue("4", "Login Logs");	 // View login logs in blue
				Choice.choiceBlue("5", "Order Cars\n");	 // Order cars in blue

				Choice.choiceGreen("6", "About Us"); // About us in green

				Choice.choiceRed("7", "Logout\n"); // Logout in red

				// Prompt for admin menu choice
				FontTextColor.fontTextColor(9);			   // Set text color to blue
				cout << "    Please insert your choice: "; // Changed from 3 spaces to 4 spaces to match menu options

				FontTextColor.fontTextColor(11); // Set text color to cyan
				cin >> adminchoice;				 // Get admin choice
				cout << endl;
				cin.ignore(); // Clear input buffer

				// Employee Data Management
				if (adminchoice == 1)
				{
				EmployeeData:

					// Set system title for employee data management
					system("TITLE Car Dealership Management Program [Employee Data]");

					TitleText.titleText("                          Employee Data                         ");

					cout << endl;
					Choice.choiceBlue("1", "Add Employee Data");
					Choice.choiceBlue("2", "View Employee Data");
					Choice.choiceBlue("3", "Search Employee Data");
					Choice.choiceBlue("4", "Edit Employee Data");
					Choice.choiceRed("5", "Delete Employee Data\n");

					Choice.choiceRed("6", "Back\n");

					FontTextColor.fontTextColor(9);
					cout << "    Please insert your choice: "; // Changed from 3 spaces to 4 spaces to match menu options

					FontTextColor.fontTextColor(11);
					cin >> employeeData;
					cout << endl;
					cin.ignore();

					// Add Employee Data
					if (employeeData == 1)
					{

						// Set system title for adding employee data
						system("TITLE Car Dealership Management Program [Add Employee Data]");

						TitleText.titleText("                         Add Employee Data                      ");

						Choice.choiceBack("ESC", "Back");

						string adduser, addpass, ru, rp;
						FontTextColor.fontTextColor(9);
						cout << "\n   Enter the Username: ";
						FontTextColor.fontTextColor(11);

						string userHider;
						char c_user;

						do
						{
							c_user = _getch();
							switch (c_user)
							{
							case 0:
								_getch();
								break;
							case 13:
								cout << endl;
								break;
							case 27:
								system("CLS");
								goto EmployeeData;
							case 8:
								if (userHider.length() > 0)
								{
									userHider.erase(userHider.end() - 1);
									cout << c_user << ' ' << c_user;
								}
								break;
							default:
								userHider += c_user;
								cout << c_user;
								break;
							}
						} while (c_user != 13);
						adduser = userHider;

						//  cin >> adduser;
						FontTextColor.fontTextColor(9);
						cout << "   Enter the Password: ";
						FontTextColor.fontTextColor(11);

						string passHider;
						char c_pass;

						do
						{
							c_pass = _getch();
							switch (c_pass)
							{
							case 0:
								_getch();
								break;
							case 13:
								cout << endl;
								break;
							case 27:
								system("CLS");
								goto EmployeeData;
							case 8:
								if (passHider.length() > 0)
								{
									passHider.erase(passHider.end() - 1);
									cout << c_pass << ' ' << c_pass;
								}
								break;
							default:
								passHider += c_pass;
								cout << '*';
								break;
							}
						} while (c_pass != 13);
						addpass = passHider;

						//  cin >> addpass;

						ofstream reg("Text Files/Employee.txt", ios::app);
						reg << adduser << '\t' << addpass << endl;
						FontTextColor.fontTextColor(9);
						cout << "\n   Employee Data Added!" << endl;
						cout << "\n   ";
						system("PAUSE");
						goto EmployeeData;
					}

					// View Employee Data
					else if (employeeData == 2)
					{
						system("CLS");

						// Set system title for viewing employee data
						system("TITLE Car Dealership Management Program [View Employee Data]");

						TitleText.titleText("                        View Employee Data                      ");

						ViewEmployeeData.viewEmployeeData();

						FontTextColor.fontTextColor(9);
						cout << endl;
						cout << "\n   All Employees Data Displayed!";
						cout << "\n   ";
						system("PAUSE");
						goto EmployeeData;
					}

					// Search Employee Data
					else if (employeeData == 3)
					{
					SearchEmployeeData:

						// Loading screen
						system("CLS");
						system("COLOR 0F");
						char a = 219;
						Coordinate.gotoxy(30, 10);
						cout << "[Loading]" << endl;
						Coordinate.gotoxy(25, 12);
						for (int r = 1; r <= 20; r++)
						{
							for (int q = 0; q <= 10000000; q++)
								;
							cout << a;
						}
						cout << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl;

						// system title
						system("TITLE Car Dealership Management Program [Search Employee Data]");

						TitleText.titleText("                      Search Employee Data                      ");

						int ch;

						cout << endl;
						Choice.choiceBlue("1", "Search your Password by Username");
						Choice.choiceBlue("2", "Search your Username by Password\n");
						Choice.choiceRed("3", "Back\n");

						FontTextColor.fontTextColor(9);
						cout << "    Enter your choice: ";

						FontTextColor.fontTextColor(11);
						cin >> ch;

						switch (ch)
						{

						// Search your Password by Username
						case 1:
						{
						SearchPasswordUsername:
							// Loading screen
							system("CLS");
							system("COLOR 0F");
							char a = 219;
							Coordinate.gotoxy(30, 10);
							cout << "[Loading]" << endl;
							Coordinate.gotoxy(25, 12);
							for (int r = 1; r <= 20; r++)
							{
								for (int q = 0; q <= 10000000; q++)
									;
								cout << a;
							}
							cout << endl
								 << endl
								 << endl
								 << endl
								 << endl
								 << endl
								 << endl;

							TitleText.titleText("                   Search Password by Username                  ");

							cout << endl;
							Choice.choiceGreen("ESC", "Back");

							int login = 0;
							string searchuser, su, sp;
							cout << endl;
							FontTextColor.fontTextColor(9);
							cout << "    Enter your Username: ";
							FontTextColor.fontTextColor(11);

							string searchuserHider;
							char c_searchuser;

							do
							{
								c_searchuser = _getch();
								switch (c_searchuser)
								{
								case 0:
									_getch();
									break;
								case 13:
									cout << endl;
									break;
								case 27:
									system("CLS");
									cin.clear();
									cin.ignore(22, '\n');
									goto SearchEmployeeData;
								case 8:
									if (searchuserHider.length() > 0)
									{
										searchuserHider.erase(searchuserHider.end() - 1);
										cout << c_searchuser << ' ' << c_searchuser;
									}
									break;
								default:
									searchuserHider += c_searchuser;
									cout << c_searchuser;
									break;
								}
							} while (c_searchuser != 13);

							if (searchuserHider.length() == 0)
							{
								system("CLS");
								system("COLOR 4F");
								cout << "\n   Please enter Username!" << endl;
								cout << endl;
								cout << "   ";
								system("PAUSE");
								goto SearchPasswordUsername;
							}

							else
							{
								searchuser = searchuserHider;
							}

							ifstream searchu("Text Files/Employee.txt");

							while (searchu >> su >> sp)
							{
								if (su == searchuser)
								{
									// if matched its found
									login = 1;
								}
							}
							searchu.close();

							if (login == 1)
							{
								cout << endl;
								FontTextColor.fontTextColor(9);
								cout << "    Noice, your account has been found";
								cout << endl;
								cout << "    Your password is [";
								FontTextColor.fontTextColor(4);
								cout << sp;
								FontTextColor.fontTextColor(9);
								cout << "]";
								cout << endl;
								cout << endl;
								cout << "    ";

								system("PAUSE");
								goto SearchEmployeeData;
							}

							else
							{
								cout << endl;
								FontTextColor.fontTextColor(9);
								cout << "    Sorry, Your username is not found in our database";
								cout << endl;
								cout << "    Please kindly contact your system administrator for more details";
								cout << endl;
								cout << endl;
								cout << "    ";
								system("PAUSE");
								goto SearchEmployeeData;
							}

							break;
						}

						case 2:
						{
						SearchUsernamePassword:
							// Loading screen
							system("CLS");
							system("COLOR 0F");
							char a = 219;
							Coordinate.gotoxy(30, 10);
							cout << "[Loading]" << endl;
							Coordinate.gotoxy(25, 12);
							for (int r = 1; r <= 20; r++)
							{
								for (int q = 0; q <= 10000000; q++)
									;
								cout << a;
							}
							cout << endl
								 << endl
								 << endl
								 << endl
								 << endl
								 << endl
								 << endl;

							TitleText.titleText("                   Search Username by Password                  ");

							cout << endl;
							Choice.choiceGreen("ESC", "Back");

							int login = 0;
							string searchpass, su2, sp2;
							cout << endl;
							FontTextColor.fontTextColor(9);
							cout << "    Enter in your Password: ";
							FontTextColor.fontTextColor(11);

							string searchpassHider;
							char c_searchpass;

							do
							{
								c_searchpass = _getch();
								switch (c_searchpass)
								{
								case 0:
									_getch();
									break;
								case 13:
									cout << endl;
									break;
								case 27:
									system("CLS");
									goto SearchEmployeeData;
								case 8:
									if (searchpassHider.length() > 0)
									{
										searchpassHider.erase(searchpassHider.end() - 1);
										cout << c_searchpass << ' ' << c_searchpass;
									}
									break;
								default:
									searchpassHider += c_searchpass;
									cout << c_searchpass;
									break;
								}
							} while (c_searchpass != 13);

							if (searchpassHider.length() == 0)
							{
								system("CLS");
								system("COLOR 4F");
								cout << "\n   Please enter Password!" << endl;
								cout << endl;
								cout << "   ";
								system("PAUSE");
								goto SearchUsernamePassword;
							}

							else
							{
								searchpass = searchpassHider;
							}

							ifstream searchp("Text Files/Employee.txt");

							while (searchp >> su2 >> sp2)
							{
								if (sp2 == searchpass)
								{
									// if matched its found
									login = 1;
								}
							}

							searchp.close();

							if (login == 1)
							{
								cout << endl;
								FontTextColor.fontTextColor(9);
								cout << "    Noice, your account has been found";
								cout << endl;
								FontTextColor.fontTextColor(9);
								cout << "    Your username is [";
								FontTextColor.fontTextColor(4);
								cout << su2;
								FontTextColor.fontTextColor(9);
								cout << "]";
								cout << endl;
								cout << endl;
								cout << "    ";
								system("PAUSE");
								goto SearchEmployeeData;
							}

							else
							{
								FontTextColor.fontTextColor(9);
								cout << endl;
								cout << "    Sorry, We cannot found your password in our database";
								cout << endl;
								cout << "    Please kindly contact your system administrator for more details";
								cout << endl;
								cout << endl;
								cout << "    ";
								system("PAUSE");
								goto SearchEmployeeData;
							}

							break;
						}

						case 3:
						{
							goto EmployeeData;
						}

						default:
							cout << endl;
							FontTextColor.fontTextColor(9);
							cout << "    Sorry, You entered wrong choice. Kindly try again";
							cout << endl;
							cout << endl;
							cout << "    ";
							system("PAUSE");
							cin.clear();
							cin.ignore(22, '\n');
							goto SearchEmployeeData;
						}
					}

					// Edit Employee Data
					else if (employeeData == 4)
					{
					EditEmployeeData:

						// Loading screen
						system("CLS");
						system("COLOR 0F");
						char a = 219;
						Coordinate.gotoxy(30, 10);
						cout << "[Loading]" << endl;
						Coordinate.gotoxy(25, 12);
						for (int r = 1; r <= 20; r++)
						{
							for (int q = 0; q <= 10000000; q++)
								;
							cout << a;
						}
						cout << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl;

						// system title
						system("TITLE Car Dealership Management Program [Edit Employee Data]");

						TitleText.titleText("                        Edit Employee Data                      ");

						ViewEmployeeData.viewEmployeeData();

						cout << endl;
						Choice.choiceBack("0", "Back");

						cout << endl;
						FontTextColor.fontTextColor(9);
						cout << "   Type the number of line to edit";

						// deletion per line

						// variables for storing the filename of the file and the line number to
						// delete in the file
						int line_number;

						// Prompt the user to enter the line number to delete in the file, store it
						// into line_number

						cout << endl;
						cout << "   Line: ";

						FontTextColor.fontTextColor(11);

						cin >> line_number;

						if (line_number == 0)
						{
							cin.clear();
							cin.ignore(22, '\n');
							system("CLS");

							goto EmployeeData;
						}

						// fstream object will be used to read all of the existing lines in the file
						fstream read_file;

						// Open the file with the provided filename
						read_file.open("Text Files/Employee.txt");

						// If file failed to open, exit with an error message and error exit status
						if (read_file.fail())
						{
							system("COLOR 4F");
							cout << endl;
							cout << "   Error opening file." << endl;

							Sleep(3000);
							goto EditEmployeeData;
							// returning 1 instead of 0 is a signal to the shell that something went
							// wrong in the execution of the program
						}

						// Create a vector to store all the file lines, and a string line to store
						// each line that we read
						vector<string> lines;
						string line;

						// Read each line of the file and store it as the next element of the vector,
						// the loop will stop when there are no more lines to read
						while (getline(read_file, line))
							lines.push_back(line);

						// Close our access to the file since we are done reading with it
						read_file.close();

						// The vector will now contain an element for each line in the file, so the
						// size of the vector is the number of lines in the file.  Check to make
						// sure the line number requested does not exceed the number of lines in
						// the file, if it does, exit with an error message and status.
						if (line_number > lines.size())
						{
							cout << endl;
							cout << "   Line " << line_number;
							cout << " not in file." << endl;

							// Inform user how many lines ARE in the file as part of the error message
							cout << "   File has " << lines.size();
							cout << " lines." << endl;

							Sleep(3000);
							goto EditEmployeeData;
						}

						// Create ofstream object for writing to the file
						ofstream write_file;

						// Open the file with the provided filename
						write_file.open("Text Files/Employee.txt");

						// If the file failed to open, exit with an error message and exit status
						if (write_file.fail())
						{
							system("COLOR 4F");

							cout << endl;
							cout << "   Error opening file." << endl;

							Sleep(3000);
							goto EditEmployeeData;
						}

						// Write all of the lines stored in the vector back to the file, EXCEPT the
						// line that we want to delete.

						// Line number 1 will be stored at vector index 0, line number 2 will be
						// stored at vector index 1, and so on because vectors are zero-indexed, so
						// decrement line_number to help us identify when we've reached the
						// associated line in the file.
						line_number--;

						// Loop through the vector elements to write each line back to the file
						// EXCEPT the line we want to delete.
						for (int i = 0; i < lines.size(); i++)
							if (i != line_number)
								write_file << lines[i] << endl;

						// Close our access to the file since we are done working with it
						write_file.close();

						string adduser, addpass, ru, rp;
						FontTextColor.fontTextColor(9);
						cout << "\n   Enter the Username: ";
						FontTextColor.fontTextColor(11);
						cin >> adduser;
						FontTextColor.fontTextColor(9);
						cout << "   Enter the Password: ";
						FontTextColor.fontTextColor(11);
						cin >> addpass;

						ofstream reg("Text Files/Employee.txt", ios::app);
						reg << adduser << '\t' << addpass << endl;
						cout << endl;
						FontTextColor.fontTextColor(1);
						cout << "   Line " << line_number + 1 << " has been edited!";
						cout << endl;
						cout << "   ";
						system("PAUSE");

						goto EmployeeData;
					}

					// Delete Employee Data
					else if (employeeData == 5)
					{
					DeleteEmployee:

						// Loading screen
						system("CLS");
						system("COLOR 0F");
						char a = 219;
						Coordinate.gotoxy(30, 10);
						cout << "[Loading]" << endl;
						Coordinate.gotoxy(25, 12);
						for (int r = 1; r <= 20; r++)
						{
							for (int q = 0; q <= 10000000; q++)
								;
							cout << a;
						}
						cout << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl;

						// system title
						system("TITLE Car Dealership Management Program [Delete Employee Data]");

						TitleText.titleText("                       Delete Employee Data                     ");

						ViewEmployeeData.viewEmployeeData();

						cout << endl;
						Choice.choiceBack("0", "Back");

						cout << endl;
						FontTextColor.fontTextColor(9);
						cout << "   Type the number of line to delete";
						cout << endl;

						// deletion per line

						// variables for storing the filename of the file and the line number to
						// delete in the file
						int line_number;

						// Prompt the user to enter the line number to delete in the file, store it
						// into line_number

						cout << "   Line: ";
						FontTextColor.fontTextColor(11);

						cin >> line_number;

						if (line_number == 0)
						{
							cin.clear();
							cin.ignore(22, '\n');
							system("CLS");

							goto EmployeeData;
						}

						// fstream object will be used to read all of the existing lines in the file
						fstream read_file;

						// Open the file with the provided filename
						read_file.open("Text Files/Employee.txt");

						// If file failed to open, exit with an error message and error exit status
						if (read_file.fail())
						{
							system("COLOR 4F");
							cout << endl;
							cout << "   Error opening file." << endl;

							Sleep(3000);
							goto DeleteEmployee;
							// returning 1 instead of 0 is a signal to the shell that something went
							// wrong in the execution of the program
						}

						// Create a vector to store all the file lines, and a string line to store
						// each line that we read
						vector<string> lines;
						string line;

						// Read each line of the file and store it as the next element of the vector,
						// the loop will stop when there are no more lines to read
						while (getline(read_file, line))
							lines.push_back(line);

						// Close our access to the file since we are done reading with it
						read_file.close();

						// The vector will now contain an element for each line in the file, so the
						// size of the vector is the number of lines in the file.  Check to make
						// sure the line number requested does not exceed the number of lines in
						// the file, if it does, exit with an error message and status.
						if (line_number > lines.size())
						{
							cout << endl;
							cout << "   Line " << line_number;
							cout << " not in file." << endl;

							// Inform user how many lines ARE in the file as part of the error message
							cout << "   File has " << lines.size();
							cout << " lines." << endl;

							Sleep(3000);
							goto DeleteEmployee;
						}

						// Create ofstream object for writing to the file
						ofstream write_file;

						// Open the file with the provided filename
						write_file.open("Text Files/Employee.txt");

						// If the file failed to open, exit with an error message and exit status
						if (write_file.fail())
						{
							system("COLOR 4F");

							cout << endl;
							cout << "   Error opening file." << endl;

							Sleep(3000);
							goto DeleteEmployee;
						}

						// Write all of the lines stored in the vector back to the file, EXCEPT the
						// line that we want to delete.

						// Line number 1 will be stored at vector index 0, line number 2 will be
						// stored at vector index 1, and so on because vectors are zero-indexed, so
						// decrement line_number to help us identify when we've reached the
						// associated line in the file.
						line_number--;

						// Loop through the vector elements to write each line back to the file
						// EXCEPT the line we want to delete.
						for (int i = 0; i < lines.size(); i++)
							if (i != line_number)
								write_file << lines[i] << endl;

						// Close our access to the file since we are done working with it
						write_file.close();

						FontTextColor.fontTextColor(9);
						cout << endl;
						cout << "   Line " << line_number + 1 << " has been deleted";

						Sleep(3000);
						goto EmployeeData;
					}

					// Back
					else if (employeeData == 6)
					{
						system("CLS");
						goto AdminMenu;
					}

					else
					{
						// system title
						system("TITLE Car Dealership Management Program [Error Employee Data]");

						system("CLS");
						system("COLOR 4F");
						cout << "\n   Wrong input entered!" << endl;
						cin.clear();
						cin.ignore(22, '\n');
						Sleep(2000);

						goto EmployeeData;
					}
				}

				// Cars Data Management
				else if (adminchoice == 2)
				{
				CarData:

					// Set system title for cars data management
					system("TITLE Car Dealership Management Program [Cars Data]");

					TitleText.titleText("                            Cars Data                           ");

					cout << endl;
					// Display menu options with car ASCII art on the right
					cout << "    [1] View Cars Name & Price                    _.-.___|__" << endl;
					cout << "    [2] Add Cars Name & Price                    |  _      _`-." << endl;
					cout << "    [3] Edit Cars Name & Price                   '-(_)----(_)--`" << endl;
					cout << "    [4] Delete Cars Name & Price" << endl;
					cout << endl;
					cout << "    [5] View Cars Color                               ," << endl;
					cout << "    [6] Add Cars Color                            .-/c-.,:::" << endl;
					cout << "    [7] Edit Cars Color                           (_)'==(_)" << endl;
					cout << "    [8] Delete Cars Color" << endl;
					cout << endl;
					cout << "    [9] Back" << endl;
					cout << endl;

					FontTextColor.fontTextColor(9);
					cout << "    Please enter your input: "; // Changed from 3 spaces to 4 spaces to match menu options

					FontTextColor.fontTextColor(11);
					cin >> CarChoice;

					// View Cars Name & Price
					if (CarChoice == 1)
					{

						// Loading screen
						system("CLS");
						system("COLOR 0F");
						char a = 219;
						Coordinate.gotoxy(30, 10);
						cout << "[Loading]" << endl;
						Coordinate.gotoxy(25, 12);
						for (int r = 1; r <= 20; r++)
						{
							for (int q = 0; q <= 10000000; q++)
								;
							cout << a;
						}
						cout << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl;

						// system title
						system("TITLE Car Dealership Management Program [View Cars Name & Price]");

						TitleText.titleText("                      View Cars Name & Price                    ");

						ViewCarData.viewCarNamePrice();

						FontTextColor.fontTextColor(9);
						cout << endl;
						cout << "   All Cars Name & Price Data Displayed!";
						cout << endl;
						cout << "   ";
						system("PAUSE");
						goto CarData;
					}

					// Add Cars Name & Price
					else if (CarChoice == 2)
					{
					AddCarsNamePrice:
						// Loading screen
						system("CLS");
						system("COLOR 0F");
						char a = 219;
						Coordinate.gotoxy(30, 10);
						cout << "[Loading]" << endl;
						Coordinate.gotoxy(25, 12);
						for (int r = 1; r <= 20; r++)
						{
							for (int q = 0; q <= 10000000; q++)
								;
							cout << a;
						}
						cout << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl;

						// system title
						system("TITLE Car Dealership Management Program [Add Cars Name & Price]");

						TitleText.titleText("                      Add Cars Name & Price                     ");

						Choice.choiceBack("ESC", "Back");

						string addname, addprice;
						FontTextColor.fontTextColor(9);
						cout << "\n   Enter the Car Name: ";
						FontTextColor.fontTextColor(11);

						string addnameHider;
						char c_addname;

						do
						{
							c_addname = _getch();
							switch (c_addname)
							{
							case 0:
								_getch();
								break;
							case 13:
								cout << endl;
								break;
							case 27:
								system("CLS");
								cin.clear();
								cin.ignore(22, '\n');
								goto CarData;
							case 8:
								if (addnameHider.length() > 0)
								{
									addnameHider.erase(addnameHider.end() - 1);
									cout << c_addname << ' ' << c_addname;
								}
								break;
							default:
								addnameHider += c_addname;
								cout << c_addname;
								break;
							}
						} while (c_addname != 13);

						if (addnameHider.length() == 0)
						{
							system("CLS");
							system("COLOR 4F");
							cout << "\n   Please enter the name of the car!" << endl;
							cout << endl;
							cout << "   ";
							system("PAUSE");
							goto AddCarsNamePrice;
						}

						else
						{
							addname = addnameHider;
						}

						FontTextColor.fontTextColor(9);
						cout << "   Enter the Car Price: ";
						FontTextColor.fontTextColor(11);

						string addpriceHider;
						char c_addprice;

						do
						{
							c_addprice = _getch();
							switch (c_addprice)
							{
							case 0:
								_getch();
								break;
							case 13:
								cout << endl;
								break;
							case 27:
								system("CLS");
								cin.clear();
								cin.ignore(22, '\n');
								goto EmployeeData;
							case 8:
								if (addpriceHider.length() > 0)
								{
									addpriceHider.erase(addpriceHider.end() - 1);
									cout << c_addprice << ' ' << c_addprice;
								}
								break;
							default:
								addpriceHider += c_addprice;
								cout << c_addprice;
								break;
							}
						} while (c_addprice != 13);

						if (addpriceHider.length() == 0)
						{
							system("CLS");
							system("COLOR 4F");
							cout << "\n   Please enter the price of the car!" << endl;
							cout << endl;
							cout << "   ";
							system("PAUSE");
							goto AddCarsNamePrice;
						}

						else
						{
							addprice = addpriceHider;
						}

						FontTextColor.fontTextColor(1);
						ofstream Cars_Name("Text Files/Cars_Name.txt", ios::app);
						Cars_Name << endl
								  << addname;
						Cars_Name.close();

						ofstream Cars_Price("Text Files/Cars_Price.txt", ios::app);
						Cars_Price << endl
								   << addprice;
						Cars_Price.close();

						cout << "\n   Car Name & Price Added!" << endl;

						cout << endl;
						cout << "   ";
						system("PAUSE");
						goto CarData;
					}

					// Edit Cars Name & Price
					else if (CarChoice == 3)
					{
						// Loading screen
						system("CLS");
						system("COLOR 0F");
						char a = 219;
						Coordinate.gotoxy(30, 10);
						cout << "[Loading]" << endl;
						Coordinate.gotoxy(25, 12);
						for (int r = 1; r <= 20; r++)
						{
							for (int q = 0; q <= 10000000; q++)
								;
							cout << a;
						}
						cout << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl;

						// system title
						system("TITLE Car Dealership Management Program [Edit Cars Name & Price]");

						TitleText.titleText("                      Edit Car Name & Price                     ");

						ViewCarData.viewCarNamePrice();

						Choice.choiceBack("0", "Back");

						cout << endl;
						FontTextColor.fontTextColor(9);
						cout << "   Type the number of line to edit";

						// deletion per line

						// variables for storing the filename of the file and the line number to
						// delete in the file
						int line_number;

						// Prompt the user to enter the line number to delete in the file, store it
						// into line_number

						cout << endl;
						cout << "   Line: ";
						FontTextColor.fontTextColor(11);

						cin >> line_number;

						if (line_number == 0)
						{
							cin.clear();
							cin.ignore(22, '\n');
							system("CLS");

							goto CarData;
						}

						// fstream object will be used to read all of the existing lines in the file
						fstream read_file;

						// Open the file with the provided filename
						read_file.open("Text Files/Cars_Name.txt");

						// If file failed to open, exit with an error message and error exit status
						if (read_file.fail())
						{
							system("COLOR 4F");
							cout << endl;
							cout << "   Error opening file." << endl;

							Sleep(3000);
							goto DeleteCarsNamePrice;
							// returning 1 instead of 0 is a signal to the shell that something went
							// wrong in the execution of the program
						}

						// Create a vector to store all the file lines, and a string line to store
						// each line that we read
						vector<string> lines;
						string line;

						// Read each line of the file and store it as the next element of the vector,
						// the loop will stop when there are no more lines to read
						while (getline(read_file, line))
							lines.push_back(line);

						// Close our access to the file since we are done reading with it
						read_file.close();

						// The vector will now contain an element for each line in the file, so the
						// size of the vector is the number of lines in the file.  Check to make
						// sure the line number requested does not exceed the number of lines in
						// the file, if it does, exit with an error message and status.
						if (line_number > lines.size())
						{
							cout << endl;
							cout << "   Line " << line_number;
							cout << " not in file." << endl;

							// Inform user how many lines ARE in the file as part of the error message
							cout << "   File has " << lines.size();
							cout << " lines." << endl;

							Sleep(3000);
							goto DeleteCarsNamePrice;
						}

						// Create ofstream object for writing to the file
						ofstream write_file;

						// Open the file with the provided filename
						write_file.open("Text Files/Cars_Name.txt");

						// If the file failed to open, exit with an error message and exit status
						if (write_file.fail())
						{
							system("COLOR 4F");

							cout << endl;
							cout << "   Error opening file." << endl;

							Sleep(3000);
							goto DeleteCarsNamePrice;
						}

						// Write all of the lines stored in the vector back to the file, EXCEPT the
						// line that we want to delete.

						// Line number 1 will be stored at vector index 0, line number 2 will be
						// stored at vector index 1, and so on because vectors are zero-indexed, so
						// decrement line_number to help us identify when we've reached the
						// associated line in the file.
						line_number--;

						// Loop through the vector elements to write each line back to the file
						// EXCEPT the line we want to delete.
						for (int i = 0; i < lines.size(); i++)
							if (i != line_number)
								write_file << lines[i] << endl;

						// Close our access to the file since we are done working with it
						write_file.close();

						// Deleting car prices
						line_number++;

						// cin >> line_number;

						// fstream object will be used to read all of the existing lines in the file
						fstream read_file_Cars_Price;

						// Open the file with the provided filename
						read_file_Cars_Price.open("Text Files/Cars_Price.txt");

						// If file failed to open, exit with an error message and error exit status
						if (read_file_Cars_Price.fail())
						{
							system("COLOR 4F");
							cout << endl;
							cout << "   Error opening file." << endl;

							Sleep(3000);
							goto DeleteCarsNamePrice;
							// returning 1 instead of 0 is a signal to the shell that something went
							// wrong in the execution of the program
						}

						// Create a vector to store all the file lines, and a string line to store
						// each line that we read
						vector<string> lines_Cars_Price;
						string line_Cars_Price;

						// Read each line of the file and store it as the next element of the vector,
						// the loop will stop when there are no more lines to read
						while (getline(read_file_Cars_Price, line_Cars_Price))
							lines_Cars_Price.push_back(line_Cars_Price);

						// Close our access to the file since we are done reading with it
						read_file_Cars_Price.close();

						// The vector will now contain an element for each line in the file, so the
						// size of the vector is the number of lines in the file.  Check to make
						// sure the line number requested does not exceed the number of lines in
						// the file, if it does, exit with an error message and status.
						if (line_number > lines_Cars_Price.size())
						{
							cout << endl;
							cout << "   Line " << line_number;
							cout << " not in file." << endl;

							// Inform user how many lines ARE in the file as part of the error message
							cout << "   File has " << lines_Cars_Price.size();
							cout << " lines." << endl;

							Sleep(3000);
							goto DeleteCarsNamePrice;
						}

						// Create ofstream object for writing to the file
						ofstream write_file_Cars_Price;

						// Open the file with the provided filename
						write_file_Cars_Price.open("Text Files/Cars_Price.txt");

						// If the file failed to open, exit with an error message and exit status
						if (write_file_Cars_Price.fail())
						{
							system("COLOR 4F");

							cout << endl;
							cout << "   Error opening file." << endl;

							Sleep(3000);
							goto DeleteCarsNamePrice;
						}

						// Write all of the lines stored in the vector back to the file, EXCEPT the
						// line that we want to delete.

						// Line number 1 will be stored at vector index 0, line number 2 will be
						// stored at vector index 1, and so on because vectors are zero-indexed, so
						// decrement line_number to help us identify when we've reached the
						// associated line in the file.
						line_number--;

						// Loop through the vector elements to write each line back to the file
						// EXCEPT the line we want to delete.
						for (int i = 0; i < lines_Cars_Price.size(); i++)
							if (i != line_number)
								write_file_Cars_Price << lines_Cars_Price[i] << endl;

						// Close our access to the file since we are done working with it
						write_file_Cars_Price.close();

						string addname, addprice;
						FontTextColor.fontTextColor(9);
						cout << "\n   Enter the Car Name: ";
						FontTextColor.fontTextColor(11);

						string userHider_addname;
						char c_user_addname;

						do
						{
							c_user_addname = _getch();
							switch (c_user_addname)
							{
							case 0:
								_getch();
								break;
							case 13:
								cout << endl;
								break;
							case 27:
								system("CLS");
								cin.clear();
								cin.ignore(22, '\n');
								goto EmployeeData;
							case 8:
								if (userHider_addname.length() > 0)
								{
									userHider_addname.erase(userHider_addname.end() - 1);
									cout << c_user_addname << ' ' << c_user_addname;
								}
								break;
							default:
								userHider_addname += c_user_addname;
								cout << c_user_addname;
								break;
							}
						} while (c_user_addname != 13);
						addname = userHider_addname;

						// cin >> addname;
						FontTextColor.fontTextColor(9);
						cout << "   Enter the Car Price: ";
						FontTextColor.fontTextColor(11);

						string userHider_addprice;
						char c_user_addprice;

						do
						{
							c_user_addprice = _getch();
							switch (c_user_addprice)
							{
							case 0:
								_getch();
								break;
							case 13:
								cout << endl;
								break;
							case 27:
								system("CLS");
								cin.clear();
								cin.ignore(22, '\n');
								goto CarData;
							case 8:
								if (userHider_addprice.length() > 0)
								{
									userHider_addprice.erase(userHider_addprice.end() - 1);
									cout << c_user_addprice << ' ' << c_user_addprice;
								}
								break;
							default:
								userHider_addprice += c_user_addprice;
								cout << c_user_addprice;
								break;
							}
						} while (c_user_addprice != 13);
						addprice = userHider_addprice;

						// cin >> addprice;
						FontTextColor.fontTextColor(1);

						ofstream Cars_Name("Text Files/Cars_Name.txt", ios::app);
						Cars_Name << addname << endl;
						Cars_Name.close();

						FontTextColor.fontTextColor(10);
						cout << endl;
						cout << "   ";
						system("PAUSE");
						goto CarData;
					}

					// Delete Cars Name & Price
					else if (CarChoice == 4)
					{
					DeleteCarsNamePrice:
						// Loading screen
						system("CLS");
						system("COLOR 0F");
						char a = 219;
						Coordinate.gotoxy(30, 10);
						cout << "[Loading]" << endl;
						Coordinate.gotoxy(25, 12);
						for (int r = 1; r <= 20; r++)
						{
							for (int q = 0; q <= 10000000; q++)
								;
							cout << a;
						}
						cout << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl;

						// system title
						system("TITLE Car Dealership Management Program [Delete Cars Name & Price]");

						TitleText.titleText("                     Delete Car Name & Price                    ");

						ViewCarData.viewCarNamePrice();

						Choice.choiceBack("0", "Back");

						cout << endl;
						FontTextColor.fontTextColor(9);
						cout << "   Type the number of line to delete";

						// deletion per line

						// variables for storing the filename of the file and the line number to
						// delete in the file
						int line_number;

						// Prompt the user to enter the line number to delete in the file, store it
						// into line_number

						cout << endl;
						cout << "   Line: ";
						FontTextColor.fontTextColor(11);

						cin >> line_number;

						if (line_number == 0)
						{
							cin.clear();
							cin.ignore(22, '\n');
							system("CLS");

							goto CarData;
						}

						// fstream object will be used to read all of the existing lines in the file
						fstream read_file;

						// Open the file with the provided filename
						read_file.open("Text Files/Cars_Name.txt");

						// If file failed to open, exit with an error message and error exit status
						if (read_file.fail())
						{
							system("COLOR 4F");
							cout << endl;
							cout << "   Error opening file." << endl;

							Sleep(3000);
							goto DeleteCarsNamePrice;
							// returning 1 instead of 0 is a signal to the shell that something went
							// wrong in the execution of the program
						}

						// Create a vector to store all the file lines, and a string line to store
						// each line that we read
						vector<string> lines;
						string line;

						// Read each line of the file and store it as the next element of the vector,
						// the loop will stop when there are no more lines to read
						while (getline(read_file, line))
							lines.push_back(line);

						// Close our access to the file since we are done reading with it
						read_file.close();

						// The vector will now contain an element for each line in the file, so the
						// size of the vector is the number of lines in the file.  Check to make
						// sure the line number requested does not exceed the number of lines in
						// the file, if it does, exit with an error message and status.
						if (line_number > lines.size())
						{
							cout << endl;
							cout << "   Line " << line_number;
							cout << " not in file." << endl;

							// Inform user how many lines ARE in the file as part of the error message
							cout << "   File has " << lines.size();
							cout << " lines." << endl;

							Sleep(3000);
							goto DeleteCarsNamePrice;
						}

						// Create ofstream object for writing to the file
						ofstream write_file;

						// Open the file with the provided filename
						write_file.open("Text Files/Cars_Name.txt");

						// If the file failed to open, exit with an error message and exit status
						if (write_file.fail())
						{
							system("COLOR 4F");

							cout << endl;
							cout << "   Error opening file." << endl;

							Sleep(3000);
							goto DeleteCarsNamePrice;
						}

						// Write all of the lines stored in the vector back to the file, EXCEPT the
						// line that we want to delete.

						// Line number 1 will be stored at vector index 0, line number 2 will be
						// stored at vector index 1, and so on because vectors are zero-indexed, so
						// decrement line_number to help us identify when we've reached the
						// associated line in the file.
						line_number--;

						// Loop through the vector elements to write each line back to the file
						// EXCEPT the line we want to delete.
						for (int i = 0; i < lines.size(); i++)
							if (i != line_number)
								write_file << lines[i] << endl;

						// Close our access to the file since we are done working with it
						write_file.close();

						// Deleting car prices
						line_number++;

						// cin >> line_number;

						// fstream object will be used to read all of the existing lines in the file
						fstream read_file_Cars_Price;

						// Open the file with the provided filename
						read_file_Cars_Price.open("Text Files/Cars_Price.txt");

						// If file failed to open, exit with an error message and error exit status
						if (read_file_Cars_Price.fail())
						{
							system("COLOR 4F");
							cout << endl;
							cout << "   Error opening file." << endl;

							Sleep(3000);
							goto DeleteCarsNamePrice;
							// returning 1 instead of 0 is a signal to the shell that something went
							// wrong in the execution of the program
						}

						// Create a vector to store all the file lines, and a string line to store
						// each line that we read
						vector<string> lines_Cars_Price;
						string line_Cars_Price;

						// Read each line of the file and store it as the next element of the vector,
						// the loop will stop when there are no more lines to read
						while (getline(read_file_Cars_Price, line_Cars_Price))
							lines_Cars_Price.push_back(line_Cars_Price);

						// Close our access to the file since we are done reading with it
						read_file_Cars_Price.close();

						// The vector will now contain an element for each line in the file, so the
						// size of the vector is the number of lines in the file.  Check to make
						// sure the line number requested does not exceed the number of lines in
						// the file, if it does, exit with an error message and status.
						if (line_number > lines_Cars_Price.size())
						{
							cout << endl;
							cout << "   Line " << line_number;
							cout << " not in file." << endl;

							// Inform user how many lines ARE in the file as part of the error message
							cout << "   File has " << lines_Cars_Price.size();
							cout << " lines." << endl;

							Sleep(3000);
							goto DeleteCarsNamePrice;
						}

						// Create ofstream object for writing to the file
						ofstream write_file_Cars_Price;

						// Open the file with the provided filename
						write_file_Cars_Price.open("Text Files/Cars_Price.txt");

						// If the file failed to open, exit with an error message and exit status
						if (write_file_Cars_Price.fail())
						{
							system("COLOR 4F");

							cout << endl;
							cout << "   Error opening file." << endl;

							Sleep(3000);
							goto DeleteCarsNamePrice;
						}

						// Write all of the lines stored in the vector back to the file, EXCEPT the
						// line that we want to delete.

						// Line number 1 will be stored at vector index 0, line number 2 will be
						// stored at vector index 1, and so on because vectors are zero-indexed, so
						// decrement line_number to help us identify when we've reached the
						// associated line in the file.
						line_number--;

						// Loop through the vector elements to write each line back to the file
						// EXCEPT the line we want to delete.
						for (int i = 0; i < lines_Cars_Price.size(); i++)
							if (i != line_number)
								write_file_Cars_Price << lines_Cars_Price[i] << endl;

						// Close our access to the file since we are done working with it
						write_file_Cars_Price.close();

						cout << endl;
						cout << "   Line " << line_number + 1 << " has been deleted";

						FontTextColor.fontTextColor(10);
						cout << endl;
						cout << "   ";
						system("PAUSE");
						goto CarData;
					}

					// View Cars Color
					else if (CarChoice == 5)
					{
						// Loading screen
						system("CLS");
						system("COLOR 0F");
						char a = 219;
						Coordinate.gotoxy(30, 10);
						cout << "[Loading]" << endl;
						Coordinate.gotoxy(25, 12);
						for (int r = 1; r <= 20; r++)
						{
							for (int q = 0; q <= 10000000; q++)
								;
							cout << a;
						}
						cout << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl;

						// system title
						system("TITLE Car Dealership Management Program [View Cars Color]");

						TitleText.titleText("                         View Cars Color                        ");

						ViewCarData.viewCarColor();

						FontTextColor.fontTextColor(9);
						cout << endl;
						cout << "   All Cars Color Data Displayed!";
						cout << endl;
						cout << "   ";
						system("PAUSE");
						goto CarData;
					}

					// Add Cars Color
					else if (CarChoice == 6)
					{
						// Loading screen
						system("CLS");
						system("COLOR 0F");
						char a = 219;
						Coordinate.gotoxy(30, 10);
						cout << "[Loading]" << endl;
						Coordinate.gotoxy(25, 12);
						for (int r = 1; r <= 20; r++)
						{
							for (int q = 0; q <= 10000000; q++)
								;
							cout << a;
						}
						cout << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl;

						// system title
						system("TITLE Car Dealership Management Program [Add Cars Color]");

						TitleText.titleText("                          Add Car Colors                        ");

						Choice.choiceBack("ESC", "Back");

						string addcolor;
						FontTextColor.fontTextColor(9);
						cout << "\n   Enter the Color: ";
						FontTextColor.fontTextColor(11);

						string addcolorHider;
						char c_Color;

						do
						{
							c_Color = _getch();
							switch (c_Color)
							{
							case 0:
								_getch();
								break;
							case 13:
								cout << endl;
								break;
							case 27:
								system("CLS");
								goto CarData;
							case 8:
								if (addcolorHider.length() > 0)
								{
									addcolorHider.erase(addcolorHider.end() - 1);
									cout << c_Color << ' ' << c_Color;
								}
								break;
							default:
								addcolorHider += c_Color;
								cout << c_Color;
								break;
							}
						} while (c_Color != 13);

						if (addcolorHider.length() == 0)
						{
							system("CLS");
							system("a 4f");
							cout << "\n   Please enter the name of the car!" << endl;
							cout << endl;
							cout << "   ";
							system("PAUSE");
							goto AddCarsNamePrice;
						}

						else
						{
							addcolor = addcolorHider;
						}

						FontTextColor.fontTextColor(1);
						ofstream Cars_Color("Text Files/Cars_Color.txt", ios::app);
						Cars_Color << addcolor << endl;
						cout << "\n   Car Color Added!" << endl;
						cout << "   ";
						goto CarData;
					}

					// Edit Cars Color
					else if (CarChoice == 7)
					{
						// Loading screen
						system("CLS");
						system("COLOR 0F");
						char a = 219;
						Coordinate.gotoxy(30, 10);
						cout << "[Loading]" << endl;
						Coordinate.gotoxy(25, 12);
						for (int r = 1; r <= 20; r++)
						{
							for (int q = 0; q <= 10000000; q++)
								;
							cout << a;
						}
						cout << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl;

						// system title
						system("TITLE Car Dealership Management Program [Edit Cars Color]");

						TitleText.titleText("                          Edit Car Color                        ");

						ViewCarData.viewCarColor();

						Choice.choiceBack("0", "Back");

						cout << endl;
						FontTextColor.fontTextColor(9);
						cout << "   Type the number of line to edit";

						// deletion per line

						// variables for storing the filename of the file and the line number to
						// delete in the file
						int line_number;
						string strColor;

						// Prompt the user to enter the line number to delete in the file, store it
						// into line_number

						cout << endl;
						cout << "   Line: ";
						FontTextColor.fontTextColor(11);

						cin >> line_number;

						if (line_number == 0)
						{
							cin.clear();
							cin.ignore(22, '\n');
							system("CLS");

							goto CarData;
						}

						// fstream object will be used to read all of the existing lines in the file
						fstream read_file;

						// Open the file with the provided filename
						read_file.open("Text Files/Cars_Color.txt");

						// If file failed to open, exit with an error message and error exit status
						if (read_file.fail())
						{
							system("COLOR 4F");
							cout << endl;
							cout << "   Error opening file." << endl;

							Sleep(3000);
							goto DeleteCarsColor;
							// returning 1 instead of 0 is a signal to the shell that something went
							// wrong in the execution of the program
						}

						// Create a vector to store all the file lines, and a string line to store
						// each line that we read
						vector<string> lines;
						string line;

						// Read each line of the file and store it as the next element of the vector,
						// the loop will stop when there are no more lines to read
						while (getline(read_file, line))
							lines.push_back(line);

						// Close our access to the file since we are done reading with it
						read_file.close();

						// The vector will now contain an element for each line in the file, so the
						// size of the vector is the number of lines in the file.  Check to make
						// sure the line number requested does not exceed the number of lines in
						// the file, if it does, exit with an error message and status.
						if (line_number > lines.size())
						{
							cout << endl;
							cout << "   Line " << line_number;
							cout << " not in file." << endl;

							// Inform user how many lines ARE in the file as part of the error message
							cout << "   File has " << lines.size();
							cout << " lines." << endl;

							Sleep(3000);
							goto DeleteCarsColor;
						}

						// Create ofstream object for writing to the file
						ofstream write_file;

						// Open the file with the provided filename
						write_file.open("Text Files/Cars_Color.txt");

						// If the file failed to open, exit with an error message and exit status
						if (write_file.fail())
						{
							system("COLOR 4F");

							cout << endl;
							cout << "   Error opening file." << endl;

							Sleep(3000);
							goto DeleteCarsColor;
						}

						// Write all of the lines stored in the vector back to the file, EXCEPT the
						// line that we want to delete.

						// Line number 1 will be stored at vector index 0, line number 2 will be
						// stored at vector index 1, and so on because vectors are zero-indexed, so
						// decrement line_number to help us identify when we've reached the
						// associated line in the file.
						line_number--;

						// Loop through the vector elements to write each line back to the file
						// EXCEPT the line we want to delete.
						for (int i = 0; i < lines.size(); i++)
							if (i != line_number)
								write_file << lines[i] << endl;

						// Close our access to the file since we are done working with it
						write_file.close();

						// Add COLOR to text file

						cout << endl;
						FontTextColor.fontTextColor(9);
						cout << "   Enter the Color: ";
						FontTextColor.fontTextColor(11);
						cin >> strColor;
						FontTextColor.fontTextColor(1);

						ofstream addcolor("Text Files/Cars_Color.txt", ios::app);
						addcolor << endl
								 << strColor;

						cout << endl;
						cout << "   Line " << line_number + 1 << " has been editted!";
						cout << endl;
						cout << "   ";

						system("PAUSE");
						goto CarData;
					}

					// Delete Cars Color
					else if (CarChoice == 8)
					{
					DeleteCarsColor:
						// Loading screen
						system("CLS");
						system("COLOR 0F");
						char a = 219;
						Coordinate.gotoxy(30, 10);
						cout << "[Loading]" << endl;
						Coordinate.gotoxy(25, 12);
						for (int r = 1; r <= 20; r++)
						{
							for (int q = 0; q <= 10000000; q++)
								;
							cout << a;
						}
						cout << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl
							 << endl;

						// system title
						system("TITLE Car Dealership Management Program [Delete Cars Color]");

						TitleText.titleText("                         Delete Car Color                       ");

						ViewCarData.viewCarColor();

						Choice.choiceBack("0", "Back");

						cout << endl;
						FontTextColor.fontTextColor(9);
						cout << "   Type the number of line to delete";

						// deletion per line

						// variables for storing the filename of the file and the line number to
						// delete in the file
						int line_number;

						// Prompt the user to enter the line number to delete in the file, store it
						// into line_number

						cout << endl;
						cout << "   Line: ";
						FontTextColor.fontTextColor(11);

						cin >> line_number;

						if (line_number == 0)
						{
							cin.clear();
							cin.ignore(22, '\n');
							system("CLS");

							goto CarData;
						}

						// fstream object will be used to read all of the existing lines in the file
						fstream read_file;

						// Open the file with the provided filename
						read_file.open("Text Files/Cars_Color.txt");

						// If file failed to open, exit with an error message and error exit status
						if (read_file.fail())
						{
							system("COLOR 4F");
							cout << endl;
							cout << "   Error opening file." << endl;

							Sleep(3000);
							goto DeleteCarsColor;
							// returning 1 instead of 0 is a signal to the shell that something went
							// wrong in the execution of the program
						}

						// Create a vector to store all the file lines, and a string line to store
						// each line that we read
						vector<string> lines;
						string line;

						// Read each line of the file and store it as the next element of the vector,
						// the loop will stop when there are no more lines to read
						while (getline(read_file, line))
							lines.push_back(line);

						// Close our access to the file since we are done reading with it
						read_file.close();

						// The vector will now contain an element for each line in the file, so the
						// size of the vector is the number of lines in the file.  Check to make
						// sure the line number requested does not exceed the number of lines in
						// the file, if it does, exit with an error message and status.
						if (line_number > lines.size())
						{
							cout << endl;
							cout << "   Line " << line_number;
							cout << " not in file." << endl;

							// Inform user how many lines ARE in the file as part of the error message
							cout << "   File has " << lines.size();
							cout << " lines." << endl;

							Sleep(3000);
							goto DeleteCarsColor;
						}

						// Create ofstream object for writing to the file
						ofstream write_file;

						// Open the file with the provided filename
						write_file.open("Text Files/Cars_Color.txt");

						// If the file failed to open, exit with an error message and exit status
						if (write_file.fail())
						{
							system("COLOR 4F");

							cout << endl;
							cout << "   Error opening file." << endl;

							Sleep(3000);
							goto DeleteCarsColor;
						}

						// Write all of the lines stored in the vector back to the file, EXCEPT the
						// line that we want to delete.

						// Line number 1 will be stored at vector index 0, line number 2 will be
						// stored at vector index 1, and so on because vectors are zero-indexed, so
						// decrement line_number to help us identify when we've reached the
						// associated line in the file.
						line_number--;

						// Loop through the vector elements to write each line back to the file
						// EXCEPT the line we want to delete.
						for (int i = 0; i < lines.size(); i++)
							if (i != line_number)
								write_file << lines[i] << endl;

						// Close our access to the file since we are done working with it
						write_file.close();

						cout << endl;
						cout << "   Line " << line_number + 1 << " has been deleted";
						cout << endl;
						cout << "   ";

						system("PAUSE");
						goto CarData;
					}

					// Back
					else if (CarChoice == 9)
					{
						system("CLS");
						goto AdminMenu;
					}

					else
					{
						// system title
						system("TITLE Car Dealership Management Program [Error Cars Data]");

						system("CLS");
						system("COLOR 4F");
						cout << "\n   Wrong input entered!" << endl;
						cin.clear();
						cin.ignore(22, '\n');
						cout << endl;
						cout << "   ";
						system("PAUSE");
						goto CarData;
					}
				}

				// Order Logs
				else if (adminchoice == 3)
				{
					// Set system title for order logs
					system("TITLE Car Dealership Management Program [Order Logs]");

					TitleText.titleText("                           Order Logs                           ");

					OrderLogs.orderLogs();

					FontTextColor.fontTextColor(9);
					cout << "\n   All Order Logs Data!";
					cout << endl;
					cout << "   ";
					system("PAUSE");
					goto AdminMenu;
				}

				// Login Logs
				else if (adminchoice == 4)
				{
					// Set system title for login logs
					system("TITLE Car Dealership Management Program [Login Logs]");

					TitleText.titleText("                           Login Logs                           ");

					LoginLogs.loginLogs();

					FontTextColor.fontTextColor(9);
					cout << "\n   All Login Logs Data!";
					cout << endl;
					cout << "   ";
					system("PAUSE");
					goto AdminMenu;
				}

				// Order Cars (Admin)
				else if (adminchoice == 5)
				{
					AccessLevel = 1; // Set access level to admin

					// Set system title for admin order cars
					system("TITLE Car Dealership Management Program [Admin Order Cars]");

					goto OrderCars; // Go to order cars section
				}

				// About Us (Admin)
				else if (adminchoice == 6)
				{
					// Set system title for about us
					system("TITLE Car Dealership Management Program [About Us]");

					AboutUs.AboutUs(); // Display about us information
					cout << "   ";
					system("PAUSE"); // Wait for user input

					goto AdminMenu; // Return to admin menu
				}

				// Logout
				else if (adminchoice == 7)
				{
					system("COLOR A");
					system("CLS");
					goto LoginForm;
				}

				// Wrong input
				else
				{
					// system title
					system("TITLE Car Dealership Management Program [Error Admin Menu]");

					system("CLS");
					system("COLOR 4F");
					cout << "\n   Wrong input entered!" << endl;
					cin.clear();
					cin.ignore(22, '\n');
					cout << endl;
					cout << "   ";
					system("PAUSE");
					goto AdminMenu;
				}
			}

			// Wrong login
			else
			{
				// system title
				system("TITLE Car Dealership Management Program [Error Wrong login]");

				system("CLS");
				system("COLOR 4F");
				cout << "\n   Wrong input entered!" << endl;
				cin.clear();
				cin.ignore(22, '\n');
				cout << endl;
				cout << "   ";
				system("PAUSE");
				goto AdminLogin;
			}
		}

		// Employee Login Process
		else if (choice == 2)
		{
		Employeelogin:
			// Set system title for employee login
			system("TITLE Car Dealership Management Program [Employee Login]");

			int Employeelogin = 0;	 // Flag for successful login
			string user, pass, u, p; // Variables for username and password

			// Display employee login form
			TitleText.titleText("                          Employee Login                        ");

			Choice.choiceBack("ESC", "Back"); // Show back option

			FontTextColor.fontTextColor(1); // Set text color to dark blue
			cout << "\n   Please enter your login details";

			FontTextColor.fontTextColor(9);
			cout << "\n   Username: ";
			FontTextColor.fontTextColor(11);

			string userHider;
			char c_user;

			do
			{
				c_user = _getch();
				switch (c_user)
				{
				case 0:
					_getch();
					break;
				case 13:
					cout << endl;
					break;
				case 27:
					system("CLS");
					goto LoginForm;
				case 8:
					if (userHider.length() > 0)
					{
						userHider.erase(userHider.end() - 1);
						cout << c_user << ' ' << c_user;
					}
					break;
				default:
					userHider += c_user;
					cout << c_user;
					break;
				}
			} while (c_user != 13);

			if (userHider.length() == 0)
			{
				system("CLS");
				system("COLOR 4F");
				cout << "\n   Please enter Username!" << endl;
				cout << endl;
				cout << "   ";
				system("PAUSE");
				goto Employeelogin;
			}

			else
			{
				user = userHider;
			}

			FontTextColor.fontTextColor(9);
			cout << "   Password: ";
			FontTextColor.fontTextColor(11);

			int i = 0;

			string passHider;
			char c;

			do
			{
				c = _getch();
				switch (c)
				{
				case 0:
					_getch();
					break;
				case 13:
					cout << endl;
					break;
				case 27:
					system("CLS");
					goto LoginForm;
				case 8:
					if (passHider.length() > 0)
					{
						passHider.erase(passHider.end() - 1);
						cout << c << ' ' << c;
					}
					break;
				default:
					passHider += c;
					cout << '*';
					break;
				}
			} while (c != 13);

			if (passHider.length() == 0)
			{
				system("CLS");
				system("COLOR 4F");
				cout << "\n   Please enter Password!" << endl;
				cout << endl;
				cout << "   ";
				system("PAUSE");
				goto Employeelogin;
			}

			else
			{
				pass = passHider;
			}

			// Display verification message with animation
			FontTextColor.fontTextColor(9);
			cout << "\n   Verifying your login details please wait";
			for (int i = 0; i < 6; i++)
			{
				cout << ".";
				Sleep(500);
			}

			// Read employee credentials from file and verify
			ifstream input("Text Files/Employee.txt");
			bool loginFound = false; // Flag to track if login is found

			// Check all lines in the employee file
			while (input >> u >> p)
			{
				if (u == user && p == pass)
				{
					loginFound = true; // Set flag if credentials match
					break;			   // Exit loop when match is found
				}
			}
			input.close();

			// Process login result
			if (loginFound)
			{
				Employeelogin = 1; // Set login success flag
				cout << "\n   Employee login successful!";
				employeeLogin = user; // Store employee username
				Sleep(2000);
				system("CLS");
			}
			else
			{
				// Login failed - credentials not found
				cout << "\n   Invalid Username or Password please try again";
				Sleep(2000);
				goto Employeelogin;
			}

			if (Employeelogin == 1)
			{

				// Login Logs [INPUT]
				auto t = time(nullptr);
				auto tm = *localtime(&t);

				ostringstream oss;
				oss << put_time(&tm, "%b/%d/%Y\t%H:%M:%S");
				auto str = oss.str();

				ofstream Login_Logs("Text Files/Login_Logs.txt", ios::app);
				Login_Logs << endl
						   << user << "\t" << "Employee" << "\t" << str;
				Login_Logs.close();

			EmployeeMenu:

				// system title
				system("TITLE Car Dealership Management Program [Employee Menu]");

				UserTitleText.userTitleText("                         Employee Menu                        ", user, "                             [", "]                             ");

				cout << endl;
				Choice.choiceBlue("1", "Order Logs");
				Choice.choiceBlue("2", "Order Cars"); // Removed \n

				Choice.choiceGreen("3", "About Us");

				Choice.choiceRed("4", "Logout"); // Removed \n

				FontTextColor.fontTextColor(9);
				cout << "    Please insert your choice: ";
				FontTextColor.fontTextColor(11);
				cin >> employeechoice;
				cout << endl;
				cin.ignore();

				// Order Logs (Employee)
				if (employeechoice == 1)
				{
					// Set system title for order logs
					system("TITLE Car Dealership Management Program [Order Logs]");

					TitleText.titleText("                           Order Logs                           ");

					OrderLogs.orderLogs();

					FontTextColor.fontTextColor(9);
					cout << "\n   All Order Logs Data!";
					cout << endl;
					cout << "   ";
					system("PAUSE");
					system("CLS");

					goto EmployeeMenu;
				}

				// Order Cars (Employee)
				else if (employeechoice == 2)
				{
					AccessLevel = 2; // Set access level to employee

					// Set system title for employee order cars
					system("TITLE Car Dealership Management Program [Employee Order Cars]");

					goto OrderCars; // Go to order cars section
				}

				// About Us (Employee)
				else if (employeechoice == 3)
				{
					// Set system title for about us
					system("TITLE Car Dealership Management Program [About Us]");

					AboutUs.AboutUs(); // Display about us information
					cout << "   ";
					system("PAUSE"); // Wait for user input

					goto EmployeeMenu; // Return to employee menu
				}

				// Logout
				else if (employeechoice == 4)
				{
					system("CLS");
					system("COLOR A");
					goto LoginForm;
				}

				// Wrong input in employee menu
				else
				{
					// Set system title for error
					system("TITLE Car Dealership Management Program [Error Wrong Input]");

					system("CLS");
					system("COLOR 4F");
					cout << "\n   Wrong input entered!" << endl;
					cin.clear();		  // Clear input error flags
					cin.ignore(22, '\n'); // Clear input buffer
					cout << endl;
					cout << "   ";
					system("PAUSE");
					goto EmployeeMenu; // Return to employee menu, not login form
				}
			}

			// Wrong login - employee credentials not found
			else
			{
				// Set system title for login error
				system("TITLE Car Dealership Management Program [Error Wrong Login]");

				system("CLS");
				system("COLOR 4F");
				cout << "\n   LOGIN ERROR!!!"
					 << "\n   Please check your Username and Password" << endl;
				Sleep(3000);
				goto LoginForm; // Return to main login form
			}
		}

		// Guest Access
		else if (choice == 3)
		{
			// Record guest login activity in log file
			auto t = time(nullptr);	  // Get current time
			auto tm = *localtime(&t); // Convert to local time

			ostringstream oss;							// String stream for formatting
			oss << put_time(&tm, "%b/%d/%Y\t%H:%M:%S"); // Format date and time
			auto str = oss.str();						// Convert to string

			// Write guest login log entry
			ofstream Login_Logs("Text Files/Login_Logs.txt", ios::app);
			Login_Logs << endl
					   << "Guest" << "\t" << "Guest" << "\t" << str;
			Login_Logs.close();

			AccessLevel = 3; // Set access level to guest

			// Set system title for guest order cars
			system("TITLE Car Dealership Management Program [Guest Order Cars]");

			goto OrderCars; // Go directly to order cars section
		}

		// About Us
		else if (choice == 4)
		{
			// Set system title for about us
			system("TITLE Car Dealership Management Program [About Us]");

			AboutUs.AboutUs(); // Display about us information
			cout << "   ";
			system("PAUSE"); // Wait for user input

			goto LoginForm; // Return to login form
		}

		// Exit Program
		else if (choice == 5)
		{
			// Set system title for exit message
			system("TITLE Car Dealership Management Program [Exiting. Thank you for using my program <3]");

			system("CLS");					// Clear the screen
			system("COLOR 01");				// Set color scheme
			FontTextColor.fontTextColor(9); // Set text color to blue
			cout << "\n   Thanks for using this program\n";
			cout << "\n   This program is created by ";
			FontTextColor.fontTextColor(11); // Set text color to cyan
			cout << "Uneeb Khan\n"
				 << endl;					 // Display programmer name
			FontTextColor.fontTextColor(10); // Set text color to green

			cout << "   ";
			system("PAUSE"); // Wait for user to press any key
			break;			 // Exit the while loop to terminate program
		}

		// Wrong input in main menu
		else
		{
			// Set system title for error
			system("TITLE Car Dealership Management Program [Error Wrong Input]");

			system("CLS");
			system("COLOR 4F");
			cout << "\n   Wrong input entered!" << endl;
			cin.clear();
			cin.ignore(22, '\n');
			Sleep(2000);

			goto LoginForm; // Return to login form
		}

	} // End of main program while loop

	return 0; // Program ends here only when user chooses exit

OrderCars:
	// Text file to array dynamically XD [Color]
	string toyota_car_color[15];
	int number_of_color = 0;
	ifstream Cars_Color("Text Files/Cars_Color.txt");
	if (Cars_Color.is_open())
	{
		string line;
		ifstream myCars_Color("Text Files/Cars_Color.txt");

		while (getline(myCars_Color, line))
			++number_of_color;

		// Ensure we don't exceed array bounds
		if (number_of_color > 15)
			number_of_color = 15;

		for (int i = 0; i < number_of_color; ++i)
		{
			Cars_Color >> toyota_car_color[i];
		}
		myCars_Color.close();
	}
	Cars_Color.close();

	// Text file to array dynamically XD [Name]
	string toyota_car_names[17];
	int number_of_name = 0;
	ifstream Cars_Name("Text Files/Cars_Name.txt");
	if (Cars_Name.is_open())
	{
		string line;
		ifstream myCars_Name("Text Files/Cars_Name.txt");

		while (getline(myCars_Name, line))
			++number_of_name;

		// Ensure we don't exceed array bounds
		if (number_of_name > 17)
			number_of_name = 17;

		for (int i = 0; i < number_of_name; ++i)
		{
			Cars_Name >> toyota_car_names[i];
		}
		myCars_Name.close();
	}
	Cars_Name.close();

	// Text file to array dynamically XD [Price]
	int toyota_car_prices[17];
	int number_of_price = 0;
	ifstream Cars_Price("Text Files/Cars_Price.txt");
	if (Cars_Price.is_open())
	{
		string line;
		ifstream myCars_Price("Text Files/Cars_Price.txt");

		while (getline(myCars_Price, line))
			++number_of_price;

		// Ensure we don't exceed array bounds
		if (number_of_price > 17)
			number_of_price = 17;

		for (int i = 0; i < number_of_price; ++i)
		{
			Cars_Price >> toyota_car_prices[i];
		}
		myCars_Price.close();
	}
	Cars_Price.close();

	system("COLOR 0A");

	if (AccessLevel == 1)
	{
		UserTitleText.userTitleText("                        Admin Order Cars                      ", "Uneeb Khan", "                             [", "]                           ");
	}

	else if (AccessLevel == 2)
	{
		UserTitleText.userTitleText("                      Employee Order Cars                     ", employeeLogin, "                             [", "]                             ");
	}

	else if (AccessLevel == 3)
	{
		UserTitleText.userTitleText("                       Guest Order Cars                       ", "Guest", "                             [", "]                            ");
	}

	else
	{
		UserTitleText.userTitleText("                     \"Unknown Order Cars\"                     ", "Unknown", "                            [", "]                           ");
	}

	Choice.choiceBack("0", "Back");

	// Callout car names/prices [formated]
	FontTextColor.fontTextColor(9);
	Display_Menu.display_menu("\n  Toyota", toyota_car_names, toyota_car_prices, number_of_name);

	FontTextColor.fontTextColor(9);
	cout << "\n  Input Number According to the type of Car: ";

	FontTextColor.fontTextColor(11);

	cin >> Cars;

	if (Cars == 0)
	{
		cin.clear();
		cin.ignore(22, '\n');
		system("CLS");

		goto LoginForm;
	}

	else if (Cars < 1 || Cars > number_of_name)
	{
		system("CLS");
		system("COLOR 4F");
		cout << "\n  Invalid input please try ordering again." << endl;
		cin.clear();
		cin.ignore(22, '\n');

		Sleep(2000);
		goto OrderCars;
	}

	else if (Cars >= 1 && Cars <= number_of_name)
	{
		// Order confirmation
		FontTextColor.fontTextColor(9);
		cout << "\n  You selected ";
		FontTextColor.fontTextColor(11);
		cout << toyota_car_names[Cars - 1];
		FontTextColor.fontTextColor(9);
		cout << " for ";
		FontTextColor.fontTextColor(11);
		cout << toyota_car_prices[Cars - 1];
		cout << " pesos";
		cout << endl;

		FontTextColor.fontTextColor(9);
		cout << "  To be confirmed type Y Yes, Type N if No: ";

		FontTextColor.fontTextColor(11);
		cin >> order_confirmation;

		if (order_confirmation == 'y' || order_confirmation == 'Y')
		{

			// Select Color
		SelectColor:
			system("CLS");
			system("COLOR 0A");

			system("COLOR 0A");

			if (AccessLevel == 1)
			{
				UserTitleText.userTitleText("                     Admin Select Car Color                   ", "Uneeb Khan", "                             [", "]                           ");
			}

			else if (AccessLevel == 2)
			{
				UserTitleText.userTitleText("                   Employee Select Car Color                  ", employeeLogin, "                             [", "]                             ");
			}

			else if (AccessLevel == 3)
			{
				UserTitleText.userTitleText("                     Guest Select Car Color                   ", "Guest", "                             [", "]                            ");
			}

			else
			{
				UserTitleText.userTitleText("                   \"Unknown Select Car Color\"                 ", "Unknown", "                            [", "]                           ");
			}

			cout << endl;
			Choice.choiceGreen("0", "Back");

			FontTextColor.fontTextColor(9);
			cout << endl;
			cout << "   What Color of ";
			FontTextColor.fontTextColor(11);
			cout << toyota_car_names[Cars - 1];
			FontTextColor.fontTextColor(9);
			cout << " do you want?";
			cout << endl;

			// Callout all Color [Fomated]
			Display_Color.display_color(toyota_car_color, number_of_color);

			FontTextColor.fontTextColor(9);
			cout << "\n   Please insert the number of Color: ";

			FontTextColor.fontTextColor(11);
			cin >> Color;

			// Verification
			if (Color == 0)
			{
				cin.clear();
				cin.ignore(22, '\n');
				system("CLS");

				goto OrderCars;
			}

			else if (Color < 1 || Color > number_of_color)
			{
				system("CLS");
				system("COLOR 4F");
				cout << "\n   Please try again if you entered the incorrect information.";
				cout << endl;
				cout << "   ";
				cin.clear();
				cin.ignore(22, '\n');
				system("PAUSE");
				goto SelectColor;
			}

			else if (Color >= 1 && Color <= number_of_color)
			{
			Cashier:
				// Cashier

				Value = toyota_car_prices[Cars - 1];

				if (AccessLevel == 1)
				{
					UserTitleText.userTitleText("                         Admin Cashier                        ", "Namias", "                             [", "]                           ");
				}

				else if (AccessLevel == 2)
				{
					UserTitleText.userTitleText("                        Employee Cashier                      ", employeeLogin, "                             [", "]                             ");
				}

				else if (AccessLevel == 3)
				{
					UserTitleText.userTitleText("                         Guest Cashier                        ", "Guest", "                             [", "]                            ");
				}

				else
				{
					UserTitleText.userTitleText("                        \"Unknown Cashier\"                     ", "Unknown", "                            [", "]                           ");
				}

				// Your order summary
				cout << endl;
				FontTextColor.fontTextColor(9);
				cout << "  You selected ";
				FontTextColor.fontTextColor(15);
				cout << "[";
				FontTextColor.fontTextColor(11);
				cout << toyota_car_color[Color - 1];
				FontTextColor.fontTextColor(15);
				cout << "] ";
				FontTextColor.fontTextColor(11);
				cout << toyota_car_names[Cars - 1];
				FontTextColor.fontTextColor(9);
				cout << " for ";
				FontTextColor.fontTextColor(11);
				cout << toyota_car_prices[Cars - 1];
				FontTextColor.fontTextColor(11);
				cout << " pesos";
				cout << endl;
				;

				// Total of the order
				cout << endl;
				FontTextColor.fontTextColor(9);
				cout << "  Total Amount: ";
				FontTextColor.fontTextColor(11);
				cout << Value;
				cout << endl;

				FontTextColor.fontTextColor(9);
				cout << endl;
				cout << "  Enter The Amount of Payment: ";

				FontTextColor.fontTextColor(11);
				cin >> Payment;

				Change = Payment - Value;

				if (Change >= 1)
				{
					FontTextColor.fontTextColor(9);
					cout << "\n  Change: ";
					FontTextColor.fontTextColor(11);
					cout << Change << "\n"
						 << endl;

					FontTextColor.fontTextColor(9);
					cout << endl;
					cout << "  ";
					system("PAUSE");
				}

				else if (Change == 0)
				{
					FontTextColor.fontTextColor(9);
					cout << "\n  You paid the exact amount of money needed." << endl;

					cout << endl;
					cout << "  ";
					system("PAUSE");
				}

				else if (Payment == 0)
				{
					system("CLS");
					system("COLOR 4F");
					cout << "\n  Invalid input please try ordering again." << endl;
					cin.clear();
					cin.ignore(22, '\n');

					Sleep(2000);
					goto Cashier;
				}

				else if (Change <= -1)
				{
					system("CLS");
					system("COLOR 4F");
					cout << "\n  You don't have enough money.";
					cin.clear();
					cin.ignore(22, '\n');

					Sleep(2000);
					goto Cashier;
				}

				else
				{
					system("CLS");
					system("COLOR 4F");
					cout << "\n  Invalid input please try ordering again." << endl;
					cin.clear();
					cin.ignore(22, '\n');

					Sleep(2000);
					goto Cashier;
				}

				// Order Logs [INPUT]
				auto t = time(nullptr);
				auto tm = *localtime(&t);

				ostringstream oss;
				oss << put_time(&tm, "%b/%d/%Y\t%H:%M:%S");
				auto str = oss.str();

				ofstream Order_Logs("Text Files/Order_Logs.txt", ios::app);
				Order_Logs << endl
						   << str << "\t" << toyota_car_names[Cars - 1] << '\t' << toyota_car_color[Color - 1] << '\t' << toyota_car_prices[Cars - 1];
				Order_Logs.close();

				// Also record in new data structures system
				// Find or create customer (using a default customer for now)
				if (dealershipSystem->findCustomer(1) == nullptr)
				{
					dealershipSystem->registerCustomer("Default Customer", "000-000-0000", "default@email.com", 100000);
				}

				// Record the sale in the stack for undo functionality
				dealershipSystem->recordSale(1, Cars, toyota_car_prices[Cars - 1]);

			Reciept:
				Reciept.Reciept(str, toyota_car_names[Cars - 1], toyota_car_color[Color - 1], toyota_car_prices[Cars - 1], Payment, Change);

				// Order again?
				FontTextColor.fontTextColor(9);
				cout << "\n  Do you want to order again? Type Y if yes and type N if no: ";
				FontTextColor.fontTextColor(11);
				cin >> order_more;

				if (order_more == 'n' || order_more == 'N')
				{
					FontTextColor.fontTextColor(9);
					cout << "  Thank you for your purchase! <3";

					Sleep(2000);
					goto LoginForm;
				}

				else if (order_more == 'y' || order_more == 'Y')
				{
					FontTextColor.fontTextColor(9);
					cout << "  Thank you for your purchase! <3";

					Sleep(2000);
					goto OrderCars;
				}

				else
				{
					system("CLS");
					system("COLOR 4F");
					cout << "\n  Invalid input! Type only Y or N" << endl;
					cout << endl;
					cin.clear();
					cin.ignore(22, '\n');
					cout << "  ";
					system("PAUSE");
					goto Reciept;
				}
			}

			else
			{
				system("CLS");
				system("COLOR 4F");
				cout << "\n  Invalid input please try again." << endl;
				goto SelectColor;
			}
		}

		else if (order_confirmation == 'n' || order_confirmation == 'N')
		{
			FontTextColor.fontTextColor(9);
			cout << "\n  You Canceled the order." << endl;
			Sleep(2000);

			goto OrderCars;
		}

		else
		{
			system("CLS");
			system("COLOR 4F");
			cout << "\n  Invalid input! Type only Y or N" << endl;
			cin.clear();
			cin.ignore(22, '\n');

			Sleep(2000);

			goto OrderCars;
		}
	}

	else
	{
		system("CLS");
		system("COLOR 4F");
		cout << "\n  Invalid input please try ordering again.";
		cin.clear();
		cin.ignore(22, '\n');

		cout << "   ";
		system("PAUSE");

		goto OrderCars;
	}
}
// Cleanup memory before program ends
// Note: This cleanup code may not be reached due to goto statements and exit() calls
// but it's good practice to include it for proper memory management