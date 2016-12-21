//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "host.h"
#include "MyPckt_m.h"
#include "Ack_m.h"

Define_Module(Host);

void Host::initialize()
{
    id=par("hostId");
    in=gate("in");
    out=gate("out");
    if(id==1)
    {
        cMessage* msg=new cMessage("event");

        scheduleAt(0,msg);

    }

    // TODO - Generated method body
}

void Host::handleMessage(cMessage *msg)
{

    if(msg->isSelfMessage())
    {
       // cMessage *first=new cMessage();
        MyPckt *pckt = new MyPckt();
        pckt->setId(counter);
        pckt->setType('D');
        send(pckt,out);
        counter++;
    }
    else
    {
        if(counter<5)
        {
            counter++;
           // cMessage *first=new cMessage();
            //send(first,out);
            MyPckt *pckt = check_and_cast<MyPckt *>(msg);
            if(id==1)
            {
                pckt->setId(counter);
                pckt->setType('D');
                send(pckt,out);
            }
            else
            {
                pckt->setType('A');
                send(pckt,out);
            }
        }
        else
            delete msg;
    }
    // TODO - Generated method body
}
