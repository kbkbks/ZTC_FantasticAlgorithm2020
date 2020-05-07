#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <map>
#include <queue>

using namespace std;

class Solution
{
public:
    //实现任意类型的转换
    template<typename out_type, typename in_value>
    out_type convert(const in_value & t){
        stringstream stream;
        stream<<t;//向流中传值
        out_type result;//这里存储转换结果
        stream>>result;//向result中写入值
        return result;
    }

    vector<vector<string>> openFile(string FileName)
    {
        ifstream inFile;
        inFile.open(FileName);
        if(!inFile.is_open())
        {
            cout << "Error opening inFile!" << endl;
            exit(1);
        }

        vector<vector<string>> strArray;
        string lineStr;
        //二维表存储，分别存为string
        while(getline(inFile, lineStr))
        {
            //打印整行字符
            //cout << lineStr << endl;
            //每一行用stingstream按逗号分隔读取流，存入str
            stringstream ss(lineStr);
            string str;
            //存入一维数组lineArray
            vector<string> lineArray;
            while(getline(ss, str, ','))
            {
                lineArray.push_back(str);
            }
            strArray.push_back(lineArray);
        }

        inFile.close();

        return strArray;
    }

    void readData()
    {
        string topoFile = {"data/topo.txt"};
        string requestFile = {"data/request.txt"};
        //读取topo
        TopoArray = openFile(topoFile);
        //读取request
        RequestArray = openFile(requestFile);
    }

    void parseTopoData()
    {
        //解析TopoArray第一行
        vector<int> IntegerLine;
        for(auto iter = TopoArray[0].begin(); iter != TopoArray[0].end(); ++iter)
        {
            int val = convert<int>(*iter);
            IntegerLine.push_back(val);
        }
        stationNumber = IntegerLine[0];
        trackNumber = IntegerLine[1];
        trainNumber = IntegerLine[2];
        maxLoad = IntegerLine[3];

        //解析站点与拣货员信息
        for(int i = 1; i < stationNumber + 1; ++i)
        {
            int val = convert<int>(TopoArray[i][1]);
            StationInfo.insert(pair<string, int>(TopoArray[i][0], val));
        }

        cout << "StationInfo数量：" << StationInfo.size() << endl;

        //解析轨道与连接站点信息
        for(int i = stationNumber + 1; i < stationNumber + trackNumber + 1; ++i)
        {
            string TopoSubstring = TopoArray[i][2].substr(0, TopoArray[i][2].size()-1); 
            vector<string> jointStation = {TopoArray[i][1], TopoSubstring};
            TrackInfo.insert(pair<string, vector<string>>(TopoArray[i][0], jointStation));
        }

        cout << "TrackInfo数量：" << TrackInfo.size() << endl;

    }

    void parseRequestData()
    {
        //解析RequestArray第一行
        int val = convert<int>(RequestArray[0][0]);
        GoodsNumber = val;

        //解析货物信息
        for(int i = 1; i < GoodsNumber + 1; ++i)
        {
            vector<string> Info;
            for(auto iter = RequestArray[i].begin(); iter != RequestArray[i].end()-1; ++iter)
            {
                Info.push_back(*iter);
            }
            string GoodsSubstring = RequestArray[i].at(RequestArray[i].size()-1).substr(0,(RequestArray[i][RequestArray[i].size()-1]).size()-1);
            Info.push_back(GoodsSubstring);
            GoodsInfo.insert(pair<string, vector<string>>(RequestArray[i][0], Info));
        }

        cout << "GoodsInfo数量：" << GoodsInfo.size() << endl;
    }

    void buildAdjacencyGraph()
    {
        for(auto it = TrackInfo.begin(); it != TrackInfo.end(); ++it)
        {
            string subFirstStr = it->second[0].substr(1);
            string subSecondStr = it->second[1].substr(1);
            int FirstStation = convert<int>(subFirstStr);
            int SecondStation = convert<int>(subSecondStr);
            if(AdjacencyGraph.find(FirstStation) == AdjacencyGraph.end())
            {
                AdjacencyGraph.insert(pair<int, vector<int>>(FirstStation, vector<int>{SecondStation}));         
            }
            else
            {
                AdjacencyGraph.at(FirstStation).push_back(SecondStation);
            }

            if(AdjacencyGraph.find(SecondStation) == AdjacencyGraph.end())
            {
                AdjacencyGraph.insert(pair<int, vector<int>>(SecondStation, vector<int>{FirstStation})); 
            }
            else
            {
                AdjacencyGraph.at(SecondStation).push_back(FirstStation);
            }
        }

        cout << "AdjacencyGraph数量：" << AdjacencyGraph.size() << endl;
    }

    void solve()
    {

    }

    bool bfs()
    {

    }

private:
    vector<vector<string>> TopoArray;    //拓扑网络输入集
    vector<vector<string>> RequestArray;    //业务需求输入集
    //Topo信息
    int stationNumber;  //站点数
    int trackNumber;    //轨道数
    int trainNumber;    //列车数量
    int maxLoad;    //单个列车容量
    unordered_map<string, int> StationInfo; //站点信息，拣货员数
    unordered_map<string, vector<string>> TrackInfo;    //轨道信息，连接站点
    //Request信息
    int GoodsNumber;    //货物数量
    unordered_map<string, vector<string>> GoodsInfo;    //货物信息，站点1，站点2，重量，必经站点列表

    unordered_map<int, vector<int>> AdjacencyGraph; //站点邻接表
};

int main ()
{
    Solution solution;
    solution.readData();
    solution.parseTopoData();
    solution.parseRequestData();
    solution.buildAdjacencyGraph();
    solution.solve();

    return 0;
}