#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <process.h>

#pragma comment(lib,"ws2_32.lib")

#define SIZE 1024
#define LIMIT 3
#define MAX_STUDENTS 60

int Client_Count = 0, S_Count = 0;

void CreateStudentData(int num)
{
    char filename[50];
    sprintf(filename, "Student Data\\Student_Data_%d.csv", num);

    FILE* WritePtr = fopen(filename, "a");
    if (WritePtr == NULL)
    {
        printf("\nError opening file %s\n", filename);
        return;
    }

    fseek(WritePtr, 0, SEEK_END);
    if (ftell(WritePtr) == 0)
    {
        fprintf(WritePtr, "Student rollno, Student name, Maths mark, Physics mark, "
            "Chemistry mark, Biology mark, English mark, Social mark\n");
    }

    fclose(WritePtr);
}

void SaveStudentData(int num, int rnum, char name[50], int mat, int che,
    int phy, int bio, int en, int so)
{
    char filename[50];
    sprintf(filename, "Student Data\\Student_Data_%d.csv", num);

    FILE* WritePtr = fopen(filename, "a");
    if (WritePtr == NULL)
    {
        printf("\nError opening file %s\n", filename);
        return;
    }

    fprintf(WritePtr, "%d, %s, %d, %d, %d, %d, %d, %d\n", rnum, name,
        mat, phy, che, bio, en, so);

    fclose(WritePtr);
}

void UpdateMarks(int num, int rollnum, char sub[50], int marks)
{
    char filename[50];
    sprintf(filename, "Student Data\\Student_Data_%d.csv", num);

    FILE* UpdatePtr = fopen(filename, "r+");
    if (UpdatePtr == NULL)
    {
        printf("\nError opening file %s\n", filename);
        return;
    }

    FILE* tempPtr = tmpfile();
    if (tempPtr == NULL)
    {
        printf("\nError creating temporary file.\n");
        fclose(UpdatePtr);
        return;
    }

    int rolln, M, P, C, B, E, S;
    char line[255], N[50];

    fgets(line, sizeof(line), UpdatePtr);
    fputs(line, tempPtr);

    while (fgets(line, sizeof(line), UpdatePtr) != NULL)
    {
        if (sscanf(line, "%d", &rolln) == 1)
        {
            if (rolln == rollnum)
            {
                sscanf(line, "%d, %[^,], %d, %d, %d, %d, %d, %d", 
                    &rolln, N, &M, &P, &C, &B, &E, &S);

                if (strcmp(sub, "Maths") == 0)
                {
                    M = marks;
                }
                else if (strcmp(sub, "Physics") == 0)
                {
                    P = marks;
                }
                else if (strcmp(sub, "Chemistry") == 0)
                {
                    C = marks;
                }
                else if (strcmp(sub, "Biology") == 0)
                {
                    B = marks;
                }
                else if (strcmp(sub, "English") == 0)
                {
                    E = marks;
                }
                else if (strcmp(sub, "Social") == 0)
                {
                    S = marks;
                }

                fprintf(tempPtr, "%d, %s, %d, %d, %d, %d, %d, %d\n",
                    rolln, N, M, P, C, B, E, S);
            }
            else
            {
                fputs(line, tempPtr);
            }
        }
    }

    fclose(UpdatePtr);
    fseek(tempPtr, 0, SEEK_SET);

    FILE* originalPtr = fopen(filename, "w");
    if (originalPtr == NULL)
    {
        printf("\nError opening file %s\n", filename);
        fclose(tempPtr);
        return;
    }

    while (fgets(line, sizeof(line), tempPtr) != NULL)
    {
        fputs(line, originalPtr);
    }

    fclose(originalPtr);
    fclose(tempPtr);
}

void DisplayList(int num)
{
    char filename3[50];
    sprintf(filename3, "Student Data\\Student_Data_%d.csv", num);

    FILE* DisplayPtr = fopen(filename3, "r");

    if (DisplayPtr == NULL)
    {
        printf("\nError opening file %s\n", filename3);
        return 1;
    }

    int rolln, M, P, C, B, E, S;
    char line[255], N[50];

    fgets(line, sizeof(line), DisplayPtr);

    while (fgets(line, sizeof(line), DisplayPtr) != NULL)
    {
        sscanf(line, "%d, %[^,], %d, %d, %d, %d, %d, %d",
            &rolln, N, &M, &P, &C, &B, &E, &S);

        printf("\n\nRoll Number: %d", rolln);
        printf("\nName: %s", N);
        printf("\nMarks of Maths: %d", M);
        printf("\nMarks of Physics: %d", P);
        printf("\nMarks of Chemistry: %d", C);
        printf("\nMarks of Biology: %d", B);
        printf("\nMarks of English: %d", E);
        printf("\nMarks of Social: %d", S);                       
    }
    fclose(DisplayPtr);
}

int roll_Number(int num, int rno)
{
    char filename[50];
    sprintf(filename, "Student Data\\Student_Data_%d.csv", num);

    FILE* ReadPtr = fopen(filename, "r");
    if (ReadPtr == NULL)
    {
        printf("\nError opening file %s\n", filename);
        return 1;
    }

    int rn;
    char line[255];

    fgets(line, sizeof(line), ReadPtr);

    while (fgets(line, sizeof(line), ReadPtr) != NULL)
    {
        if (sscanf(line, "%d", &rn) == 1)
        {
            if (rn == rno)
            {
                fclose(ReadPtr);
                return rno;
            }
        }
    }

    fclose(ReadPtr);
    return 0;
}

void Client_requests(void* arg)
{
    int reVal, iLen, Uni_Id[2], i, Choice = 0, Up_Count = 0,
        R_Num, math = 0, phys = 0, chem = 0, bio = 0, eng = 0,
        soc = 0, t, State, CRN, rn, mark, re_n, re_rn, re_ma,
        re_ph, re_ch, re_bi, re_en, re_so;

    SOCKET client_sock = *((SOCKET*)arg);
    char Buffer[SIZE], name[50], TemBuff[50], Sub[50];

    iLen = strlen("\nWelcome to ABC school student server\n"
        "\nPress 1 - student marks entry\n"
        "\nPress 2 - student marks update\n"
        "\nPress 3 - to print the data base\n"
        "\nPress 4 - exiting the server\n");

    do
    {
        send(client_sock, "\n\nWelcome to ABC school student server\n"
        "\nPress 1 - student marks entry\n"
        "\nPress 2 - student marks update\n"
        "\nPress 3 - to print the data base\n"
        "\nPress 4 - exiting the server\n", iLen, 0);

        reVal = recv(client_sock, Buffer, sizeof(Buffer), 0);

        if (reVal > 0)
        {
            memcpy(Uni_Id, Buffer, sizeof(Uni_Id));

            for (i = sizeof(Uni_Id); i < reVal; i += sizeof(int))
            {
                memcpy(&Choice, &Buffer[i], sizeof(Choice));
            }

            CreateStudentData(Uni_Id[1]);

            printf("\n\nMessage received from client ");

            for (i = 0; i < sizeof(Uni_Id) / sizeof(int); i++)
            {
                printf("[%d]", Uni_Id[i]);
            }

            printf(": %d\n", Choice);

            if (Choice == 1)
            {
                if (S_Count >= MAX_STUDENTS)
                {
                    printf("\nStudent limit reached\n");
                    continue;
                }
                else
                {
                    re_n = recv(client_sock, Buffer, sizeof(Buffer), 0);
                    memcpy(&Up_Count, Buffer, sizeof(int));

                    if (re_n > 1)
                    {
                        t = 0;

                        while (t < Up_Count)
                        {
                            re_rn = recv(client_sock, Buffer, sizeof(Buffer) - 1, 0);

                            if (re_rn > 1)
                            {
                                memcpy(name, Buffer, strlen(Buffer));
                                name[strlen(Buffer)] = '\0';

                                while (1)
                                {
                                    recv(client_sock, Buffer, sizeof(Buffer), 0);
                                    memcpy(&R_Num, Buffer, sizeof(int));

                                    CRN = roll_Number(Uni_Id[1], R_Num);

                                    State = 0;

                                    if (R_Num == CRN)
                                    {
                                        State = 1;
                                        memcpy(TemBuff, &State, sizeof(int));
                                        send(client_sock, TemBuff, sizeof(int), 0);
                                    }

                                    if (State == 0)
                                    {
                                        memcpy(TemBuff, &State, sizeof(int));
                                        send(client_sock, TemBuff, sizeof(int), 0);
                                        break;
                                    }
                                }
                                memcpy(&rn, &R_Num, sizeof(int));

                                re_ma = recv(client_sock, Buffer, sizeof(Buffer), 0);

                                if (re_ma > 1)
                                {
                                    memcpy(&math, Buffer, sizeof(int));

                                    re_ph = recv(client_sock, Buffer, sizeof(Buffer), 0);

                                    if (re_ph > 1)
                                    {
                                        memcpy(&phys, Buffer, sizeof(int));

                                        re_ch = recv(client_sock, Buffer, sizeof(Buffer), 0);

                                        if (re_ch > 1)
                                        {
                                            memcpy(&chem, Buffer, sizeof(int));

                                            re_bi = recv(client_sock, Buffer, sizeof(Buffer), 0);

                                            if (re_bi > 1)
                                            {
                                                memcpy(&bio, Buffer, sizeof(int));

                                                re_en = recv(client_sock, Buffer, sizeof(Buffer), 0);

                                                if (re_en > 1)
                                                {
                                                    memcpy(&eng, Buffer, sizeof(int));

                                                    re_so = recv(client_sock, Buffer, sizeof(Buffer), 0);

                                                    if (re_so > 1)
                                                    {
                                                        memcpy(&soc, Buffer, sizeof(int));

                                                        S_Count++;

                                                        t++;

                                                        printf("\nStudent Details of Roll No.%d Updated\n", rn);

                                                        SaveStudentData(Uni_Id[1], rn, name, math, phys, chem,
                                                            bio, eng, soc);
                                                    }
                                                    else
                                                    {
                                                        break;
                                                    }
                                                   
                                                }
                                                else
                                                {
                                                    break;
                                                }
                                                
                                            }
                                            else
                                            {
                                                break;
                                            }
                                            
                                        }
                                        else
                                        {
                                            break;
                                        }
                                        
                                    }
                                    else
                                    {
                                        break;
                                    }                    
                                    
                                }
                                else
                                {
                                    break;
                                }
                                
                            }
                            else
                            {
                                break;
                            }                            
                        }

                    }
                    else
                    {
                        break;
                    }                    
                }
            }
            else if (Choice == 2)
            {
                while (1)
                {
                    recv(client_sock, Buffer, sizeof(Buffer), 0);
                    memcpy(&R_Num, Buffer, sizeof(int));

                    CRN = roll_Number(Uni_Id[1], R_Num);

                    State = 0;

                    if (R_Num == CRN)
                    {
                        State = 1;
                        memcpy(TemBuff, &State, sizeof(int));
                        send(client_sock, TemBuff, sizeof(int), 0);

                        break;
                    }

                    if (State == 0)
                    {
                        memcpy(TemBuff, &State, sizeof(int));
                        send(client_sock, TemBuff, sizeof(int), 0);
                    }
                }

                recv(client_sock, Buffer, sizeof(Buffer), 0);
                memcpy(Sub, Buffer, strlen(Buffer));
                Sub[strlen(Buffer)] = '\0';

                recv(client_sock, Buffer, sizeof(Buffer), 0);
                memcpy(&mark, Buffer, sizeof(int));
                UpdateMarks(Uni_Id[1], CRN, Sub, mark);
            }
            else if (Choice == 3)
            {
                DisplayList(Uni_Id[1]);
            }
            else if (Choice == 4)
            {
                printf("\n\nConnection close request by client ");

                for (i = 0; i < sizeof(Uni_Id) / sizeof(int); i++)
                {
                    printf("[%d]", Uni_Id[i]);
                }

                closesocket(client_sock);
            }
        }
        else
        {
            printf("\n\nrecv failed, client ");

            for (i = 0; i < sizeof(Uni_Id) / sizeof(int); i++)
            {
                printf("[%d]", Uni_Id[i]);
            }

            printf(": disconnected\n");
            closesocket(client_sock);
            Client_Count--;

            break;
        }
    } while (reVal > 0);

    _endthread();
}

int main()
{
    WSADATA Structure;
    SOCKET Server, Client[LIMIT];
    struct sockaddr_in Server_sock;

    if (WSAStartup(MAKEWORD(2, 2), &Structure) != 0)
    {
        printf("\nServer socket initialization failed\n");
        return 1;
    }
    else
    {
        printf("\nServer socket initialized\n");
    }

    Server = socket(AF_INET, SOCK_STREAM, 0);

    if (Server == INVALID_SOCKET)
    {
        printf("\nServer socket creation failed\n");
        return 1;
    }
    else
    {
        printf("\nServer socket created\n");
    }

    Server_sock.sin_family = AF_INET;
    Server_sock.sin_addr.s_addr = INADDR_ANY;
    Server_sock.sin_port = htons(55878);

    if (bind(Server, (struct sockaddr*)&Server_sock, sizeof(Server_sock))
        == SOCKET_ERROR)
    {
        printf("\nServer socket is not binded with the socket address\n");
        closesocket(Server);
        return 1;
    }
    else
    {
        printf("\nServer socket binded with the socket address\n");
    }

    if (listen(Server, 0) == SOCKET_ERROR)
    {
        printf("\nListening failed\n");
        closesocket(Server);
        return 1;
    }

    printf("\nServer actively listening\n");

    while (1)
    {
        Client[Client_Count] = accept(Server, NULL, NULL);

        if (Client[Client_Count] == INVALID_SOCKET)
        {
            printf("\nClient connection failed\n");
            continue;
        }

        printf("\nNew client connected\n");

        _beginthread(Client_requests, 0, (void*)&Client[Client_Count]);
        Client_Count++;

        if (Client_Count > LIMIT)
        {
            printf("\nMaximum number of clients reached\n");
            break;
        }
    }

    closesocket(Server);

    WSACleanup();

    return 0;
}
