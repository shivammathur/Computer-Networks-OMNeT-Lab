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

#include "Alayer.h"
#include "A_PDU_m.h"

Define_Module(Alayer);

void Alayer::initialize()
{
    id=1;
    address = par("apId");
    in= gate("inl");
    out=gate("outl");
    if (address==1){
        cMessage* msg = new cMessage("event");
        scheduleAt(0,msg);
    }
     // TODO - Generated method body
}
void Alayer::handleMessage(cMessage *msg)
{
    if (msg-> isSelfMessage()){
        A_PDU * pkt= new A_PDU();
        pkt->setId(id);
        pkt->setType(1);
        send(pkt,out);
    }
    else {
        if (id==10){
            delete msg;
        }

        else{
            A_PDU* pkt = check_and_cast<A_PDU*> (msg);

           if (pkt->getType()==1){
               pkt->setType(0);
               pkt->setId(id);
               if (id<=10){
                   send(pkt,out);
               }
               else {
                   delete pkt;
               }
           }
           else{
               id++;
               pkt->setType(1);
               pkt->setId(id);
               if (id<=10){
                  send(pkt,out);
               }
               else {
                   delete pkt;
               }
           }

        }
    }

    // TODO - Generated method body
}
