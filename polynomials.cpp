#include <bitset>
#include <iostream>
#include <vector>

using namespace std;

/*
Implementations of arithmetic operations of polynomials over a finite field
of characteristic 2. Implemented are also:
- Check if a polynomial is primitive.
- Generate the finite field GF(2^q) given a primitive polynomial of degree q.
*/

// Set below variable RUN_TESTS to true to run tests
const bool RUN_TESTS = false;
const int SIZE = 64;
using Polynomial = bitset<SIZE>;

Polynomial operator+(const Polynomial& a, const Polynomial& b) { return a ^ b; }

int degree(const Polynomial& a) {
    int deg = SIZE - 1;
    for (; deg > 0; deg--) {
        if (a[deg]) {
            break;
        }
    }
    return deg;
}

Polynomial operator%(const Polynomial& a, const Polynomial& b) {
    Polynomial rem = a;

    int aDeg = degree(a);
    int bDeg = degree(b);

    int remDegree = degree(rem);
    int i = aDeg - bDeg;
    while (i >= 0) {
        Polynomial x = b << i;
        rem = rem + x;

        int newDegree = degree(rem);
        i -= (remDegree - newDegree);
        remDegree = newDegree;
    }

    return rem;
}

Polynomial operator*(const Polynomial& a, const Polynomial& b) {
    Polynomial total;

    for (int i = 0; i < SIZE; i++) {
        if (!a[i]) {
            continue;
        }

        Polynomial x = b << i;
        total = total + x;
    }

    return total;
}

// The polynomial p is assumed to be irreducable
vector<Polynomial> findFieldElements(const Polynomial& p) {
    Polynomial first(0b1);
    Polynomial alpha(0b10);
    vector<Polynomial> res = {Polynomial(0), first};
    Polynomial current = alpha;

    while (current != first) {
        res.push_back(current);
        current = current * alpha;
        current = current % p;
    }

    return res;
}

// The polynomial p is assumed to be irreducable
bool isPrimitive(const Polynomial& p) {
    int deg = degree(p);

    if (deg < 2) {
        return false;
    }

    Polynomial first(0b1);
    Polynomial alpha(0b10);
    Polynomial current = alpha;

    int groupOrder = (1 << degree(p)) - 1;
    int order = 1;

    while (current != first) {
        current = current * alpha;
        current = current % p;
        order++;
    }

    return order == groupOrder;
}

void prettyPrint(const Polynomial& a, int deg = -1) {
    if (deg == -1) {
        deg = degree(a);
    }
    for (int i = 0; i <= deg; i++) {
        cout << a[i];
    }
    cout << '\n';
}

void prettyPrint(const vector<Polynomial>& polynomials) {
    if (polynomials.empty()) {
        return;
    }
    int maxDegree = degree(polynomials[0]);
    for (const Polynomial& i : polynomials) {
        maxDegree = max(maxDegree, degree(i));
    }

    for (const Polynomial& i : polynomials) {
        prettyPrint(i, maxDegree);
    }
}

void testRemainder() {
    cout << "Remainder tests:\n";
    cout << (Polynomial(0b11111101111110) % Polynomial(0b100011011) ==
             Polynomial(1));
    cout << '\n';
}

void testAddition() {
    cout << "Addition tests:\n";
    cout << (Polynomial(0b101) + Polynomial(0b11) == Polynomial(0b110));
    cout << (Polynomial(0) + Polynomial(0b11011) == Polynomial(0b11011));
    cout << '\n';
}

void testMultiplication() {
    cout << "Multiplication tests:\n";
    Polynomial p1(0b101);
    Polynomial p2(0b11);
    Polynomial p3(0);

    cout << (p1 * p2 == Polynomial(0b1111));
    cout << (p2 * p2 == Polynomial(0b101));
    cout << (p1 * p3 == p3);

    cout << '\n';
}

void testFindFieldElements() {
    Polynomial a(0b11001);
    vector<Polynomial> elements = findFieldElements(Polynomial(0b11001));
    prettyPrint(elements);

    cout << isPrimitive(a) << '\n';
}

void runTests() {
    testAddition();
    testMultiplication();
    testRemainder();
    testFindFieldElements();
}

// The polynomial p is assumed to be irreducable and primitive
void printField(const Polynomial& p) {
    vector<Polynomial> field = findFieldElements(p);

    cout << "Field size: " << field.size() << '\n';
    cout << "Field elements:\n";
    cout << "----------------------------------\n";
    prettyPrint(field);
    cout << "----------------------------------\n";
}

int main() {
    if (RUN_TESTS) {
        runTests();
        return 0;
    }

    cout << "Polynomials are displayed in degree increasing order.\n\n";

    vector<Polynomial> candidates{Polynomial(0b1000001), Polynomial(0b0000001),
                                  Polynomial(0b1001001), Polynomial(0b1000011)};

    cout << "Candidates for primitive polynomials are (I've added more for "
            "testing reasons):\n";
    prettyPrint(candidates);

    for (Polynomial& p : candidates) {
        if (!isPrimitive(p)) {
            continue;
        }

        cout << "Found primitive polynomial: ";
        prettyPrint(p);
        printField(p);
    }

    return 0;
}