#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
#include <vector>

using namespace std;

void linearFit(const vector<double>, const vector<double>, int);
void powerLawFit(vector<double>, vector<double>, int);
void polate(int, double, double);
void fileRead(vector<double>&, vector<double>&, int&);

/******************
Function name: linearFit
Purpose: Calculate m and c (slope and y intercept) of the linear line best representing the data points
Function In parameters: x and y datapoints, and the number of datapoints (n)
Function Out parameters: none
Version: 1.0
******************/

void linearFit(const vector<double> x, const vector<double> y, const int n) {
	/* number of datapoints */
	double S = n;
	/* x_1*y_1 + x_2*y_2 + ... + x_n*y_n */
	double S_xy = 5 * 3.5 + 10 * 8.5 + 15 * 14.5 + 20 * 21.5 + 25 * 29.5 + 30 * 36.5 + 35 * 43.5 + 40 * 52 + 45 * 61.5;
	/* x_1 + x_2 + ... + x_n */
	double S_x = 5 + 10 + 15 + 20 + 25 + 30 + 35 + 40 + 45;
	/* y_1 + y_2 + ... + y_n */
	double S_y = 3.5 + 8.5 + 14.5 + 21.5 + 29.5 + 36.5 + 43.5 + 52 + 61.5;
	/* x_1*x_1 + x_2*x_2 + ... + x_n*x_n */
	double S_xx = 5 * 5 + 10 * 10 + 15 * 15 + 20 * 20 + 25 * 25 + 30 * 30 + 35 * 35 + 40 * 40 + 45 * 45;
	/*(x_1 + x_2 + .. + x_n)*(x_1 + x_2 + .. + x_n) */
	double S_x2 = S_x*S_x;

	double m = (S*S_xy - S_x*S_y) / (S*S_xx - S_x2);
	double c = (S_xx*S_y - S_x*S_xy) / (S*S_xx - S_x2);
	cout << "y = " << m << "x " << "+ " << c << endl;

	polate(1, m, c);
}

/******************
Function name: powerLawFit
Purpose: Calculate m and c (slope and y intercept) of the linear line best representing the data points using the power law to linearize the datapoints
Function In parameters: x and y datapoints, and the number of datapoints (n)
Function Out parameters: none
Version: 1.0
******************/

void powerLawFit(vector<double> x, vector<double> y, const int n) {
	/*
	double x[9] = {5,10,15,20,25,30,35,40,45};
	double y[9] = { 3.5, 8.5, 14.5, 21.5, 29.5, 36.5, 43.5, 52, 61.5};
	*/
	int dataLength = 9;
	int i = 0;
	double S = 9;
	double S_x = 0;
	double S_y = 0;
	double S_xx = 0;
	double S_xy = 0;
	double S_x2;
	double b;
	double m;
	double c;
	double a;

	while (i < dataLength) {
		x[i] = log(x[i]);
		y[i] = log(y[i]);
		S_x += x[i];
		S_y += y[i];
		S_xx += x[i] * x[i];
		S_xy += x[i] * y[i];
		i++;
	}
	S_x2 = S_x*S_x;

	b = (S*S_xy - S_x*S_y) / (S*S_xx - S_x2);
	m = b;
	a = (S_xx*S_y - S_x*S_xy) / (S*S_xx - S_x2);
	c = exp(a);

	cout << "y = " << c << "x^" << m << endl;

	polate(2, m, c);

	/*
	cout << "S_xy: " << S_xy << endl;
	cout << "S_x: " << S_x << endl;
	cout << "S_y: " << S_y << endl;
	cout << "S_xx: " << S_xx << endl;
	cout << "S_x2: " << S_x2 << endl;

	cout << "S*S_xy: " << S*S_xy << endl;
	cout << "S_x*S_y: " << S_x*S_y << endl;
	cout << "S*S_xx: " << S*S_xx << endl;
	cout << "S_x2: " << S_x2 << endl;

	cout << "m: " << m << endl;
	cout << "c: " << c << endl;
	*/

}

/******************
Function name: polate
Purpose: estimate the value of the data using a point on a line that best fits the data
Function In parameters: m and c (slope and y intercept)
Function Out parameters: none
Version: 1.0
******************/

void polate(int selection, double m, double c) {
	double year;
	double startYear = 1960;
	double startCO2 = 316.5;
	double y = 0;
	int selection2 = 0;
	do {
		do {
			cout << "MENU" << endl;
			cout << "1. Interpolate/Extrapolate" << endl;
			cout << "2. Main Menu" << endl;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
			cin >> selection2;
		} while (selection2 != 1 && selection2 !=2 || cin.fail());
		if (selection2 == 2)
			return;
		cout << "Please enter a year to interpolate/extrapolate to: ";
		cin >> year;
		cout << endl;

		year = year - startYear;

		switch (selection) {
		case 1:
			y = m*year + c;
			break;
		case 2:
			y = c*(pow(year, m));
			break;
		}

		y += startCO2;
		cout << "The linear interpolated/extrapolated CO2 level in the year " << year + startYear << " is " << y << endl;
	} while (selection2 != 2);
}

/******************
Function name: fileRead
Purpose: Read a file, and store x and y values, and the number of datapoints
Function In parameters: x and y datapoints, and the number of datapoints (n)
Function Out parameters: none
Version: 1.0
******************/

void fileRead(vector<double>& x, vector<double>& y, int& n) {
	ifstream file;
	string fileName;
	double num;
	int i = 0;

	do {
		cout << "Please enter the name of the file to open: ";
		cin >> fileName;
		file.open(fileName.c_str());
	} while (!file.is_open());

	while (file >> num) {
		if (i % 2 == 0)
			x.push_back(num);
		else
			y.push_back(num);
		i++;
	}
	n = x.size();
	cout << "There are " << n << " records" << endl;
}

int main(void) {

	int selection;
	string fileName;
	vector <double> x;
	vector <double> y;
	int n;
	do {
		do {
			cout << "\t\tLEAST_SQUARES LINEAR REGRESSION" << endl;
			cout << "MENU" << endl;
			cout << "1. Linear Fit" << endl;
			cout << "2. Power Law Fit" << endl;
			cout << "3. Quit" << endl;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
			cin >> selection;
		} while (selection > 3 || selection < 1 || cin.fail());
		switch (selection) {
		case 1:
			fileRead(x, y, n);
			linearFit(x, y, n);
			break;
		case 2:
			fileRead(x, y, n);
			powerLawFit(x, y, n);
			break;
		}
	} while (selection != 3);
}