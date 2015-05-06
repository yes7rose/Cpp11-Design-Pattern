#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <memory>

using namespace std;

enum class Input{DUCK_DOWN, STAND_UP, JUMP, DIVE};

class Fighter;

class StandingState;
class JumpingState;
class DivingState;

class FighterState
{
public:
    FighterState() = default;
    virtual ~FighterState() = default;

    virtual void handleInput(Fighter&, Input&) = 0;
    virtual void update(Fighter&) = 0;

public:
    static shared_ptr<StandingState> standing;
    static shared_ptr<DivingState> diving;
};

class DuckingState : public FighterState
{
private:
    int chargingTime;
    static const int FullRestTime = 5;

public:
    DuckingState()
        : chargingTime(0) {}

    virtual void handleInput(Fighter&, Input&) override final;
    virtual void update(Fighter&) override final;
};

class StandingState : public FighterState
{
public:
    void handleInput(Fighter &, Input &) override final;
    void update(Fighter &) override final;
};

class JumpingState : public FighterState
{
private:
    int jumpingHeight;
public:
    JumpingState() {jumpingHeight = rand() % 5 + 1;}

    virtual void handleInput(Fighter &, Input &) override final;
    virtual void update(Fighter &) override final ;
};

class DivingState : public FighterState
{
public:
   virtual void handleInput(Fighter &, Input &) override final;
   virtual void update(Fighter &) override final;
};

shared_ptr<StandingState> FighterState:: standing(new StandingState);
shared_ptr<DivingState> FighterState::diving(new DivingState);

class Fighter
{
private:
    string name;
    shared_ptr<FighterState> state;
    int fatigueLevel = rand()%10;
public:
    Fighter(const string& newName)
        :name(newName), state(FighterState::standing) {}

    string getName() const
    {
        return name;
    }

    int getFatigueLevel()
    {
        return fatigueLevel;
    }

    virtual void handleInput(Input input)
    {
        state->handleInput(*this, input);
    }

    virtual void changeState(shared_ptr<FighterState> newState)
    {
        state = newState;
        upadteWithNewState();
    }

    void standUp()
    {
        cout<< getName() << " stands up." << endl;
    }

    void duckDown()
    {
        cout<< getName() << " ducks down." << endl;
    }

    void jump()
    {
        cout<< getName() << " jump into the air." << endl;
    }

    void dives()
    {
        cout<< getName() << " make a dive attack in the middle of the jump." << endl;
    }

    void feelsStrong()
    {
        cout<< getName() << " stands up." << endl;
    }

    void changeFatigueLevelBy(int change)
    {
        fatigueLevel+=change;
        cout<< getName() << " stands up." << endl;
    }

private:
    virtual void upadteWithNewState()
    {
        state->update(*this);
    }
};

void StandingState::handleInput(Fighter& fighter, Input& input)
{
    switch (input) {
    case Input::STAND_UP:
        std::cout << fighter.getName() << " remains standing." << endl;
        return;
        break;
    case Input::DUCK_DOWN:
        fighter.changeState(make_shared<DuckingState> ());
        fighter.duckDown();
        return;
        break;
    case Input::JUMP:
        fighter.jump();
        fighter.changeState(make_shared<JumpingState> ());
        return;
    default:
        cout<< "One cannot do that while standing." << fighter.getName() << " remains standing by default." << std::endl;
        break;
    }
}

void StandingState:: update(Fighter & fighter)
{
    if (fighter.getFatigueLevel() > 0)
        fighter.changeFatigueLevelBy(-1);
}

void DuckingState::handleInput(Fighter &fighter, Input &input)
{
    switch(input)
    {
        case Input::STAND_UP:
            fighter.changeState(FighterState::standing);
            fighter.standUp();
            return;
            break;
        case Input::DUCK_DOWN:
            cout << fighter.getName() << " remains in ducking position, ";
            if (chargingTime < FullRestTime)
                cout << "recovering in the meantime." << endl;
            else
                cout<< "fully recovered." << endl;
            update(fighter);
            return;

        default:
            cout << " One cannot do that while ducking. " << fighter.getName()
                 << " remains in ducking position by default." << endl;
            update(fighter);
    }
}

void DuckingState::update (Fighter& fighter)
{
    chargingTime++;
    std::cout << "Charging time = " << chargingTime << "." << std::endl;
    if (fighter.getFatigueLevel() > 0)
        fighter.changeFatigueLevelBy(-1);
    if (chargingTime >= FullRestTime && fighter.getFatigueLevel() <= 3)
        fighter.feelsStrong();
}

void JumpingState::handleInput(Fighter& fighter, Input& input)
{
    switch (input) {
        case Input::DIVE:
            fighter.changeState (FighterState::diving);
            return fighter.dives();
        default:
            std::cout << "One cannot do that in the middle of a jump.  " << fighter.getName()
                      << " lands from his jump and is now standing again." << std::endl;
            fighter.changeState (FighterState::standing);
    }
}

void JumpingState::update (Fighter& fighter)
{
    std::cout << fighter.getName() << " has jumped " << jumpingHeight
              << " feet into the air." << std::endl;
    if (jumpingHeight >= 3)
        fighter.changeFatigueLevelBy(1);
}

void DivingState::handleInput(Fighter& fighter, Input& input)
{
    std::cout << "Regardless of what the user input is, " << fighter.getName()
              << " lands from his dive and is now standing again." << std::endl;
    fighter.changeState (FighterState::standing);
}

void DivingState::update (Fighter& fighter)
{
    fighter.changeFatigueLevelBy(2);
}

int main()
{
    srand(time(nullptr));

    Fighter xiaoLong("XiaoLong");
    Fighter yeWen("YeWen");

    unsigned int choice;

    auto chooseAction = [&choice](Fighter& fighter)
    {
        cout<< endl
            << static_cast<const int> (Input::DUCK_DOWN) << ":Duck Down" << endl
            <<static_cast<const int> (Input::STAND_UP) << ":Stand up"<<endl
            << static_cast<const int> (Input::JUMP) << ":Jump into the air" << endl
           <<static_cast<const int> (Input::DIVE) << ":Dive in the middle of a jump" <<endl;
        cin>> choice;

        Input input;

        if(choice == 0)
        {
            input = Input::DUCK_DOWN;
        }
        else if(choice == 1)
        {
            input = Input::STAND_UP;
        }
        else if(2==choice)
        {
            input = Input::JUMP;
        }
        else {
            input = Input::DIVE;
        }

        fighter.handleInput(input);

    };

    while(true)
    {
        chooseAction(xiaoLong);
       // chooseAction(yeWen);
    }

    return 0;

}

