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

#include "dllayer.h"
#include "A_PDU_m.h"
#include "DL_PDU_m.h"
#include "Ack_m.h"
#include <cqueue.h>
Define_Module(Dllayer);

void Dllayer::initialize()
{
    // TODO - Generated method body
    start=par("start");
    end=par("end");
    start=0;
    end=0;
    counter=1;
}

void Dllayer::handleMessage(cMessage *msg)
{

    // TODO - Generated method body
    if(msg->getArrivalGate()==gate("fromAin"))
    {
        counter=(counter+1)%2;
        char msgname[20];
        A_PDU *apdu=check_and_cast<A_PDU*>(msg);
        sprintf(msgname,"-%d",counter);
        DL_PDU* dpdu=new DL_PDU(msgname);
        dpdu->encapsulate(apdu);
        dpdu->setType('D');
        buffer[end]=dpdu;
        end++;
        dpdu->setId(counter);
        if(start==0)
        {
            send(buffer[start],gate("toPout"));
            start++;
        }
    }else if(msg->getArrivalGate()==gate("fromPin"))
    {
        DL_PDU *dpdu=check_and_cast<DL_PDU*> (msg);
        if(dpdu->getType()=='A')
        {
            delete(dpdu->decapsulate());
            if(start<10)
            {
                send(buffer[start],gate("toPout"));
                start++;
            }
        }else if(dpdu->getType()=='D')
        {
            char msgname[20];
            send(dpdu->decapsulate(),gate("toAout"));
            Ack *ack= new Ack();
            sprintf(msgname,"-%d",counter);
            DL_PDU* dlpdu=new DL_PDU(msgname);
            dlpdu->setType('A');
            dlpdu->setId(counter);
            dlpdu->encapsulate(ack);
            send(dlpdu,gate("toPout"));
            counter=(counter+1)%2;
        }
    }
}
