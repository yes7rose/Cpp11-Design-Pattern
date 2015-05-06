#include <iostream>
#include <memory>

using namespace std;

class DrawingAPI
{
    public:
        virtual void drawCircle(double x, double y, double radius)=0;
        virtual ~DrawingAPI(){}
};

class DrawingAPI1:public DrawingAPI
{
    public:
        void drawCircle(double x, double y, double radius) override
        {
            cout << "API1.circle at "<< x << ":"<< y <<" " << radius << endl;
        }
};

class DrawingAPI2 : public DrawingAPI
{
    public:
        void drawCircle(double x, double y, double radius)
        {
            cout << "API2.circle as " << x << ":" << y << " " << radius << endl;
        }
};

class Shape
{
    public:
        virtual ~Shape(){}
        virtual void draw() = 0;
        virtual void resizeByPercentage(double pct) = 0;
};

class CircleShape : public Shape
{
    public:
        CircleShape(double x, double y, double radius, shared_ptr<DrawingAPI> drawingAPI)
            :m_x(x), m_y(y), m_radius(radius), m_drawingAPI(drawingAPI)
        {}
        void draw(){
            m_drawingAPI->drawCircle(m_x, m_y, m_radius);
        }
        void resizeByPercentage(double pct)
        {
            m_radius *= pct;
        }
   private:
        double m_x, m_y, m_radius;
        shared_ptr<DrawingAPI> m_drawingAPI;
};

int main(int argc, char const* argv[])
{
    shared_ptr<CircleShape> circle1(new CircleShape(1, 2, 3, make_shared<DrawingAPI1> () ));
    shared_ptr<CircleShape> circle2(new CircleShape(5, 7, 10, make_shared<DrawingAPI2> () ));

    circle1->draw();
    circle2->draw();

    circle1->resizeByPercentage(2.5);
    circle2->resizeByPercentage(2.5);

    circle1->draw();
    circle2->draw();

    return 0;
}

