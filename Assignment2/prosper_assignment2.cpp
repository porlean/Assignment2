/* File Name: prosper.assignment2.cpp
   Author: Orlean Prosper
   Email: oprosper2022@fau.edu
   Assignment Number: 2
   Descrition: A program that will calculate the amortization of a loan, then create a graph that outputs the balance, interest, and principle 
				every month for the full term of the loan. 
   Last changed: 06/15/2023
*/
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

// A struct to hold the total calculations needed for the graph
struct total_amount
{
	double total_payments;
	double total_interest;
};

// A struct needed to hold the account information
struct account
{
	double interest;
	double balance;
	int n;
	double r;
	double principle;
	total_amount end_of_loan;
};

// Function prototypes
double amortization(double p, double r, int n);
void calculate_interest(account& account, double m);
void input_account(account& account);
void calculate_total(total_amount& total, account& account, int iterate, double n, double m);
void Greeting();
void print_output(account& account, double m, ofstream& file_output, int n);
void check_input(double& account);
void check_input(int& account);

// reference: cplusplus.com/forum/beginner/181119/
const string column = " |";
const string row = column + string(87, '-') + '|';

int main()
{
	account my_account;
	ofstream file_output;

	double m = 0.0, monthly_r = 0.0;
	Greeting();
	int option = 0;
	my_account.principle = 0;
	do
	{
		// Displaying the menu options
		cout << "\n--------------------\n"
			<< "\nSelect an option\n"
			<< "(1) Enter information\n"
			<< "(2) Quit\n"
			<< "Your choice: ";
		cin >> option;

		switch (option)
		{
			case 1:
				//reference-lecture09 code
				file_output.open("outfile.dat");
				if (file_output.fail())
				{
					cout << "Output file opening failed.\n";
					exit(1);
				}

				// Getting user input and calculating the amortization of the loan then storing the values in a struct
				input_account(my_account);
				monthly_r = (my_account.r / 100) / 12;
				m = amortization(my_account.balance, monthly_r, my_account.n);
				my_account.end_of_loan.total_payments = m;
				my_account.end_of_loan.total_interest = 0;

				// This is the first row of the graph
				cout << row << endl << column
					<< setw(20) << "Beginning Balance" << column
					<< setw(20) << "Interest" << column
					<< setw(20) << "Principal" << column
					<< setw(20) << "Ending Balance" << column << endl << row << endl;
				file_output << row << endl << column
					<< setw(20) << "Beginning Balance" << column
					<< setw(20) << "Interest" << column
					<< setw(20) << "Principal" << column
					<< setw(20) << "Ending Balance" << column << endl << row << endl;

				// A loop that will populate a graph that will display the different account information for each month on each row
				// Also calculates the total payments and total interest after the end of the loan
				for (int i = 0; i < my_account.n; i++)
				{
					print_output(my_account, m, file_output, i+1);
					calculate_total(my_account.end_of_loan, my_account, i, m, my_account.n);
				}
				cout << row;
				file_output << row;

				// Prints the total interest and payments to the end of the graph
				cout << "\n\n" << "Payment Every Month\t$" << m << endl
					<< "Total 24 Payments\t$" << my_account.end_of_loan.total_payments << endl
					<< "Total Interest\t$" << my_account.end_of_loan.total_interest << endl;
				file_output << "\n\n" << "Payment Every Month\t$" << m << endl
					<< "Total" << " " << my_account.n << " Payments\t$" << my_account.end_of_loan.total_payments << endl
					<< "Total Interest\t$" << my_account.end_of_loan.total_interest << endl;

				break;
			case 2:
				cout << "\nEnding the program. Have a nice day!\n";
				break;
			default:
				check_input(option);
				cout << endl << "Please enter a valid menu option: 1 or 2" << endl;
				break;
		}

		// close the output file so it can be updated
		file_output.close();

	} while (option != 2);
	return 0;




}

// A function that will calculate the amorticaion if the loan
double amortization(double p, double r, int n)
{
	double payment_amount;
	payment_amount = (p * r * pow(1 + r, n)) / (pow(1 + r, n) - 1);
	return payment_amount;
}

// A fuction that will calculate and update the monthly interest rate and balance. 
void calculate_interest(account& account, double m)
{
	account.interest = (0.1 * account.balance) / 12;
	account.balance -= (m - account.interest);
}

void calculate_total(total_amount& total, account& account, int iterate, double n, double m)
{
	total.total_interest += account.interest;
	
	// Makes sure this calculation is done only once
	if (iterate == 0)
	{
		total.total_payments = m * n;
	}
}

// A function that gets the balance, interest rate, and loan term from the user
void input_account(account& account)
{
	cout << "\nWhat is your desired loan amount (max $99,999): $";
	cin >> account.balance;
	check_input(account.balance);
	cout << "What is your annual interest rate (in percent): ";
	cin >> account.r;
	check_input(account.r);
	cout << "What is the term of the loan (in months): ";
	cin >> account.n;
	check_input(account.n);
}

// A greeting function
void Greeting()
{
	cout << "Welcome to the loan calculator. All of the payment information for your loan will be generated below\n"
		<< "Please tell me about your loan\n"
		<< "\n\n-------------------------------------------------------------------------------------------------\n\n";
}

// A function that prints out a graph in an organized format
void print_output(account& account, double m, ofstream& file_output, int n)
{
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(2);
	file_output.setf(ios::fixed);
	file_output.setf(ios::showpoint);
	file_output.precision(2);

	if (n < 10)
	{
		cout << column << setw(3) << " " << n << column;
		file_output << column << setw(3) << " " << n << column;
	}
	else
	{
		cout << column << setw(2) << " " << n << column;
		file_output << column << setw(2) << " " << n << column;
	}

	if (account.balance >= 10000)
	{
		cout << setw(6) << " " << account.balance << column;
		file_output << setw(6) << " " << account.balance << column;
	}
	else if (account.balance < 10000 && account.balance >= 1000)
	{
		cout << setw(7) << " " << account.balance << column;
		file_output << setw(7) << " " << account.balance << column;
	}
	else if (account.balance < 1000 && account.balance >= 100)
	{
		cout << setw(8) << " " << account.balance << column;
		file_output << setw(8) << " " << account.balance << column;
	}
	else if (account.balance < 100 && account.balance >= 10)
	{
		cout << setw(9) << " " << account.balance << column;
		file_output << setw(9) << " " << account.balance << column;
	}
	else
	{
		cout << setw(10) << " " << account.balance << column;
		file_output << setw(10) << " " << account.balance << column;
	}

	calculate_interest(account, m);
	account.principle = m - account.interest;

	if (account.interest >= 100)
	{
		cout << setw(14) << "" << account.interest << column;
		file_output << setw(14) << " " << account.interest << column;
	}
	else if (account.interest < 100 && account.interest >= 10)
	{
		cout << setw(15) << "" << account.interest << column;
		file_output << setw(15) << " " << account.interest << column;
	}
	else if (account.interest < 10 && account.interest >= 0)
	{
		cout << setw(16) << "" << account.interest << column;
		file_output << setw(16) << " " << account.interest << column;
	}
	else
	{
		cout << setw(13) << "" << account.interest << column;
		file_output << setw(13) << " " << account.interest << column;
	}

	if (account.principle >= 100)
	{
		cout << setw(14) << " " << account.principle << column;
		file_output << setw(14) << " " << account.principle << column;
	}
	else if (account.principle < 100 && account.principle >= 10)
	{
		cout << setw(15) << " " << account.principle << column;
		file_output << setw(15) << " " << account.principle << column;
	}
	else if (account.principle < 10 && account.principle >= 0)
	{
		cout << setw(16) << " " << account.principle << column;
		file_output << setw(16) << " " << account.principle << column;
	}
	else
	{
		cout << setw(13) << " " << account.principle << column;
		file_output << setw(13) << " " << account.principle << column;
	}

	if (account.balance >= 10000)
	{
		cout << setw(12) << " " << account.balance << column;
		file_output << setw(12) << " " << account.balance << column;
	}
	else if (account.balance < 10000 && account.balance >= 1000)
	{
		cout << setw(13) << " " << account.balance << column;
		file_output << setw(13) << " " << account.balance << column;
	}
	else if (account.balance < 1000 && account.balance >= 100)
	{
		cout << setw(14) << " " << account.balance << column;
		file_output << setw(14) << " " << account.balance << column;
	}
	else if (account.balance < 100 && account.balance >= 10)
	{
		cout << setw(15) << " " << account.balance << column;
		file_output << setw(15) << " " << account.balance << column;
	}
	else if (account.balance <= 0)
	{
		cout << setw(19) << " " << 0 << column;
		file_output << setw(19) << " " << 0 << column;
	}
	else
	{
		cout << setw(16) << " " << account.balance << column;
		file_output << setw(16) << " " << account.balance << column;
	}
	cout << endl; 
	file_output << endl;
}

void check_input(double& account)
{
	//Checks to make sure the input is a positive number and not a character
	while (account < 0 || cin.fail() || account > 99999)
	{
		//reference: stackoverflow.com/questions/11523569/how-can-i-avoid-char-input-for-an-int-variable
		cin.clear();
		cin.ignore(100, '\n');
		cout << "Invalid entry. Please input again: ";
		cin >> account;
	}
}

void check_input(int& account)
{
	//Checks to make sure the input is a positive number and not a character
	while (account < 0 || cin.fail() || account > 99999)
	{
		//reference: stackoverflow.com/questions/11523569/how-can-i-avoid-char-input-for-an-int-variable
		cin.clear();
		cin.ignore(100, '\n');
		cout << "Invalid entry. Please input again: ";
		cin >> account;
	}
}