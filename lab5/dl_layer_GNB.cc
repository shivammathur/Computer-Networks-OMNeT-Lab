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

#include "dl_layer_GNB.h"
#include "A_PDU_m.h"
#include "DL_PDU_m.h"
#include "Ack_m.h"
Define_Module(Dl_layer_GNB);

void Dl_layer_GNB::initialize()
{
    // TODO - Generated method body
    d_id=par("d_id");
    k=3;
    s=sf=sl=-1;
    flag=0;
    counter=0;
    copydpdu=NULL;
    numSent=numRec=0;
    delayStats.setName("Delay of Packets");
    delayStats.setRangeAutoUpper(0,10,2.0);
    delayVector.setName("Delay Vector");
    rttStats.setName("RTT of Packets");
    rttStats.setRangeAutoUpper(0,10,2.0);
    rttVector.setName("RTT of Packets vector");
    WATCH(numRec);
    WATCH(numSent);
    delaysen=new cMessage("delay_sender");
    delayrec=new cMessage("delay_reciever");
    if(d_id==1)
    {
        timeout = 1.0;
        timeoutEvent = new cMessage("timeoutEvent");
       // scheduleAt(simTime()+timeout, timeoutEvent);
    }
}

void Dl_layer_GNB::handleMessage(cMessage *msg)
{
    if(msg==delayrec)
    {
            simtime_t delay=simTime()- copydpdu->getTime();
          EV << "Delay for packet id: " << counter  << " is " << delay;
          delayStats.collect(delay);
          delayVector.record(delay);
            char msgname[20];
            sprintf(msgname,"-%d",(counter+1)%(k+1));
            Ack *ack = new Ack();
            DL_PDU *ackdpdu = new DL_PDU(msgname);
            ackdpdu->encapsulate(ack);
            ackdpdu->setId((counter+1)%(k+1));
            ackdpdu->setType('A');
            send(ackdpdu,"to_pl");
            numSent++;
           if (ev.isGUI())
               updateDisplay();
           counter++;
    }else if(msg==timeoutEvent)
    {
        EV << "Timeout!!";
        s=sf;
        scheduleAt(simTime()+timeout,timeoutEvent);
        while(s<=sl)
        {
            iniTime[vec[s]->getId()]=simTime();
            send(vec[s++]->dup(),"to_pl");
            numSent++;
            if (ev.isGUI())
                        updateDisplay();
        }
    }else if(msg->getArrivalGate()==gate("from_al"))
    {
        numRec++;
       if (ev.isGUI())
           updateDisplay();
        A_PDU *apdu = check_and_cast<A_PDU*>(msg);
        char msgname[20];
        sprintf(msgname,"-%d",(counter)%(k+1));
        DL_PDU *dpdu = new DL_PDU(msgname);
        dpdu->encapsulate(apdu);
        dpdu->setId((counter)%(k+1));
        dpdu->setType('D');
        counter++;
        vec.push_back(dpdu);
        if(!flag)
        {
            flag=1;
            sl=sf=s=vec.size()-1;
            vec[s]->setTime(simTime());
            send(vec[s]->dup(),"to_pl");
            iniTime[vec[s]->getId()]=simTime();
            numSent++;
            if (ev.isGUI())
                updateDisplay();
            scheduleAt(simTime()+timeout,timeoutEvent);
        }else
        {
            if(sl-sf<k-1)
            {
                sl++;
                if(sl-1==s)
                {
                    vec[s+1]->setTime(simTime());
                    send(vec[++s]->dup(),"to_pl");
                    iniTime[vec[s]->getId()]=simTime();
                    numSent++;
                    if (ev.isGUI())
                       updateDisplay();
                }
            }
        }
    }else if(msg->getArrivalGate()==gate("from_pl"))
    {
        numRec++;
       if (ev.isGUI())
           updateDisplay();
        DL_PDU *dpdu = check_and_cast<DL_PDU*> (msg);
        if(dpdu->getType()=='D')
        {
            cancelEvent(delayrec);
            if(!(copydpdu==NULL || (copydpdu->getId()+1)%(k+1)==dpdu->getId()) )
            {
                EV << "DL_PDU discarded: " << dpdu->getId();
                delete(dpdu);

            }else
            {
                copydpdu=dpdu->dup();
                send(dpdu->decapsulate(),"to_al");
                numSent++;
               if (ev.isGUI())
                   updateDisplay();
                scheduleAt(simTime()+1.0,delayrec);
            }

        }else if(dpdu->getType()=='A')
        {
            //cancelEvent(timeoutEvent);
            DL_PDU *dpdu = check_and_cast<DL_PDU*> (msg);
            if((vec[sf]->getId()+1)%(k+1)==dpdu->getId())
            {
                finalTime[vec[sf]->getId()]=simTime();
                EV << "Round Trip Time for Packet id: " << vec[sf]->getId() << " is " << finalTime[vec[sf]->getId()]-iniTime[vec[sf]->getId()];
              rttStats.collect(finalTime[vec[sf]->getId()]-iniTime[vec[sf]->getId()]);
              rttVector.record(finalTime-iniTime);
                int flag1=0;
                if((unsigned)sl<vec.size()-1)
                {
                    sl++;
                    sf++;
                    s=sf;
                }else
                {
                    sf++;
                    s=sf;
                    if(sf==26)
                        cancelEvent(timeoutEvent);
                }
                /*while(s<=sl)
                {
                    send(vec[++s],"to_pl");
                    numSent++;
                    if (ev.isGUI())
                                updateDisplay();
                }*/
                /*if(!flag1)
                    scheduleAt(simTime(),timeoutEvent);*/

            }
            EV << "Acknowledged: " << dpdu->getId();
        }

    }
}
void Dl_layer_GNB::updateDisplay()
{
    char buf[40];
    sprintf(buf, "rcvd: %d sent: %d", numRec, numSent);
    getDisplayString().setTagArg("t",0,buf);
}
void Dl_layer_GNB::finish()
{
        if(d_id==2)
        {
            EV << "Minimum Delay: " << delayStats.getMin() << endl;
            EV << "Maximum Delay: " << delayStats.getMax() << endl;
            EV << "Average Delay: " << delayStats.getMean() << endl;
            recordScalar("#sent", numSent);
             recordScalar("#received", numRec);
             delayStats.recordAs("Delay of packets");
        }else
        {

            EV << "Minimum RTT: " << rttStats.getMin() << endl;
            EV << "Maximum RTT: " << rttStats.getMax() << endl;
            EV << "Average RTT: " << rttStats.getMean() << endl;
            recordScalar("#sent", numSent);
             recordScalar("#received", numRec);
             rttStats.recordAs("RTT of packets");
        }
}
