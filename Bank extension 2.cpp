#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

const string ClientsFileName = "ClientsFile.txt";
const string UsersFileName = "UsersFile.txt";

enum enMainMenuOption { eShowClientsList = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClientInfo = 4, eFindClient = 5, eTransactions = 6, eManageUsers = 7, eLogout = 8 };
enum enTransactionsMenuOption { eDeposit = 1, eWithDraw = 2, eTotalBalances = 3, eMainMenu = 4 };
enum enManageUsersMenuOption { eListUsers = 1, eAddNewUser = 2, eDeleteUser = 3, eUpdateUser = 4, eFindUser = 5, eeMainMenu = 6 };
enum enPermissions { pFullAccess = -1, pShowClientList = 1, pAddNewClient = 2, pDeleteClient = 4, pUpdateClient = 8, pFindClient = 16, pTransactions = 32, pManageUsers = 64 };

struct stClient
{
    string AccountNumber;
    string PinCode;
    string name;
    string phone;
    double AccountBalance;
    bool isMarkedToDelete;
};

struct stUser
{
    string name;
    string password;
    short Permissions;
    bool isMarkedToDelete;
};

stUser CurrentUser;

int ReadNumber(string message)
{
    int Num;
    cout << message << endl;
    cin >> Num;
    return Num;
}

void PerformMainMenue(enMainMenuOption MenuOption);

void ShowMainMenuScreen()
{
    cout << "====================================================\n";
    cout << "                  Main Menu Screen                  \n";
    cout << "====================================================\n";
    cout << "               [1] Show Clients List                \n";
    cout << "               [2] Add New Client                   \n";
    cout << "               [3] Delete Client                    \n";
    cout << "               [4] Update Client Info               \n";
    cout << "               [5] Find Client                      \n";
    cout << "               [6] Transactions                     \n";
    cout << "               [7] Manage users                     \n";
    cout << "               [8] Logout                           \n";
    cout << "====================================================\n";
    PerformMainMenue((enMainMenuOption)ReadNumber("Choose what do you want to do ?"));
}

string ReadString(string message)
{
    string S1;
    cout << message << endl;
    cin.ignore(1, '\n');
    getline(cin, S1);
    return S1;
}

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord; // define a string variable  // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }
    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.    
    }
    return vString;
}

stClient ConvertLinetoClientRecord(string Line, string Seperator = "#//#")
{
    stClient Client;
    vector <string> vClients;
    vClients = SplitString(Line, Seperator);
    Client.AccountNumber = vClients[0];
    Client.PinCode = vClients[1];
    Client.name = vClients[2];
    Client.phone = vClients[3];
    Client.AccountBalance = stod(vClients[4]);//cast string to doublereturn Client; 
    return Client;
}

stUser ConvertLinetoUserRecord(string Line, string Seperator = "#//#")
{
    stUser User;
    vector <string> vUsers;
    vUsers = SplitString(Line, Seperator);
    User.name = vUsers[0];
    User.password = vUsers[1];
    User.Permissions = stoi(vUsers[2]);
    return User;
}

string ConvertClientRecordToLine(stClient Client, string delim)
{
    return Client.AccountNumber + delim + Client.PinCode + delim + Client.name + delim + Client.phone + delim + to_string(Client.AccountBalance);
}

string ConvertUserRecordToLine(stUser User, string delim)
{
    return User.name + delim + User.password + delim + to_string(User.Permissions);
}

vector <stClient> LoadClientsFromFile(string FileName)
{

    fstream ClientsFile;
    vector <stClient> vClients;
    ClientsFile.open(FileName, ios::in);
    if (ClientsFile.is_open())
    {
        string Line;
        stClient Client;
        while (getline(ClientsFile, Line))
        {
            Client = ConvertLinetoClientRecord(Line);
            vClients.push_back(Client);
        }
        ClientsFile.close();
    }
    return vClients;
}

stClient ReadClientData()
{
    stClient Client;
    cout << "enter account number : ";
    getline(cin >> ws, Client.AccountNumber);
    cout << "enter Pin Code : ";
    getline(cin, Client.PinCode);
    cout << "enter name : ";
    getline(cin, Client.name);
    cout << "enter phone : ";
    getline(cin, Client.phone);
    cout << "enter account balance : ";
    cin >> Client.AccountBalance;

    return Client;
}

short ReadPermissions()
{
    short Permissions = 0;
    char ans;
    cout << "Do you want to give full access?(y/n) ";
    cin >> ans;
    if (ans == 'y' || ans == 'Y')
    {
        return -1;
    }
    else
    {
        cout << "Do you want to give access to:" << endl;

        cout << "Show client list ? y/n? ";
        cin >> ans;
        if (ans == 'y' || ans == 'Y')
        {
            Permissions += enPermissions::pShowClientList;
        }

        cout << "Add client ? y/n? ";
        cin >> ans;
        if (ans == 'y' || ans == 'Y')
        {
            Permissions += enPermissions::pAddNewClient;
        }

        cout << "Delete client ? y/n? ";
        cin >> ans;
        if (ans == 'y' || ans == 'Y')
        {
            Permissions += enPermissions::pDeleteClient;
        }

        cout << "Update client ? y/n? ";
        cin >> ans;
        if (ans == 'y' || ans == 'Y')
        {
            Permissions += enPermissions::pUpdateClient;
        }

        cout << "Find client ? y/n? ";
        cin >> ans;
        if (ans == 'y' || ans == 'Y')
        {
            Permissions += enPermissions::pFindClient;
        }

        cout << "Transactions ? y/n? ";
        cin >> ans;
        if (ans == 'y' || ans == 'Y')
        {
            Permissions += enPermissions::pTransactions;
        }

        cout << "Manage Users ? y/n? ";
        cin >> ans;
        if (ans == 'y' || ans == 'Y')
        {
            Permissions += enPermissions::pManageUsers;
        }

    }
    return Permissions;
}

bool isUserFound(vector <stUser> vUsers, string Username, stUser& User);

vector <stUser> LoadUsersFromFile(string FileName);

void ReReadUserData(stUser &User)
{
    vector <stUser> vUsers = LoadUsersFromFile(UsersFileName);
    cout << "enter user name : ";
    getline(cin >> ws, User.name);
    while (isUserFound(vUsers, User.name, User) && User.name!= CurrentUser.name)
    {
        cout << "User with [" << User.name << "] already exists, enter another username ";
        getline(cin >> ws, User.name);
    }
    cout << "enter password : ";
    getline(cin, User.password);
    User.Permissions = ReadPermissions();
}

stUser ReadUserData()
{
    stUser User;
    vector <stUser> vUsers = LoadUsersFromFile(UsersFileName);
    cout << "enter user name : ";
    getline(cin >> ws, User.name);
    while (isUserFound(vUsers, User.name, User))
    {
        cout << "User with [" << User.name << "] already exists, enter another username ";
        getline(cin >> ws, User.name);
    }
    cout << "enter password : ";
    getline(cin, User.password);
    User.Permissions = ReadPermissions();
    return User;
}
bool CheckAccessPermission(enPermissions Permission);

void ShowAccessDeniedMessage();

void ShowClientsData()
{
    if (!CheckAccessPermission(enPermissions::pShowClientList))
    {
        ShowAccessDeniedMessage();
        return;
    }
    vector <stClient> vClients = LoadClientsFromFile(ClientsFileName);
    cout << "                   Clients(" << vClients.size() << ") Client(s)" << endl;
    cout << "---------------------------------------------------------------------------------------------" << endl;
    cout << "| " << setw(20) << "Account Number" << "| " << setw(8) << "Pin Code" << "| " << setw(30) << "ClientName" << "| " << setw(14) << "Phone" << "| " << setw(10) << "Balance" << "|" << endl;
    cout << "---------------------------------------------------------------------------------------------" << endl;
    if (vClients.size() > 0)
    {
        for (stClient& Client : vClients)
        {
            cout << "| " << setw(20) << Client.AccountNumber << "| " << setw(8) << Client.PinCode << "| " << setw(30) << Client.name << "| " << setw(14) << Client.phone << "| " << setw(10) << Client.AccountBalance << "|" << endl;
        }
        cout << "---------------------------------------------------------------------------------------------" << endl;
    }
    else
    {
        cout << "                                             NO CLIENTS" << endl;
        cout << "---------------------------------------------------------------------------------------------" << endl;
    }

}

void AddClients()
{
    if (!CheckAccessPermission(enPermissions::pAddNewClient))
    {
        ShowAccessDeniedMessage();
        return;
    }
    cout << "--------------------------\n";
    cout << "     ADD NEW CLIENT\n";
    cout << "--------------------------\n";
    stClient Client;
    char AddMore = 'y';
    fstream ClientsFile;
    ClientsFile.open("ClientsFile.txt", ios::out | ios::app);
    if (ClientsFile.is_open())
    {
        while (AddMore == 'y' || AddMore == 'Y')
        {
            cout << "Adding new clients :" << endl;
            Client = ReadClientData();
            ClientsFile << ConvertClientRecordToLine(Client, "#//#") << "\n";
            cout << "Client added succesfully, do you want to add more clients? (y/n) : ";
            cin >> AddMore;
        }
    }
}

bool isClientFound(vector <stClient> vClients, string AccountNumber, stClient& Client)
{
    for (stClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

void ShowClientCard(stClient Client)
{
    cout << "The following are the client details" << endl;
    cout << "Account Number  : " << Client.AccountNumber << endl;
    cout << "Pin Code        : " << Client.PinCode << endl;
    cout << "Name            : " << Client.name << endl;
    cout << "Phone           : " << Client.phone << endl;
    cout << "Account balance : " << Client.AccountBalance << endl;
}

bool Find_Client()
{
    if (!CheckAccessPermission(enPermissions::pFindClient))
    {
        ShowAccessDeniedMessage();
        return false;
    }
    cout << "--------------------------\n";
    cout << "     FIND CLIENT\n";
    cout << "--------------------------\n";
    vector <stClient> vClients = LoadClientsFromFile(ClientsFileName);
    string AccountNumber = ReadString("please enter Account number ? ");
    stClient Client;
    if (isClientFound(vClients, AccountNumber, Client))
    {
        ShowClientCard(Client);
        return true;
    }
    else
    {
        cout << "Client with accout number (" << AccountNumber << ") Not found !" << endl;
        return false;
    }
}

bool MarkToDelete(vector <stClient>& vClients, string AccountNumber)
{
    for (stClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.isMarkedToDelete = true;
            return true;
        }
    }
    return false;
}

vector <stClient> SaveDataClientsToFile(string FileName, vector <stClient> vClients)
{
    fstream ClientsFile;
    ClientsFile.open(FileName, ios::out);
    string DataLine;
    if (ClientsFile.is_open())
    {
        for (stClient Client : vClients)
        {
            if (Client.isMarkedToDelete == false)
            {
                DataLine = ConvertClientRecordToLine(Client, "#//#");
                ClientsFile << DataLine << endl;
            }
        }
        ClientsFile.close();
    }
    return vClients;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <stClient>& vClients)
{
    stClient Client;
    char Answer = 'n';

    if (isClientFound(vClients, AccountNumber, Client))
    {

        ShowClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkToDelete(vClients, AccountNumber);
            SaveDataClientsToFile(ClientsFileName, vClients);

            //Refresh Clients 
            vClients = LoadClientsFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

void Delete_Client()
{
    if (!CheckAccessPermission(enPermissions::pDeleteClient))
    {
        ShowAccessDeniedMessage();
        return;
    }
    cout << "--------------------------\n";
    cout << "     DELETE CLIENT\n";
    cout << "--------------------------\n";
    vector <stClient> vClients = LoadClientsFromFile(ClientsFileName);
    string AccountNumber = ReadString("please enter Account number ? ");
    DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ReadClientNewData(stClient& Client)
{
    cout << "enter Pin Code : ";
    getline(cin >> ws, Client.PinCode);
    cout << "enter name : ";
    getline(cin, Client.name);
    cout << "enter phone : ";
    getline(cin, Client.phone);
    cout << "enter account balance : ";
    cin >> Client.AccountBalance;
}

void UpdateFileWithUpdatedClient(vector <stClient>& vClients)
{
    fstream ClientsFile;
    ClientsFile.open(ClientsFileName, ios::out);
    if (ClientsFile.is_open())
    {
        for (stClient& C : vClients)
            ClientsFile << ConvertClientRecordToLine(C, "#//#") << endl;
        ClientsFile.close();
    }
}

void UpdateClientByAccountNumber()
{
    if (!CheckAccessPermission(enPermissions::pUpdateClient))
    {
        ShowAccessDeniedMessage();
        return;
    }
    cout << "--------------------------\n";
    cout << "     UPDATE CLIENT\n";
    cout << "--------------------------\n";
    vector <stClient> vClients = LoadClientsFromFile(ClientsFileName);
    stClient Client;
    string AccountNumber;
    AccountNumber = ReadString("please enter Account number ? ");
    if (isClientFound(vClients, AccountNumber, Client))
    {
        ShowClientCard(Client);
        char upd;
        cout << "Do you want to update this client? (y/n) : ";
        cin >> upd;
        if (upd == 'y' || upd == 'Y')
        {
            for (stClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    ReadClientNewData(C);
                    break;
                }
            }
            cout << "Client data updated succesfully !" << endl;
            UpdateFileWithUpdatedClient(vClients);
        }
    }
    else
    {
        cout << "Client with accout number (" << AccountNumber << ") Not found !" << endl;
    }
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...\n";
    system("pause>0");
    system("cls");
    ShowMainMenuScreen();
}

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";
}

bool DepositByAccountNumber(string AccountNumber, double DepositAmount, vector <stClient>& vClients)
{
    char Ans;
    cout << "Are you sure you want to perform this transaction?(y/n) ";
    cin >> Ans;
    if (Ans == 'y' || Ans == 'Y')
    {
        for (stClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += DepositAmount;
                UpdateFileWithUpdatedClient(vClients);
                cout << "Done succesfully! New balance : " << C.AccountBalance << endl;
                return true;
            }
        }
    }
    return false;
}

void Deposit()
{
    cout << "--------------------------\n";
    cout << "     DEPOSIT \n";
    cout << "--------------------------\n";
    stClient Client;
    vector <stClient> vClients = LoadClientsFromFile(ClientsFileName);
    string AccountNumber = ReadString("please enter Account number ? ");
    while (isClientFound(vClients, AccountNumber, Client) == false)
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        AccountNumber = ReadString("please enter Account number ? ");
    }
    ShowClientCard(Client);
    double DepositAmount;
    cout << "Please enter deposit amount : ";
    cin >> DepositAmount;
    DepositByAccountNumber(AccountNumber, DepositAmount, vClients);
}

bool Withdraw()
{
    cout << "--------------------------\n";
    cout << "     WITHDRAW \n";
    cout << "--------------------------\n";
    stClient Client;
    vector <stClient> vClients = LoadClientsFromFile(ClientsFileName);
    string AccountNumber = ReadString("please enter Account number ? ");
    while (isClientFound(vClients, AccountNumber, Client) == false)
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        AccountNumber = ReadString("please enter Account number ? ");
    }
    ShowClientCard(Client);
    double WithdrawAmount;
    cout << "Please enter withdraw amount : ";
    cin >> WithdrawAmount;
    while (WithdrawAmount > Client.AccountBalance)
    {
        cout << "Amount exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "please enter another amount : ";
        cin >> WithdrawAmount;
    }
    DepositByAccountNumber(AccountNumber, WithdrawAmount * -1, vClients);
    return true;
}

void TotalBalancesScreen()
{
    vector <stClient> vClients = LoadClientsFromFile(ClientsFileName);
    cout << "                   Clients(" << vClients.size() << ") Client(s)" << endl;
    cout << "----------------------------------------------------------------------------------------------" << endl;
    cout << "| " << setw(20) << "Account Number" << "| " << setw(25) << "Client Name" << "| " << setw(10) << "Balance" << "|" << endl;
    cout << "----------------------------------------------------------------------------------------------" << endl;
    double TotalBalances = 0;
    if (vClients.size() > 0)
    {
        for (stClient& Client : vClients)
        {
            cout << "| " << setw(20) << Client.AccountNumber << "| " << setw(25) << Client.name << "| " << setw(10) << Client.AccountBalance << "|" << endl;
            TotalBalances += Client.AccountBalance;
        }
        cout << "----------------------------------------------------------------------------------------------" << endl;

        cout << "                                   Total Balances = " << TotalBalances << endl;
    }
    else
    {
        cout << "                                             NO CLIENTS" << endl;
        cout << "----------------------------------------------------------------------------------------------" << endl;
    }
}

void ShowTransactionsMenue();

void GoBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to Transactions Menue...\n";
    system("pause>0");
    system("cls");
    ShowTransactionsMenue();
}

void PerformTransactionsMenu(enTransactionsMenuOption TransactionsMenuOption)
{
    switch (TransactionsMenuOption)
    {
    case enTransactionsMenuOption::eDeposit:
        system("cls");
        Deposit();
        GoBackToTransactionsMenue();
    case enTransactionsMenuOption::eWithDraw:
        system("cls");
        Withdraw();
        GoBackToTransactionsMenue();
    case enTransactionsMenuOption::eTotalBalances:
        system("cls");
        TotalBalancesScreen();
        GoBackToTransactionsMenue();
    case enTransactionsMenuOption::eMainMenu:
        system("cls");
        ShowMainMenuScreen();
    }
}

void ShowTransactionsMenue()
{
    if (!CheckAccessPermission(enPermissions::pTransactions))
    {
        ShowAccessDeniedMessage();
        return;
    }
    cout << "=============================\n";
    cout << "\tTransactions Menue\n";
    cout << "=============================\n";
    cout << "\t[1] Deposit\n";
    cout << "\t[2] Withdraw\n";
    cout << "\t[3] Total Balances\n";
    cout << "\t[4] Main Menue\n";
    cout << "=============================\n";
    PerformTransactionsMenu((enTransactionsMenuOption)ReadNumber("Choose what do you want to do ?"));

}

vector <stUser> LoadUsersFromFile(string FileName)
{
    fstream UsersFile;
    vector <stUser> vUsers;
    UsersFile.open(FileName, ios::in);
    if (UsersFile.is_open())
    {
        string Line;
        stUser User;
        while (getline(UsersFile, Line))
        {
            User = ConvertLinetoUserRecord(Line);
            vUsers.push_back(User);
        }
        UsersFile.close();
    }
    return vUsers;
}

void ShowListUsers()
{
    vector <stUser> vUsers = LoadUsersFromFile(UsersFileName);
    cout << "                   Users List (" << vUsers.size() << ") user(s)" << endl;
    cout << "---------------------------------------------------------------------------------------------" << endl;
    cout << "| " << setw(20) << "User Name" << "| " << setw(8) << "Password" << "| " << setw(10) << "Permissions" << "|" << endl;
    cout << "---------------------------------------------------------------------------------------------" << endl;
    if (vUsers.size() > 0)
    {
        for (stUser& User : vUsers)
        {
            cout << "| " << setw(20) << User.name << "| " << setw(8) << User.password << "| " << setw(10) << User.Permissions << "|" << endl;
        }
        cout << "---------------------------------------------------------------------------------------------" << endl;
    }
    else
    {
        cout << "                                             NO Users" << endl;
        cout << "---------------------------------------------------------------------------------------------" << endl;
    }

}

void ShowManageUsersMenu();

void GoBackToManageUsersMenu()
{
    cout << "\n\nPress any key to go back to Manage users Menu...\n";
    system("pause>0");
    system("cls");
    ShowManageUsersMenu();
}

void AddUsers()
{
    cout << "--------------------------\n";
    cout << "     ADD NEW User\n";
    cout << "--------------------------\n";
    stUser User;
    char AddMore = 'y';
    fstream UsersFile;
    UsersFile.open(UsersFileName, ios::out | ios::app);
    if (UsersFile.is_open())
    {
        while (AddMore == 'y' || AddMore == 'Y')
        {
            cout << "Adding new user :" << endl;
            User = ReadUserData();
            UsersFile << ConvertUserRecordToLine(User, "#//#") << "\n";
            cout << "User added succesfully, do you want to add more Users? (y/n) : ";
            cin >> AddMore;
        }
        UsersFile.close();
    }
}

bool isUserFound(vector <stUser> vUsers, string Username, stUser& User)
{
    for (stUser& U : vUsers)
    {
        if (U.name == Username)
        {
            User = U;
            return true;
        }
    }
    return false;
}

void ShowUserCard(stUser User)
{
    cout << "The following are the client details" << endl;
    cout << "Username    : " << User.name << endl;
    cout << "Password    : " << User.password << endl;
    cout << "Permissions : " << User.Permissions << endl;
}

bool MarkToDeleteUser(vector <stUser>& vUsers, string Username)
{
    for (stUser& U : vUsers)
    {
        if (U.name == Username)
        {
            U.isMarkedToDelete = true;
            return true;
        }
    }
    return false;
}

vector <stUser> SaveDataUsersToFile(string FileName, vector <stUser> vUsers)
{
    fstream UsersFile;
    UsersFile.open(FileName, ios::out);
    string DataLine;
    if (UsersFile.is_open())
    {
        for (stUser User : vUsers)
        {
            if (User.isMarkedToDelete == false)
            {
                DataLine = ConvertUserRecordToLine(User, "#//#");
                UsersFile << DataLine << endl;
            }
        }
        UsersFile.close();
    }
    return vUsers;
}

bool DeleteUserByUsername(string Username, vector <stUser>& vUsers)
{
    char Answer = 'n';

    if (isUserFound(vUsers, Username, CurrentUser))
    {
        ShowUserCard(CurrentUser);
        cout << "\n\nAre you sure you want delete this user? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkToDeleteUser(vUsers, Username);
            SaveDataUsersToFile(UsersFileName, vUsers);

            //Refresh Clients 
            vUsers = LoadUsersFromFile(UsersFileName);

            cout << "\n\nUser Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nUser with username (" << Username << ") is Not Found!";
        return false;
    }
}

void DeleteUsers()
{
    cout << "--------------------------\n";
    cout << "     DELETE USER\n";
    cout << "--------------------------\n";
    vector <stUser> vUsers = LoadUsersFromFile(UsersFileName);
    string UserName = ReadString("please enter Username ? ");
    while (UserName == "Admin")
    {
        cout << "You can't delete Admin\n";
        UserName = ReadString("please enter Username ? ");
    }
    DeleteUserByUsername(UserName, vUsers);
}

void UpdateFileWithUpdatedUser(vector <stUser>& vUsers)
{
    fstream UsersFile;
    UsersFile.open(UsersFileName, ios::out);
    if (UsersFile.is_open())
    {
        for (stUser& U : vUsers)
            UsersFile << ConvertUserRecordToLine(U, "#//#") << endl;
        UsersFile.close();
    }
}

void UpdateUserByUsername()
{
    cout << "--------------------------\n";
    cout << "     UPDATE User\n";
    cout << "--------------------------\n";
    vector <stUser> vUsers = LoadUsersFromFile(UsersFileName);
    string Username = ReadString("please enter Username ? ");
    while (Username == "Admin")
    {
        cout << "You can't update Admin\n";
        Username = ReadString("please enter Username ? ");
    }
    CurrentUser.name = Username;
    if (isUserFound(vUsers, Username, CurrentUser))
    {
        ShowUserCard(CurrentUser);
        char upd;
        cout << "Do you want to update this user? (y/n) : ";
        cin >> upd;
        if (upd == 'y' || upd == 'Y')
        {
            for (stUser& U : vUsers)
            {
                if (U.name == Username)
                {
                    ReReadUserData(U);
                    break;
                }
            }
        }
        cout << "User data updated succesfully !" << endl;
        UpdateFileWithUpdatedUser(vUsers);
    }
}

bool FindUser()
{
    cout << "--------------------------\n";
    cout << "     FIND User\n";
    cout << "--------------------------\n";
    vector <stUser> vUsers = LoadUsersFromFile(UsersFileName);
    string username = ReadString("please enter username ? ");
    if (isUserFound(vUsers, username, CurrentUser))
    {
        ShowUserCard(CurrentUser);
        return true;
    }
    else
    {
        cout << "User with username (" << username << ") Not found !" << endl;
        return false;
    }
}

void PerformManageUsersMenu(enManageUsersMenuOption ManageUsersMenuOption)
{
    switch (ManageUsersMenuOption)
    {
    case enManageUsersMenuOption::eListUsers:
        system("cls");
        ShowListUsers();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOption::eAddNewUser:
        system("cls");
        AddUsers();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOption::eDeleteUser:
        system("cls");
        DeleteUsers();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOption::eUpdateUser:
        system("cls");
        UpdateUserByUsername();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOption::eFindUser:
        system("cls");
        FindUser();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOption::eeMainMenu:
        system("cls");
        ShowMainMenuScreen();
        break;
    }
}

void ShowManageUsersMenu()
{
    if (!CheckAccessPermission(enPermissions::pManageUsers))
    {
        ShowAccessDeniedMessage();
        return;
    }
    cout << "=============================\n";
    cout << "\tManage Users Menu\n";
    cout << "=============================\n";
    cout << "\t[1] List Users\n";
    cout << "\t[2] Add New User\n";
    cout << "\t[3] Delete User\n";
    cout << "\t[4] Update User\n";
    cout << "\t[5] Find User\n";
    cout << "\t[6] Main Menu\n";
    cout << "=============================\n";
    PerformManageUsersMenu((enManageUsersMenuOption)ReadNumber("Choose what do you want to do [1 to 6]? "));
}

void Login();

bool CheckAccessPermission(enPermissions Permission)
{
    if (CurrentUser.Permissions == enPermissions::pFullAccess)
        return true;
    if ((Permission & CurrentUser.Permissions) == Permission)
        return true;
    else
        return false;
}

void ShowAccessDeniedMessage()
{
    cout << "--------------------------\n";
    cout << "Access Denied,\nYou don't have permission to do this,\nPlease contact your admin.\n";
    cout << "--------------------------\n";
}

void PerformMainMenue(enMainMenuOption MenuOption)
{
    switch (MenuOption)
    {
    case enMainMenuOption::eShowClientsList:
        system("cls");
        ShowClientsData();
        GoBackToMainMenue();
        break;
    case enMainMenuOption::eAddNewClient:
        system("cls");
        AddClients();
        GoBackToMainMenue();
        break;
    case enMainMenuOption::eDeleteClient:
        system("cls");
        Delete_Client();
        GoBackToMainMenue();
        break;
    case enMainMenuOption::eUpdateClientInfo:
        system("cls");
        UpdateClientByAccountNumber();
        GoBackToMainMenue();
        break;
    case enMainMenuOption::eFindClient:
        system("cls");
        Find_Client();
        GoBackToMainMenue();
        break;
    case enMainMenuOption::eTransactions:
        system("cls");
        ShowTransactionsMenue();
        GoBackToMainMenue();
        break;
    case enMainMenuOption::eManageUsers:
        system("cls");
        ShowManageUsersMenu();
        GoBackToMainMenue();
        break;
    case enMainMenuOption::eLogout:
        system("cls");
        Login();
        break;
    }
}

bool isUserFound(vector <stUser> vUsers, string UserName, string password, stUser& User)
{
    for (stUser &U : vUsers)
    {
        if (U.name == UserName && U.password == password)
        {
            User = U;
            return true;
        }
    }
    return false;
}

void Login()
{
    cout << "--------------------------\n";
    cout << "     LOGIN \n";
    cout << "--------------------------\n";
    vector <stUser> vUsers = LoadUsersFromFile(UsersFileName);
    string UserName, password;
    cout << "Enter username" << endl;
    cin >> UserName;
    cout << "Enter password" << endl;
    cin >> password;
    while (!isUserFound(vUsers, UserName, password, CurrentUser))
    {
        system("cls");
        cout << "--------------------------\n";
        cout << "     LOGIN \n";
        cout << "--------------------------\n";
        cout << "Invalid username/password!" << endl;        cout << "Enter username" << endl;
        cin >> UserName;
        cout << "Enter password" << endl;
        cin >> password;
    }
    system("cls");
    ShowMainMenuScreen();
}

int main()
{
    Login();
    return 0;
}