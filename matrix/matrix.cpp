#include <iostream>
using namespace std;
int action, m0, n0, m1, n1;


int main()
{
	setlocale(LC_ALL, "ru");
	
	try
	{
		cout << "\t\t\t\t\tДействия с матрицами\t\n\n";
		cout << "Выберите действие: \n";
		cout << "1. Вынесение минуса из матрицы (внесение минуса в матрицу).\n";
		cout << "2. Умножение матрицы на число.\n";
		cout << "3. Транспонирование матрицы.\n";
		cout << "4. Сумма матриц.\n";
		cout << "5. Разность матриц.\n";
		cout << "6. Умножение матриц.\n";
		cout << "7. Нахождение обратной матрицы.\n\n";

		cin >> action;

		if (action < 1 || action > 7) throw 37;
		else
		{
			cout << "\t\t\t\tМатрица A\n";
			cout << "Введите количество строк(m): ";
			cin >> m0;
			cout << "Введите количество столбцов(n): ";
			cin >> n0;

			cout << "\t\t\t\tМатрица B\n";
			cout << "Введите количество строк(m): ";
			cin >> m1;
			cout << "Введите количество столбцов(n): ";
			cin >> n1;
		}
		
	}
	catch (int x)
	{
		cout << "От 1 до 6! Попробуй еще раз. " << x << endl;
	}
	
	
	

	

	return 0;
}

