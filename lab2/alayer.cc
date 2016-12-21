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

#include "alayer.h"
#include "A_PDU_m.h"
#include "DL_PDU_m.h"
#include "MyData_m.h"
#include "Ack_m.h"

Define_Module(Alayer);

void Alayer::initialize()
{
    aid=par("aid");
    counter=par("counter");
    counter=0;
    if(aid==1)
    {
        cMessage *msg=new cMessage("event");
        scheduleAt(0,msg);
    }
}

void Alayer::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())
    {
        char msgname[20];
        for(int i=1;i<=10;i++)
        {
            MyData *data= new MyData();
            sprintf(msgname,"-%d",i);
            A_PDU* apdu=new A_PDU(msgname);
            apdu->encapsulate(data);
            apdu->setId(i);
            apdu->setType('D');
            send(apdu,"todlout");
        }
    }
    else if(msg->getArrivalGate()==gate("fromdlin"))
    {
        A_PDU *ap=check_and_cast<A_PDU*>(msg);
        delete(ap->decapsulate());
        /* Ack *ack= new Ack();
        A_PDU* apdu=new A_PDU();
        apdu->encapsulate(ack);
        apdu->setId(ap->getId());
        apdu->setType('A');
        send(ap,"todlout");*/
    }



}
