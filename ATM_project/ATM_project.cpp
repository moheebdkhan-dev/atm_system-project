#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include "myStringLib.h"
#include "myInputLib.h"

using namespace std;

// requirements
// 1- quick withdraw.
// 2- normal withdraw.
// 3- deposit.
// 4- check balance.
// 5- logout.
//     - log in by account number and pin code.
//     - store the current client in a global variable to make transactions.
string Delim = "/##/";
string ClientsFile = "clients_records.txt";
struct stClient {
	string AccountNumber;
	string PinCode;
	string Name;
	string PhoneNumber;
	float AccountBalance = 0;
};
stClient CurrentClient;
enum enOptions {
	eQuickWithdraw = 1, eNormalWithdraw = 2, eDeposit = 3, eCheckBalance = 4, eLogout = 5
};
stClient SplittedLineToClient(vector<string> SplittedLine) {
	stClient Client;
	Client.AccountNumber = SplittedLine[0];
	Client.PinCode = SplittedLine[1];
	Client.Name = SplittedLine[2];
	Client.PhoneNumber = SplittedLine[3];
	Client.AccountBalance = stof(SplittedLine[4]);
	return Client;
}
vector<stClient> LoadClientsInfoFromFile(string FileName) {
	vector<stClient> vAllClients;
	fstream myFile;
	myFile.open(FileName, ios::in);
	if (myFile.is_open()) {
		stClient TempClient;
		string Line;
		vector<string> vSplittedLine;
		while (getline(myFile, Line)) {
			if (Line != "") {
				vSplittedLine = myStringLib::SplitString(Line, ::Delim);
				vAllClients.push_back(SplittedLineToClient(vSplittedLine));
			}
		}
	}
	return vAllClients;
}
void ShowATMMenu();
void PrintCurrentScreenHeader(string currentScreen) {
	cout << "=============================================\n";
	cout << "\t\t" << currentScreen << " Screen\n";
	cout << "=============================================\n";
}
void GoBackToATMMenu() {
	cout << "\nPress any key to go back to main menu...";
	system("pause>0");
	ShowATMMenu();
}
string ClientToLine(stClient Client) {
	string ClientLine = "";
	ClientLine.append(Client.AccountNumber + ::Delim);
	ClientLine.append(Client.PinCode + ::Delim);
	ClientLine.append(Client.Name + ::Delim);
	ClientLine.append(Client.PhoneNumber + ::Delim);
	ClientLine.append(to_string(Client.AccountBalance));
	return ClientLine;
}// Quick withdraw
void PrintWithdrawMenu() {
	system("cls");
	cout << "=================================================\n";
	cout << "\t\tQuick Withdraw Menu Screen\n";
	cout << "=================================================\n";
	cout << "\t [1] 20 \t [2] 50\n";
	cout << "\t [3] 100\t [4] 200\n";
	cout << "\t [5] 400\t [6] 600\n";
	cout << "\t [7] 800\t [8] 1000\n";
	cout << "\t [9] Exit\n";
	cout << "=================================================\n";
}
void SaveClientsInfoToFile(vector<stClient> vClients, string FileName) {
	fstream myFile;
	myFile.open(FileName, ios::out);
	if (myFile.is_open()) {
		for (stClient& Client : vClients) {
			myFile << ClientToLine(Client) << endl;
		}
		myFile.close();
	}
}
void SaveCurrentClientUpdates() {
	vector<stClient> vAllClients = LoadClientsInfoFromFile(::ClientsFile);
	for (stClient& C : vAllClients) {
		if (C.AccountNumber == ::CurrentClient.AccountNumber && C.PinCode == ::CurrentClient.PinCode) {
			C = ::CurrentClient;
			break;
		}
	}
	SaveClientsInfoToFile(vAllClients, ::ClientsFile);
}
// Normal withdraw
void NormalWithdraw() {
	PrintCurrentScreenHeader("Normal Withdraw");
	cout << "\nYour balance is " << ::CurrentClient.AccountBalance << endl;
	short AmountToWithdraw;
	do {
		AmountToWithdraw = myInputLib::ReadInteger("Enter the amount to withdraw which is multiple's of 5 ");
	} while (AmountToWithdraw % 5 != 0);
	if (AmountToWithdraw > ::CurrentClient.AccountBalance) {
		cout << "The amount exceeds your balance\n";
		return;
	}
	else {
		char Answer = myStringLib::ReadChar("Are you sure you want to perform this transaction? y/n ");
		if (tolower(Answer) == 'y') {
			::CurrentClient.AccountBalance -= AmountToWithdraw;
			SaveCurrentClientUpdates();
			cout << "Done successfuly, New balance is " << ::CurrentClient.AccountBalance << endl;
		}
	}
}
// Quick Withdraw
void QuickWithdraw() {
	PrintCurrentScreenHeader("Quick Withdraw");
	PrintWithdrawMenu();
	cout << "\nYour balance is " << ::CurrentClient.AccountBalance << endl;
	short WithdrawAmount[8] = { 20,50,100,200,400,600,800,1000 };
	short AmountToWithdraw = myInputLib::ReadInteger("Choose what to do from? [1 - 9] ");
	if (AmountToWithdraw == 9) {
		ShowATMMenu();
		return;
	}
	if (WithdrawAmount[AmountToWithdraw - 1] <= ::CurrentClient.AccountBalance) {
		char SureToWithdraw = myStringLib::ReadChar("Are you sure you want to perform this transaction? y/n ");
		if (tolower(SureToWithdraw) == 'y') {
			::CurrentClient.AccountBalance -= WithdrawAmount[AmountToWithdraw - 1];
			SaveCurrentClientUpdates();
			cout << "Done successfuly, New balance is " << ::CurrentClient.AccountBalance << endl;
		}
	}
	else {
		cout << "The amount exceeds your balance\n";
	}
}
// Deposit
void Deposit() {
	PrintCurrentScreenHeader("Deposit");
	cout << "\nYour balance is " << ::CurrentClient.AccountBalance << endl;
	int AmountToDeposit = myInputLib::ReadFloat("Enter the amount you want to deposit? ");
	char Answer = myStringLib::ReadChar("Are you sure you want to perform this transaction? y/n ");
	if (Answer == 'y' || Answer == 'Y') {
		::CurrentClient.AccountBalance += AmountToDeposit;
		SaveCurrentClientUpdates();
		cout << "Done successfuly, New balance is " << ::CurrentClient.AccountBalance << endl;
	}
}
// Check balance
void CheckBalance() {
	PrintCurrentScreenHeader("Check Balance");
	cout << "\nYour balance is " << ::CurrentClient.AccountBalance << endl;
}


void Login();
bool IsClientFoundByAccNumberPinCode(string AccNumber, string PinCode, stClient& FoundClient) {
	vector<stClient> vAllClients = LoadClientsInfoFromFile(::ClientsFile);
	for (stClient& C : vAllClients) {
		if (C.AccountNumber == AccNumber && C.PinCode == PinCode) {
			FoundClient = C;
			return true;
		}
	}
	return false;
}
void PerformSelectedOption(enOptions SelectedOption) {
	switch (SelectedOption) {
	case enOptions::eQuickWithdraw:
		system("cls");
		QuickWithdraw();
		GoBackToATMMenu();
		break;
	case enOptions::eNormalWithdraw:
		system("cls");
		NormalWithdraw();
		GoBackToATMMenu();
		break;
	case enOptions::eDeposit:
		system("cls");
		Deposit();
		GoBackToATMMenu();
		break;
	case enOptions::eCheckBalance:
		system("cls");
		CheckBalance();
		GoBackToATMMenu();
		break;
	case enOptions::eLogout:
		system("cls");
		Login();
		break;
	}
}
void ShowATMMenu() {
	system("cls");
	cout << "=================================================\n";
	cout << "\t\tATM Menu Screen\n";
	cout << "=================================================\n";
	cout << "\t [1] Quick Withdraw.\n";
	cout << "\t [2] Normal Withraw.\n";
	cout << "\t [3] Deposit.\n";
	cout << "\t [4] Check Balance.\n";
	cout << "\t [5] Logout\n";
	cout << "=================================================\n";
	PerformSelectedOption((enOptions)myInputLib::ReadInteger("Enter what do you want to do? [1 - 5] "));
}
void Login() {
	PrintCurrentScreenHeader("Login");
	string AccNumber, PinCode;
	AccNumber = myStringLib::ReadString("Enter the account number? ");
	PinCode = myStringLib::ReadString("Enter the pin code? ");
	while (!IsClientFoundByAccNumberPinCode(AccNumber, PinCode, ::CurrentClient)) {
		system("cls");
		PrintCurrentScreenHeader("Login");
		cout << "\nInvalid account number/pin code\n\n";
		AccNumber = myStringLib::ReadString("Enter the account number? ");
		PinCode = myStringLib::ReadString("Enter the pin code? ");
	}
	ShowATMMenu();
}
int main()
{
	Login();

	return 0;
}