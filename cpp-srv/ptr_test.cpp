#include <memory>
#include <list>
#include <iostream>

using namespace std;

struct test_str {
	int a;
	test_str(int a) : a(a) {}
};

int main()
{
	typedef shared_ptr<test_str> p;
	list<p> l;
	l.push_back(p(new test_str(1)));
	l.push_back(p(new test_str(2)));
	l.push_back(p(new test_str(3)));
	p t = l.back();
	l.push_back(p(new test_str(4)));
	l.remove_if([](const p &n)
	            { return n->a % 2; });
	for (auto num : l)
		cout << num->a << " ";
	cout << t->a;
	cout << endl;
}
