#include <iostream>
#include <vector>
#include <memory>
#include <chrono>

using namespace std;

class Vehicle
{
private:
    int rowNum;
    int spotNum;
    string vehicleNumber;
    chrono::time_point<chrono::system_clock>
        parkingStartTime;

public:
    Vehicle()
    {
        rowNum = -1;
        spotNum = -1;
    }

    void occupySpot(int r, int c)
    {
        rowNum = r;
        spotNum = c;
        parkingStartTime = std::chrono::system_clock::now();
    }

    void vacateSpot()
    {
        rowNum = -1;
        spotNum = -1;
    }

    // datatype calculate parking duration in hours
    /*
        use parking start time and end time
    */

    int calculateParkingDurationInHours()
    {
        auto now = chrono::system_clock::now();
        auto duration = chrono::duration_cast<chrono::hours>(now - parkingStartTime);
        return duration.count();
    }
    virtual int calculateParkingFee() = 0;
    virtual string getType() = 0;
};

class Bike : public Vehicle
{
public:
    string getType()
    {
        return "bike";
    }

    int calculateParkingFee()
    {
        return 10 + 2 * calculateParkingDurationInHours();
    }
};
class Car : public Vehicle
{
private:
public:
    string getType()
    {
        return "car";
    }

    int calculateParkingFee()
    {
        return 20 + 5 * calculateParkingDurationInHours();
    }
};

class Parking
{
private:
    int rows;
    int spotsPerRow;
    vector<vector<Vehicle *>> grid;

public:
    void create(int r, int c)
    {
        rows = r;
        spotsPerRow = c;
        grid.resize(r);

        for (int i = 0; i < r; i++)
        {
            grid[i].resize(c);
        }
    }

    pair<int, int> findVacantSpot()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < spotsPerRow; j++)
            {
                if (grid[i][j] == NULL)
                {
                    return {i, j};
                }
            }
        }

        cout << "no vacant parking spot found" << endl;
        return {-1, -1};
    }

    void park(Vehicle *v, int rowNum, int spotNum)
    {
        v->occupySpot(rowNum, spotNum);
        grid[rowNum][spotNum] = v;
    }

    void park(Vehicle *v)
    {
        pair<int, int> spot = findVacantSpot();
        if (spot.first >= 0 && spot.first < rows && spot.second >= 0 && spot.second < spotsPerRow)
        {
            park(v, spot.first, spot.second);
            cout << "thanks for parking , your" << v->getType() << "is parked at spot" << spot.first << " , " << spot.second << endl;
        }
        else
        {
            cout << "cannot park sorry" << endl;
        }
    }

    void leave(Vehicle *v)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < spotsPerRow; j++)
            {
                if (grid[i][j] == v)
                {
                    grid[i][j] = nullptr;

                    int fee = v->calculateParkingFee();
                    cout << "thnaks for leaving .fee is " << fee << endl;
                    v->vacateSpot();
                }
            }
        }
    }

    void printParking()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < spotsPerRow; j++)
            {
                cout << grid[i][j] << " ";
            }
            cout << endl;
        }
    }
};

int main()
{
    Parking p;
    p.create(3, 2);
    Car c1, c2;

    cout << c1.getType() << endl;
    p.printParking();

    p.park(&c1);

    p.printParking();
    p.park(&c2);

    p.printParking();

    Bike b1;
    p.park(&b1);

    p.leave(&c1);
    p.printParking();

    Bike b2;
    p.park(&b2);
    p.printParking();

    return 0;
}