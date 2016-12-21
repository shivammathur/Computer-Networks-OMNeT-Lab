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

#include "nwlayerB.h"
#include "A_PDU_m.h"
#include "NW_PDU_m.h"
Define_Module(NwlayerB);

void NwlayerB::initialize()
{
    nid=par("nid");
    src=par("src");
    dest=par("dest");
    numSent=0;
    numRec=0;
    table[1]=make_pair(5,0);
    table[2]=make_pair(0,0);
    table[3]=make_pair(4,0);
    table[4]=make_pair(8,1);
    table[5]=make_pair(3,0);
    todlgate[1]=gate("todlA");
    todlgate[3]=gate("todlC");
    todlgate[5]=gate("todlE");
    delayStats.setName("Delay of Packets");
    delayStats.setRangeAutoUpper(0,10,2.0);
    delayVector.setName("Delay Vector");
}

void NwlayerB::handleMessage(cMessage *msg)
{
    numRec++;
    if (ev.isGUI())
        updateDisplay();
    if(msg->getArrivalGate()==gate("fromal"))
        {
            A_PDU *apdu = check_and_cast<A_PDU*>(msg);
            NW_PDU *npdu =new NW_PDU();
            npdu->setSrcAdd(src);
            npdu->setDestAdd(dest);
            npdu->setTime(simTime());
            npdu->encapsulate(apdu);
            if(table[dest].second==0)
            {
                send(npdu,todlgate[dest]);
            }
            else{
                send(npdu,todlgate[table[dest].second]);
            }

        }else
        {
            NW_PDU *npdu = check_and_cast<NW_PDU*>(msg);
            if(npdu->getDestAdd()==nid)
            {
                simtime_t delay=simTime()-npdu->getTime();
                delayStats.collect(delay);
                delayVector.record(delay);
                send(npdu->decapsulate(),"toal");
                EV << "Network layer accepts PDU!!";
            }
            else{
                if(table[npdu->getDestAdd()].second==0)
                {
                    send(npdu,todlgate[npdu->getDestAdd()]);
                }
                else{
                    send(npdu,todlgate[table[npdu->getDestAdd()].second]);
                }
            }
        }
    numSent++;
    if (ev.isGUI())
         updateDisplay();
}
void NwlayerB::updateDisplay()
{
    char buf[40];
    sprintf(buf, "rcvd: %d sent: %d", numRec, numSent);
    getDisplayString().setTagArg("t",0,buf);
}
void NwlayerB::finish(){
            EV << "Minimum Delay: " << delayStats.getMin() << endl;
            EV << "Maximum Delay: " << delayStats.getMax() << endl;
            EV << "Average Delay: " << delayStats.getMean() << endl;
            recordScalar("#sent", numSent);
            recordScalar("#received", numRec);
            delayStats.recordAs("Delay of packets");
}
