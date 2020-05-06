#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class Solution
{
public:
    void parseInput()
    {
        //FILE * inFile = fopen("/data/topo.txt", "r");
        ifstream inFile;
        inFile.open("data/topo.txt");
        if(!inFile.is_open())
        {
            cout << "Error opening file!" << endl;
            exit(1);
        }

        
        // string s;
        // while(getline(inFile, s))
        // {
        //     cout << s << endl;
        //     int a, b, c, d;
        //     int count = 0;
        //     int comma = 0;
        //     char ah[100];
        //     char bh[100];
        //     char ch[100];
        //     char dh[100];
        //     for (auto iter = s.begin(); iter != s.end(); iter++)
        //     {
        //         cout << *iter << endl;
        //         if (*iter != ',')
        //         {
        //             switch (comma)
        //             {
        //             case 0:
        //             {
        //                 ah[count] = *iter;
        //                 break;
        //             }

        //             case 1:
        //             {
        //                 bh[count] = *iter;
        //                 break;
        //             }

        //             case 2:
        //             {
        //                 ch[count] = *iter;
        //                 break;                       
        //             }

        //             case 3:
        //             {
        //                 dh[count] = *iter;
        //                 break;                       
        //             }    

        //             default:
        //                 break;
        //             }
        //         }
        //         else
        //         {
        //             ++comma;
        //             count = 0;
        //         }
                
        //     }

        //     if (comma == 4)
        //     {
                
        //     }
        //}

        inFile.close();
    }

private:
    int stationNumber;
    int trackNumber;
    int trainNumber;
    int maxLoad;
};

int main ()
{
    Solution solution;
    solution.parseInput();

    return 0;
}