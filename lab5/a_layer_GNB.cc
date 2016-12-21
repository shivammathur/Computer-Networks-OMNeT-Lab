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

#include "a_layer_GNB.h"
#include "Data_m.h"
#include "A_PDU_m.h"
Define_Module(A_layer_GNB);

void A_layer_GNB::initialize()
{
    // TODO - Generated method body
    numSent=numRec=0;
    a_id=par("a_id");
    if(a_id==1)
    {
        cMessage *msg=new cMessage("event");
        scheduleAt(0,msg);
    }
}

void A_layer_GNB::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    if(msg->isSelfMessage())
    {
        for(int i=1;i<=26;i++)
        {
               Data *data = new Data();
               char a='A'+i-1;
               data->setAlpha(a);
               char msgname[20];
               sprintf(msgname,"-%c",a);
               A_PDU *apdu = new A_PDU(msgname);
               apdu->encapsulate(data);
               send(apdu,"to_dl");
               numSent++;
               if (ev.isGUI())
                   updateDisplay();
        }
    }
    if(msg->getArrivalGate()==gate("from_dl"))
    {
        numRec++;
        if (ev.isGUI())
                    updateDisplay();
        A_PDU *apdu=check_and_cast<A_PDU*>(msg);

        EV << "Received Data: " ;
        delete(apdu);
    }
}
void A_layer_GNB::updateDisplay()
{
    char buf[40];
    sprintf(buf, "rcvd: %d sent: %d", numRec, numSent);
    getDisplayString().setTagArg("t",0,buf);
}
