#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

/*Creation of Course object to contain course input*/
class Course {
public:											//public attributes and functions
	string courseNumber;						//string for course number
	string courseTitle;							//string for course title
	vector<string> prereqs;						//vector to contain course prerequisites
	Course();									//default constructor
	Course(vector<string> t_prereqs);			//constructor with prerequisites
	vector<string> getPrereqs();				//function to return prerequisites
};


Course::Course() {										//Constructor for Course object
	courseNumber = "0";										//default courseNumber to 0
	courseTitle = "0";										//default courseTitle to 0
}

Course::Course(vector<string> t_prereqs) {				//Constructor for Course object with prerequisite
	prereqs = t_prereqs;									//this course's prereqs is equal to t_prereqs
}

vector<string> Course::getPrereqs() {					//function to return course's prerequisites
	return prereqs;											//return prereqs
}





/*Creation of Node strucutre for use as container
for courses to be used in binary search tree*/

struct Node {
	Course course;								//node should hold course
	Node* left;									//create pointer to left
	Node* right;								//create pointer to right

	Node() {									//default constructor for empty node
		left = nullptr;							
		right = nullptr;
	}

	Node(Course t_course) :						//constructor with course
		Node() {
			course = t_course;
	}
};

/*Creation of BinarySearchTree object to store courses*/

class BinarySearchTree {
private:												//private attributes and functions
	Node* root;											//create root of BST
	void addNode(Node* t_node, Course t_course);		//function to add node to tree
	void inOrder(Node* t_node);							//function to output tree in ascending order

public:													//public attributes and functions
	void Insert(Course t_course);						//function to insert course into BST
	Course Search(string t_courseNum);					//function to search for course in BST given course number
	void PrintSchedule();								//function to print all of the courses in the bst

};


void BinarySearchTree::addNode(Node* t_node, Course t_course) {						//Function to add Node to BST
	if (t_course.courseNumber < t_node->course.courseNumber) {							//IF courseNumber to be added is less than current node's
		if (t_node->left == nullptr) {														//IF current node's left is null
			t_node->left = new Node(t_course);													//current node's left becomes new Node with course
		}
		else {																				//ELSE
			addNode(t_node->left, t_course);													//Recursively addNode to the left
		}
	}
	else {																				//ELSE 
		if (t_node->right == nullptr) {														//IF current node's right is null
			t_node->right = new Node(t_course);													//current node's right becomes new Node with course
		}		
		else {																				//ELSE
			addNode(t_node->right, t_course);													//Recursiviely addNode to the right
		}
	
	}
}

void BinarySearchTree::inOrder(Node* t_node) {										//Function to output BST in ascending order																
	if (t_node != nullptr) {															//IF current node is not null
		inOrder(t_node->left);																//Recursively pass current node's left
		cout << t_node->course.courseNumber << " " << t_node->course.courseTitle << endl;	//OUTPUT current nodes course information
		inOrder(t_node->right);																//Recursively pass current node's right
	}
}

void BinarySearchTree::Insert(Course t_course) {						//Function to insert course into BST
	if (root == nullptr) {													//IF root is null
		root = new Node(t_course);												//root becomes new Node with course
		root->left = nullptr;													//roots left is equal to null
		root->right = nullptr;													//roots right is equal to null
	}
	else {																	//ELSE
		addNode(root, t_course);												//Pass root and course through addNode() function
	}
}

Course BinarySearchTree::Search(string t_courseNum) {				//Function to find course given course number
	Course empty = Course();										//Create empty course
	Node* current = root;											//Create pointer set equal to root
	while (current != nullptr) {									//WHILE pointer is not null
		if (current->course.courseNumber == t_courseNum) {				//IF pointer's courseNumber is equal to searched courseNumber
			return current->course;											//return pointer's course
		}	
		else if (current->course.courseNumber < t_courseNum) {			//ELSE IF pointer's courseNumber is less than searched courseNumber
			current = current->right;										//move to the right
		}
		else {															//ELSE
			current = current->left;										//move to the left
		}
	}

	return empty;													//return empty course if loop completes without returning course was not found
}

void BinarySearchTree::PrintSchedule() {						//Function to print all of the courses in the BST
	inOrder(root);													//call inOrder function pass root as parameter
}



/*Main Program Help Functions*/


/*Function to display menu for program*/
void DisplayMenu() {											
	cout << "" << endl;
	cout << "" << endl;
	cout << "1." << " Load Data Structure" << endl;
	cout << "2." << " Print Course List" << endl;
	cout << "3." << " Print Course" << endl;
	cout << "9." << " Exit Program" << endl;
	cout << "" << endl;
	cout << "Please select an option from the menu" << endl;

}

/*Function to load data from file,
parse file seperating data, construct course, 
and add course to the bst for use*/
void LoadFile(BinarySearchTree* t_bst) {
	ifstream inFile;							//declare filestream
	stringstream inStream;						//declare stringstrea

	string wholeLine;							//string variable to store whole line from file
	Course newCourse;							//Course variable to serve as new course to be added
	string currentString;						//string variable for each word of the line
	
	vector<string> parsedLine;					//vector to store parsed line words
	vector<string> currentPrereqs;				//vector to store course prerequisites

	bool dataToRead = true;						//boolean variable for file loop
	size_t i;									//size_t variable for loop initialized to true

	inFile.open("CourseList.txt");				//open file in filestream
	if (!inFile.is_open()) {					//IF file is not open
		cout << "Unable to open file" << endl;		//ouput issue opening file
		return;										//return
	}
	else {										//ELSE
		while (dataToRead == true) {				//WHILE boolean is true
			if (inFile.eof()) {							//IF the end of the file is reached
				dataToRead = false;							//change boolean to false
			}
			getline(inFile, wholeLine);					//get whole line from file
			if (inFile.good()) {						//IF the line was recieved fine
				inStream.clear();							//clear the stringstream
				inStream.str(wholeLine);					//place the whole line from file into stringstream
				while (inStream.good()) {					//WHILE the string stream is good
					getline(inStream, currentString, ',');		//Seperate words by commas and store each word as currentString
					parsedLine.push_back(currentString);		//add each word from line to vector
				}
				if (parsedLine.size() < 2) {				//IF the number of words is less than two (must have course number and title)
					cout << "Error reading file" << endl;		//output issue reading file
					return;										//return

				}
				newCourse.courseNumber = parsedLine.at(0);	//Set new course courseNumber equal to first word of line
				newCourse.courseTitle = parsedLine.at(1);	//Set new course courseTitle equal to second word of line
				if (parsedLine.size() > 2) {				//IF the line had more than two words
					for (i = 2; i < parsedLine.size(); i++) {	//FOR each word left starting with the third word
						currentPrereqs.push_back(parsedLine.at(i));	//add word to vector of prerequisites
					}
					newCourse.prereqs = currentPrereqs;			//set new course prerequisites equal to the vector of prerequsites
					currentPrereqs.clear();						//clear the vector
				}
				t_bst->Insert(newCourse);						//insert the new course into the BST
				parsedLine.clear();								//clear the vector of words
			}
			
		}
		if (!inFile.eof()) {							//IF the end of the file was not reached
			cout << "Unable to read all of file" << endl;	//output issue reading entire file
			return;											//return
		}
	}
}

string Capital(string t_userString) {						//Function to change user input to uppercase
	size_t i;													//size_t variable for loop
	for (i = 0; i < t_userString.length(); i++) {				//FOR all of the characters in the string
		t_userString[i] = toupper(t_userString[i]);					//change character to uppercase
	}
	return t_userString;										//return string
}




/*Main ABCU Program Begins Here*/

int main() {
	BinarySearchTree* bst;									
	bst = new BinarySearchTree();													//Creation of binary search tree to store all courses
	Course searchCourse;															//declare course variable to serve as returned course from search
	char userKey;																	//dclare character variable for user input
	string userCourse;																//declare string variable for user course search input
	bool running = true;															//boolean variable for menu loop

	cout << "Welcome to The Course Planner" << endl;								//Output introduction to program
	while (running != false) {														//WHILE there is still data to read
		DisplayMenu();																	//call function to display menu options
		cin >> userKey;																	//Get input from user
		if (userKey == '1') {															//IF user input is equal to 1
			LoadFile(bst);																	//call function to load file into binary search tree
		}
		else if (userKey == '2') {														//ELSE IF user key is equal to 2
			cout << endl;																	//output empty line
			bst->PrintSchedule();															//call function to print all courses in binary search tree in ascending order
		}
		else if (userKey == '3') {														//ELSE IF user key is eqaul to 3
			cout << endl;																	//output empty line
			cout << "Please enter the course number of the course you desire: ";			//prompt user for course number	
			cin >> userCourse;																//Get input form user
			searchCourse = bst->Search(Capital(userCourse));								//Set course variable equal to the results of the search in the binary search tree
			if (searchCourse.courseNumber != "0") {											//IF course is not empty
				cout << endl;																	//Output empty line
				cout << searchCourse.courseNumber << ", " << searchCourse.courseTitle << endl;	//Output the course number and course title of the returned course
				cout << "Prerequisites: ";														//Output title for prerequisites
				for (size_t i = 0; i < searchCourse.prereqs.size(); ++i) {						//for each prerequisite of the returned course
					if (i == searchCourse.prereqs.size() - 1) {										//IF it is the last prerequisite of the course 
						cout << searchCourse.prereqs.at(i) << endl;										//Output prerequisite move to newline
					}
					else {																			//ELSE 
						cout << searchCourse.prereqs.at(i) << ",";										//Ouput prerequisite with comma preparing for next
					}
				}
			}
			else {																			//ELSE
				cout << endl;																	//Output empty line
				cout << "Course not found in the system." << endl;								//Output that the course was not found in the system
			}
		}
		else if (userKey == '9') {														//ELSE IF user key is equal to 9
			cout << endl;																	//Output empty line
			cout << "You have now exited the program. Thank you." << endl;					//Ouput that the user has exited the program
			running = false;																//Set boolean variable to false

		}
		else {																			//ELSE (catch all invalid menu options)
			cout << endl;																	//Output empty line
			cout << "Invalid input. Please select and option from the menu" << endl;		//Output message informing user input was invalid
			continue;																		//CONTINUE
		}
	}


}