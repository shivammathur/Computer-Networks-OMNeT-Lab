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

#include "p_layer_GNB.h"
#include "P_PDU_m.h"
#include "DL_PDU_m.h"

Define_Module(P_layer_GNB);

void P_layer_GNB::initialize()
{
    // TODO - Generated method body
    numSent=0;
    numRec=0;
    WATCH(numSent);
    WATCH(numRec);
}

void P_layer_GNB::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    if(msg->getArrivalGate()==gate("from_dl"))
       {
            numRec++;
            if (ev.isGUI())
                        updateDisplay();
           P_PDU* ppdu=new P_PDU();
           DL_PDU *dpdu=check_and_cast<DL_PDU *>(msg);
           ppdu->encapsulate(dpdu);
           send(ppdu,gate("to_node"));
           numSent++;
           if (ev.isGUI())
                       updateDisplay();
       }else if(msg->getArrivalGate()==gate("from_node"))
       {
           numRec++;
           if (ev.isGUI())
                       updateDisplay();
           P_PDU* ppdu=check_and_cast<P_PDU*>(msg);
               send(ppdu->decapsulate(),gate("to_dl"));
              numSent++;
              if (ev.isGUI())
                          updateDisplay();
       }
}
void P_layer_GNB::updateDisplay()
{
    char buf[40];
    sprintf(buf, "rcvd: %d sent: %d", numRec, numSent);
    getDisplayString().setTagArg("t",0,buf);
}
