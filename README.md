# ZTC_FantasticAlgorithm2020

## 中兴捧月神算师算法精英挑战赛

_使用广度优先搜索(bfs)寻找最短路径，在一定的规则下（不变道，不汇聚）暴力搜索出可行解。_

_算法设计和程序编写时间较紧凑，最终线上得分12.642。_

算法整体设计：

1. 读取输入，解析输入至 `StationInfo`(站点信息，拣货员数)，`TrackInfo`(轨道信息，连接站点)，`GoodsInfo`(货物信息，站点1，站点2，重量，必经站点列表)。

2. 建立邻接表，用于BFS求解Request的最短路径。

3. 在规则内运行BFS，规则如下：

* 只求解不包含必经站点的Request。

* 货物不变更车厢，不汇聚，每条轨道上的车厢只被一个货物占据。

* 拣货员只在货物出发和到达时消耗。

核心算法，以BFS为基础，加入上述规则后为如下形式：

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
