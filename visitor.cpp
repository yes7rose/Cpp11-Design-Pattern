#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Wheel;
class Engine;
class Body;
class Car;

class CarElementVisitor:public enable_shared_from_this<CarElementVisitor>
{
    public:
    virtual void visit(const shared_ptr<const Wheel>& wheel) const = 0;
    virtual void visit(const shared_ptr<const Engine>& engine) const = 0;
    virtual void visit(const shared_ptr<const Body>& body) const = 0;
    virtual void visitCar(const shared_ptr<const Car>& car) const = 0;

    virtual ~CarElementVisitor(){};
};

class CarElement:public enable_shared_from_this<CarElement>
{
    public:
    virtual void accept(const shared_ptr<const CarElementVisitor>& visitor)const=0;
    virtual ~CarElement(){}
};

class Wheel: public  CarElement
{
    private:
        string name;
    public:
        Wheel(const string& n)
            :name(n)
        {}
        virtual void accept(const shared_ptr<const CarElementVisitor>& visitor) const override final
        {
            visitor->visit(static_pointer_cast<const Wheel>(shared_from_this()));
        }
        ~Wheel(){}

        string getName() const
        {
            return name;
        }
};

class Engine:public CarElement
{
    public:
        virtual void accept(const shared_ptr<const CarElementVisitor>& visitor)const override final
        {
            visitor->visit(static_pointer_cast<const Engine>(shared_from_this()));
        }
        Engine() = default;
        ~Engine() = default;
};

class Body : public CarElement
{
    public:
        Body() = default;
        ~Body() = default;

        void accept(const shared_ptr<const CarElementVisitor>& visitor)const override final
        {
            visitor->visit(static_pointer_cast<const Body>(shared_from_this()));
        }
};

class Car
{
    private:
        vector<shared_ptr<CarElement>> elements;
    public:
        Car()
        {
            elements.push_back(shared_ptr<CarElement> (new Wheel("zuo qian")));
            elements.push_back(shared_ptr<CarElement> (new Wheel("yuo qian")));
            elements.push_back(shared_ptr<CarElement> (new Wheel("zuo hou")));
            elements.push_back(shared_ptr<CarElement> (new Wheel("you hou")));
            elements.push_back(shared_ptr<CarElement> (new Body()));
            elements.push_back(shared_ptr<Engine> (new Engine()));

        }
        ~Car() = default;

        vector<shared_ptr<CarElement>> getElenments() const
        {
            return elements;
        }
};

class CarElementPrintVisitor : public CarElementVisitor
{
    public:
        CarElementPrintVisitor() = default;
        ~CarElementPrintVisitor() = default;

        virtual void visit(const shared_ptr<const Wheel>& wheel) const override final
        {
            cout<<"Visiting " << wheel->getName() << " wheel" << endl;
        }
        virtual void visit(const shared_ptr<const Body>& body) const override final
        {
            cout<< "Visiting body" <<endl;
        }
        virtual void visit(const shared_ptr<const Engine>& engine) const override final
        {
            cout << "Visiting engine"<<endl;
        }
        virtual void visitCar(const shared_ptr<const Car>& car) const override final
        {
            cout<<endl << "Visiting car" << endl;
            vector<shared_ptr<CarElement>> elements = car->getElenments();
            for(auto element : elements)
            {
                element->accept(shared_from_this());
            }
        }
};

class CarElementDoVisitor: public CarElementVisitor
{
public:
    CarElementDoVisitor()  = default;
        virtual void visit(const shared_ptr<const Wheel>& wheel) const override final
        {
            cout<<"Kicking " << wheel->getName() << " wheel" << endl;
        }
        virtual void visit(const shared_ptr<const Body>& body) const override final
        {
            cout<< "Cleaning body" <<endl;
        }
        virtual void visit(const shared_ptr<const Engine>& engine) const override final
        {
            cout << "Checking engine"<<endl;
        }
        virtual void visitCar(const shared_ptr<const Car>& car) const override final
        {
            cout<<endl << "Checking the car" << endl;
            vector<shared_ptr<CarElement>> elements = car->getElenments();
            for(auto element : elements)
            {
                element->accept(shared_from_this());
            }
        }

};

int main(int argc, char const* argv[])
{
    auto car = make_shared<Car> ();
    auto carPrintVisitor = make_shared<CarElementPrintVisitor> ();
    auto carDoVisittor = make_shared<CarElementDoVisitor> ();

    carDoVisittor->visitCar(car);
    carPrintVisitor->visitCar(car);
    return 0;
}
