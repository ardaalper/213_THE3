#include "CENGFlight.h"
#include <iostream>

//=======================//
// Implemented Functions //
//=======================//
void CENGFlight::PrintCanNotHalt(const std::string& airportFrom,
                                 const std::string& airportTo,
                                 const std::string& airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines is not found and cannot be halted"
              << std::endl;
}

void CENGFlight::PrintCanNotResumeFlight(const std::string& airportFrom,
                                         const std::string& airportTo,
                                         const std::string& airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines cannot be resumed"
              << std::endl;
}

void CENGFlight::PrintFlightFoundInCache(const std::string& airportFrom,
                                         const std::string& airportTo,
                                         bool isCostWeighted)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              <<  " is found in cache." << std::endl;
}

void CENGFlight::PrintFlightCalculated(const std::string& airportFrom,
                                       const std::string& airportTo,
                                       bool isCostWeighted)
{
    std::cout << "A flight path is calculated between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              <<  "." << std::endl;
}

void CENGFlight::PrintPathDontExist(const std::string& airportFrom,
                                    const std::string& airportTo)
{
    std::cout << "A flight path does not exists between \""
              << airportFrom << "\" and \""
              << airportTo <<"\"." << std::endl;
}

void CENGFlight::PrintSisterAirlinesDontCover(const std::string& airportFrom)
{
    std::cout << "Could not able to generate sister airline list from \""
              << airportFrom <<"\"." << std::endl;
}

void CENGFlight::PrintMap()
{
    navigationMap.PrintEntireGraph();
}

void CENGFlight::PrintCache()
{
    lruTable.PrintTable();
}

CENGFlight::CENGFlight(const std::string& flightMapPath)
    : navigationMap(flightMapPath)
{}

//=======================//
//          TODO         //
//=======================//
/* Sometimes a flight is not available due to plane maintenance. In this case, the edge with the
 corresponding should temporarily be removed from the flight map. This function should try to
 remove the edge defined by the airport names and the airline name. If such an edge exists CENG
 Flight System should temporarily store the names and weights of this particular edge in a data
 structure. We did not specify the actual temporary data structure, you can use any data structure you
 want. We do provide HaltedFlight structure for you to use, however. If such an edge or airport does 
 not exist, the function should print PrintCanNotHalt(...).*/
void CENGFlight::HaltFlight(const std::string& airportFrom,
                            const std::string& airportTo,
                            const std::string& airlineName)
{
    // 1. Check if the flight path exists in the navigationMap.
    // 2. If it exists, remove the edge corresponding to the halted flight.
    // 3. Add the halted flight to the haltedFlights vector.
    // 4. Print messages accordingly using PrintCanNotHalt if needed.
    int flag1=1;
    int flag2=1;
    int flag3=1;
    
    int fromindex=0;
    for(;fromindex<navigationMap.getVertexList().size();fromindex++){
        if(navigationMap.getVertexList()[fromindex].name==airportFrom){
            flag1=0;
            break;
        }
    }
    
    if(flag1){
        PrintCanNotHalt(airportFrom, airportTo, airlineName);
        return;
    }
    
    int toindex=0;
    for(;toindex<navigationMap.getVertexList().size();toindex++){
        if(navigationMap.getVertexList()[toindex].name==airportTo){
            flag2=0;
            break;
        }
    }
    
    if(flag2){
        PrintCanNotHalt(airportFrom, airportTo, airlineName);
        return;
    }
    
    int edgeindex=0;
    for(;edgeindex<navigationMap.getVertexList()[fromindex].edges.size(); edgeindex++){
        if(navigationMap.getVertexList()[fromindex].edges[edgeindex].name==airlineName){
            flag3=0;
            break;
        }
    }
    if(flag3){
        PrintCanNotHalt(airportFrom, airportTo, airlineName);
        return;
    }
    /////////////////////////////////////////////////
    HaltedFlight mytemp;
    mytemp.airportFrom=airportFrom;
    mytemp.airportTo=airportTo;
    mytemp.airline=airlineName;
    
    mytemp.w0=navigationMap.getVertexList()[fromindex].edges[edgeindex].weight[0];
    mytemp.w1=navigationMap.getVertexList()[fromindex].edges[edgeindex].weight[1];
    
    haltedFlights.push_back(mytemp);
    navigationMap.RemoveEdge(airlineName,airportFrom,airportTo);
}


    // (Direct Function call)
/*Similar to the function above this function re-enables previously halted flight. 
If this flight either does not exist or it is not halted, it should print 
PrintCanNotResumeFlight(...)*/
void CENGFlight::ContinueFlight(const std::string& airportFrom,
                                const std::string& airportTo,
                                const std::string& airlineName)
{
    /* TODO */
    int flag=1;
    for(int i=0; i<haltedFlights.size(); i++){
        if(haltedFlights[i].airportFrom==airportFrom, haltedFlights[i].airportTo==airportTo, haltedFlights[i].airline==airlineName){
            navigationMap.AddEdge(airlineName,airportFrom,airportTo,haltedFlights[i].w0,haltedFlights[i].w1);
            haltedFlights.erase(haltedFlights.begin()+i);
            flag=0;
            break;
        }
    }
    if(flag){
        PrintCanNotResumeFlight(airportFrom, airportTo, airlineName);
    }
}

void CENGFlight::FindFlight(const std::string& startAirportName,
                            const std::string& endAirportName,
                            float alpha)
{
    /* TODO */
    std::vector<int> orderedVertexEdgeIndexList;
    bool found = navigationMap.HeuristicShortestPath(orderedVertexEdgeIndexList, startAirportName, endAirportName, alpha);

    if (found) {
        PrintFlightCalculated(startAirportName, endAirportName, true);
        navigationMap.PrintPath(orderedVertexEdgeIndexList, alpha);
    } else {
        PrintPathDontExist(startAirportName, endAirportName);
    }
    
}

void CENGFlight::FindSpecificFlight(const std::string& startAirportName,
                                    const std::string& endAirportName,
                                    float alpha,
                                    const std::vector<std::string>& unwantedAirlineNames) const
{
    /* TODO */

}

void CENGFlight::FindSisterAirlines(std::vector<std::string>& airlineNames,
                               const std::string& startAirlineName,
                               const std::string& airportName) const
{
    /* TODO */
}

int CENGFlight::FurthestTransferViaAirline(const std::string& airportName,
                                           const std::string& airlineName) const
{
    /* TODO */
    return -1;
}