#include <iostream>
#include <fstream>
#include <iomanip> 
using namespace std;

typedef float datatype;

void	outputMatrix(datatype **a, int n)
{
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
			cout << setprecision(3) << a[i][j] << "\t\t";
		cout << endl;
	}
	cout << endl;
}

void	outputMatrixVector(datatype **a, datatype *b, int n)
{
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
			cout << setprecision(3) << a[i][j] << "\t\t";
		cout << "\t" << setprecision(3) << b[i];
		cout << endl;
	}
	cout << endl;
}

void	outputVector(datatype *b, int n)
{
	for (int i = 0; i < n; ++i)
		cout << setprecision(3) << b[i] << endl;
	cout << endl;
}

void	DeleteA(datatype **a, int n)
{
	for (int i = 0; i < n; ++i)
		delete[] a[i];
	delete[] a;
}

int	main()
{
	int n;

	//Считывание из файла
	ifstream input;
	input.open("D1.TXT");
	input >> n;

		//Выделяем память
	datatype **a = new datatype*[n];
	datatype *b = new datatype[n];

	for (int i = 0; i < n; ++i)
		a[i] = new datatype[n];
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			input >> a[i][j];
		}
		input >> b[i];
	}
	input.close();

	//Вывод того, что считали из файла
	cout << "Old matrix\n";
	outputMatrixVector(a, b, n);

	// Заводим массив с индексами иксов
	int *xind = new int[n];
	for(int i = 0; i < n; ++i)
		xind[i] = i;

	for(int countmax = 0; countmax < n; ++countmax)
	{
		// Поиск максимального элемента в нижней диагонали
		datatype max = a[countmax][countmax];
		int	maxrow = countmax, maxcol = countmax;
		for(int i = countmax; i < n; ++i)
		{
			for(int j = countmax; j < i; ++j)
			{
				if (a[i][j] > max)
				{
					max = a[i][j];
					maxrow = i;
					maxcol = j;
				}
			}
		}
		// cout << "==========countmax: " << countmax << endl;
		// cout << "max = "<< max << endl << "maxrow, maxcol: " << maxrow << " " << maxcol << endl;
		// Перестановка местами строк
		datatype tmp;
		for(int i = 0; i < n; ++i)
		{
			tmp = a[countmax][i];
			a[countmax][i] = a[maxrow][i];
			a[maxrow][i] = tmp;
		}
		tmp = b[countmax];
		b[countmax] = b[maxrow];
		b[maxrow] = tmp;
		
		// Перестановка местами столбцов
		for(int i = 0; i < n; ++i)
		{
			tmp = a[i][countmax];
			a[i][countmax] = a[i][maxcol];
			a[i][maxcol] = tmp;
		}
		tmp = xind[countmax];
		xind[countmax] = xind[maxcol];
		xind[maxcol] = tmp;

		// outputMatrixVector(a, b, n);

		// Построчное вычитание
		datatype c;
		for(int i = countmax + 1; i < n; ++i)
		{ 
			c = a[i][countmax] / a[countmax][countmax];
			// cout << "c = " << c << endl;
			for(int j = countmax; j < n; ++j)
				a[i][j] = a[i][j] - c * a[countmax][j];
			b[i] = b[i] - c * b[countmax];
		}	
		// cout << "after minus: " << endl;
		// outputMatrixVector(a, b, n);
	}

	// Вывод матрицы, приведённой к диагональному виду
	cout << "New matrix\n";
	outputMatrixVector(a, b, n);

	// Вычисление икса
	datatype	*x = new datatype[n];
	for (int i = 0; i < n; ++i)
		x[i] = 0;
	for (int i = n - 1; i >= 0; --i)
	{
		datatype sum = 0;
		for (int j = i + 1; j < n; ++j)
			sum += a[i][j] * x[j];
		x[i] = (b[i] - sum) / a[i][i];
	}

	// Перестановка на правильные места
	// datatype tmp;
	datatype	*xfin = new datatype[n];
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			if (xind[j] == i)
			{
				xfin[i] = x[xind[j]];
				break ;
			}
		}
	}

	cout << "====Solution: \n";
	outputVector(x, n);
	// Освобождение памяти
	DeleteA(a, n);
	delete[] b;
	delete[] xind;
	delete[] x;
	delete[] xfin;
	return 0;
}