#include <iostream>
#include <fstream>
#include <string>
//Including algorithm and cc type for functions using inside of the readPinFromFile() function
#include <algorithm> // For all_of
#include <cctype> // For isdigit


using namespace std;

// Function to display the menu
void displayMenu()
{
    // Then display the menu options after the console is cleared
    cout << "Please select from the following options:" << endl;
    cout << "\n1. Create a blank text file" << endl;
    cout << "\n2. Write a 4 digit pin to a .txt file" << endl;
    cout << "\n3. Reverse the read PIN and write to a .bin file" << endl;
    cout << "\n4. Read a 4-digit PIN from a .txt or .bin file" << endl;
    cout << "\n5. Exit \n" << endl; // Return 0 to exit the program
    cout << "Please enter your choice here: "; // Prompt the user to enter their choice via the while/switch statements
}

// Ended up realizing a lot of the functions I made in lab 1 could be reused here so I just copied them over


// Function to check if a file exists
bool fileExists(const string& filename) { // Using a const reference to avoid copying the string
    ifstream file(filename);
    return file.good();
}

/* In theory I should have had this function exit the program since the requirements say to exit if the file doesn't exist
   but I decided to just return false instead since I didn't want to exit the program in the middle of the function
   and handling the error this way feels more user friendly/intuitive */ 
bool handleFileOpeningError(const string& fileName) { // Using a const reference to avoid copying the string
    // No clearing console here since this function is only called if there's an error
    char choice;
    if (!fileExists(fileName)) { // Here's where we check if the file exists or not
        // If the file doesn't exist we ask the user if they want to create it
        cout << "File '" << fileName << "' does not exist. Would you like to create it now? (y/n) ";
        cin >> choice; 

        if (choice == 'y' || choice == 'Y') {
            ofstream newFile(fileName);
            if (newFile.is_open()) {
                cout << "File '" << fileName << "' created successfully!" << endl;
                newFile.close();
                return true; // File was successfully created
            }
            else {
                cout << "The file was not created. Press enter to return to the main menu." << endl;
                cin.get();
                system("cls");
                return false; // File was not created
            }
        }
        else {
            // The user chose not to create the file.
            cout << "Returning to main menu. Press enter to continue..." << endl;
            cin.get();
            system("cls");
            return false; // File was not created
        }
    }
    else {
        cout << "Error opening file. Please try again." << endl;
    }
    return false; // File was not created
}

void createTxtFile() {
    system("cls"); // Clear the menu to make it look nicer for the user to keep relevant information on the screen
    string fileName;

    while (true) {
        cout << "Please enter the name of the file you would like to create (or type 'exit' to return to main menu): ";
        cin >> fileName; // User will enter the name of the file they want to create

        if (fileName == "exit") {
            cout << "Returning to main menu. Press enter to continue..." << endl;
            cin.get();
            system("cls");
            return; // Exit the function to return to the main menu incase user accidentally chose this option
        }

        fileName += ".txt"; // add the .txt extension to the file name the user chooses

        if (fileExists(fileName)) { // Check if the file already exists
            // If the file exist we loop back to the beginning to see if the user wants to try again or exit
            cout << "File already exists, please choose another name." << endl;
        }
        else {
            ofstream file(fileName);
            if (file.is_open()) {
                cout << "File created successfully!" << endl;
                file.close();
                break; // exit the while loop
            }
            else { // If there's an issue that isn't the file already existing
                cout << "Error creating file. Please try again." << endl;
            }
        }
    }
    cout << "Press enter to continue..." << endl;
    cin.get();
    /* Wait for user input to continue before we go back and clear the console
       this gives the user time to read the message at their own pace */
    system("cls"); // Clear the console to make it look nicer when looping back to the main menu
}

void writePinToFile() {
    system("cls");
    string fileName;
    string pin;
    int fileType;

    cout << "Which type of file would you like to write the PIN to?" << endl;
    cout << "1. Text file (.txt)" << endl;
    cout << "2. Binary file (.bin)" << endl;
    cout << "Please enter your choice: ";
    cin >> fileType;
    cin.ignore();  // Clear the newline

    if (fileType != 1 && fileType != 2) {
        cout << "Invalid choice. Returning to main menu." << endl;
        return;
    }

    cout << "Please enter the name of the file you would like to write the PIN to (without extension): ";
    cin >> fileName;
    cin.ignore();  // Clear out any remaining newline

    if (fileType == 1) {
        fileName += ".txt";
    }
    else {
        fileName += ".bin";
    }

    while (true) {
        cout << "Please enter a 4-digit PIN: ";
        cin >> pin;

        if (pin.size() == 4 && all_of(pin.begin(), pin.end(), ::isdigit)) {
            break;  // If we have a valid PIN we can exit the loop
        }
        else {
            cout << "Invalid PIN. Please enter a valid 4-digit PIN." << endl;
        }
    }

    ofstream file(fileName, (fileType == 2) ? ios::binary : ios::out);

    if (file.is_open()) {
        if (fileType == 1) {
            file << pin;
            cout << "PIN written to text file successfully." << endl;
        }
        else {
            file.write(pin.c_str(), 4);
            cout << "PIN written to binary file successfully." << endl;
        }
        file.close();
    }
    else {
        cout << "Error writing to the file." << endl;
    }
}

// New function to reverse the pin found in the txt file and write it to a binary file
void writeReversedPinToBinary() {
    system("cls"); // Clear the console for clean user interaction

    string originalFileName, pin;
    cout << "Please enter the name of the text file containing the PIN you would like to reverse (without extension): ";
    cin >> originalFileName;
    cin.ignore();  // Clear out any remaining newline

    originalFileName += ".txt";

    ifstream origFile(originalFileName);
    if (origFile.is_open()) {
        getline(origFile, pin);  // Read the PIN from the text file
        origFile.close();
    }
    else {
        cout << "Error reading the original text file." << endl;
        if (!handleFileOpeningError(originalFileName)) { // Check if file doesn't exist and handle accordingly
            return;  // If user doesn't create the file, just return
        }
    }

    if (pin.size() != 4 || !all_of(pin.begin(), pin.end(), ::isdigit)) {
        cout << "Invalid PIN in the text file." << endl;
        return;  // If PIN is invalid, return without doing the reversal
    }

    // Extract the base name from the original file name
    string baseFileName = originalFileName.substr(0, originalFileName.find_last_of("."));
    // Create the reversed filename based on the original name
    string reversedFileName = baseFileName + "_reversed.bin";

    // Reverse the pin
    string reversedPin = string(pin.rbegin(), pin.rend());

    ofstream binFile(reversedFileName, ios::binary);
    if (binFile.is_open()) {
        binFile.write(reversedPin.c_str(), 4);
        binFile.close();
        cout << "Reversed PIN written to " << reversedFileName << " successfully." << endl;
    }
    else {
        cout << "Error writing to reversed binary file." << endl;
    }
}

// Function to read a file (text or binary) and display the contents to the console
void readFromFile() {
    system("cls");
    string fileName;
    string line;
    char choice;
    bool isEmpty = true; // Assume the file is empty initially
    int fileType; // 1 for text, 2 for binary

    cout << "Which type of file would you like to read from?" << endl;
    cout << "1. Text file (.txt)" << endl;
    cout << "2. Binary file (.bin)" << endl;
    cout << "Please enter your choice: ";
    cin >> fileType;

    cout << "Please enter the name of the file you would like to read from (without extension): ";
    cin >> fileName;
    cin.ignore(); // Clear out any remaining newline

    if (fileType == 1) {
        fileName += ".txt";
    }
    else if (fileType == 2) {
        fileName += ".bin";
    }
    else {
        cout << "Invalid choice. Returning to main menu." << endl;
        return;
    }
    // Open the file for reading using the ? operator to determine if we're opening a text or binary file
    ifstream file(fileName, (fileType == 2) ? ios::binary : ios::in);

    if (file.is_open()) {
        if (fileType == 1) { // If we're reading a text file
            while (getline(file, line)) {
                cout << line << endl;
                isEmpty = false; // If we're here, then the file isn't empty
            }
        }
        else { // If we're reading a binary file
            char binContent[5] = { 0 }; // Assuming max size of binary content is 4 bytes + 1 for null terminator
            file.read(binContent, 4);
            if (file.gcount() > 0) { // gcount returns the number of bytes read
                isEmpty = false;
                cout << "Binary content (as string for display purposes): " << binContent << endl;
            }
        }
        file.close();

        if (isEmpty) {
            cout << "I'd like to display the contents but I don't see anything in this file." << endl;
        }
    }
    else if (!handleFileOpeningError(fileName)) {
        // If handleFileOpeningError returns false, it means there was an error in file handling that cannot be recovered from.
        return; // Exit the function.
    }

    cout << "Press enter to continue..." << endl;
    cin.get();
    system("cls");
}

int main() {
    string input;
    int choice;

    while (true) {
        displayMenu();
        getline(cin, input); // Read the entire line

        try {
            choice = stoi(input);

            switch (choice) {
            case 1:
                createTxtFile();
                break;
            case 2:
                writePinToFile();
                break;             
            case 3:
                writeReversedPinToBinary();
                break;
            case 4:
                readFromFile();
                break;
            case 5:
                cout << "Exiting program..." << endl;
                return 0;
            default:
                cout << "Hey! Last time I checked that wasn't a number between 1 and 5. Try again." << endl;
            }
        }
        /* I throw exceptions if the user enters something that isn't a number or if the number is too large I did
         this since entering in a char or string would cause an infinite loop since the input would never be a number */
        catch (const invalid_argument&) {
            // This exception will be thrown by stoi if conversion fails
            cout << "Hey that's not even a number! Maybe read the instructions again?" << endl;
        }
        catch (const out_of_range&) {
            // This exception will be thrown by stoi if the number is too large
            cout << "Hey! Last time I checked that wasn't a number between 1 and 5. Try again." << endl;
        }
    }

    return 0;
}