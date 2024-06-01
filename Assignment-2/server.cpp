#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "structs.cpp"

using namespace std;

void handleClient(int client_socket);

vector<Student> studentsData;
vector<Teacher> teachersData;

int main(){

    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;

    socklen_t client_address_len = sizeof(client_address);
    pid_t pid;

    // Create the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Configure server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(3001);

    // Bind the socket to the specified IP and port
    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 2 ) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    cout<<"Server started. Listening on port 3001\n\n";
    while (1) {

        // Accept incoming connection
        client_socket = accept(server_socket, (struct sockaddr *) &client_address, &client_address_len);
        if (client_socket == -1){
            perror("Accept failed");
            continue;
        }

        // Fork a new process to handle the client
        pid = fork();
        if (pid == -1){
            perror("Fork failed");
            close(client_socket);
            continue;
        }
        else if (pid == 0){ // Child process
            close(server_socket); // Close the server socket in child process
            handleClient(client_socket);
            close(client_socket);
            exit(EXIT_SUCCESS);
        }
        else{ // Parent process
            close(client_socket); // Close the client socket in parent process
            // Clean up terminated child processes to avoid zombie processes
            while (waitpid(-1, NULL, WNOHANG) > 0);
        }
    }
    // Close the server socket
    close(server_socket);
    return 0;
}

void handleClient(int clientSocket){

    char buffer[1024] = {0};
    //Loading Database
    fstream studentsFile("students.csv", ios::in);
    fstream teachersFile("teachers.csv", ios::in);

    string line;
    // Read and store students data
    while (getline(studentsFile, line)){

        Student student;
        stringstream ss(line);
        string name, roll, grade, attendance;
        getline(ss, name, ',');
        getline(ss, roll, ',');
        getline(ss, grade, ',');
        getline(ss, attendance, ',');
        student.name = name;
        student.roll = roll;
        student.grade = grade;
        student.attendance = attendance;

        studentsData.push_back(student);
    }
    // Read and store teachers data
    while (getline(teachersFile, line)){

        Teacher teacher;
        stringstream ss(line);
        string name, id, subject;
        getline(ss, name, ',');
        getline(ss, id, ',');
        getline(ss, subject, ',');
        teacher.name = name;
        teacher.id = id;
        teacher.subject = subject;

        teachersData.push_back(teacher);
    }
    // Close the files
    studentsFile.close();
    teachersFile.close();

    // Receive user type from client
    int clientChoice;
    read(clientSocket, &clientChoice, sizeof(clientChoice));

    if (clientChoice==1){

        // Display students data
        cout << "//--------------// Students Data //--------------//" << endl;
        for (Student student : studentsData) {
            cout<<"Name: "<<student.name<<endl
                <<"Roll:"<<student.roll<<endl
                <<"Grade:"<<student.grade<<endl
                <<"Attendance:"<<student.attendance<<"\n\n";
        }

        // Display teachers data
        cout <<endl<< "//--------------// Teachers Data //--------------//" << endl;
        for (Teacher teacher : teachersData) {
            cout<<"Name: "<<teacher.name<<endl
                <<"ID: "<<teacher.id<<endl
                <<"Subject: "<<teacher.subject<<"\n\n";
        }

    }
    else if (clientChoice == 2){

        // Receive username and password from client
        char username[1024];
        recv(clientSocket, username, sizeof(username), 0);
        char password[1024];
        recv(clientSocket, password, sizeof(password), 0);

        for(int i=0; i<teachersData.size(); i++){
            if(teachersData[i].name == username && teachersData[i].id == password){
                cout<<teachersData[i].name<<" Logged in"<<endl;
                // Send signal to client indicating teacher found
                bool signal = true;
                send(clientSocket, &signal, sizeof(signal), 0);

                // Receive choice from client
                int choice;
                recv(clientSocket, &choice, sizeof(choice), 0);

                if(choice==1){

                    // Receive roll# from client
                    char roll[1024];
                    recv(clientSocket, roll, sizeof(roll), 0);

                    for(int j=0; j<studentsData.size(); j++){
                        if(studentsData[j].roll == roll){
                            // Send student data to client
                            send(clientSocket, studentsData[j].name.c_str(), studentsData[j].name.size() + 1, 0); int a=1;
                            send(clientSocket, studentsData[j].roll.c_str(), studentsData[j].roll.size() + 1, 0); a=2;
                            send(clientSocket, studentsData[j].grade.c_str(), studentsData[j].grade.size() + 1, 0); a=3;
                            send(clientSocket, studentsData[j].attendance.c_str(), studentsData[j].attendance.size() + 1, 0);
                            break;
                        }
                    }

                }
                else if(choice==2){
                        
                        // Receiving roll# from client
                        char roll[1024];
                        recv(clientSocket, roll, sizeof(roll), 0);
    
                        // Receiving new grade from client
                        char newGrade[1024];
                        recv(clientSocket, newGrade, sizeof(newGrade), 0);
                        
                        string success = "";
                        for(int j=0; j<studentsData.size(); j++){
                            if(studentsData[j].roll == roll){
                                success+=studentsData[j].name;
                                // Updating student grade
                                studentsData[j].grade = newGrade;
                                cout<<studentsData[j].name<<" Grade updated"<<endl;
                                break;
                            }
                        }
    
                        // Sending success message to client
                        success += " Grade updated successfully";
                        send(clientSocket, success.c_str(), success.size() + 1, 0);
                }
                else{
                    // Send invalid choice message to client
                    string invalidChoice = "Invalid choice";
                    send(clientSocket, invalidChoice.c_str(), invalidChoice.size() + 1, 0);
                }

                break;
            }
        }

    }
    else if (clientChoice == 3) {

        cout<<"Student"<<endl;
        // Receive username from client
        char username[1024];
        recv(clientSocket, username, sizeof(username), 0);
        cout << username << endl;

        // Receive password from client
        char password[1024];
        recv(clientSocket, password, sizeof(password), 0);
        cout << password << endl;

        for(int i=0; i<studentsData.size(); i++){
            if(studentsData[i].name == username && studentsData[i].roll == password){
                cout<<"Student found"<<endl;
                // Send signal to client indicating student found
                bool signal = true;
                send(clientSocket, &signal, sizeof(signal), 0);

                // Receive choice from client
                int choice;
                recv(clientSocket, &choice, sizeof(choice), 0);

                if (choice == 1) {
                    // Send grades data to client
                    string gradesData = "Grades data"; // Replace with actual grades data
                    send(clientSocket, studentsData[i].grade.c_str(), gradesData.size() + 1, 0);
                } else if (choice == 2) {
                    // Send attendance data to client
                    string attendanceData = "Attendance data"; // Replace with actual attendance data
                    send(clientSocket, studentsData[i].attendance.c_str(), attendanceData.size() + 1, 0);
                } else {
                    // Send invalid choice message to client
                    string invalidChoice = "Invalid choice";
                    send(clientSocket, invalidChoice.c_str(), invalidChoice.size() + 1, 0);
                }
                break;
            }
        }

    }

    close(clientSocket);
}