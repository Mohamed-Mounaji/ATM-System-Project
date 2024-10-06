#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;


//     ===  Structs ===
struct stClientData
{
	string AccountNum = "";
	string PinCode = "";
	string FullName = "";
	string PhoneNum = "";
	double AccountBalance = 0;
	bool MarkedToDelete = false;
};

//     ===  Enums  ===
enum enMainMenuOptions {
	QuickWithdraw = 1, NormalWithdraw = 2, Deposit = 3, CheckBalance = 4, Logout = 5
};

//     ===  Global Variables ===
const string ClientsFileName = "ClientsData.txt";
stClientData CurrentClient;

//     ===  Declarations  ==
void LoginScreen();
void MainMenuScreen();
void QuickWithdrawScreen();

int ReadNumInRange(string Message, int From, int To)
{
	int Num = 0;
	do { 
		cout << Message;
	if (cin >> Num)
	{
		if (Num < From || Num > To)
			continue;
		else
			return Num;

	}
	else
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	} while (Num < From || Num > To);

}

vector <string> SplitLine(string Line, string Delim)
{
	vector <string> vWords;
	short DeliPos = 0;
	string Word = "";
	while ((DeliPos = Line.find(Delim)) != string::npos)
	{
		Word = Line.substr(0, DeliPos);
		if (Word != "")
			vWords.push_back(Word);
		Line.erase(0, DeliPos + Delim.length());
	}
	if (Line != "")
		vWords.push_back(Line);
	return vWords;
}

stClientData ConvertClientLineToRecord(string Line, string Delim = "#//#")
{
	vector <string> vLineInfo = SplitLine(Line, Delim);
	stClientData Client;
	Client.AccountNum = vLineInfo[0];
	Client.PinCode = vLineInfo[1];
	Client.FullName = vLineInfo[2];
	Client.PhoneNum = vLineInfo[3];
	Client.AccountBalance = stod(vLineInfo[4]);
	return Client;
}

vector <stClientData> LoadClientsDataFromFile(string FileName)
{
	vector <stClientData> vClients;
	fstream File;
	File.open(FileName, ios::in);//Read Mode

	if (File.is_open())
	{
		string Line = "";
		while (getline(File, Line))
		{
			if (Line != "")
				vClients.push_back(ConvertClientLineToRecord(Line));
		}
		File.close();
	}
	return vClients;
}

string ConvertClientsRecordToLine(stClientData Client, string Separator = "#//#")
{
	string Text = "";
	Text += Client.AccountNum + Separator;
	Text += Client.PinCode + Separator;
	Text += Client.FullName + Separator;
	Text += Client.PhoneNum + Separator;
	Text += to_string(Client.AccountBalance);
	return Text;
}

void SaveClientsVectorToFile(string FileName, vector <stClientData>& vClients)
{
	fstream File;
	File.open(FileName, ios::out);//Write Mode
	if (File.is_open())
	{
		string Line = "";
		for (stClientData& Client : vClients)
		{
			Line = ConvertClientsRecordToLine(Client);
			if (Line != "")
				File << Line << endl;
		}
		File.close();
	}
}

void SaveClientToFile()
{
	vector <stClientData> vClients = LoadClientsDataFromFile(ClientsFileName);
	for (stClientData& Client : vClients)
	{
		if (Client.AccountNum == CurrentClient.AccountNum)
		{
			Client = CurrentClient;
			break;
		}
	}
	SaveClientsVectorToFile(ClientsFileName, vClients);
}

bool CheckLoginInfo(string AccountNum, string PinCode)
{
	vector <stClientData> vClients = LoadClientsDataFromFile(ClientsFileName);

	for (stClientData &Client : vClients)
	{
		if (Client.AccountNum == AccountNum && Client.PinCode == PinCode)
		{
			CurrentClient = Client;
			return true;
		}
	}
	return false;
}

void PrintMainMenuTable()
{
	cout << "=================================================\n";
	cout << "\t\tATM Main Menu Screen\n";
	cout << "=================================================\n";
	cout << "\t[1] Quick Withdraw.\n";
	cout << "\t[2] Normal Withdraw.\n";
	cout << "\t[3] Deposit.\n";
	cout << "\t[4] Check Your Balance.\n";
	cout << "\t[5] Logout.\n";
	cout << "=================================================\n";
}

enMainMenuOptions ReadMainMenuOption()
{
	return (enMainMenuOptions)ReadNumInRange("Choose what do you want to do [1 to 5] : ", 1, 5);
}

void BackToMainMenu()
{
	cout << "\n\nBack any key to back to main menu . . .";
	system("pause>0");
	MainMenuScreen();
}

void CheckBalanceScreen()
{
	system("cls");
	cout << "=================================================\n";
	cout << "\t\tCheckBalanceScreen\n";
	cout << "=================================================\n";
	cout << "\nYour Balance Is : " << CurrentClient.AccountBalance;
}

void ExceedBalanceCase()
{
	cout << "\nThe amount exceeds the Account Balance, Make another choice.\n";
	cout << "\nPress any key to continue . . . ";
	system("pause>0");
}

void SubtractFromAccountBalance(double Amount)
{
	char Answer = 'n';
	cout << "\nAre you sure you want to perform this transaction y/n : ";
	cin >> Answer;
	if (tolower(Answer) == 'y')
	{
		CurrentClient.AccountBalance -= Amount;
		SaveClientToFile();
		cout << "\nDone Successfully,   New Balance = " << CurrentClient.AccountBalance;
	}
}

short GetQuickWithdrawAmount(short Option)
{
	switch (Option)
	{
	case 1: return 20;
	case 2: return 50;
	case 3: return 100;
	case 4: return 200;
	case 5: return 400;
	case 6: return 600;
	case 7: return 800;
	case 8: return 1000;
	default: return 0;
	}
}

void PerformWithdrawOption(short Option)
{
	if (Option == 9)
		return;

	short WithdrawAmount = GetQuickWithdrawAmount(Option);

	if (WithdrawAmount > CurrentClient.AccountBalance)
	{
		ExceedBalanceCase();
		QuickWithdrawScreen();
	}

	SubtractFromAccountBalance(WithdrawAmount);
}

void PrintQuickWithdrawTable()
{
	cout << "=================================================\n";
	cout << "\t\tQuick Withdraw\n";
	cout << "=================================================\n";
	cout << "\t[1] 20        [2] 50\n";
	cout << "\t[3] 100       [4] 200\n";
	cout << "\t[5] 400       [6] 600\n";
	cout << "\t[7] 800       [8] 1000\n";
	cout << "\t[9] Exit.\n";
	cout << "=================================================\n";
}

void QuickWithdrawScreen()
{
	system("cls");
	PrintQuickWithdrawTable();
	short Option;

	cout << "\nYour Balance is : " << CurrentClient.AccountBalance << endl;
	Option = ReadNumInRange("Choose the amount to withdraw [1 to 9] : ", 1, 9);

	PerformWithdrawOption(Option);
}

void NormalWithdrawScreen()
{
	system("cls");
	cout << "=================================================\n";
	cout << "\t\tNormal Withdraw\n";
	cout << "=================================================\n";
	int Amount;

	do{
	cout << "\nEnter an amount multiple of 5's : ";
	cin >> Amount;
	} while (Amount % 5 != 0);

	if (Amount > CurrentClient.AccountBalance)
	{
		ExceedBalanceCase();
		NormalWithdrawScreen();
	}
	SubtractFromAccountBalance(Amount);
}

void DepositScreen()
{
	system("cls");
	cout << "=================================================\n";
	cout << "\t\tDeposit Screen\n";
	cout << "=================================================\n";

	int Amount = 0;
	do
	{
		cout << "\nEnter a positive deposit amount : ";
		cin >> Amount;
	} while (Amount < 1);

	SubtractFromAccountBalance(-Amount);
}

void PerformMainMenuOption(enMainMenuOptions Option)
{
	switch (Option)
	{
	case enMainMenuOptions::QuickWithdraw:
		QuickWithdrawScreen();
		BackToMainMenu();
		break;

	case enMainMenuOptions::NormalWithdraw:
		NormalWithdrawScreen();
		BackToMainMenu();
		break;
	case enMainMenuOptions::Deposit:
		DepositScreen();
		BackToMainMenu();
		break;

	case enMainMenuOptions::CheckBalance:
		CheckBalanceScreen();
		BackToMainMenu();
		break;

	case enMainMenuOptions::Logout:
		LoginScreen();
	}
}

void MainMenuScreen()
{
	system("cls");
	PrintMainMenuTable();
	enMainMenuOptions UserChoice = ReadMainMenuOption();
	PerformMainMenuOption(UserChoice);
}

void LoginScreen()
{
	bool LoginFailed = false;
	string AccountNum = "";
	string PinCode = "";
	do
	{
		system("cls");
		cout << "_____________________________________\n\n";
		cout << "\tLogin Screen\n";
		cout << "_____________________________________\n";
		if (LoginFailed)
			cout << "\nInvalid UserName/Password!!\n";
		cout << "\nPlease, Enter the Account Number : ";
		getline(cin >> ws, AccountNum);
		cout << "\nPlease, Enter the Pin Code : ";
		getline(cin, PinCode);

		LoginFailed = !CheckLoginInfo(AccountNum, PinCode);			
	} while (LoginFailed);

	MainMenuScreen();
}

int main()
{
	LoginScreen();

	system("pause>0");
	return 0;
}