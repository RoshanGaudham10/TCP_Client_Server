#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define SIZE 1024
#define MAX_STUDENTS 60

int Uni_Id[2] = { 10, 40 }, S_Count = 0;

int main()
{
    WSADATA structure;
    SOCKET Client;
    struct sockaddr_in client_sock;
    char Recv_Buff[SIZE], Send_Buff[SIZE], Subject[50], Name[50];
    int SBLen, reVal, Choice, val = 0, N_Len, R_Num, Update, Up_Count,
        Math, Phys, Chem, Bio, Eng, Soc, Temp, Rollnum, State;

    printf("\nInitializing client socket\n");

    if (WSAStartup(MAKEWORD(2, 2), &structure) != 0)
    {
        printf("\nSocket initialization failed\n");
        return 1;
    }
    else
    {
        printf("\nSocket initialized\n");
    }

    printf("\nCreating client socket\n");

    Client = socket(AF_INET, SOCK_STREAM, 0);

    if (Client == INVALID_SOCKET)
    {
        printf("\nSocket creation failed\n");
        return 1;
    }
    else
    {
        printf("\nSocket created\n");
    }

    client_sock.sin_family = AF_INET;
    client_sock.sin_addr.s_addr = inet_addr("192.168.137.96");
    client_sock.sin_port = htons(55878);

    printf("\nAttempting to connect to the server\n");

    if (connect(Client, (struct sockaddr*)&client_sock, sizeof(client_sock)) == SOCKET_ERROR)
    {
        printf("\nCouldn't connect with the server\n");
        return 1;
    }
    else
    {
        printf("\nConnected with the server\n");
    }

    while (1)
    {
        reVal = recv(Client, Recv_Buff, sizeof(Recv_Buff) - 1, 0);

        if (reVal > 0)
        {
            Recv_Buff[reVal] = '\0';

            printf("\nMessage received from server:\n%s\n", Recv_Buff);

            printf("\nEnter your choice: ");

            if (scanf("%d", &Choice) != 1) 
            {
                printf("\nInvalid input\n");
                continue;
            }

            memcpy(Send_Buff, Uni_Id, sizeof(Uni_Id));
            memcpy(Send_Buff + sizeof(Uni_Id), &Choice, sizeof(Choice));

            SBLen = sizeof(Uni_Id) + sizeof(int);

            send(Client, Send_Buff, SBLen, 0);

            if (Choice == 1)
            {
                if (S_Count == MAX_STUDENTS)
                {
                    printf("\nStudent limit reached\n");
                    continue;
                }
                else
                {
                    printf("\nHow many students: ");
                    scanf("%d", &Up_Count);
                    memcpy(Send_Buff, &Up_Count, sizeof(int));
                    send(Client, Send_Buff, sizeof(int), 0);
                    
                    Temp = 0;
                    
                    while (Temp < Up_Count && Up_Count < MAX_STUDENTS)
                    {
                        printf("\n\nEnter the name of the student" 
                            "(Not more than 45 char): ");
                        scanf(" %[^\n]s", &Name);
                        N_Len = strlen(Name) + 1;
                        memcpy(Send_Buff, Name, N_Len);
                        send(Client, Send_Buff, N_Len, 0);
                    
                        printf("\nEnter the roll number of student: ");
                        scanf("%d", &Rollnum);
                        memcpy(Send_Buff, &Rollnum, sizeof(int));
                        send(Client, Send_Buff, sizeof(int), 0);

                        while (1)
                        {
                            recv(Client, Recv_Buff, sizeof(Recv_Buff) - 1, 0);
                            memcpy(&State, Recv_Buff, sizeof(int));

                            if (State == 1)
                            {
                                printf("\nRoll number already exists, ");
                                printf("enter correct number: ");
                                scanf("%d", &R_Num);
                                memcpy(Send_Buff, &R_Num, sizeof(int));
                                send(Client, Send_Buff, sizeof(int), 0);
                            }
                            else
                            {
                                break;
                            }
                        }
                    
                        printf("\nEnter the student mark of Math: ");
                        scanf("%d", &Math);
                        memcpy(Send_Buff, &Math, sizeof(int));
                        send(Client, Send_Buff, sizeof(int), 0);
                    
                        printf("\nEnter the student mark of Psy: ");
                        scanf("%d", &Phys);
                        memcpy(Send_Buff, &Phys, sizeof(int));
                        send(Client, Send_Buff, sizeof(int), 0);
                    
                        printf("\nEnter the student mark of Chem: ");
                        scanf("%d", &Chem);
                        memcpy(Send_Buff, &Chem, sizeof(int));
                        send(Client, Send_Buff, sizeof(int), 0);
                    
                        printf("\nEnter the student mark of Bio: ");
                        scanf("%d", &Bio);
                        memcpy(Send_Buff, &Bio, sizeof(int));
                        send(Client, Send_Buff, sizeof(int), 0);
                    
                        printf("\nEnter the student mark of Eng: ");
                        scanf("%d", &Eng);
                        memcpy(Send_Buff, &Eng, sizeof(int));
                        send(Client, Send_Buff, sizeof(int), 0);
                    
                        printf("\nEnter the student mark of Soc: ");
                        scanf("%d", &Soc);
                        memcpy(Send_Buff, &Soc, sizeof(int));
                        send(Client, Send_Buff, sizeof(int), 0);
                    
                        S_Count++;
                    
                        Temp++;
                    }
                }
            }
            else if (Choice == 2)
            {
                printf("\nEnter the roll number: ");
                scanf("%d", &R_Num);
                memcpy(Send_Buff, &R_Num, sizeof(int));
                send(Client, Send_Buff, sizeof(int), 0);
                
                while (1)
                {
                    recv(Client, Recv_Buff, sizeof(Recv_Buff) - 1, 0);
                    memcpy(&State, Recv_Buff, sizeof(int));                     

                    if (State == 0)
                    {
                        printf("\nRoll number doesn't exists, ");
                        printf("enter correct number: ");
                        scanf("%d", &R_Num);
                        memcpy(Send_Buff, &R_Num, sizeof(int));
                        send(Client, Send_Buff, sizeof(int), 0);
                    }
                    else
                    {
                        break;
                    }
                }
                
                while (1)
                {
                    printf("\nMaths / Physics / Chemistry\n");
                    printf("\nBiology / English/ Social\n");
                    printf("\nEnter the subject name you want to update: ");

                    scanf(" %[^\n]s", Subject);
                    N_Len = strlen(Subject) + 1;
                    

                    if (strcmp(Subject, "Maths") == 0)
                    {
                        memcpy(Send_Buff, Subject, N_Len);
                        send(Client, Send_Buff, N_Len, 0);

                        printf("\nEnter the new grade: ");
                        scanf("%d", &Math);
                        memcpy(Send_Buff, &Math, sizeof(int));
                        send(Client, Send_Buff, sizeof(int), 0);
                        break;
                    }
                    else if (strcmp(Subject, "Physics") == 0)
                    {
                        memcpy(Send_Buff, Subject, N_Len);
                        send(Client, Send_Buff, N_Len, 0);

                        printf("\nEnter the new grade: ");
                        scanf("%d", &Phys);
                        memcpy(Send_Buff, &Phys, sizeof(int));
                        send(Client, Send_Buff, sizeof(int), 0);
                        break;
                    }
                    else if (strcmp(Subject, "Chemistry") == 0)
                    {
                        memcpy(Send_Buff, Subject, N_Len);
                        send(Client, Send_Buff, N_Len, 0);

                        printf("\nEnter the new grade: ");
                        scanf("%d", &Chem);
                        memcpy(Send_Buff, &Chem, sizeof(int));
                        send(Client, Send_Buff, sizeof(int), 0);
                        break;
                    }
                    else if (strcmp(Subject, "Biology") == 0)
                    {
                        memcpy(Send_Buff, Subject, N_Len);
                        send(Client, Send_Buff, N_Len, 0);

                        printf("\nEnter the new grade: ");
                        scanf("%d", &Bio);
                        memcpy(Send_Buff, &Bio, sizeof(int));
                        send(Client, Send_Buff, sizeof(int), 0);
                        break;
                    }
                    else if (strcmp(Subject, "English") == 0)
                    {
                        memcpy(Send_Buff, Subject, N_Len);
                        send(Client, Send_Buff, N_Len, 0);

                        printf("\nEnter the new grade: ");
                        scanf("%d", &Eng);
                        memcpy(Send_Buff, &Eng, sizeof(int));
                        send(Client, Send_Buff, sizeof(int), 0);
                        break;
                    }
                    else if (strcmp(Subject, "Social") == 0)
                    {
                        memcpy(Send_Buff, Subject, N_Len);
                        send(Client, Send_Buff, N_Len, 0);

                        printf("\nEnter the new grade: ");
                        scanf("%d", &Soc);
                        memcpy(Send_Buff, &Soc, sizeof(int));
                        send(Client, Send_Buff, sizeof(int), 0);
                        break;
                    }
                    else
                    {
                        printf("\nInvalid Subject\n");
                    }
                }                
            }
            else if (Choice == 3)
            {
                
            }
            else if (Choice == 4)
            {
                printf("\nInformations Saved!!!!\n\nExiting Server\n");
                break; 
            }
            else
            {
                printf("\nInvalid Choice\n");
            }
        }
        else if (reVal == 0)
        {
            printf("\nServer closed the connection\n");
            break;
        }
        else
        {
            printf("\nFailed to receive message from server\n");
            break;
        }
    }

    closesocket(Client);
    WSACleanup();
    return 0;
}
