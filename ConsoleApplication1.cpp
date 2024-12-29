//Объявление необходимых библиотек 
#include <iostream>
#include <string>
using namespace std;

// Абстрактный класс Vrlue
class Vrlue {
public:
    // Виртуальные функции, которые должны быть реализованы в производных классах
    virtual void out() = 0;  // Функция для вывода значения
    virtual float sum() = 0;  // Функция для вычисления суммы
    virtual float max() = 0;  // Функция для нахождения максимального значения
    virtual float min() = 0;  // Функция для нахождения минимального значения
    virtual float len() = 0;  // Функция для нахождения длины (количества элементов)
    virtual int depth() = 0;  // Функция для нахождения глубины структуры

    // Функции для реализации арифметических операций ( +, -, *, /)
    virtual Vrlue* operator+(float num) = 0;
    virtual Vrlue* operator-(float num) = 0;
    virtual Vrlue* operator*(float num) = 0;
    virtual Vrlue* operator/(float num) = 0;
};

class Atom : public Vrlue {
public:
    float atom_value;  // Переменная для хранения атомарного значения
    Atom(float a_v) {
        atom_value = a_v;
    }
    // Реализация функций 
    void out() override {
        cout << atom_value;
    }
    float sum() override {
        return atom_value;
    }
    float max() override {
        return atom_value;
    }
    float min() override {
        return atom_value;
    }
    float len() override {
        return 1;
    }
    int depth() override {
        return 0;
    }
    Vrlue* operator+(float num) override {
        return new Atom(atom_value + num);
    }
    Vrlue* operator-(float num) override {
        return new Atom(atom_value - num);
    }
    Vrlue* operator*(float num) override {
        return new Atom(atom_value * num);
    }
    Vrlue* operator/(float num) override {
        return new Atom(atom_value / num);
    }
};

class Pair : public Vrlue {
public:
    Vrlue* left, * right;  // Указатели на левый и правый элементы пары
    Pair(Vrlue* l, Vrlue* r) {
        left = l;
        right = r;
    }
    // Реализация функций 
    void out() override {
        cout << "[";
        left->out();
        cout << ",";
        right->out();
        cout << "]";
    }
    float sum() override {
        return left->sum() + right->sum();
    }
    float max() override {
        float l = left->max();
        float r = right->max();
        if (l > r) {
            return l;
        }
        return r;
    }
    float min() override {
        if (left->min() < right->min()) {
            return left->min();
        }
        return right->min();
    }
    float len() override {
        return  left->len() + right->len();
    }
    int depth() override {
        return std::max(left->depth(), right->depth()) + 1;
    }
    Vrlue* operator+(float num) override {
        return new Pair((*left + num), (*right + num));
    }
    Vrlue* operator-(float num) override {
        return new Pair((*left - num), (*right - num));
    }
    Vrlue* operator*(float num) override {
        return new Pair((*left * num), (*right * num));
    }
    Vrlue* operator/(float num) override {
        return new Pair((*left / num), (*right / num));
    }
};

// Функция разбора строки
Vrlue* parse(string str) {
    if (str[0] == '(') { // Лишние скобки или начало пары
        int i = 1;  // Индекс i, чтобы проходить по строке
        int cnt = 1;  // Счетчик для отслеживания количества открывающих скобок
        // Цикл для поиска закрывающей скобки
        while (cnt != 0) { 
            if (str[i] == '(') {
                cnt++;
            }if (str[i] == ')') {
                cnt--;
            }
            i++;
        }
        if (i == str.length()) {  // Если дошли до конца строки, это лишние скобки
            return parse(str.substr(1, str.length() - 2));
        }
        else { // Первый элемент пары заканчивается в i-1. Создаем новую пару, разбирая левую и правую части
            return new Pair(parse(str.substr(1, i - 1)), parse(str.substr(i + 2, str.length() - i - 2)));
        }
    }
    else {  // Если строка не начинается со скобки
        if (str.find(',') != -1) {  // Проверяем наличие запятой. Создаем пару, используя значение до запятой и разбираем оставшуюся строку
            return new Pair(new Atom(stof(str.substr(0, str.find(',')))), parse(str.substr(str.find(',') + 2, str.length() - str.find(','))));
        }
        else {  // Если запятых нет, создаем атом
            return new Atom(stof(str));
        } 

    }
}


int main()
{
    setlocale(LC_ALL, "Russian");
    Vrlue* p = parse("((((9,(-3, 0.52)), -4), (7.2, -5.1)))");
    cout << "p = ";
    p->out();
    cout << "\n" << "Глубина:" << p->depth() << "\n" << "\n";

    p = (*p + 2);
    cout << "p + 2 = ";
    p->out();
    cout << "\n";

    p = (*p - 5);
    cout << "p - 5 = ";
    p->out();
    cout << "\n";

    p = (*p * 3);
    cout << "p*3 = ";
    p->out();
    cout << "\n";

    p = (*p / 4);
    cout << "p/4 = ";
    p->out();
    return 0;
}