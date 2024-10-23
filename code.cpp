#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

struct Term
{
    double coefficient;
    int degree;
    Term* next;

    Term(double coeff, int deg) : coefficient(coeff), degree(deg), next(nullptr) {}
};

class Polynomial
{
private:
    Term* head;

    void insertTerm(double coefficient, int degree)
    {
        if (coefficient == 0) return;
        Term* newTerm = new Term(coefficient, degree);
        if (!head || head->degree > degree)
        {
            newTerm->next = head;
            head = newTerm;
            return;
        }
        Term* current = head;
        while (current->next && current->next->degree < degree)
        {
            current = current->next;
        }
        if (current->degree == degree)
        {
            current->coefficient += coefficient;
            delete newTerm;
        }
        else
        {
            newTerm->next = current->next;
            current->next = newTerm;
        }
    }

public:
    Polynomial() : head(nullptr) {}

    ~Polynomial()
    {
        while (head)
        {
            Term* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void readFromFile(const string& filename)
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

    Polynomial add(const Polynomial& other) const
    {
        Polynomial result;
        Term* p1 = head;
        Term* p2 = other.head;
        while (p1 || p2)
        {
            if (p1 && (!p2 || p1->degree < p2->degree))
            {
                result.insertTerm(p1->coefficient, p1->degree);
                p1 = p1->next;
            }
            else if (p2 && (!p1 || p2->degree < p1->degree))
            {
                result.insertTerm(p2->coefficient, p2->degree);
                p2 = p2->next;
            }
            else
            {
                result.insertTerm(p1->coefficient + p2->coefficient, p1->degree);
                p1 = p1->next;
                p2 = p2->next;
            }
        }
        return result;
    }

    Polynomial multiply(const Polynomial& other) const
    {
        Polynomial result;
        for (Term* p1 = head; p1; p1 = p1->next)
        {
            for (Term* p2 = other.head; p2; p2 = p2->next)
            {
                result.insertTerm(p1->coefficient * p2->coefficient, p1->degree + p2->degree);
            }
        }
        return result;
    }

    double evaluate(double x) const
    {
        double result = 0.0;
        for (Term* current = head; current; current = current->next)
        {
            result += current->coefficient * pow(x, current->degree);
        }
        return result;
    }

    void print() const {
        for (Term* current = head; current; current = current->next)
        {
            cout << current->coefficient << "x^" << current->degree;
            if (current->next) cout << " + ";
        }
        cout << endl;
    }
};

int main()
{
    Polynomial p1, p2;
    char choice;
    p1.readFromFile("pol1.txt");
    p2.readFromFile("pol2.txt");

    cout << "Polynomial 1: ";
    p1.print();
    cout << "Polynomial 2: ";
    p2.print();
    cout << "1) Add the polynomials" << endl;
    cout << "2) Multiply the polynomials" << endl;
    cout << "3) Evaluate Polynomial 1" << endl;
    cin >> choice;

    if (choice == '1')
    {
        Polynomial sum = p1.add(p2);
        cout << "Sum: ";
        sum.print();
    }
    else if (choice == '2')
    {
        Polynomial product = p1.multiply(p2);
        cout << "Product: ";
        product.print();
    }
    else if (choice == '3')
    {
        double x;
        cout << "Enter the value of x: ";
        cin >> x;
        cout << "Evaluation of Polynomial 1 at x = " << x << ": " << p1.evaluate(x) << endl;
    }
    else
    {
        cout << "Invalid choice" << endl;
    }

    return 0;
}
