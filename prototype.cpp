#include <iostream>
#include <map>
#include <string>

using namespace std;

enum class RECORD_TYPE{CAR, BIKE, PERSON};

class Record
{
public:
    Record() {};
    virtual ~Record(){};
    virtual Record * clone()=0;
    virtual void print()=0;
};

class CarRecod: public Record
{
private:
    string m_carName;
    int m_ID;

public:
    CarRecod(string carName, int ID)
             :Record(), m_carName(carName), m_ID(ID){};

    CarRecod(const CarRecod& carRecord)
        :Record(carRecord)
    {
        m_carNam = carRecord.m_carName;
        m_ID = carRecord.m_ID;
    }

    ~CarRecod(){};

    Record * clone()
    {
        return new CarRecod(*this);
    }
    void print()
    {
        cout<<"Car Record"<<endl
           << "Name : "<< m_carName <<endl
           << "Number:"<< m_ID <<endl<<endl;
    }
};

class BikeRecord : public Record
{
private:
    string m_bikeName;
    int m_ID;

public:
    BikeRecord() {}

};

