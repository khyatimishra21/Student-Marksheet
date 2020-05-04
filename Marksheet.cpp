// Marksheet.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include<iostream>
#include<fstream>
#include<cstring>
#include<math.h>
#include<iomanip>
using namespace std;
const int MAX = 20;
class Student
{
    int rollno;
    char name[20];
    int marks_maths, marks_english, marks_science;
public:
    Student()    //constructor
    {
        strcpy_s(name, "");
        rollno = marks_maths = marks_english = marks_science = 0;
    }
    Student(int rollno, char name[], int marks_maths, int marks_english, int marks_science)   //constructor
    {
        strcpy_s(this->name, name);
        this->rollno = rollno;
        this->marks_maths = marks_maths;
        this->marks_english = marks_english;
        this->marks_science = marks_science;
    }
    int getRollNo()    //fucntion to get roll no
    {
        return rollno;
    }
    void displayRecord()    //function to display record
    {

       cout << endl <<"Roll number: " <<rollno << endl << "Name: " << name << endl << "Mathematics: " << marks_maths << endl << "English: " << marks_english << endl << "Science: " << marks_science << endl << "Total Marks: " << marks_maths+marks_english+marks_science;
    }
    void getdata(int a, char str[], int m_m, int m_e, int m_s)    //function to get data
    {
        rollno = a;
        strcpy_s(name, str);
        marks_maths = m_m;
        marks_english = m_e;
        marks_science = m_s;

    }
};
//==========File Operations ===========
class FileOperations
{
    fstream file;
    public:
    FileOperations(char* filename)
    {
        file.open(filename, ios::in | ios::out | ios::ate | ios::binary);
    }
    
    //function to insert record
    void insertRecord(int rollno, char name[], int marks_maths, int marks_english, int marks_science)    
    {
        Student s1(rollno, name, marks_maths, marks_english, marks_science);
        file.seekp(0, ios::end);
        file.write((char*)&s1, sizeof(Student));
        file.clear();
    }
    //function to display all record
    void displayAll()
    {
        Student s1;
        file.seekg(0, ios::beg);
        while (file.read((char*)&s1, sizeof(Student)))
        {
            s1.displayRecord();
        }
        file.clear();
    }
    //function to display record based on roll no
    void displayRecord(int rollNo)
    {
        Student s1;
        file.seekg(0, ios::beg);
        bool flag = false;
        while (file.read((char*)&s1, sizeof(Student)))
        {
            if (s1.getRollNo() == rollNo)
            {
                s1.displayRecord();
                flag = true;
                break;
            }
        }
        if (flag == false)
        {
            cout << "\nRecord of " << rollNo << "is not present.";
        }
        file.clear();
    }
    //function to delete record
    void deleteRecord(int rollno)  
    {
        ofstream outFile("new.dat", ios::binary);
        file.seekg(0, ios::beg);
        bool flag = false;
        Student s1;

        while (file.read((char*)&s1, sizeof(Student)))
        {
            if (s1.getRollNo() == rollno)
            {
                flag = true;
                continue;
            }
            outFile.write((char*)&s1, sizeof(Student));
        }
        if (!flag)
        {
            cout << "\nRecord of " << rollno << " is not present.";
        }
        file.close();
        outFile.close();
        remove("student.dat");
        rename("new.dat", "student.dat");
        file.open("student.dat", ios::in | ios::out | ios::ate | ios::binary);
    }
    //function to update record
    void update(int rollNo, int m_maths, int m_english, int m_science, char str[])
    {
        file.open("student.dat", ios::in | ios::out | ios::ate | ios::binary);
        int pos;
        file.seekg(0, ios::beg);
        Student s1;
        bool flag = false;
        while (!file.eof())
        {
            pos = file.tellg();
            while(file.read((char*)&s1, sizeof(Student)));
            {
                if (s1.getRollNo() == rollNo) 
                {
                    flag = true;
                    s1.getdata(rollNo, str, m_maths, m_english, m_science);
                    file.seekp(pos);
                    file.write((char*)&s1, sizeof(Student));
                    s1.displayRecord();
                    break;
                }

            }
        }
            if (flag == false)
            {
                cout << "\nRecord of " << rollNo << "is not present.";
            }
            else {
                cout << "\nRecord Modified";
            }
            file.clear();
        
    }
    ~FileOperations()
    {
        file.close();
        cout << "\nFile Closed.";
    }
};
int main() {
    ofstream newFile("student.dat", ios::app | ios::binary);
    newFile.close();
    FileOperations file((char*)"student.dat");
    int rollNo, marks_maths, marks_english, marks_science, choice = 0;
    char name[MAX];
    while (choice != 5)
    {
        //clrscr();
        cout << "\n*****Student Marksheet*****\n";
        cout << "1) Add New Record\n";
        cout << "2) View\n";
        cout << "3) Deleting a Record\n";
        cout << "4) Update\n";
        cout << "Choose your choice : ";
        cin >> choice;
        int a;
        switch (choice)
        {
        case 1: //New Record
            cout << endl << "Enter RollNo : ";
            cin >> rollNo;

            cout << endl << "Enter Name : ";
            cin >> name;
            cout << "Enter marks ( Mathematics, English, Science ) : \n";
            cin >> marks_maths >> marks_english >> marks_science;
            file.insertRecord(rollNo, name, marks_maths, marks_english, marks_science);
            cout << "\nRecord Inserted.";
             break;
        case 2: //display record
                char s;
                cout << "Do you want to enter a query (y, n) :";
                cin >> s;
                if (s == 'y' || s == 'Y')
                {
                    cout << "Enter Roll Number";
                    cin >> rollNo;
                    file.displayRecord(rollNo);
                }
                else if (s == 'n' || s == 'N')
                {
                    file.displayAll();
                }
                else {
                    cout << "Invalid choice.";
                }
            break; 
        case 3:  //delete record
            cout << "Enter rollNo";
            cin >> rollNo;
            file.deleteRecord(rollNo);
            break;
        case 4:  //update record
            cout << "Enter the roll number of the student whose record you want to update : ";
            cin >> rollNo;
            cout << "Enter the details in the given order (Rollno, Name, marks_maths, marks_english, marks_science )";
            cin >> rollNo >> name >> marks_maths >> marks_english >> marks_science;
            file.update(rollNo, marks_maths, marks_english, marks_science, name);
            break;

        default: 
            cout << "Invalid choice";
        }
        return 0;
    }
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu;

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
