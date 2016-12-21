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

#include "Dlayer.h"
#include "D_PDU_m.h"

Define_Module(Dlayer);

void Dlayer::initialize()
{
    id=0;
    address = par("daId");
    inA = gate("inh");
    outA = gate("outh");
    inP = gate("inl");
    outP = gate("outl");

    // TODO - Generated method body
}

void Dlayer::handleMessage(cMessage *msg)
{
    id++ ;
    id =id%2;
    cPacket* pkt1= new cPacket();
    pkt1 = check_and_cast<cPacket*> (msg);

    if (pkt1->getArrivalGate()==inA){

        D_PDU* bpdu= new D_PDU();

        bpdu->encapsulate(pkt1);

        bpdu->setId(id);
        EV << "sending id";
        send(bpdu,outP);
    }
    else {
        D_PDU* bpdu= new D_PDU();
        bpdu = check_and_cast<D_PDU*> (pkt1);
        if(bpdu->getType()>=2){
            if(bpdu->getType()==3)
                delete msg;
            else{
            delete msg;
            D_PDU* kpdu=new D_PDU();
                    kpdu->setType(3);
                    send(kpdu,outP);
            }
        }
        else{
            bpdu->setId(id);
        D_PDU* abpdu=new D_PDU();
        abpdu->setType(2);
        send(abpdu,outP);
        send(bpdu->decapsulate(),outA);
        }
    }
    // TODO - Generated method body
}
