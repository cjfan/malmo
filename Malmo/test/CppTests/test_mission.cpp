// --------------------------------------------------------------------------------------------------
//  Copyright (c) 2016 Microsoft Corporation
//  
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
//  associated documentation files (the "Software"), to deal in the Software without restriction,
//  including without limitation the rights to use, copy, modify, merge, publish, distribute,
//  sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in all copies or
//  substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
//  NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
//  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// --------------------------------------------------------------------------------------------------

// Malmo:
#include <MissionSpec.h>
using namespace malmo;

// STL:
#include <exception>
#include <iostream>
#include <string>
using namespace std;

int main() 
{
    MissionSpec my_mission = MissionSpec();
    my_mission.timeLimitInSeconds( 10 );
    my_mission.drawBlock( 19, 0, 19, "redstone_block" );
    my_mission.createDefaultTerrain();
    my_mission.setTimeOfDay(6000,false);
    my_mission.drawCuboid(50,0,50,100,10,100,"redstone_block");
    my_mission.drawItem(3,0,2,"diamond_pickaxe");
    my_mission.drawSphere(50,10,50,10,"ice");
    my_mission.drawLine(50,20,50,100,20,100,"redstone_block");
    my_mission.startAt( 2, 0, 2 );
    my_mission.endAt( 19.5f, 0.0f, 19.5f, 1.0f );
    my_mission.requestVideo( 320, 240 );
    my_mission.setModeToCreative();
    my_mission.rewardForReachingPosition(19.5f,0.0f,19.5f,100.0f,1.1f);
    my_mission.observeRecentCommands();
    my_mission.observeHotBar();
    my_mission.observeFullInventory();
    my_mission.observeGrid(-2,0,-2,2,1,2, "Cells");
    my_mission.observeDistance(19.5f,0.0f,19.5f,"Goal");
    my_mission.removeAllCommandHandlers();
    my_mission.allowContinuousMovementCommand("move");
    my_mission.allowContinuousMovementCommand("strafe");
    my_mission.allowDiscreteMovementCommand("movenorth");
    my_mission.allowInventoryCommand("selectInventoryItem");

    // check that the XML we produce validates
    const bool pretty_print = false;
    string xml = my_mission.getAsXML(pretty_print);
    
    try
    {
        const bool validate = true;
        MissionSpec my_mission2 = MissionSpec( xml, validate );

        // check that we get the same XML if we go round again
        string xml2 = my_mission2.getAsXML(pretty_print);
        if( xml2 != xml )
        {
            cout << "Mismatch between first generation XML and the second:\n\n" << xml << "\n\n" << xml2 << endl;
            return EXIT_FAILURE;
        }
    } 
    catch( const xml_schema::exception& e )
    {
        cout << "Error validating the XML we generated: " << e.what() << "\n" << e << endl;
        return EXIT_FAILURE;
    }
    
    // check that known-good XML validates
    const string xml3 = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?><Mission xmlns=\"http://ProjectMalmo.microsoft.com\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://ProjectMalmo.microsoft.com Mission.xsd\">\
        <About><Summary>Run the maze!</Summary></About>\
        <ServerSection><ServerInitialConditions><AllowSpawning>true</AllowSpawning><Time><StartTime>1000</StartTime><AllowPassageOfTime>true</AllowPassageOfTime></Time><Weather>clear</Weather></ServerInitialConditions>\
        <ServerHandlers>\
        <FlatWorldGenerator generatorString=\"3;7,220*1,5*3,2;3;,biome_1\" />\
        <ServerQuitFromTimeUp timeLimitMs=\"20000\" />\
        <ServerQuitWhenAnyAgentFinishes />\
        </ServerHandlers></ServerSection>\
        <AgentSection><Name>Jason Bourne</Name><AgentStart><Placement x=\"-204\" y=\"81\" z=\"217\"/></AgentStart><AgentHandlers>\
        <VideoProducer want_depth=\"true\"><Width>320</Width><Height>240</Height></VideoProducer>\
        <RewardForReachingPosition><Marker reward=\"100\" tolerance=\"1.1\" x=\"-104\" y=\"81\" z=\"217\"/></RewardForReachingPosition>\
        <ContinuousMovementCommands />\
        <AgentQuitFromReachingPosition><Marker x=\"-104\" y=\"81\" z=\"217\"/></AgentQuitFromReachingPosition>\
        </AgentHandlers></AgentSection></Mission>";
    try 
    {
        const bool validate = true;
        MissionSpec my_mission3( xml3, validate );
    } 
    catch( const xml_schema::exception& e )
    {
        cout << "Error validating known-good XML: " << e.what() << "\n" << e << endl;
        return EXIT_FAILURE;
    }
       
    return EXIT_SUCCESS;
}
