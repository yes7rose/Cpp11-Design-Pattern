#include <iostream>
#include <memory>

using namespace std;

class Muslim
{
    public:
    virtual ~Muslim()=default;
    virtual void performMuslimRitual()=0;
};

class MuslimFemale:public Muslim
{
    public:
        ~MuslimFemale(){ cout<<"destroying MuslimFemale Object..."<<endl; }
    virtual void performMuslimRitual() override 
    {
       cout<<"Muslim girl performs Muslim ritrual."<<endl;
    }
};

class Hindu
{
    public:
    virtual ~Hindu() = default;
    virtual void performsHinduRitual()=0;
};

class HinduFemale: public Hindu
{
    public:
        ~HinduFemale(){cout<< "destroying HinduFemale Object..."<<endl;}
    virtual void performsHinduRitual() override 
    {
        cout<<"Hindu girl performs Hindu ritual."<<std::endl;
    }
};

class MuslimRitual
{
    public:
    void carryOutRitual(shared_ptr<Muslim> muslim)
    {
        cout<<"On with the Muslim rituals!"<<endl;
        muslim->performMuslimRitual();
    }
};

class MuslimAdapter:public Muslim
{
    private:
                shared_ptr<Hindu> hindu;
    public:
                MuslimAdapter(shared_ptr<Hindu> h)
                    :hindu(h)
                {
                }
                virtual void performMuslimRitual() override
                {
                    hindu->performsHinduRitual();
                }
};

int main(int argc, char const* argv[])
{
    shared_ptr<HinduFemale> hinduGirl(new HinduFemale);
    shared_ptr<MuslimFemale> muslimGirl(new MuslimFemale);

    shared_ptr<MuslimRitual> muslimRitual(new MuslimRitual);
    shared_ptr<MuslimAdapter> adapteHindu(new MuslimAdapter(hinduGirl));

    muslimRitual->carryOutRitual(muslimGirl);
    muslimRitual->carryOutRitual(adapteHindu);

    return 0;
}
