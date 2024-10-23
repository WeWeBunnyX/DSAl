#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

struct Term
{
    double coeff;  // Changed from 'coefficient' to 'coeff'
    int deg;       // Changed from 'degree' to 'deg'
    Term* next;

    Term(double coefficient, int degree) : coeff(coefficient), deg(degree), next(nullptr) {}
};

class Polynomial
{
private:
    Term* head;

    void insertTerm(double coefficient, int degree);

public:
    Polynomial();
    ~Polynomial();
    void readFromFile(const string& filename);
    Polynomial add(const Polynomial& other) const;
    Polynomial multiply(const Polynomial& other) const;
    double evaluate(double x) const;
    void print() const;
};


Polynomial::Polynomial() : head(nullptr) {}

Polynomial::~Polynomial()
{
    while (head)
    {
        Term* temp = head;
        head = head->next;
        delete temp;
    }
}

void Polynomial::insertTerm(double coefficient, int degree)
{
    if (coefficient == 0) return;
    Term* newTerm = new Term(coefficient, degree);
    if (!head || head->deg > degree)
    {
        newTerm->next = head;
        head = newTerm;
        return;
    }
    Term* current = head;
    while (current->next && current->next->deg < degree)
    {
        current = current->next;
    }
    if (current->deg == degree)
    {
        current->coeff += coefficient;
        delete newTerm;
    }
    else
    {
        newTerm->next = current->next;
        current->next = newTerm;
    }
}

void Polynomial::readFromFile(const string& filename)
{
    ifstream file(filename);
    if (!file)
    {
        cerr << "Unable to open file " << filename << endl;
        return;
    }
    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        double coefficient;
        char x, caret;
        int degree;
        while (iss >> coefficient >> x >> caret >> degree)
        {
            insertTerm(coefficient, degree);
        }
    }
    file.close();
}

Polynomial Polynomial::add(const Polynomial& other) const
{
    Polynomial result;
    Term* p1 = head;
    Term* p2 = other.head;
    while (p1 || p2)
    {
        if (p1 && (!p2 || p1->deg < p2->deg))
        {
            result.insertTerm(p1->coeff, p1->deg);
            p1 = p1->next;
        }
        else if (p2 && (!p1 || p2->deg < p1->deg))
        {
            result.insertTerm(p2->coeff, p2->deg);
            p2 = p2->next;
        }
        else
        {
            result.insertTerm(p1->coeff + p2->coeff, p1->deg);
            p1 = p1->next;
            p2 = p2->next;
        }
    }
    return result;
}

Polynomial Polynomial::multiply(const Polynomial& other) const
{
    Polynomial result;
    for (Term* p1 = head; p1; p1 = p1->next)
    {
        for (Term* p2 = other.head; p2; p2 = p2->next)
        {
            result.insertTerm(p1->coeff * p2->coeff, p1->deg + p2->deg);
        }
    }
    return result;
}

double Polynomial::evaluate(double x) const
{
    double result = 0.0;
    for (Term* current = head; current; current = current->next)
    {
        result += current->coeff * pow(x, current->deg);
    }
    return result;
}

void Polynomial::print() const
{
    for (Term* current = head; current; current = current->next)
    {
        cout << current->coeff << "x^" << current->deg;
        if (current->next) cout << " + ";
    }
    cout << endl;
}

int main()
{
    Polynomial poly1, poly2;
    char option;
    poly1.readFromFile("pol1.txt");
    poly2.readFromFile("pol2.txt");

    cout << "\nPolynomial 1: ";
    poly1.print();
    cout << "Polynomial 2: ";
    poly2.print();
    cout << "\n1) Add the polynomials" << endl;
    cout << "2) Multiply the polynomials" << endl;
    cout << "3) Evaluate Polynomial 1 at a specific value of x" << endl; // Added option 3

    cin >> option;

    if (option == '1')
    {
        Polynomial sum = poly1.add(poly2);
        cout << "Sum: ";
        sum.print();
    }
    else if (option == '2')
    {
        Polynomial product = poly1.multiply(poly2);
        cout << "Product: ";
        product.print();
    }
    else if (option == '3') 
    {
        double x;
        cout << "Enter the value of x: ";
        cin >> x;
        cout << "Evaluation of Polynomial 1 at x = " << x << ": " << poly1.evaluate(x) << endl;
    }
    else
    {
        cout << "Invalid choice" << endl;
    }

    return 0;
}
