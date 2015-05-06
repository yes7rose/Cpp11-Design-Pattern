#include <iostream>
#include <memory>
#include <string>

using namespace std;

class Subject
{
public:
    virtual void execute() = 0;
};

class RealSubject:public Subject
{
private:
    string str;
public:
    RealSubject(string s)
        :str(s)
    {}

    virtual void execute() override final
    {
        cout << str << endl;
    }
};

class ProxySubject: public Subject
{
private:
    string first, second, third;
    shared_ptr<RealSubject> realSubject;
public:
    ProxySubject(string s)
    {
        int num = s.find_first_of(' ');
        first = s.substr(0, num);

        s = s.substr(num + 1);
        num = s.find_first_of(' ');
        second = s.substr(0, num);

        s = s.substr(num + 1);
        num = s.find_first_of(' ');
        third = s.substr(0, num);
        s = s.substr(num + 1);
        realSubject = make_shared<RealSubject> (s);
    }

    ~ProxySubject() = default;

    virtual void execute() override final
    {
        cout << first << ' ' << third << ' ';
        realSubject->execute();
    }
};

int main(int argc, char const* argv[])
{
    auto proxySubject = make_shared<ProxySubject> ("the quick brown fox jumped over the dog");
    proxySubject->execute();
    return 0;
}
