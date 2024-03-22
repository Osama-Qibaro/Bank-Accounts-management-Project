#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <fstream>
#include <iomanip>
#include "MyInputLib.h"
using namespace MyInputLib;
using namespace std;

struct stClientInfos {
	string AccountNumber,
		PinCode,
		Name,
		PhoneNumber;
	float Balance = 0.0;
	bool MarkDelete = false;
};

void ShowScreenLable(string ScreenLable) {
	cout << "\t-------------------------------\n";
	cout << "\t";
	cout << setw(15 + ScreenLable.length() / 2) << ScreenLable << endl;
	cout << "\t-------------------------------\n\n";
}

void ShowHeader(int NumberOfClients) {
	cout << endl;
	cout << setw(44) << "Client List (";
	cout << NumberOfClients << ")" << endl;
	cout << "___________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(17) << "Account Number";
	cout << "| " << left << setw(11) << "Pin Code";
	cout << "| " << left << setw(33) << "Client Name";
	cout << "| " << left << setw(13) << "Phone Number";
	cout << "| " << left << setw(9) << "Balance" << endl;
	cout << "--------------------------------------------------------------------------------------------\n";

}
void ShowOneClientInfos(stClientInfos CI) {
	cout << "| " << left << setw(17) << CI.AccountNumber;
	cout << "| " << left << setw(11) << CI.PinCode;
	cout << "| " << left << setw(33) << CI.Name;
	cout << "| " << left << setw(13) << CI.PhoneNumber;
	cout << "| " << left << setw(9) << CI.Balance << endl;
}
void ShowOneClientCard(stClientInfos CI) {
	cout << "\n--------------------------------------------------------------------------------------------\n";
	cout << "| " << left << setw(17) << "Account Number";
	cout << "| " << left << setw(11) << "Pin Code";
	cout << "| " << left << setw(33) << "Client Name";
	cout << "| " << left << setw(13) << "Phone Number";
	cout << "| " << left << setw(9) << "Balance" << endl;
	cout << "--------------------------------------------------------------------------------------------\n";
	ShowOneClientInfos(CI);
	cout << endl;
}
void ShowAllClientsInfos(vector <stClientInfos>& vCIs) {
	for (stClientInfos& CI : vCIs) {
		ShowOneClientInfos(CI);
	}
	cout << "--------------------------------------------------------------------------------------------\n\n";
}

vector<string> LoadDataFromFile(string FilePath) {
	vector<string> vData;
	string Line;
	fstream File;
	
	File.open(FilePath, ios::in);

	if(File.is_open())
	{
		while (getline(File, Line)) {
			if (Line != "")
				vData.push_back(Line);
		}
	}
	
	File.close();

	return vData;
}
vector<string> SplitData(string Line, string delim) {
	vector <string> vLine;
	string Sub;
	size_t pos;

	while ((pos = Line.find(delim)) != string::npos) {
		Sub = Line.substr(0, pos);
		if (Sub != "")
			vLine.push_back(Sub);
		Line.erase(0, pos + delim.length());
	}
	if (Line != "")
		vLine.push_back(Line);

	return vLine;
}
stClientInfos ConvertLineToRecord(vector<string> vOneClientInfos) {
	stClientInfos OneClientInfos;
	OneClientInfos.AccountNumber = vOneClientInfos.at(0);
	OneClientInfos.PinCode = vOneClientInfos.at(1);
	OneClientInfos.Name = vOneClientInfos.at(2);
	OneClientInfos.PhoneNumber = vOneClientInfos.at(3);
	OneClientInfos.Balance = stof(vOneClientInfos.at(4));

	return OneClientInfos;
}
vector<stClientInfos> LoadCIsFromFile(string FilePath) {

	vector<string> vData = LoadDataFromFile(FilePath);
	vector<stClientInfos> vCIs;

	for (string& Line : vData) {
		vCIs.push_back(ConvertLineToRecord(SplitData(Line, "#$#@")));
	}

	return vCIs;
}

bool isFindAccount(vector<stClientInfos> vCIs, string AccountNumber , stClientInfos &FindedCI) {
	for (stClientInfos& CI : vCIs) {
		if (CI.AccountNumber == AccountNumber)
		{
			FindedCI = CI;
			return true;
		}
	}
	return false;
}
bool isFindAccount(string AccountNumber) {
	fstream File;
	string Line;
	File.open("file.txt", ios::in);

	if (File.is_open()) {
		while (getline(File, Line)) {
			if (Line.find(AccountNumber) != string::npos)
				return true;
		}
	}
	return false;
}
bool isFindAccount(vector<stClientInfos> vCIs, string AccountNumber) {
	for (stClientInfos& CI : vCIs) {
		if (CI.AccountNumber == AccountNumber) {
			return true;
		}
	}
	return false;
}
void FindClient(vector<stClientInfos> &vCIs) {
	ShowScreenLable("FIND CLIENT SCREEN");
	string AccountNumber;
	ReadString("Enter account number for search: ", AccountNumber);

	stClientInfos FoundedCI;
	if (isFindAccount(vCIs, AccountNumber, FoundedCI)) {
		ShowOneClientCard(FoundedCI);
	}
	else
		cout << "Client account with Number(" << AccountNumber << ") isn't fount.\n\n";
}

string ConvertRecordToLine(stClientInfos CI, string delim = "#$#@") {
	string String = "";

	String += CI.AccountNumber;
	String += delim;
	String += CI.PinCode;
	String += delim;
	String += CI.Name;
	String += delim;
	String += CI.PhoneNumber;
	String += delim;
	String += to_string(CI.Balance);

	return String;
}
void UpdateFileChanges(vector<stClientInfos>& vCIs, string FilePath) {
	fstream File;
	File.open(FilePath, ios::out);

	if (File.is_open()) {
		for (stClientInfos& CI : vCIs) {
			if(CI.MarkDelete == false)
			File << ConvertRecordToLine(CI) << endl;
		}
	}

	File.close();
}
void UpdateClientInfo(vector<stClientInfos>& vCIs) {
	string AccountNumber;
	stClientInfos FoundedCI;

	ShowScreenLable("UPDATE CLIENT ACCOUNT SCREEN");

	ReadString("Enter account number to edite its infos: ", AccountNumber);

	if (isFindAccount(vCIs, AccountNumber, FoundedCI)) {

		
		ShowOneClientCard(FoundedCI);

		if (YesOrNo("\nAre you sure do you want to update this account informations? y/n : "))
		{
			for (stClientInfos& CI : vCIs)
			{
				if (CI.AccountNumber == FoundedCI.AccountNumber && AccountNumber == FoundedCI.AccountNumber)
				{
					cout << endl;
					cout << "Enter pin code: ";
					getline(cin >> ws, CI.PinCode);
					ReadString("Enter name: ", CI.Name);
					ReadString("Enter Phone number: ", CI.PhoneNumber);
					ReadNumber("Enter balance: ", CI.Balance);
					FoundedCI.PinCode = CI.PinCode;
					FoundedCI.Name = CI.Name;
					FoundedCI.PhoneNumber = CI.PhoneNumber;
					FoundedCI.Balance = CI.Balance;

					break;
				}
			}
			cout << "Account updated successfully.\n\n";
		}
	}
	else
		cout << "Account with number (" << AccountNumber << ") isn't found.\n\n";


}

void MarkToDelete(vector<stClientInfos> &vCIs ,stClientInfos & CI) {
	for (stClientInfos& CI1 : vCIs) {
		if (CI1.AccountNumber == CI.AccountNumber)
			CI1.MarkDelete = true;
	}
}
void DeleteAccount(vector<stClientInfos>& vCIs) {
	stClientInfos CI;
	string AccountNumber;

	ShowScreenLable("DELETE CLIENT ACCOUNT SCREEN");

	ReadString("Enter Account number: ", AccountNumber);
	if (isFindAccount(vCIs, AccountNumber, CI)) {
		cout << endl;
		ShowOneClientCard(CI);

		if (YesOrNo("\nAre you sure to delete this account? y/n : ")) {
			MarkToDelete(vCIs,CI);

			UpdateFileChanges(vCIs, "file.txt");

			if (isFindAccount(AccountNumber))
				cout << "Filed to delete this account.\n";
			else
				cout << "Account deleted successfully.\n";
		}
	}
	else
		cout << "Client with account number (" << AccountNumber << ") isn't found.\n\n";
}

void ReadNewClient(stClientInfos &CI) {
	
	ReadString("Enter pin code: ", CI.PinCode);
	ReadString("Enter client name: ", CI.Name);
	ReadString("Enter phone number: ", CI.PhoneNumber);
	ReadNumber("Enter balance: ", CI.Balance);

}
void AddNewClients(vector <stClientInfos>& vCIs) {
	stClientInfos CI;
	
	ShowScreenLable("ADD NEW CLINET SCREEN");
	do {

		cout << "Enter Account number: ";
		getline(cin >> ws, CI.AccountNumber);

		if (isFindAccount(CI.AccountNumber)) {
			cout << "\nSorry, but this account (" << CI.AccountNumber << ") is already exists.\n";
		}
		else {
			system("cls");
			ReadNewClient(CI);
			vCIs.push_back(CI);
		}

	} while (YesOrNo("Do you want to add more clients? y/n : "));

}

void Deposit(vector<stClientInfos> &vCIs) {
	float DepositAmount;
	string AccountNumber;
	stClientInfos FoundedCI;
	ShowScreenLable("Deposit Screen");

	cout << "Enter Account number: ";
	cin >> AccountNumber;

	while (!isFindAccount(vCIs, AccountNumber)) {
		cout << "The Account with number(" << AccountNumber << ") doesn't exist.\n";

		cout << "Enter Account number: ";
		cin >> AccountNumber;
	}


	for (stClientInfos& CI : vCIs) {
		if (AccountNumber == CI.AccountNumber) {
			cout << "\nThese are account details:\n";
			ShowOneClientCard(CI);
			
			if (YesOrNo("Are you sure do you want to preform this transaction? y/n : "))
			{
				ReadNumber("\nEnter deposit amount: ", DepositAmount);
				cout << endl;
				CI.Balance += DepositAmount;
				break;
			}
		}
	}
}
void Withdraw(vector<stClientInfos> &vCIs) {
	float WithdrawAmount;
	string AccountNumber;
	stClientInfos FoundedCI;
	ShowScreenLable("Withdraw Screen");

	cout << "Enter Account number: ";
	cin >> AccountNumber;

	while (!isFindAccount(vCIs, AccountNumber)) {
		cout << "The Account with number(" << AccountNumber << ") doesn't exist.\n";

		cout << "Enter Account number: ";
		cin >> AccountNumber;
	}


	for (stClientInfos& CI : vCIs) {
		if (AccountNumber == CI.AccountNumber) {
			cout << "\nThese are account details:\n";
			ShowOneClientCard(CI);

			if (YesOrNo("Are you sure do you want to preform this transaction? y/n : "))
			{
				ReadNumber("\nEnter Withdraw amount: ", WithdrawAmount);
				if (CI.Balance < WithdrawAmount)
					cout << "Withdraw amount is begger than your balance.\n\n";
				else
				{
					CI.Balance -= WithdrawAmount;
					cout << "Withdraw done successfully.\n\n";
				}

				break;
			}
		}
	}
}
double TolatBalance(vector<stClientInfos>& vCIs) {
	double balance = 0;
	for (stClientInfos& CI : vCIs) {
		balance += CI.Balance;
	}
	return balance;
}
void ShowTotalBalance(vector<stClientInfos>& vCIs) {
	ShowAllClientsInfos(vCIs);
	cout << "Total balance: ";
	cout << TolatBalance(vCIs) << endl;
}

bool TransactoinFunctions(short);
void TransactionScreen();

void MainMenue();
bool FunctionsList(short);


int main() {
	MainMenue();
}


void MainMenue() {
	short Choise;

	cout << " ============================================\n";
	cout << " |                Main Menue                |\n";
	cout << " ============================================\n";
	cout << " |  [1] Show clients list.                  |\n";
	cout << " |  [2] Add new client.                     |\n";
	cout << " |  [3] Delete client.                      |\n";
	cout << " |  [4] Update client info.                 |\n";
	cout << " |  [5] Find client.                        |\n";
	cout << " |  [6] Transaction.                        |\n";
	cout << " |  [0] Exite.                              |\n";
	cout << " ============================================\n";

	ReadShortNumBetween("Choose what you want to do [0 to 6]? ",Choise, 0, 6);
	FunctionsList(Choise);
}
bool FunctionsList(short Choise) {
	vector <stClientInfos> vCIs = LoadCIsFromFile("file.txt");
	system("cls");

	switch (Choise) {

	case 1 :
		ShowHeader(vCIs.size());
		ShowAllClientsInfos(vCIs);
		break;

	case 2 :
		AddNewClients(vCIs);
		break;

	case 3 :
		DeleteAccount(vCIs);
		break;

	case 4 :
		UpdateClientInfo(vCIs);
		break;

	case 5:
		FindClient(vCIs);
		break;
	case 6:
		TransactionScreen();
		break;

	default:
		return true;
	}

	UpdateFileChanges(vCIs, "file.txt");
	cout << "Press Any key to continue to Main Menue...";
	system("pause>0");
	system("cls");
	MainMenue();
}


void TransactionScreen() {
	short choise;

	cout << " ============================================\n";
	cout << " |           Transaction Screen             |\n";
	cout << " ============================================\n";
	cout << " |  [1] Deposit.                            |\n";
	cout << " |  [2] Withdraw.                           |\n";
	cout << " |  [3] Total balance.                      |\n";
	cout << " |  [4] Main menue.                         |\n";
	cout << " ============================================\n";
	ReadShortNumBetween("Choose what you want to do [1 to 4] : ", choise, 1,4);

	TransactoinFunctions(choise);
}
bool TransactoinFunctions(short choise) {
	vector<stClientInfos> vCIs = LoadCIsFromFile("file.txt");
	system("cls");

	switch (choise) {
	case 1:
		Deposit(vCIs);
	break;
	
	case 2:
		Withdraw(vCIs);
	break;

	case 3:
		ShowTotalBalance(vCIs);
	break;

	default:
		return true;
	}

	UpdateFileChanges(vCIs, "file.txt");
	cout << "Press any key to continue to transaction screen...";
	system("pause>0");
	system("cls");
	TransactionScreen();
}
