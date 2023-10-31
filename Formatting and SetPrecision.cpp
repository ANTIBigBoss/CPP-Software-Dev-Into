#include <iostream>
#include <fstream>
#include <string>
#include <iomanip> 
#include <sstream>

using namespace std;

class FileHandler {
private:
    string filename; // class to store the name of a file
    float value1, value2; // two floating point values

public:
    void getFileName();
    void getUserInput();
    void writeToFile();
    void readFromFile();
    void displayOutput();
    bool handleFileOpeningError(const string& fileName);
};

bool fileExists(const string& filename) {
    ifstream file(filename, ios::binary);
    return file.good();
}

void FileHandler::getFileName() {
    // Anything with more than 8 characters messes up table formatting when
    // we also have it displaying the .bin file name which is 4 characters
    cout << "Enter the file name (8 characters max without extension): ";
    cin >> filename;
    cin.ignore();

    /* |123456789012345| <- Only 8 characters per line this if statement
    ensures our file name doesn't exceed the limits of our table size */
    if (filename.size() > 8) {
        cout << "Filename too long. Ending the program now." << endl;
        // If invalid end the program
        exit(0);
    }
    // add the .bin extension similar to how we did .txt in the previous assignment
    filename += ".bin"; 
}

void FileHandler::getUserInput() {
    cout << "Enter the first value (float): ";
    cin >> value1;

    if (cin.fail()) {
        cout << "\nThe first float entered is an invalid float value. Ending the program now." << endl;
        exit(0);
    }

    cout << "Enter the second value (float): ";
    cin >> value2;

    if (cin.fail()) {
        cout << "\nThe second float entered is an invalid float value. Ending the program now." << endl;
        exit(0);
    }

    // This cin.ignore() ensures any extra characters (like a newline from pressing 'Enter') are removed.
    cin.ignore();
}


/* This function was the result of a lot of researching how reinterpret_cast works 
   so I'm going to break down the whole function step by step */
void FileHandler::writeToFile() {
    // Step 1: Check if the file already exists
    if (!fileExists(filename) && !handleFileOpeningError(filename)) {
        return;
    }

    // Step 2: Try to open the file to write data
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        // If there's an issue, tell the user and exit
        cout << "Error opening file. Ending the program." << endl;
        exit(0);
    }

     /* Step 3: Write value1/our first float to the file
     We use reinterpret_cast to "translate" our float value to a format
     that can be saved in a binary file. It's converting the memory
     representation of the float to characters that can be written to file.*/
    file.write(reinterpret_cast<char*>(&value1), sizeof(float));

    // Step 4: Write value2/float2 to the file
    file.write(reinterpret_cast<char*>(&value2), sizeof(float));

    // Step 5: Close the file after we're done
    file.close();
}

/* Similar to the writing to a file the read from file also uses reinterpret_cast
   so will also break this down step by step for clarity */
void FileHandler::readFromFile() {
    // Step 1: Check if the file exists
    if (!fileExists(filename) && !handleFileOpeningError(filename)) {
        return;
    }

    // Step 2: Try to open the file to read data
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        // If there's an issue, tell the user and exit
        cout << "Error opening file. Ending the program." << endl;
        exit(0);
    }

    /* Step 3: Read value1/float 1 from the file.

    To read bin file data we need to specify where in our program's memory 
    we want that data to be stored. We're storing it directly into our 
    float variables. The & gets the memory address of value1 so that the 
    file.read() function knows where to put the data it reads.

    However, file.read() expects a pointer to char (bytes), not float.
    So, reinterpret_cast<char*> tells the compiler, "Hey, treat the
    memory address of this float as if it were a char pointer." This
    ensures we're reading raw byte data correctly into our float variable. */
    file.read(reinterpret_cast<char*>(&value1), sizeof(float));

    // Step 4: Similar process for value2/float2
    file.read(reinterpret_cast<char*>(&value2), sizeof(float));


    // Step 5: Close the file after we're done
    file.close();
}

// Read and output the contents of the file to the console
void FileHandler::displayOutput() {
    cout << "+---------------+---------------+---------------+" << endl;
    cout << "|   File Name   |   Value One   |   Value Two   |" << endl;
    cout << "+---------------+---------------+---------------+" << endl;

    stringstream ss; // Use a stringstream to format value2
    ss << setprecision(10) << value2;
    string value2Str = ss.str();
    if (value2Str.size() > 15) {
        // Trim value2 string to 15 characters if it exceeds
        value2Str = value2Str.substr(0, 15); 
    }

    cout << "| " << setw(13) << left << filename.substr(0, 15)
        << " | " << setw(13) << fixed << setprecision(2) << value1
        << " | " << setw(13) << left << value2Str // Use the trimmed string for value2
        << " |" << endl;
    cout << "+---------------+---------------+---------------+" << endl;
}


bool FileHandler::handleFileOpeningError(const string& fileName) {
    char choice;
    if (!fileExists(fileName)) {
        cout << "File '" << fileName << "' does not exist. Would you like to create it now? (y/n) ";
        cin >> choice;
        cin.ignore();

        if (choice == 'y' || choice == 'Y') {
            ofstream newFile(fileName, ios::binary);
            if (newFile.is_open()) {
                cout << "File '" << fileName << "' created successfully!" << endl;
                newFile.close();
                return true;
            }
            else {
                cout << "The file was not created. Returning to main menu." << endl;
                return false;
            }
        }
        else {
            cout << "Returning to main menu. Press enter to continue..." << endl;
            cin.get();
            return false;
        }
    }
    return false;
}

int main() {
    FileHandler fileHandler;
    int choice;

    while (true) {
        cout << "1. Enter data and save to binary file" << endl;
        cout << "2. Read from binary file and display" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            fileHandler.getFileName();
            fileHandler.getUserInput();
            fileHandler.writeToFile();
            break;
        case 2:
            fileHandler.getFileName();
            fileHandler.readFromFile();
            fileHandler.displayOutput();
            break;
        case 3:
            exit(0);
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }

    return 0;
}