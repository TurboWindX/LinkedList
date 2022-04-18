#include <algorithm>
#include <iostream>
#include <time.h>
#include <string>
#include "CSVparser.hpp"

using namespace std;
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount = 0.0;
};

//============================================================================
// Linked-List class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a linked-list.
 */
class LinkedList {
private:
    //Internal structure for list entries, housekeeping variables
    struct Node {
        Bid bid;
        Node *next;
        // default constructor
        Node() {
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) {
            bid = aBid;
            next = nullptr;
        }
    };
    Node * head;
    Node * tail;

public:
    LinkedList();
    virtual ~LinkedList();
    void Append(Bid bid);
    void Prepend(Bid bid);
    void PrintList();
    void Remove(string bidId);
    bool IsUnique(string bidId);
    Bid Search(string bidId);
    int GetSize();
};

bool LinkedList::IsUnique(string bidId) {
    Node* curNode = head;
    while (curNode != NULL) {
        if (curNode->bid.bidId == bidId) {
            return false;
        }
        curNode = curNode->next;
    }
    return true;
}

LinkedList::LinkedList() {
    head = NULL;
    tail = NULL;
}

/**
 * Destructor
 */
LinkedList::~LinkedList() {
    // start at the head
    Node * current = head;
    Node * temp;

    // loop over each node, detach from list then delete
    while (current != nullptr) {
        temp = current; // hang on to current node
        current = current->next; // make current the next node
        delete temp; // delete the orphan node
    }
}

/**
 * Append a new bid to the end of the list
 */
void LinkedList::Append(Bid bid) {
    // FIXME (2): Implement append logic
    //Create new node
    Node* newNode = new Node(bid);
    if (!IsUnique(bid.bidId)) {
        return;
    }
    //if there is nothing at the head...
    if (head == NULL) {
        // new node becomes the head and the tail
        head = newNode;
        tail = newNode;
    }
    //else 
    else {
        // make current tail node point to the new node
        tail->next = newNode;
        // and tail becomes the new node
        tail = newNode;
    }
}

/**
 * Prepend a new bid to the start of the list
 */
void LinkedList::Prepend(Bid bid) {
    // FIXME (3): Implement prepend logic
    // Create new node
    Node * newNode = new Node(bid);
    if (!IsUnique(bid.bidId)) {
        return;
    }
    // if there is already something at the head...
    if (head != NULL) {
        // new node points to current head as its next node
        newNode->next = head;
        // head now becomes the new node
        head = newNode;
    }
    else {
        head = newNode;
        tail = newNode;
    }
}

/**
 * Simple output of all bids in the list
 */
void LinkedList::PrintList() {
    // FIXME (4): Implement print logic
    // start at the head
    Node* theBidPointer = new Node();
    theBidPointer = head;

    // while loop over each node looking for a match
        //output current bidID, title, amount and fund
        //set current equal to next
    while (theBidPointer != NULL) {
        std::cout << theBidPointer->bid.bidId << ": " << theBidPointer->bid.title << " | " << theBidPointer->bid.amount << " | " << theBidPointer->bid.fund << endl;
        theBidPointer = theBidPointer->next;
    }
}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */

void LinkedList::Remove(string bidId) {
    // FIXME (5): Implement remove logic
    // special case if matching node is the head
    if (head->bid.bidId == bidId) {
        // make head point to the next node in the list
        head = head->next;
    }

        //return

    else {
        // start at the head
        Node * curNode = head;
        Node* temp = NULL;

        if (head->bid.bidId == bidId) {
            Node* temp = NULL;
            temp = head;
            head = head->next;
            delete temp;
        }
        else {
            // start at the head
            Node* curNode = head;
            Node* temp = NULL;

            // while loop over each node looking for a match
            while (curNode->next != NULL) {
                if (curNode->next->bid.bidId == bidId) {
                    temp = curNode->next;
                    curNode->next = curNode->next->next;
                    delete temp;
                    return;
                }
                curNode = curNode->next;
            }
        }
    }
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid LinkedList::Search(string bidId) {
    Node* curNode = head;
    Bid bid;
    while (curNode != NULL) {
        if (curNode->bid.bidId == bidId) {
            return curNode->bid;
        }
        curNode = curNode->next;
    }
    return bid;
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::GetSize() {
    Node * curNode = head;
    int count = 0;
    while (curNode != NULL) {
        ++count;
        curNode = curNode->next;
    }
    return count;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    std::cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | " << bid.fund << endl;
    return;
}

/**
 * Prompt user for bid information
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    std::cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    std::cout << "Enter title: ";
    getline(cin, bid.title);

    std::cout << "Enter fund: ";
    cin >> bid.fund;

    std::cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

/**
 * Load a CSV file containing bids into a LinkedList
 *
 * @return a LinkedList containing all the bids read
 */
void loadBids(string csvPath, LinkedList *list) {
    std::cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // initialize a bid using data from current row (i)
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //std::cout << bid.bidId << ": " << bid.title << " | " << bid.fund << " | " << bid.amount << endl;

            // add this bid to the end
            list->Append(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 *
 * @param arg[1] path to CSV file to load from (optional)
 * @param arg[2] the bid Id to use when searching the list (optional)
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidId;
    switch (argc) {
        case 2:
            csvPath = argv[1];
            bidId = "98109";
            break;
        case 3:
            csvPath = argv[1];
            bidId = argv[2];
            break;
        default:
            csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
            bidId = "98109";
            cout << "Advanced usage: LinkedList.exe <csvPath> <bidId>" << endl;
    }

    clock_t ticks;
    LinkedList bidList;
    Bid bid;
    int choice = 0;
    string tofind;
    while (choice != 9) {
        std::cout << "Menu:" << endl;
        std::cout << "  1. Prepend a Bid" << endl;
        std::cout << "  2. Append a Bid" << endl;
        std::cout << "  3. Load Bids" << endl;
        std::cout << "  4. Display All Bids" << endl;
        std::cout << "  5. Find Bid" << endl;
        std::cout << "  6. Remove Bid" << endl;
        std::cout << "  9. Exit" << endl;
        std::cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            printf("\033c");
            bid = getBid();

            bidList.Prepend(bid);

            break;

        case 2:
            printf("\033c");
            bid = getBid();

            bidList.Append(bid);

            break;

        case 3:
            printf("\033c");
            ticks = clock();

            loadBids(csvPath, &bidList);

            std::cout << bidList.GetSize() << " bids read" << endl;

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            std::cout << "time: " << ticks << " milliseconds" << endl;
            std::cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 4:
            printf("\033c");
            bidList.PrintList();

            break;

        case 5:
            ticks = clock();
            cout << "Enter the Bid Id you are looking for" << endl;
            cin >> tofind;
            bid = bidList.Search(tofind);
            printf("\033c");
            printf("\n");
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
            	std::cout << "Bid Id " << bidId << " not found." << endl;
            }
            printf("\n");
            std::cout << "time: " << ticks << " clock ticks" << endl;
            std::cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 6:
            std::cout << "Please enter the Bid ID of the Bid you would like to remove." << endl;
            cin >> bidId; 
            bidList.Remove(bidId);

            break;
        }
    }

    std::cout << "Good bye." << endl;

    return 0;
}
