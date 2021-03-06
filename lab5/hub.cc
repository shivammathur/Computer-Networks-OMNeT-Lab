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

#include "hub.h"
#include "P_PDU_m.h"

Define_Module(Hub);

void Hub::initialize()
{
    // TODO - Generated method body
}

void Hub::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    P_PDU *ppdu=check_and_cast<P_PDU*>(msg);
    if(msg->getArrivalGate()==gate("from_node1"))
    {
        send(ppdu,gate("to_node2"));
    }
    else if(msg->getArrivalGate()==gate("from_node2"))
    {
        send(ppdu,gate("to_node1"));
    }
}
