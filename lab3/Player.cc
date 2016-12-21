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

#include "Player.h"
#include "P_PDU_m.h"

Define_Module(Player);

void Player::initialize()
{
    id = par("phId");
    inD = gate("inh");
    outD = gate("outh");
    inN = gate("inl");
    outN = gate("outl");
    // TODO - Generated method body
}

void Player::handleMessage(cMessage *msg)
{
    cPacket* pkt = new cPacket();
    pkt = check_and_cast<cPacket*> (msg);

    if (pkt->getArrivalGate()==inD) {
        P_PDU* ppdu= new P_PDU();
        ppdu->encapsulate(pkt);

        send (ppdu,outN);
    }
    else {
        P_PDU* ppdu= new P_PDU();
        ppdu = check_and_cast<P_PDU*> (pkt);
        send (pkt->decapsulate(),outD);

    }
    // TODO - Generated method body
}
