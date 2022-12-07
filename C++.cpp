#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>

using namespace std;



class Site
{

public: 
    Site() = default;
    Site (int x, int y)
    :x(x),
    y(y)
    {}
    ~Site() = default;

    const int getOwner() {return owner;}
    void setOwner(const int& owner) {this->owner = owner;}

    const int getX() {return x;}
    void setX(const int& x) {this->x = x;}

    const int getY() {return y;}
    void setY(const int& y) {this->y = y;}

private:
    int x;
    int y;
    int owner; // -1 = No structure, 0 = Friendly, 1 = Enemy
};

//TODO: make this lambda
float computeDistance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

int closest(std::map<int, Site> allSites, std::pair<int, int> myQueen) {
    float reference = 10000, found = -1;
    float computedDistance = -1;

    for (auto it : allSites)//= allSites.begin(); it != allSites.end(); it++)
    {
        computedDistance = computeDistance(it.second.getX(), it.second.getY(), myQueen.first, myQueen.second);
        if ( computedDistance < reference)
        {
            found = it.first;
            reference = computedDistance;
        }
    }

    return found;
}


int main()
{
    int numSites;
    cin >> numSites; cin.ignore();

    std::map<int, Site> allSites;
    
    bool builtFirstSite = false;
    bool availableTraining = false;
    int availableTrainingSite = -1;
    int availableUnitTrain = -1;

    //Initial loop. The only way to get the coordinates for the sites
    for (int i = 0; i < numSites; i++) 
    {
        int siteId;
        int x;
        int y;
        int radius;
        cin >> siteId >> x >> y >> radius; cin.ignore();
        allSites.insert(std::pair<int, Site>(siteId, Site(x, y)));
    }

    int closestSite;

    // game loop
    while (1) 
    {
        std::pair<int, int> myQueen;
        int gold;
        int touchedSite; // -1 if none, siteId of the touched site
        bool builtTouchedSite = false;
        cin >> gold >> touchedSite; cin.ignore();
        //loop for each site
        for (int i = 0; i < numSites; i++) 
        {
            int siteId; //The numeric identifier of the site
            int ignore1; // used in future leagues
            int ignore2; // used in future leagues
            int structureType; // -1 = No structure, 2 = Barracks
            int owner; // -1 = No structure, 0 = Friendly, 1 = Enemy
            int param1; // -1 = No Barracks, 0 = Unit training available, X (1, 2, 3) = number of turns until the unit training is available
            int param2; // -1 = No structure, 0 = KNIGHT, 1 = ARCHER
            cin >> siteId >> ignore1 >> ignore2 >> structureType >> owner >> param1 >> param2; cin.ignore();

            allSites[siteId].setOwner(owner);
            if (touchedSite == siteId && 0 == owner)
            {
                builtTouchedSite = true;
            }

            if (param1 == 0 && owner == 0)
            {
                availableTraining = true;
                availableTrainingSite = siteId;
                //availableUnitTrain = param2; TODO: Not yet used
            }
        }
        int numUnits;
        cin >> numUnits; cin.ignore();
        for (int i = 0; i < numUnits; i++) 
        {
            int x; //Coordinates of the units
            int y; //Coordinates of the units
            int owner; // 0 = Friendly, 1 = Enemy
            int unitType; // -1 = QUEEN, 0 = KNIGHT, 1 = ARCHER
            int health;
            cin >> x >> y >> owner >> unitType >> health; cin.ignore();
            //looking for the friendly queen
            if (-1 == unitType && 0 == owner)
            {
                myQueen = std::make_pair(x, y);
            }
            
        }



            /***********************************************************/
        /************************* MOVE/BUILD COMMANDS *********************/
            /***********************************************************/


        int safeSite = closestSite;                    
        if (!builtFirstSite)
        {
            closestSite = closest(allSites, myQueen);   
            cout << "BUILD " << allSites.find(closestSite)->first << " BARRACKS-KNIGHT" << endl;
        }

        if (builtFirstSite)
        {
            if (touchedSite == allSites.find(closestSite)->first && builtTouchedSite)
            {
                allSites.erase(closestSite);
                closestSite = closest(allSites, myQueen);    
                if (1 == allSites[closestSite].getOwner())
                {
                    closestSite = safeSite;
                }
                
            }
            cout << "BUILD " << allSites.find(closestSite)->first << " BARRACKS-KNIGHT" << endl;           
        }



            /***********************************************************/
        /************************* TRAIN COMMANDS **************************/
            /***********************************************************/
        


        if (availableTraining)
        {
            builtFirstSite = true;
            cout << "TRAIN " << availableTrainingSite << endl;
        }
        else
        {
            cout << "TRAIN" << endl;    
        }
    }
}
