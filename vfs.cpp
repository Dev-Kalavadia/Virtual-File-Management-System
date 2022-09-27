// Name:: Dev Kalavadiya
// NetID: Dk3936

//including header files
#include<iostream>
#include<string>
#include<ctime>
#include<ctype.h>
#include<fstream>
#include<sstream>

using namespace std;
class NodeList;
class Inode;
class VFS;
class Queue;

void listCommands() // display message 
{
	cout<<"------------------------------------------------------------------------------------------------"<<endl;
	cout<< "\t\t\tWelcome to Virtual File System" << endl;
	cout<<"------------------------------------------------------------------------------------------------"<<endl;
	cout<< "Please enter one of the following commands!!" << endl;
	cout<<"Help                 			:Prints the available commands"<<endl
		<<"pwd    			    		:Prints the path of the current inode"<<endl
		<<"realpath filename    			:Prints the full path of a given file"<<endl
		<<"ls            			        :Prints the children if the current inode"<<endl
		<<"ls_sort              			:order by descending file size"<<endl
		<<"mkdir foldername     			:Creates  folder under the curent folder"<<endl
		<<"touch filename size  			:Creates a file under the current inode location"<<endl
		<<"cd foldername        			:change current to specific folder location"<<endl
		<<"cd ..                            	:change current inode location to its parent folder"<<endl
		<<"cd -        				:change current inode location to its previous folder"<<endl
		<<"cd         				:change current inode location to the root"<<endl
		<<"cd path        			        :change current inode location to the specified path"<<endl
		<<"find foldername      			:Returns the path of a file, if it exists"<<endl
		<<"mv filename foldername			:Moves a file location "<<endl
		<<"rm foldername/filename			:Removes the specific folder of file and moves it to the bin"<<endl
		<<"size foldername/filename       	        :Returns the total size of the folder,or the size of the file"<<endl
		<<"emptybin             			:Empties the bin" <<endl
		<<"showbin              			:Shows the oldest inode of the bin"<<endl
		<<"recover              			:Reinstates the oldest inode back from the bin to its original postion"<<endl
		<<"Exit                 			:THe program stops and the filesystem is saved"<<endl;
}

// method that returns current date as string
string getCurrentDate(){ 
	string Date = ""; 
	time_t date = time(0); 
	tm* current = localtime(&date);
	Date += to_string((current->tm_mday))  + "-" + to_string((current->tm_mon+1)) + "-" + to_string((current->tm_year-100));
	return Date;
}

class Inode // INode class
{
	private:
		string nodeName, date;
		int size = 0; // 
		Inode* next; //Link (pointer) to the next Inode
		Inode* prev; //Link (pointer) to the previous Inode
		friend class NodeList; 
		friend class VFS;
		friend class Queue;
	public:
		Inode(); // empty costructor
		Inode(string nodeName, int size, string date, bool isFile); // copy constructor
		~Inode(); // Destrcutor
		Inode* Parent; // PArent inode
		NodeList* children; // Children's list
		bool isFile = false; // check if is file or filder
		int getSize() {return this->size;} // getter methods (not necesarily needed since its all friended)
		string getName() {return this->nodeName;} 
		string getDate() {return this->date;} 
		bool check_isFile() {return this->isFile;}  
		
};

class NodeList // Double linked list for children
{
	private:
		Inode *head; 	// pointer to the head of List
		Inode *tail; 	// pointer to the tail of List
		friend class Inode;
		friend class VFS;
		friend class Queue;
	public:
		NodeList (); // empty list constructor
		~NodeList (); // destructor to clean up all nodes
		bool empty() const; // is list empty
		Inode* front(); // get the value (Inodeent) from front Inode of list
		void back() const;  // get the value (Inodeent) from last Inode of the List 
		void addBack(Inode *ptr); //add a new node to the back of the list
		void removeFront(); // remove front node from list
		void showChildren(); // Show children prints each files details
		Inode* remove(string parameter); // removes the file
		Inode* findchild(string parameter); // returns a pointer to the 
		Inode* checkChild(string parameter); //Checks if the child is present
		void skipOver(Inode* current); //Skips over the target That is passed
		bool checkIfChild(string parameter); //checks if the child is present in the list
	 
		class Iterator; //Forward declaration of the Iterator class
		Iterator begin() const{ // Defines to begin
			return Iterator(head->next);
		}
		Iterator end() const{ //defines the endd
			return Iterator(tail);
		}
		class Iterator{
		private:
			Inode *node; // Node which is a pointer to Inote
		public:
			Iterator(Inode* new_node) : node(new_node){} // constructor
			Iterator() : node(NULL) {} // eempty constructor

			Inode* operator*(){
				return node; 
			}

			Iterator& operator ++(){
				node = node->next;
				return *this;
			}

			Iterator& operator --(){
				node = node->prev;
				return *this;
			}

			bool operator ==(const Iterator& other) const{  //Overloading operator
				return node == other.node;
			}
			bool operator !=(const Iterator& other) const{ //Overloading operator
				return node != other.node;
			}
		};
};

Inode::Inode(){ // Empty constructor
	nodeName = "";
	date = getCurrentDate();
	size = 0;
	next = NULL; //Link (pointer) to the next Inode
	prev = NULL;
}

Inode::~Inode(){ //Destructor
	if (!isFile){
		delete children;
	}
}

Inode::Inode(string nodeName, int size, string date, bool isFile){ // Main constructor
	this->nodeName = nodeName;
	this->size = size;
	this->date = date;
	this->isFile = isFile;
	next = NULL; //Link (pointer) to the next Inode
	prev = NULL;
	if (!isFile){
		children = new NodeList(); // If it is a file create a new children list
	}
}
NodeList::NodeList(){ // Nodelist constructor
	this->head = new Inode();
	this->tail = new Inode();
	this->head->next = tail;
	this->tail->prev = head;
}

//So basically my nodelist destructor is giving me an error every time I exit the program it gives a segmentation fault
//I'm unable to identify the error I tried really hard!!! Other than this everything else works perfectly
NodeList::~NodeList () // destructor to clean up all nodes
{
	// while(!empty())
		// removeFront();
	// delete head;
	// delete tail;
}
bool NodeList::empty() const // is list empty
{
	return (head->next == tail);
}

void NodeList::removeFront() // remove front item from list
{
    Inode* temp = head->next; // temp node  assigned as head
    head->next = temp->next; // head is assigned to the next node 
	temp->next->prev = head; //The head is assigned to be the next previous
    delete temp; // Delete the temporary node
}
void NodeList::addBack(Inode *ptr) // add to Back of the list
{
  	ptr->next = tail; // New node pointing to the next of the pointer
  	ptr->prev = tail->prev; // new node's prev poninting to the previous of the pointer
  	tail->prev = ptr; // Shut the pointer as tail's previous
	ptr->prev->next = ptr; // Pointer where the new node needs to be added
}	

Inode* NodeList::remove(string parameter) // remove last item from list
{
	Inode *current = head->next; // Create a new note that points to heads next
	for (auto node: *this){ // Using auto to iterate through the list
		if(node->getName() == parameter){
			node->prev->next = node->next; // skip over it 
			node->next->prev = node->prev;
			cout << parameter << " has been deleted and moved to bin" << endl;
			return node; 
		}
	}
	if (parameter == "/"){
		cout << "Sorry cannot delete the root" << endl;
		}else{
		throw runtime_error("The File does not exist in the folder or has been deleted already");
	}
	return current;
}

Inode* NodeList::findchild(string parameter){
	Inode *curr=head->next; // set current head as the next 
	while(curr->next!=NULL){ // if the next is null it will iterate through
		if (curr->getName() == parameter){
			return curr;
		}
		curr = curr->next; 
	}
	return NULL;
}

void NodeList::showChildren() {
	string type;
	for(auto i: *this){ // Using auto to iterate through the list
		if (i->isFile){ // checks if its file 
			type = "file";
		}else{
			type = "dir";
		} 
		cout<< type << " "<<i->getName() << " " << i->getDate() << " " << i->getSize()<< "bytes" << endl; // prints the output
	}
}

Inode* NodeList::front(){
	return head->next;
}

class VFS
{
	private:
		Inode *root; 
		friend class Inode;
		friend class NodeList;
	public:
		VFS();									//constructor
		~VFS();									//destructor
		Inode* getRoot();						//returns the root of the Tree
		Inode* insert(Inode* ptr,int key);		//Insert key into tree/subtree with root ptr
		void printInorder(Inode* ptr);			//Traverse (inroder) and print the key of a tree/subtree with root ptr
		int height(Inode *ptr);				    //Find the height(MaxDepth) of a tree/subtree with root ptr
		void remove(Inode *ptr,int key);		//Remove a Inode with key from the tree/subtree with root
		void mkdir(string parameter, Inode *current); // Makes a folder
		void touch(string parameter, string second_parameter, Inode *current); // Creates a file
		Inode* changeDirectory(string parameter, Inode* &current); // Changes the directory
		void realpath(string parameter, Inode *current); // prints realpath
		void find(string parameter, Inode* current); // Finds the node
		void ls_sort(Inode *current); // Prints the sorted list
		void changePathDirectory(string parameter, Inode* current); // chnages the path directory
		int size(Inode* current); // returns the size of the node passed
		void saveData(Inode* current, ofstream &newFile); // saves data (dump)
		void recover(Inode* current); // recovers the file from the bin
		void move(string parameter, string second_parameter, Inode* current); // moves the file to the folder
		int sizeDirectory(string paramter,  Inode *current); // size of directory
		void loadData(string path); // Loads the data from the vfs file 
		Inode* findNode(string parameter, Inode* current); // finds the node 
		void Mkdir(string current_directory, int size, string file_date, Inode* current); // Makes the folder with the specified arguments
		void Touch(string file_name,int size,string  date , Inode* current);// Makes the file with the specified arguments

};

VFS::VFS(){
	this->root= new Inode("",0,getCurrentDate(), false); // sets the root as with size 0 and current date
}
VFS::~VFS(){ // Destructor
	delete root;
}
Inode* VFS::getRoot(){ // gets the root
	return root;
}

class Queue
{
	public: 
		int capacity_size = 0;
		NodeList list;
		void add(Inode* node);
		void remove();
		bool isEmpty();
		void display();
		void recovery();
		Inode* front();
};

bool Queue::isEmpty()
{
	if (capacity_size==0){
		return true;
	}else{
		return false;
	}
}

void Queue::remove()
{
	if (capacity_size==0){ // if size is 0
		cout << "Recycle Bin is empty!!" << endl;
	}else{
		list.removeFront(); 
		capacity_size--; // decrese the size
	}
}

void Queue::add(Inode* elem){
	if (capacity_size > 10){ // if its more then 10
		cout << "Recycle Bin is Full!!" << endl;
	}else{
		capacity_size++; // increase the size
		list.addBack(elem); // add the element to the queue
	}
}

void Queue::display(){
	if(capacity_size == 0){
		cout << "The recycle bin is empty" << endl;
	}else{
		cout<< list.front()->getName() << " " << list.front()->getSize() << " " << list.front()->getDate() << endl;
	}
}

Inode* Queue::front(){
	if (capacity_size == 0){
		cout << "Recycle Bin is empty!!" << endl; 
	}else{
		return list.front(); // returns tbe front from the queue
	}
	return nullptr;
}

void Queue::recovery(){
	if (capacity_size==0){ // if its empty 
	cout << "Recycle Bin is empty!!" << endl;
	}else{
		Inode* ptr = list.head->next; // set the head to next

		Inode* temp = list.head->next; // temp node  assigned as head
		Inode* temp2 = temp; 

	while(temp2 && temp2->Parent != NULL){ // of its the next is null of the parent of temp2 is NULL
		temp2->Parent->size -= temp->size; // decrease the size
		temp2 = temp2->Parent;
	}
		ptr->prev->next = ptr->next; // 
		ptr->next->prev = ptr->prev;
		capacity_size--;
	}
}

Queue recycleBin;

class CStack // CStack class
{
public: 
CStack(); // Construtor
CStack(int cap); // Construtor
~CStack();	// destrutor
int size() const; // size method
bool empty() const; // empty method
const string& top() const; // get the top stringlement
void push(const string& e);  // push method
void pop(); // ...housekeeping functions omitted
private: 
	string* array; // declaring an array
	int capacity; // capacity variable
	int index; 
}; 
CStack::CStack(){
	int cap = 10000;
	index = -1;
	this->array = new string[cap]; 
}
CStack::~CStack(){
	delete[] array; // destructor to delete the array
}
int CStack::size() const 
{
	return (index+1); // incrementing the size
}
bool CStack::empty() const 
{
	return (index<0); // if the size is less then 0
}

const string& CStack::top() const
{
	if (empty()) {
		throw runtime_error("Top of empty stack");	// throw exception
	}else{
		return array[index];
	}
}
void CStack::push(const string& e) 
{
	if (size() == capacity){ // if size is same as capacity throw exceptiom
		throw runtime_error("Push to full stack");	// throw exception
	}else{
		array[++index] = e; // Increment the counter
	}
}
void CStack::pop() // removing the last element fromt the stack
{
	if (empty()){ 
		throw runtime_error("Pop from empty stack"); // throw exception
	}
	else{
		--index; // Decleremnt the counter
	}
}

template <typename T>
class MyVector
{
	private:
		T *data;						//pointer to int(array) to store elements
		int v_size;						//current size of vector (number of elements in vector)
		int v_capacity;					//capacity of vector
	public:
		MyVector();						//No argument constructor
		MyVector(int cap);				//One Argument Constructor
		~MyVector();					//Destructor
		void push_back(T element);		//Add an element at the end of vector
		void insert(int index, T element);
		T& at(int index); 				//return reference to the element at index
		const T& front();				//Returns reference to the first element in the vector
		const T& back();				//Returns reference to the Last element in the vector
		int size() const;				//Return current size of vector
		int capacity() const;			//Return capacity of vector
		bool empty() const; 			//Rturn true if the vector is empty, False otherwise
};

template <typename T>
MyVector<T>::MyVector(){ // empty constructor
	this->v_capacity = 0;
	this->v_size = 0;
	this->data = new T[v_capacity]; // creaitng a new array
}

template <typename T>
MyVector<T>::MyVector(int cap){	 
	this->v_capacity = cap; // 1 argument constructor.
	this->v_size = 0;
	this->data = new T[v_capacity];
}

template <typename T>
MyVector<T>::~MyVector(){	// destructor
	delete[] data;
}

template <typename T>
void MyVector<T>::push_back(T element){
{
	if (v_size == v_capacity)
	{
		T* data1; //creating a new pointer
		int new_size = max(1, 2 * v_size); // accounting for when the size of the vector is zero
		data1 = new T[new_size];
		// copying all the elements from the old vector to the new one
		for (int i = 0; i < v_size; i++)
		{
			data1[i] = data[i];
		}
		if (data != NULL) { // deleting the current array
			delete[] data;
			data = data1; // setting the previous array to the current one
			v_capacity = new_size; // updating the size
		}
	}
	data[v_size++] = element;
	}
}

template <typename T>
void MyVector<T>::insert(int index, T element){
	if (v_size == v_capacity){
		if (v_capacity==0){ // if its empty grow capacity
			v_capacity = v_capacity+1;
		}else{
			v_capacity = v_capacity*2; 
		}
	}
	if (index>v_size){//if index is greater them the size (not v_size-1 beacause we should be able to add element one index back
		throw runtime_error("Out of Range");
	}else{
		int temp_index = v_size; // setting temp varriable to the size
		for (temp_index; temp_index > index; temp_index--){ // iterating throught the array in the resverrse
			data[temp_index] = data[temp_index-1]; // moving the element behind to the back
		}
		data[temp_index] = element; // assigning the element at the index
		v_size++; // increasing the size
	}
}

template <typename T>
T& MyVector<T>::at(int index){ 
	if (index>v_size-1){ // throwing exception if the index is out of range
		throw runtime_error("Out of range");
	}else{
		return data[index];	// returning the element at the particilar index
	}
}

template <typename T>
int MyVector<T>::size() const{
	return v_size; // returning the size
}

template <typename T>
int MyVector<T>::capacity() const{
	return v_capacity; // returning the capacity
}

template <typename T>
bool MyVector<T>::empty() const{ 
	if (size() == 0){ // if the size returns 0
		return true; // its empty
	}
	else 
		return false; // not empty
}

template <typename T>
const T& MyVector<T>::front(){ 
	return data[0]; // returning the first element 

}

template <typename T>
const T& MyVector<T>::back(){
	return data[v_size-1]; // returning the element at the last index
}

VFS virutalFileSystem; // instance of the vfs tree
Inode* current = virutalFileSystem.getRoot(); // set currrent and pre node as root
Inode* prevDir = virutalFileSystem.getRoot();

string pwd(Inode* current){ // prints the current working directory
	Inode* tmpCur = current; // sets tmpCur as current
	if (tmpCur) 
	{
		CStack stackpath; // create an instance of the stack
		string path = "/"; // prints the first "/" at the beginning 

		if (tmpCur->getName() == ""){ // if its root
			path = "//"; // set it as "//" since later the last "/" is removed
		}
		while(tmpCur->Parent != NULL){ // if its parent is not null
			stackpath.push(tmpCur->getName()); // push the name to the stack
			tmpCur = tmpCur->Parent; // set the current as the parent 
		}
		
		while(!stackpath.empty()){ // while its not empty
			path += stackpath.top() + "/"; // add the name to the path and concatenate the slash
			stackpath.pop(); // pop it from the stack
		}
		path = path.substr(0,path.length()-1); // remove the last slash 
		return path; // rerturn the path 
	}
	return "/"; // else the slash is printed
}

void VFS::realpath(string parameter, Inode *current) { // prints the realpath 
	string real_path = pwd(current); // gets the path from pwd
	real_path += parameter; // add the file name 
	cout<< real_path << endl; //print the real path 
}

void ls(Inode *current) {
	current->children->showChildren(); // shows all the current children
}

void VFS::ls_sort(Inode *current){
	NodeList* list = current->children; 
	for (auto first: *list){ // Using auto to iterate through the list 
		for (auto second: *list){ // Using auto to iterate through the list
			if (second == list->tail){ // till its the tail of the list 
				break;
			}
			if (second->getSize() < second->next->getSize()){ // if the size of the second is less then the first 
				Inode* ptr = second->next; // performing bubble sort
				second->prev->next = second->next;
				second->next = second->next->next;
				ptr->next = second;
				second->next->prev = second;
				ptr->prev = second->prev;
				second->prev = ptr;
			}
		}
	}
	current->children->showChildren(); // print the show children
}

void VFS::mkdir(string parameter, Inode *current) {
	for(char c: parameter){ // for each character in parameter
		if (!isalnum(c)){ // if its not alpha numeric 
			throw runtime_error("Invalid Folder Name!! Please ensure name is alphanumeric");
		}
	}
	if (parameter.find('.') != -1){// if it has a . foler name is invalid 
		throw runtime_error("Invalid Folder Name!! Please try again with a different name");
	}
	for(auto j: *current->children){ // Using auto to iterate through the current's children list
		if (j->getName() == parameter){ // if there is already a folder with that name 
			throw runtime_error("Folder already exist!! Please try again with a different name");
		}
	}
		Inode* new_folder = new Inode(parameter,10, getCurrentDate(), false); // make a new folder
		new_folder->Parent = current;
		current->children->addBack(new_folder); // add the folder in the list
		cout << "Folder named " << parameter << " has been created" << endl;
}

void VFS::touch(string parameter, string second_parameter, Inode *current) {
	for(char c: parameter){
		if (!isalnum(c) && c!='.'){
			throw runtime_error("Invalid Folder Name!! Please ensure name is alphanumeric");
		}
	}
	if (stoi(second_parameter) < 0){
		throw runtime_error("Invalid File size!! Please enter a valid file size");
	}
	if (parameter.find('.') == -1){
		throw runtime_error("Invalid File Name!! Please try again with a different name (include .extension )");
	}
	for(auto j: *current->children){ // Using auto to iterate through the current's children list
		if (j->getName() == parameter){
			throw runtime_error("File already exist!! Please try again with a different name");
		}
	}
	Inode* new_file = new Inode(parameter, stoi(second_parameter), getCurrentDate(), true);
	new_file->Parent = current;	
	current->children->addBack(new_file);
	cout << "File named " << parameter << " has been created" << endl;
}

void VFS::Mkdir(string current_directory, int size, string file_date, Inode* current){
	Inode* new_folder = new Inode(current_directory, size , file_date, false);
	new_folder->Parent = current;
	current->children->addBack(new_folder);
}

void VFS::Touch(string file_name,int size,string  date , Inode* current){
		for(char c: file_name){
		if (!isalnum(c) && c!='.'){
			throw runtime_error("Invalid Folder Name!! Please ensure name is alphanumeric");
		}
	}
	if (size < 0){
		throw runtime_error("Invalid File size!! Please enter a valid file size");
	}
	if (file_name.find('.') == -1){
		throw runtime_error("Invalid File Name!! Please try again with a different name (include .extension )");
	}
	for(auto j: *current->children){ // Using auto to iterate through the current's children list
		if (j->getName() == file_name){
			throw runtime_error("File already exist!! Please try again with a different name");
		}
	}
	Inode* new_file = new Inode(file_name, size , date, true);
	new_file->Parent = current;
	current->children->addBack(new_file);
}

Inode* VFS::changeDirectory( string parameter, Inode* &current){
	Inode* Trackercurrent = current; // set the tracker node to the current one 
	if (parameter == "cd" || parameter == " "|| parameter == ""){ // if its cd or empty or nothing 
		prevDir = current;
		current = virutalFileSystem.getRoot(); // set the current to point to root
	}
	else if(parameter == ".."){ // if its .. 
		if (current->Parent == NULL){ // if no parent then we are at root
			cout << "You are already at the root! Can not go up" << endl;
		}else{ // set current as parent and previous and current
			prevDir = current;
			current = current->Parent;
		}
	}
	else if (parameter == "-"){ // if its -
		Inode* temp = current; //set temp as current 
		current = prevDir;
		prevDir = temp; // keep copy of previous 
		cout << pwd(current) << endl; // prints the directory
	}
	else if (parameter[0]=='/'){ // if a path is specified 
		prevDir = current; // set previous as current
		current = virutalFileSystem.getRoot(); // 
		string target;
		string path = parameter;
		while(path.find('/') != -1){ // if there is a "/"
			path  = path.substr(path.find("/")+1); // split the line into each path found after the slash
			target = path.substr(0,path.find("/")); 
			prevDir = current;
			current = changeDirectory(target, current); // set current directory to the taget and passed current 
		}
	}
	else{
		Inode* currentDirectory = current->children->findchild(parameter); //set current as the directory of findchild of the currents children
		if (currentDirectory == NULL){ // if the current points to NULL
			cout << "There is no Directory" << endl;
			// return nullptr;
		}else if (currentDirectory->isFile){ // If its file
			cout << "Sorry! Unable to change Directory of a file" << endl;
		}else if (!currentDirectory->isFile){
			prevDir = current;
			current = currentDirectory;
		}
	}
	return current; 
}

void VFS::find(string parameter, Inode* current){
	if (current->getName() == parameter){ // check if file/folder name is equal to the parameter passed
		cout<<pwd(current) << endl; // print the current working directory
	}
	else if (!current->isFile){ // if its not a file
		for (auto trav: *current->children){ // Using auto to iterate through the current's children list
			find(parameter, trav); // call the find recursively method which will print the path if its found
		}
	}
}

Inode* VFS::findNode(string parameter, Inode* current){ // method which finds the specified node by its name
	if (current->getName() == parameter){ // check if file/folder name is equal to the parameter passed
		return current; 
	}
	if (!current->isFile){ // if its not a file
		for (auto trav: *current->children){ // Using auto to iterate through the current's children list
			find(parameter, trav); // call the find method which will print the path if its found
		}
	}
	return current;
}

Inode* NodeList::checkChild(string parameter){
	for (auto i: *this){ // Using auto to iterate through the list
		if (i->getName() == parameter){ // check if file/folder name is equal to the parameter passed
			return i;
		}
	}
	throw runtime_error("No children with the provided name found");
}

bool NodeList::checkIfChild(string parameter){
	for (auto i: *this){ // Using auto to iterate through the list
		if (i->getName() == parameter){ // check if file/folder name is equal to the parameter passed
			return true;
		}
	}
	return false;
}

void NodeList::skipOver(Inode* current){ // method that skips over the current node which is passed
	current->prev->next = current->next;
	current->next->prev = current->prev;
} 

void VFS::move(string parameter, string second_parameter, Inode* current){ // moves the file to the specified destination
	Inode* temp1;
	Inode* temp2; 
	temp1 = current->children->checkChild(parameter); // check child which will return the pointer to that folder name passed as parameter
	if (!temp1->isFile){ //if its not filr
		throw runtime_error("Sorry it is not a file!! Please provide a valid filename");
	}
	temp2 = current->children->checkChild(second_parameter); // check if its a folder
	if (temp2->isFile){
		throw runtime_error("Sorry it is not a folder!! Please provide a valid foldername");
	}
	current->children->skipOver(temp1); // skip over the pointer to temp1
	temp2->children->addBack(temp1); // add temp1 as a children under temp2 folder
	temp1->Parent = temp2; // set temp1's parent as temp2
}

int VFS::sizeDirectory(string parameter, Inode *current){ 
	if(parameter == "") { // if its nothign return the root
		return size(current);
	}
 	else if(parameter == "/") { // if its "/" return the root
		return size(root); 
	}
	else return size( current->children->checkChild(parameter)); // 
}

int VFS::size(Inode* current){ //size method that recursively calls the function  
	int real_size = current->getSize(); // 
	if (current->isFile){ // if its a file
		return current->getSize(); // return the files size
	}else{
		for (auto i: *current->children){ // for each children in the list
			real_size += size(i); // update the size variable by addding 
		}
	return real_size;
	}
}
void emptybin() { // empties the bin by calling the remove till its empty
	while(!recycleBin.isEmpty()){ 
		recycleBin.remove();
	}
	cout << "Bin has been emptied out!!" << endl;
}

void showbin() { // Shows the bin by calling its display method which prints the first file/folder
	if(recycleBin.capacity_size == 0){ // if its empty
		cout << "Bin is empty!!" << endl;
	}else{
		cout << "Next element to remove is :: " ;
		recycleBin.display();
		
	}
}

void VFS::recover(Inode* current) { // recovers the file from the bin
	Inode* recoveryNode;
	if (!recycleBin.capacity_size==0){ // if the bin is not empty
		recoveryNode = recycleBin.front(); // get the pointer to the front of the queue
		recycleBin.recovery(); // call recovery method
		recoveryNode->Parent->children->addBack(recoveryNode); // add the file back to where is was 
		cout << "Succesfully recovered "<< recoveryNode->getName() << endl;
	}else{
		cout << "Nothing to recover!! Bin is empty" << endl;
	}
}

void VFS::saveData(Inode* current, ofstream &newFile){ // Dumps data back to the file which was passed as address
	newFile  << pwd(current) << "," << current->getSize() << "," << current->getDate() << endl; // " calls the pwd method"
	if (!current->isFile) { // if its not a fole go inside the folder
		for (auto trav: *current->children){ // iterates through all the children 
			saveData(trav, newFile); // recursively called the save data method
		}
	}
}

bool checkFile(string name){// Checks if its a file and returs a bool value
	for(char c: name){
		if (c=='.'){
			return true;
		}		
	}
	return false;
}

// Loads data from the file path
void VFS::loadData(string path){
	ifstream myFileStream(path); // Opening the file from user path
	string line, rootline;
	string file_name, file_date, full_line, remainder, file_size;
	string prev_parent;
	int size;

	myFileStream >> rootline; // Reads the first line
	Inode* prevParent_node; // defining a preParent node
	 
	if (myFileStream){ // if there is such a file
		while(myFileStream>>line){ // while there is a line (read it into myFIle stream)
			Inode* current = getRoot(); // set current as root
			MyVector<string> loadVector; // create a vector 
			int begin = 0; // set begin and end variables for splitting
			int end = 0;
			while((end = line.find('/', begin))!= string::npos){  // Find slash in the string and push it to the vector
				if (end != begin){ 
					loadVector.push_back(line.substr(begin, end - begin));
				}
				begin = end +1; // start searching after the slash thus increment it 
			}
			if (end!= begin){ // while there is a last file/folder left, push it as well
				loadVector.push_back(line.substr(begin));
			}

			if (loadVector.size() <= 0){ // if vector is empty, there was nothing thus return
				return;
			}
			if (loadVector.size() ==1){ // if there is only 1 element 
				full_line = loadVector.at(0); // set the full line as the string at index 0
				file_name = full_line.substr(0,full_line.find(',')); // find a ',' and save it 
				bool is_file = checkFile(file_name); // check if the file is there and retuns a bool value
				remainder = full_line.substr(full_line.find(',')+1); // rest of the string
				file_size = remainder.substr(0, remainder.find(',')); // find a ',' and save it
				size = stoi(file_size);// type cast it to an int
				file_date = remainder.substr(remainder.find(',')+1); // find a ',' and save it
				if (is_file){ // if its file make it 
					Touch(file_name, size, file_date , current);
				} else{ // if its folder make it 
					Mkdir(file_name, size, file_date, current);
				}
			}
			else{ // multiple sub folders 
				Inode* prevParent_node = current;
				string newFileName; 

				for (int i  =0; i<loadVector.size()-1; i++){ // till we reach the last value in the vector, iterate through the vector
					newFileName = loadVector.at(i); // set the full line as the string at index 0
					if (prevParent_node->children->checkIfChild(newFileName)){ // if there is a child as per the specified name
						prevParent_node = prevParent_node->children->checkChild(newFileName); // set the node to the pointer of that file location
					}else{
						if (prev_parent.find('.') != -1){ // if there is a '.' make a file
							Touch(prev_parent, size, file_date , prevParent_node);
						} 
						else{ // make a folder
							Mkdir(newFileName, size, file_date, prevParent_node); 
							prevParent_node = prevParent_node->children->checkChild(newFileName); // set the node to the pointer of that file location
						}
					}
				}
				// Same as before
				full_line = loadVector.at(loadVector.size()-1);
				file_name = full_line.substr(0,full_line.find(','));
				bool is_file = checkFile(file_name);
				remainder = full_line.substr(full_line.find(',')+1);
				file_size = remainder.substr(0, remainder.find(','));
				size = stoi(file_size);
				file_date = remainder.substr(remainder.find(',')+1);
				if (is_file){ // it its file make it
					Touch(file_name, size, file_date , prevParent_node);
				} 
				else{ // make folder
					Mkdir(file_name, size, file_date, prevParent_node); 
				}
			}
		}
	}else{
		cout << "File does not exist!! Directory starting from root node" << endl;
	}
}

int main()
{
	listCommands(); // calls the command menu display method
	string path = "vfs.dat";
	virutalFileSystem.loadData(path); // Calling the load data mehtod that loads the 
	
	string user_input;
	string command,  parameter, second_parameter; 
	NodeList myList; // creating instance of node list

	while(true) // Runs repetitively till exit is called
	{
		cout<<">";
		try{
			getline(cin,user_input); //Gets Jess get to get the user line as input
			// This basically splits the input into differnet words by finding spaces
			command = user_input.substr(0,user_input.find(" ")); 
			parameter = user_input.substr(user_input.find(" ")+1);
			second_parameter = parameter.substr(parameter.find(" ")+1);
			parameter = parameter.substr(0,parameter.find(" "));

			if (user_input.find(" ") == -1){ // Find space in user input and sets parameter and second parameter as empty
				parameter ="";
				second_parameter = ""; 
			}		
			if( command =="help" or command=="Help")	listCommands(); //Prints the load command  menu
			else if( command =="pwd")	cout << pwd(current) << endl; //Print the current working directory
			else if( command =="realpath" or command=="r")	virutalFileSystem.realpath(parameter, current); //prints the real part of the specified parameter
			else if( command =="ls")	ls(current); //This all the files in the folder
			else if( command =="ls_sort")	virutalFileSystem.ls_sort(current); //sorts the file in the folder according to the sizes
			else if( command =="mkdir" or command=="m")	virutalFileSystem.mkdir(parameter, current);//Create a folder
			else if( command =="touch" or command=="t")	virutalFileSystem.touch(parameter, second_parameter,current); // create a file
			else if(command == "cd")  virutalFileSystem.changeDirectory(parameter, current);// changes this current directory 
			else if( command =="find")	virutalFileSystem.find(parameter, virutalFileSystem.getRoot()); // find the file
			else if( command =="mv")	virutalFileSystem.move(parameter, second_parameter, current); // moves the file the specified destination
			else if( command =="rm"){ 
				if (parameter == "" || parameter == " "){ // if its blank space or smth else
					cout << "Please specify a folder you wish to remove!!" << endl;
				}else{
					recycleBin.add(current->children->remove(parameter)); //removes the parameter file and adds it to the bi
				}
			} 
			else if (command == "size") {
					cout << virutalFileSystem.sizeDirectory(parameter, current) << " bytes" <<endl; //displaus the size of the specific parameter passed
			}
			else if (command == "emptybin") emptybin(); // basically empties the file
			else if (command == "showbin") showbin(); // shows the first file from the bin
			else if (command == "recover") virutalFileSystem.recover(current);// call the recovery method that brings the file back from the bin
			else if( command == "Exit" or command=="exit") break; 
			else cout<<"Invalid command !!!"<<endl;
		}
		catch(exception &e)
		{
			cout<<"Exception: Please try again " << endl <<e.what()<<endl; //throws exception for unusual errors
		}
	}
	ofstream newFile("vfs.dat"); // opens the file and overwrites all the data into it
	virutalFileSystem.saveData(virutalFileSystem.getRoot(), newFile);  // call the save data method in virtual file system tree that dumps the data into the VFS file
	newFile.close(); // close the file before exiting
	return 0;
}
