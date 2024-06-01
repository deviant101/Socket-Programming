#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include "structs.cpp"
using namespace std;

vector<Student> studentsData;

int main(){

    char request[256];
    char buf[200];

    // create the socket
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    // setup an addressNational University of Computer and Emerging Sciences (NUCES) Islamabad
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(3001);
    connect(sock, (struct sockaddr *) &server_address, sizeof(server_address));
    
    // Send the first message (number) to the server
    int choice;
    cout<<"1 - Admin\n2 - Teacher\n3 - Student\n";
    cin>>choice;
    cin.ignore();
        
    if(choice==1){

        cout<<"//------------// Admin Terminal //------------//"<<endl;
        string username, password;
        cout << "Enter username: ";
        getline(cin, username);
        cout << "Enter password: ";
        getline(cin, password);

        send(sock, &choice, sizeof(choice), 0);
        send(sock, username.c_str(), sizeof(username), 0);
        send(sock, password.c_str(), sizeof(password), 0);
    }
    else if(choice==2){

        cout<<"//------------// Teacher Terminal //------------//"<<endl;
        while(true){
            // string username, password;
            char username[1024];
            char password[1024];

            cout << "Enter username: ";
            cin.getline(username, 1024);
            cout << "Enter password: ";
            cin.getline(password, 1024);

            send(sock, &choice, sizeof(choice), 0);
            send(sock, &username, sizeof(username), 0);
            send(sock, &password, sizeof(password), 0);

            bool signal;
            recv(sock, &signal, sizeof(signal), 0);

            if(signal){
                
                cout<<"1 - View Student Data\n2 - Edit Student Grade\n";
                int choice;
                cin>>choice;
                cin.ignore();
                send(sock, &choice, sizeof(choice), 0);

                if(choice==1){

                    char roll[1024];
                    cout << "Enter Student Roll#: ";
                    cin.getline(roll, 1024);
                    send(sock, &roll, sizeof(roll), 0);

                    // Receive the student data from the server
                    Student stdData;

                    char buf1[200];
                    recv(sock, buf1, sizeof(buf1), 0);
                    stdData.name = buf1;

                    char buf2[200];
                    recv(sock, buf2, sizeof(buf2), 0);
                    stdData.roll = buf2;

                    char buf3[200];
                    recv(sock, buf3, sizeof(buf3), 0);
                    stdData.grade = buf3;
                    
                    char buf4[200];
                    recv(sock, buf4, sizeof(buf4), 0);
                    stdData.attendance = buf4;

                    cout<<"Name: "<<stdData.name<<endl
                        <<"Roll: "<<stdData.roll<<endl
                        <<"Grade: "<<stdData.grade<<endl
                        <<"Attendance: "<<stdData.attendance<<"\n\n";
                    break;
                }
                else if(choice==2){
                        
                        char roll[1024];
                        cout << "Enter Student Roll#: ";
                        cin.getline(roll, 1024);
                        send(sock, &roll, sizeof(roll), 0);
    
                        char grade[1024];
                        cout << "Enter Student Grade: ";
                        cin.getline(grade, 1024);
                        send(sock, &grade, sizeof(grade), 0);
    
                        recv(sock, buf, sizeof(buf), 0);
                        cout<<buf<<endl;
                        break;
                }
                else{
                    cout<<"Invalid choice"<<endl;
                }

            }
            else{
                cout<<"\nInvalid credentials\nTry Again\n\n";
            }
        }
    }
    else if(choice==3){

        cout<<"//------------// Student Terminal //------------//"<<endl;

        while(true){
            // string username, password;
            char username[1024];
            char password[1024];

            cout << "Enter username: ";
            cin.getline(username, 1024);
            cout << "Enter password: ";
            cin.getline(password, 1024);

            cout<<username<<endl<<password<<endl;

            send(sock, &choice, sizeof(choice), 0);
            send(sock, &username, sizeof(username), 0);
            send(sock, &password, sizeof(password), 0);

            bool signal;
            recv(sock, &signal, sizeof(signal), 0);

            if(signal){
                cout<<"1 - View Grades\n2 - View Attendance\n";
                int choice;
                cin>>choice;
                cin.ignore();
                send(sock, &choice, sizeof(choice), 0);
                if(choice==1){
                    recv(sock, buf, sizeof(buf), 0);
                    cout<<buf<<endl;
                }
                else if(choice==2){
                    recv(sock, buf, sizeof(buf), 0);
                    cout<<buf<<endl;
                }
                else{
                    cout<<"Invalid choice"<<endl;
                }
            }
            else{
                cout<<"\nInvalid credentials\nTry Again\n\n";
            }
        }
    }
    else{
        cout<<"Invalid choice"<<endl;
        return 0;
    }
    
    // close the socket
    close(sock);
    return 0;
}