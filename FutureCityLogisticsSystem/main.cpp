/*
# Copyright (c) 2020 Xinyan Han. All rights reserved.
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <map>
#include <queue>
#include <iomanip>

using namespace std;

typedef struct Node
{
    int station;    //站点编号
    float capacity;   //列车容量
    int people; //拣货员数量
    vector<string> train;  //下标为列车编号，值为货物编号
}Node;

typedef struct Route
{
    string GoodNum; //货物编号
    float Weight; //货物重量
    vector<int> Path;   //路径
    int trainNum;   //列车编号
    vector<string> TrackNum;    //轨道编号
}Route;

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

    vector<vector<string>> getInfoArray()
    {
        vector<vector<string>> strArray;
        string lineStr;
        //二维表存储，分别存为string
        //cout << "输入：" << endl;
        while(getline(cin, lineStr))
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

        return strArray;
    }

    void ReadFromTerminal()
    {
        AllArray = getInfoArray();

        vector<int> IntegerLine;
        for(auto iter = AllArray[0].begin(); iter != AllArray[0].end() - 1; ++iter)
        {
            int val = convert<int>(*iter);
            IntegerLine.push_back(val);
        }
        float Weight = convert<float>(AllArray[0][3]);
        stationNumber = IntegerLine[0];
        trackNumber = IntegerLine[1];
        trainNumber = IntegerLine[2];
        maxLoad = Weight;

        for(int i = 0; i < stationNumber + trackNumber + 1; ++i)
        {
            TopoArray.push_back(AllArray[i]);
        }

        for(int i = stationNumber + trackNumber + 1; i < AllArray.size(); ++i)
        {
            RequestArray.push_back(AllArray[i]);
        }
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

        //cout << "StationInfo数量：" << StationInfo.size() << endl;

        //解析轨道与连接站点信息
        for(int i = stationNumber + 1; i < stationNumber + trackNumber + 1; ++i)
        {
            string TopoSubstring = TopoArray[i][2].substr(0, TopoArray[i][2].size()-1); 
            vector<string> jointStation = {TopoArray[i][1], TopoSubstring};
            TrackInfo.insert(pair<string, vector<string>>(TopoArray[i][0], jointStation));
        }

        //cout << "TrackInfo数量：" << TrackInfo.size() << endl;

    }

    void NewparseTopoData()
    {
        //解析站点与拣货员信息
        for(int i = 1; i < stationNumber + 1; ++i)
        {
            int val = convert<int>(TopoArray[i][1]);
            StationInfo.insert(pair<string, int>(TopoArray[i][0], val));
        }

        //cout << "StationInfo数量：" << StationInfo.size() << endl;

        //解析轨道与连接站点信息
        for(int i = stationNumber + 1; i < stationNumber + trackNumber + 1; ++i)
        { 
            vector<string> jointStation = {TopoArray[i][1], TopoArray[i][2]};
            TrackInfo.insert(pair<string, vector<string>>(TopoArray[i][0], jointStation));
        }

        //cout << "TrackInfo数量：" << TrackInfo.size() << endl;

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

            //去\r专用
            string GoodsSubstring = RequestArray[i].at(RequestArray[i].size()-1).substr(0,(RequestArray[i][RequestArray[i].size()-1]).size()-1);
            Info.push_back(GoodsSubstring);
            GoodsInfo.insert(pair<string, vector<string>>(RequestArray[i][0], Info));
        }

        //cout << "GoodsInfo数量：" << GoodsInfo.size() << endl;
    }

    void NewparseRequestData()
    {
        //解析RequestArray第一行
        int val = convert<int>(RequestArray[0][0]);
        GoodsNumber = val;

        //解析货物信息
        for(int i = 1; i < GoodsNumber + 1; ++i)
        {
            vector<string> Info;
            for(auto iter = RequestArray[i].begin(); iter != RequestArray[i].end(); ++iter)
            {
                Info.push_back(*iter);
            }
            GoodsInfo.insert(pair<string, vector<string>>(RequestArray[i][0], Info));
        }

        //cout << "GoodsInfo数量：" << GoodsInfo.size() << endl;
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
                visited.insert(pair<int, bool>(FirstStation, false));
                buildStationUsage(FirstStation);
            }
            else
            {
                AdjacencyGraph.at(FirstStation).push_back(SecondStation);
            }

            if(AdjacencyGraph.find(SecondStation) == AdjacencyGraph.end())
            {
                AdjacencyGraph.insert(pair<int, vector<int>>(SecondStation, vector<int>{FirstStation}));
                visited.insert(pair<int, bool>(SecondStation, false));  
                buildStationUsage(SecondStation);
            }
            else
            {
                AdjacencyGraph.at(SecondStation).push_back(FirstStation);
            }
        }

        //cout << "AdjacencyGraph数量：" << AdjacencyGraph.size() << endl;
        //cout << "visited数量：" << visited.size() << endl;
    }

    void buildStationUsage(int StaionNum)
    {
        Node node; 
        string substrStation = convert<string>(StaionNum);
        string strStation = {"Z"};
        strStation.append(substrStation);

        node.station = StaionNum;
        node.capacity = maxLoad;
        node.people = StationInfo.at(strStation);
        for(int i = 0; i < trainNumber; ++i)
        {
            node.train.push_back("null");
        }

        StationUsage.insert(pair<int, Node>(StaionNum, node));        
    }

    void solve()
    {
        for(auto iter = GoodsInfo.begin(); iter != GoodsInfo.end(); ++iter)
        {
            string subStartStr = iter->second[1].substr(1);
            int start = convert<int>(subStartStr);
            string subEndStr = iter->second[2].substr(1);
            int end = convert<int>(subEndStr);
            float weight = convert<float>(iter->second[3]);
            vector<int> MustPass;
            string error = {"null"};
            if(iter->second[4] != error)
            {
                //string subMP1Str = iter->second[4].substr(1);
                //string subMP2Str = iter->second[5].substr(1);
                //MustPass = {convert<int>(subMP1Str), convert<int>(subMP2Str)};
                MustPass.push_back(1);
            }

            bool PlanningStatus = bfs(iter->first, start, end, weight, MustPass);

            if(PlanningStatus != true)
            {
                saveFailed(iter->first, weight);
            }

            for (auto it = visited.begin(); it != visited.end(); ++it)
            {
                it->second = false;
            }  
        }      
    }

    bool bfs(string Good, int startNode, int endNode, float weight, vector<int> MustPass)
    {
        queue<int> STqueue; //站点队列
        vector<int> path;   //规划路径 
        unordered_map<int, int> prev;   //搜索路径，记录前驱顶点
        //首节点入队列
        STqueue.push(startNode);
        visited.at(startNode) = true;   //首节点已访问，设为true

        //出发点拣货员数大于0，列车容量大于等于载货量, 没有必经节点
        if(StationUsage[startNode].people > 0 && StationUsage[startNode].capacity >= weight && MustPass.empty())
        {
            for(int Tr = 0; Tr < trainNumber; ++Tr)
            {
                //出发节点列车是否被占用
                if(StationUsage[startNode].train[Tr] == "null")
                {
                    //StationUsage[startNode].train[i] = Good;

                    while(!STqueue.empty())
                    {
                        //取出当前队列的头节点
                        int currentNode = STqueue.front();
                        STqueue.pop();

                        for (int adj : AdjacencyGraph[currentNode])
                        {
                            //判断后驱节点列车是否为空
                            if(StationUsage[adj].train[Tr] == "null")
                            {
                                if(!visited[adj])
                                {
                                    //记录path
                                    prev.insert(pair<int, int>(adj, currentNode));

                                    if(adj == endNode)
                                    {
                                        //判断到达点拣货员数是否足够
                                        if(StationUsage[adj].people > 0)
                                        {
                                            path = getPath(prev, startNode, endNode, path);
                                            //printPath(Good, path, Tr);
                                            recordStationUsage(Good, path, Tr);
                                            saveResult(Good, weight, path, Tr);

                                            return true;
                                        }
                                    }

                                    STqueue.push(adj);
                                    visited.at(adj) = true;
                                }                                
                            }

                        }
                    }

                }
            }
        }

        return false;
    }

    vector<int> getPath(unordered_map<int, int> prev, int startNode, int endNode, vector<int> path)
    {
        if (prev.find(endNode) != prev.end() && startNode != endNode)
        {
            path = getPath(prev, startNode, prev[endNode], path);
        }
        path.push_back(endNode);

        return path;
    }

    void printPath(string Good, vector<int> path, int Tr)
    {
        cout << Good << "--" << endl;
        for (auto it = path.begin(); it != path.end(); ++it)
        {
            cout << *it << ",";
        }
        cout << endl;
        for (int i  = 0; i < path.size(); ++i)
        {
            cout << Tr << ",";
        }
        cout << endl;
    }

    void recordStationUsage(string Good, vector<int> path, int Tr)
    {
        int startNode = *path.begin();
        int endNode = *(path.end()-1);
        for(auto iter = path.begin(); iter != path.end(); ++iter)
        {
            StationUsage[*iter].train[Tr] = Good;
        }
        StationUsage[startNode].people--;
        StationUsage[endNode].people--;
    }

    void saveResult(string Good, float weight, vector<int> path, int Tr)
    {
        Route route;
        route.GoodNum = Good;
        route.Weight = weight;
        route.Path = path;
        route.trainNum = Tr + 1;    //列车编号从1开始
        for(int i = 0; i < path.size() - 1; ++i)
        {
            string trackNum = findTrack(path[i], path[i + 1]);
            route.TrackNum.push_back(trackNum);
        }

        result.push_back(route);
    }

    void saveFailed(string Good, float weight)
    {
        Route route;
        route.GoodNum = Good;
        route.Weight = weight;
        route.trainNum = -1;
        string FailedTrack = {"null"};
        route.TrackNum.push_back(FailedTrack);

        Failed.push_back(route);
    }

    string findTrack(int firstStaion, int secondStation)
    {
        vector<string> strPairStation;
        vector<string> strReverseStation;

        string subFirstStation = convert<string>(firstStaion);
        string strFirstStation = {"Z"};
        strFirstStation.append(subFirstStation);
        strPairStation.push_back(strFirstStation);

        string subSecondStation = convert<string>(secondStation);
        string strSecondStation = {"Z"};
        strSecondStation.append(subSecondStation);
        strPairStation.push_back(strSecondStation);

        strReverseStation.push_back(strSecondStation);
        strReverseStation.push_back(strFirstStation);

        for (auto iter = TrackInfo.begin(); iter != TrackInfo.end(); ++iter)
        {
            if (iter->second == strPairStation)
            {
                //cout << iter->first << endl;
                
                return iter->first;
            }
            if (iter->second == strReverseStation)
            {
                return iter->first;
            }
            
        }

        string error = "null";
        return error;
    }

    void printResult()
    {
        int successPlanedNum = result.size();
        float SumSuccessWeight = 0;  //规划成功货物总重量
        for (auto iter = result.begin(); iter != result.end(); ++iter)
        {
            SumSuccessWeight  = SumSuccessWeight + iter->Weight;
        }

        int failPlanedNum = Failed.size();
        float SumFailedWeight = 0;    //规划失败总重量
        for (auto iter = Failed.begin(); iter != Failed.end(); ++iter)
        {
            SumFailedWeight = SumFailedWeight + iter->Weight;
        }

        //cout << successPlanedNum << "," << SumSuccessWeight << endl;
        cout << failPlanedNum << "," << setiosflags(ios::fixed) << setprecision(3) << SumFailedWeight << endl;

        //打印成功的Track和tr
        for (auto iter = result.begin(); iter != result.end(); ++iter)
        {
            //打印Good
            cout << iter->GoodNum << endl;
            //打印Track
            for (auto it = iter->TrackNum.begin(); it != iter->TrackNum.end() - 1; ++it)
            {
                cout << *it << ",";
            }
            cout << iter->TrackNum[iter->TrackNum.size()-1] << endl;
            //打印Tr
            for (int i = 0; i < iter->TrackNum.size() - 1; ++i)
            {
                cout << iter->trainNum << ",";
            }
            cout << iter->trainNum << endl;
        }

        //打印失败的货物
        for (auto iter = Failed.begin(); iter != Failed.end(); ++iter)
        {
            //打印Good
            cout << iter->GoodNum << endl;
            //打印Track
            cout << "null" << endl;
            //打印Tr
            cout << "null" << endl;
        }

    }

    void writeResult()
    {
        ofstream outFile("data/result.txt");

        int successPlanedNum = result.size();
        float SumSuccessWeight = 0;  //规划成功货物总重量
        for (auto iter = result.begin(); iter != result.end(); ++iter)
        {
            SumSuccessWeight  = SumSuccessWeight + iter->Weight;
        }

        int failPlanedNum = Failed.size();
        float SumFailedWeight = 0;    //规划失败总重量
        for (auto iter = Failed.begin(); iter != Failed.end(); ++iter)
        {
            SumFailedWeight = SumFailedWeight + iter->Weight;
        }

        //cout << successPlanedNum << "," << SumSuccessWeight << endl;
        outFile << failPlanedNum << "," << setiosflags(ios::fixed) << setprecision(3) << SumFailedWeight << endl;

        //打印成功的Track和tr
        for (auto iter = result.begin(); iter != result.end(); ++iter)
        {
            //打印Good
            outFile << iter->GoodNum << endl;
            //打印Track
            for (auto it = iter->TrackNum.begin(); it != iter->TrackNum.end() - 1; ++it)
            {
                outFile << *it << ",";
            }
            outFile << iter->TrackNum[iter->TrackNum.size()-1] << endl;
            //打印Tr
            for (int i = 0; i < iter->TrackNum.size() - 1; ++i)
            {
                outFile << iter->trainNum << ",";
            }
            outFile << iter->trainNum << endl;
        }

        //打印失败的货物
        for (auto iter = Failed.begin(); iter != Failed.end(); ++iter)
        {
            //打印Good
            outFile << iter->GoodNum << endl;
            //打印Track
            outFile << "null" << endl;
            //打印Tr
            outFile << "null" << endl;
        }
    }


private:
    //输入集信息
    vector<vector<string>> AllArray;    //所有数据输入集
    vector<vector<string>> TopoArray;    //拓扑网络输入集
    vector<vector<string>> RequestArray;    //业务需求输入集
    //Topo信息
    int stationNumber;  //站点数
    int trackNumber;    //轨道数
    int trainNumber;    //列车数量
    float maxLoad;    //单个列车容量
    unordered_map<string, int> StationInfo; //站点信息，拣货员数
    unordered_map<string, vector<string>> TrackInfo;    //轨道信息，连接站点
    //Request信息
    int GoodsNumber;    //货物数量
    unordered_map<string, vector<string>> GoodsInfo;    //货物信息，站点1，站点2，重量，必经站点列表
    //邻接表信息
    unordered_map<int, vector<int>> AdjacencyGraph; //站点邻接表
    unordered_map<int, bool> visited;   //已访问的节点为true
    unordered_map<int, Node> StationUsage;  //站点使用情况

    vector<Route> result;  //规划成功结果
    vector<Route> Failed;   //规划失败
};

int main ()
{
    Solution solution;
    //标准输入
    solution.ReadFromTerminal();
    solution.NewparseTopoData();
    solution.NewparseRequestData();

    //文件读取
    //solution.readData();
    //solution.parseTopoData();
    //solution.parseRequestData();
    solution.buildAdjacencyGraph();
    solution.solve();
    solution.printResult();
    solution.writeResult();

    return 0;
}