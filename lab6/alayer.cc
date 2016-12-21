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
    numSent=0;
    numRec=0;
    src=par("src");
    pktSent=par("pktSent");
    WATCH(numSent);
    WATCH(numRec);
    if(aid==src)
    {
        cMessage *msg=new cMessage("event");
        scheduleAt(0,msg);
        EV << "Application layer sends self message";
    }
}

void Alayer::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())
       {
           char msgname[20];
           for(int i=1;i<=pktSent;i++)
           {
               MyData *data= new MyData();
               sprintf(msgname,"-%d",i);
               A_PDU* apdu=new A_PDU(msgname);
               apdu->encapsulate(data);
               apdu->setId(i);
               apdu->setType('D');
               send(apdu,"tonw");
               numSent++;
               if (ev.isGUI())
                           updateDisplay();
               EV << "Sending APDU to data link layer";
           }
       }
    else if(msg->getArrivalGate()==gate("fromnw"))
       {
           A_PDU *ap=check_and_cast<A_PDU*>(msg);
           numRec++;
           if (ev.isGUI())
                       updateDisplay();
           EV << "Message Recieved in Application layer id:" << ap->getId();
           delete(ap->decapsulate());
           /* Ack *ack= new Ack();
           A_PDU* apdu=new A_PDU();
           apdu->encapsulate(ack);
           apdu->setId(ap->getId());
           apdu->setType('A');
           send(ap,"todlout");*/
       }
}
void Alayer::updateDisplay()
{
    char buf[40];
    sprintf(buf, "rcvd: %d sent: %d", numRec, numSent);
    getDisplayString().setTagArg("t",0,buf);
}




