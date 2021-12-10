#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>
using namespace std;

const int maxpop = 100; // максимум в популяции
const int maxstring = 10; // размер в строке значений
const int lchrom = 30; // число генов
const int popsize = 20; //число популяции 
const double pmutation = 0.05; // вероятность мутации
const double pcross = 0.7;  // вероятность скрещивания
double max0; // максимальное значение выживаемости (близость к ответу )
int p1 = 0;
int p2 = 1; // для скрещивания 
double sred;

class individual //особь
{
public:

    double chrom[maxstring];  // строка вещественных чисел
    double fenotype; //корень уравнения (х)
    double fitness;  // коэффициент выживаемости (значение функции) 

};

individual oldpop[maxpop]; //старое поколение популяции
individual newpop[maxpop]; // новое поколение популяции
individual intpop[maxpop]; // полонение для сортировки 

const double xmax = 0;
const double xmin = -4;

double randomV(double a, double b)
{
    //вещественная часть
    double m;
    m = rand() % 6500 + 0;
    double n;
    n = m / 65000;
    //целая часть
    int c = b - a;
    double x;
    x = rand() % c + 0;
    double z;
    z = a + x + n;
    return z;
}
double random_() //вероятность (от 0 до 1)
{
    double a;
    a = rand() % 6500 + 0;
    double b;
    b = a / 6500;

    return  b;
}

// если входное значение равно 1, то true, если нет, то рандом
bool flip(double probability)
{
    double x;
    x = random_();
    return (x <= probability); // чем меньше probability, тем меньше вероятность появления true
}

//основная функция y(x)=1/x
double objfunc(double x)
{
    return (1 / x);
}

//поиск х путем находения среднего значения по генам особи 
double decode(double* chrom)
{
    double summ = 0;
    for (int i = 0; i < maxstring; i++)
    {
        summ = summ + chrom[i];
    }
    summ = summ / maxstring;
    return summ;
}

// поиск большего значени выживаемости у поколения
double statistics(individual* pop)
{
    max0 = pop[0].fitness;
    sred = pop[0].fitness;

    for (int j = 1; j < popsize; j++)
    {
        sred = sred + pop[j].fitness;
        if (pop[j].fitness > max0)
            max0 = pop[j].fitness; //  большее значение в популяции
    }
    sred = sred / popsize;
    return max0;
}

// создаем первое поколение популяции
void initpop(individual* oldpop)
{

    for (int j = 0; j < popsize; j++) {

        for (int i = 0; i < lchrom; i++)
        {
            oldpop[j].chrom[i] = randomV(xmin, xmax); // случайная величина от -4 до 0

        }
        oldpop[j].fenotype = decode(oldpop[j].chrom);
        oldpop[j].fitness = objfunc(oldpop[j].fenotype); // вычисляем коф выживания
    }
}


// сортировка по вероятности выживания (приближенности к ответу) ТУРНИРНЫЙ ОТБОР t=2
void select()
{
    int j1, j2;
    int ipick = 0;

    j1 = ipick;
    j2 = ipick + 1;
    for (int j = 0; j < popsize / 2; j++)
    {
        if (oldpop[j2].fitness > oldpop[j1].fitness) //выбираем более приспособленную (турнир между парой) сильные- в начале, слабые в конце
        {
            intpop[j] = oldpop[j2];
            intpop[popsize - j - 1] = oldpop[j1];
        }
        else
        {
            intpop[j] = oldpop[j1];
            intpop[popsize - j - 1] = oldpop[j2];
        }
        ipick = ipick + 2;

        j1 = ipick;
        j2 = ipick + 1;

    }

    for (int i = 0; i < popsize; i++)
        oldpop[i] = intpop[i];  // "отсортированная популяция с большими приближением к ответу в начале" 
}

//мутация с вероятность появления 0,01
double mutation(double alleleval)
{
    bool mutate;
    double m;
    mutate = flip(pmutation);
    if (mutate)
    {
        m = alleleval + randomV(-0.5, 0.5);
        if (m >= xmin && m <= xmax)
            return m;
        else return alleleval;
    }
    else
        return alleleval;

}

// скрещивание
void crossover(int par1, int par2, int ch1, int ch2)
{
    // присвоение двух точек разрыва
    int l1 = rand() % (lchrom - 1) + 1; // случайная точка разрыва 1
    int l2 = rand() % (lchrom - 1) + 1; // случайная точка разрыва 2
    int raz1, raz2;
    if (l1 > l2)
    {
        raz1 = l2;
        raz2 = l1;
    }
    if (l1 < l2)
    {
        raz1 = l1;
        raz2 = l2;
    }
    if (l1 = l2)
    {
        if (l1 > lchrom / 2)
        {
            l1 = rand() % (lchrom / 2) + 1;
            raz1 = l1;
            raz2 = l2;
        }
        if (l1 < lchrom / 2)
        {
            l1 = rand() % (lchrom - 1) + lchrom / 2;
            raz1 = l2;
            raz2 = l1;
        }
        if (l1 = lchrom / 2)
        {
            l1 = rand() % (lchrom / 2) + 1;
            raz1 = l1;
            l2 = rand() % (lchrom - 1) + lchrom / 2;
            raz2 = l2;
        }
    }
    int p = rand() % 2 + 1;  // случайный родителель 
    if (flip(pcross)) // вероятность скрещивания 
    {

        for (int j = 0; j < raz1; j++) // гены до 1 точки разрыва
        {
            if (p = 1)
            {
                newpop[ch1].chrom[j] = mutation(oldpop[par1].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[par2].chrom[j]);

            }
            else
            {
                newpop[ch1].chrom[j] = mutation(oldpop[par2].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[par1].chrom[j]);
            }
        }
        for (int j = raz2; j < lchrom; j++) // гены от 1 до 2 точки разрыва
        {
            if (p = 1)
            {
                newpop[ch1].chrom[j] = mutation(oldpop[par2].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[par1].chrom[j]);
            }
            else
            {
                newpop[ch1].chrom[j] = mutation(oldpop[par1].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[par2].chrom[j]);
            }
        }
        for (int j = 0; j < raz1; j++) // гены от 2  точки разрыва
        {
            if (p = 1)
            {
                newpop[ch1].chrom[j] = mutation(oldpop[par1].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[par2].chrom[j]);

            }
            else
            {
                newpop[ch1].chrom[j] = mutation(oldpop[par2].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[par1].chrom[j]);
            }
        }
    }
    else //если не скрестились, по очереди скрещиваются более приспособленные особи
    {
        for (int j = 0; j < raz1; j++) // гены до 1 точки разрыва
        {
            if (p = 1)
            {
                newpop[ch1].chrom[j] = mutation(oldpop[p1].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[p2].chrom[j]);
            }
            else
            {
                newpop[ch1].chrom[j] = mutation(oldpop[p2].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[p1].chrom[j]);
            }
        }
        for (int j = raz1; j < raz2; j++) // гены от 1 до 2 точки разрыва
        {
            if (p = 1)
            {
                newpop[ch1].chrom[j] = mutation(oldpop[p2].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[p1].chrom[j]);
            }
            else
            {
                newpop[ch1].chrom[j] = mutation(oldpop[p1].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[p2].chrom[j]);
            }
        }
        for (int j = raz2; j < lchrom; j++) // гены до 1 точки разрыва
        {
            if (p = 1)
            {
                newpop[ch1].chrom[j] = mutation(oldpop[p1].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[p2].chrom[j]);
            }
            else
            {
                newpop[ch1].chrom[j] = mutation(oldpop[p2].chrom[j]);
                newpop[ch2].chrom[j] = mutation(oldpop[p1].chrom[j]);
            }
        }
        //переход к следующей паре особей для скрещивания
        p1 = p1 + 2;
        p2 = p2 + 2;
    }

}

// генерация нового поколения 
void generation()
{
    int j, mate1, mate2;
    select(); // сортируем старое поколение
    j = 0;
    do {
        mate1 = j;     // берем пару рядом стоящих особей 
        mate2 = j + 1;

        // скрещиваем, потомками заполняем новую популяцию 
        crossover(mate1, mate2, j, j + 1);

        // подсчитываем коф выжив у первого потомка
        newpop[j].fenotype = decode(newpop[j].chrom);
        newpop[j].fitness = objfunc(newpop[j].fenotype);

        // подсчитываем коф выжив у второго потомка
        newpop[j + 1].fenotype = decode(newpop[j + 1].chrom);
        newpop[j + 1].fitness = objfunc(newpop[j + 1].fenotype);

        j = j + 2;  //переходим к следующей паре
    } while (j < popsize);
    //обнуляем для скрещивания
    p1 = 0;
    p2 = 1;
}


int main()
{
    setlocale(LC_ALL, "rus");
    int gen; //номер поколения
    int maxgen; // число поколений
    int nmutation;
    int ncross;
    double t; //коэф максимальной выживаемости у поколения

    cout << "   ВЕЩЕСТВЕННОЕ КОДИРОВАНИЕ";
    
    cout << endl;

    cout << "Введите число поколений  ";
    cin >> maxgen;

    nmutation = 0;
    ncross = 0;

    initpop(oldpop); //создаем первое поколение популяции
    t = statistics(oldpop); // подсчет большего коэф выживаемости у первого поколения (max0 заполнены первым поколением)

   //for (int i = 0; i < popsize; i++) //печатаем первое поколение
   // {
   //     for (int j = 0; j < maxstring; j++)
   //         cout << oldpop[i].chrom[j] << " ";
   //     cout << endl;
   //     cout << oldpop[i].fenotype << endl;
   //     cout << oldpop[i].fitness << endl;
   // }

    cout << endl;
 
    cout << endl;
    cout << endl;

    //массив для хранния максимальных значений
    double* maxmass;
    maxmass = new double[maxgen];
    // массив для хранения средних значений
    double* sredmass;
    sredmass = new double[maxgen];
    maxmass[0] = max0;
    sredmass[0] = sred;
    gen = 0;
    do
    {
        gen++;
        generation(); // создаем новое поколение 
        statistics(newpop); // находим больший коэффициент выживаемости у поколения
      //  cout << max0 << endl;
        if (max0 > t)
            t = max0;
        for (int i = 0; i < maxpop; i++) // новое поколение превращаем в старое 
            oldpop[i] = newpop[i];
        maxmass[gen] = max0;
        sredmass[gen] = sred;
    } while (gen < maxgen); // сколько раз было задано 

    cout << "Ответ: " << t;
    cout << endl;

    ofstream Result("ResultVCode.txt"); //Созданием Файл  с результатом

    if (Result.is_open()) // проверка на открытие
        cout << "Ответ записан в файл ResultVCode!\n\n" << endl;
    else
    {
        cout << "Ошибка записи в файл" << std::endl;
        return -1;
    }
    Result << "Максимальное в поколении: " << endl;
    for (int i = 0; i < maxgen; i++)
    {
        Result << maxmass[i] << endl;
    }
    Result << endl;
    Result << "Среднее в поколении: " << endl;

    for (int i = 0; i < maxgen; i++)
    {
        Result << sredmass[i] << endl;
    }
    Result.close();
}
