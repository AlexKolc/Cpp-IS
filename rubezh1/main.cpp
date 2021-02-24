#include <iostream>
#include <fstream>

using namespace std;

class Rational {
private:
    int *numerator;
    int *denominator;

    int gcd(int num1, int num2) {
        while (num1 && num2) {
            if (num1 > num2)
                num1 %= num2;
            else
                num2 %= num1;
        }
        return num1 + num2;
    }

public:
    Rational(int num): numerator(new int), denominator(new int) {
        *numerator = num;
        *denominator = 1;
    }

    Rational(int num, int denom): numerator(new int), denominator(new int) {
        *numerator = num;
        *denominator = denom;
    }

    Rational(const Rational &fraction) {
        this->~Rational();
        new (this) Rational(*(fraction.numerator), *(fraction.denominator));
    }

    ~Rational() {
        if (numerator)
            delete numerator;
        if (denominator)
            delete denominator;
    }

    int getNumerator() {
        return *numerator;
    }

    int getDenominator() {
        return *denominator;
    }

    void setNumerator(const int &num) {
        *numerator = num;
    }

    void setDenominator(const int &denom) {
        *denominator = denom;
    }

    void setter(const Rational &fraction) {
        new(this) Rational(fraction);
    }

    Rational getter() const {
        return Rational(*numerator, *denominator);
    }

    void reduction() {
        int x = gcd(*numerator, *denominator);
        *numerator /= x;
        *denominator /= x;
    }

    void add(const Rational &fraction) {
        Rational oldFraction(*this);
        *numerator = *(oldFraction.numerator) * *(fraction.denominator) + *(oldFraction.denominator) * *(fraction.numerator);
        *denominator = *(oldFraction.denominator) * *(fraction.denominator);
        this->reduction();
    }

    Rational &operator=(const Rational & fraction) {
        setter(fraction);
        return (*this);
    }

    Rational operator+(const Rational &fraction) const {
        Rational oldFraction(*this);
        oldFraction += fraction;
        return oldFraction;
    }

    Rational &operator+=(const Rational &fraction) {
        Rational oldFraction(*this);
        *numerator = *(oldFraction.numerator) * *(fraction.denominator) + *(oldFraction.denominator) * *(fraction.numerator);
        *denominator = *(oldFraction.denominator) * *(fraction.denominator);
        this->reduction();
        return (*this);
    }
};

int main() {

    Rational obj1(2, 3);

    //Rational obj2(obj1);
    Rational obj2 = obj1.getter();
    obj2.setNumerator(4);
    obj2.setDenominator(6);
    Rational obj3 = Rational(2,3) + Rational(5, 6);
    cout << obj3.getNumerator() << ' ' << obj3.getDenominator() << endl;

    obj1.add(obj2);
    cout << obj1.getNumerator() << ' ' << obj1.getDenominator() << endl;
    return 0;
}