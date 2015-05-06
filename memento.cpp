#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <functional>

using namespace std;

class Number;

class Memento
{
private:
    int state;
public:
    friend class Number;
    Memento(int v) :state(v) { }
};

class Number
{
private:
    int value;
public:
    Number(int v) : value(v) {}

    void dubble ()
    {
        value *= 2;
    }

    void half()
    {
        value /=2;
    }

    int getValue()
    {
        return value;
    }

    shared_ptr<Memento> createMemento()
    {
        return make_shared<Memento> (value);
    }

    void reinstateMemento(shared_ptr<Memento> mem)
    {
        value = mem->state;
    }
};

class Command:public enable_shared_from_this<Command>
{
protected:
//    typedef void(Number::*Action)();
    shared_ptr<Number> receiver;
    function<void (Number*)> action;
    static array< shared_ptr<Memento>, 10 > mementoList;
    static array< shared_ptr<Command>, 10 > commandList;
    static unsigned int numCmd;
    static unsigned int cmdCount;

public:
    Command(shared_ptr<Number> n, function<void(Number*)> a)
        :receiver(n), action(a) {}

    virtual void execute()
    {
        mementoList[numCmd] = receiver->createMemento();
        commandList[numCmd] = shared_from_this();

        if(numCmd > cmdCount)
            cmdCount = numCmd;
        numCmd++;

        action(receiver.get());
    }
    static void undo()
    {
        if(numCmd == 0)
        {
            cout << "---Attempt to run off the end.---";
            return;
        }
        commandList[numCmd-1]->receiver->reinstateMemento(mementoList[numCmd-1]);
        numCmd--;
    }

    static void redo()
    {
        if(numCmd > cmdCount)
        {
            cout<<"---Attempt to run off the end.---" <<endl;
            return;
        }
        commandList[numCmd]->action(commandList[numCmd]->receiver.get());
        numCmd++;
    }
};

array<shared_ptr<Command>, 10> Command::commandList;
array<shared_ptr<Memento>, 10> Command::mementoList;
unsigned int Command::numCmd=0;
unsigned int Command::cmdCount=0;

int main()
{
    int i;
    cout << "Integer:";
    cin >> i;
    auto object = make_shared<Number> (i);

    vector<shared_ptr<Command>> commands
    {
        nullptr,
        make_shared<Command> (object, &Number::dubble),
        make_shared<Command> (object, &Number::half)
    };

    cout<<"Exit[0], Double[1], Half[2], Undo[3], Redo[4]: ";
    cin >> i;

    while(i)
    {
        if(i == 3)
        {
            Command::undo();
        }
        else if(i==4)
        {
            Command::redo();
        }
        else
        {
            commands[i]->execute();
        }

        cout << "  " << object->getValue() << endl;
        cout << "Exit[0], Double[1], Half[2], Undo[3], Redo[4]: ";
        cin >> i;
    }

    return 0;
}

