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
#include "DL_PDU_m.h"
#include "A_PDU_m.h"
#include "P_PDU_m.h"
#include "Ack_m.h"
#include <cqueue.h>
Define_Module(Dllayer);

void Dllayer::initialize()
{
    timeout = 1.0;
    start=par("start");
    end=par("end");
    start=0;
    end=0;
    counter=0;
    copydpdu=NULL;
    did=par("did");
    numSent=0;
    numRec=0;
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
    if(did==1)
    {
        timeout = 1.0;
        timeoutEvent = new cMessage("timeoutEvent");
        scheduleAt(simTime()+timeout, timeoutEvent);
    }
}

void Dllayer::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
     if(msg==delaysen)
    {
        copysen=buffer[start]->dup();
        send(buffer[start],gate("toPout"));
        iniTime=simTime();
        numSent++;
        if (ev.isGUI())
                    updateDisplay();
      scheduleAt(simTime()+timeout, timeoutEvent);
      start++;
    }else if(msg==delayrec)
    {
        simtime_t delay=simTime()- copydpdu->getTime();
       EV << "Delay for packet id: " << counter  << " is " << delay;
       delayStats.collect(delay);
       delayVector.record(delay);
        char msgname[20];
      Ack *ack= new Ack();
      sprintf(msgname,"-%d",(counter)%2);
      DL_PDU* dlpdu=new DL_PDU(msgname);
      dlpdu->setType('A');
      dlpdu->setId((counter)%2);
      dlpdu->encapsulate(ack);
      send(dlpdu,gate("toPout"));
      numSent++;
      if (ev.isGUI())
                  updateDisplay();
    }
    else if(msg==timeoutEvent)
    {
        send(buffer[start-1]->dup(),gate("toPout"));
        scheduleAt(simTime()+timeout, timeoutEvent);
        numSent++;
        if (ev.isGUI())
                    updateDisplay();
    }else if(msg->getArrivalGate()==gate("fromAin"))
       {
            numRec++;
            if (ev.isGUI())
                        updateDisplay();
           counter++;
           char msgname[20];
           A_PDU *apdu=check_and_cast<A_PDU*>(msg);
           sprintf(msgname,"-%d",(counter+1)%2);
           DL_PDU* dpdu=new DL_PDU(msgname);
           dpdu->encapsulate(apdu);
           dpdu->setType('D');
           //dpdu->setTime(simTime());
           buffer[end]=dpdu;
           end++;
           dpdu->setId((counter+1)%2);
           if(start==0)
           {
               cancelEvent(timeoutEvent);
               copysen=buffer[start]->dup();
               send(buffer[start],gate("toPout"));
               scheduleAt(simTime()+timeout, timeoutEvent);
               start++;
               numSent++;
               if (ev.isGUI())
                           updateDisplay();
           }
       }else if(msg->getArrivalGate()==gate("fromPin"))
       {
           numRec++;
           if (ev.isGUI())
                       updateDisplay();
           DL_PDU *dpdu=check_and_cast<DL_PDU*> (msg);
           if(dpdu->getType()=='A')
           {
               if (uniform(0,1) < 0.10)
              {
                  EV << "\"Losing\" message.\n";
                  bubble("message lost");  // making animation more informative...
                  delete msg;
              }else
               if(copydpdu!=NULL && dpdu->getId()==copydpdu->getId())
               {
                   delete(dpdu->decapsulate());
               }
               else
               {
                  copydpdu=dpdu->dup();
                  cancelEvent(timeoutEvent);
                  delete(dpdu->decapsulate());
                  finalTime=simTime();
                  EV << "Round Trip Time for Packet id: " << start << " is " << finalTime-iniTime;
                  rttStats.collect(finalTime-iniTime);
                  rttVector.record(finalTime-iniTime);
                  if(start<50)
                  {
                      buffer[start]->setTime(simTime());
                      if(uniform(0,1)<0.30)
                          scheduleAt(simTime()+2.0,delaysen);
                      else
                          scheduleAt(simTime()+1.0,delaysen);
                  }
               }

           }else if(dpdu->getType()=='D')
           {
               if (uniform(0,1) < 0.15)
              {
                  EV << "\"Losing\" message.\n";
                  bubble("message lost");  // making animation more informative...
                  delete msg;
              }else
               if(copydpdu!=NULL && dpdu->getId()==copydpdu->getId() )
               {
                  delete(dpdu->decapsulate());
                    cancelEvent(delayrec);
                    if(uniform(0,1)<0.40)
                          scheduleAt(simTime()+2.0,delayrec);
                      else
                          scheduleAt(simTime()+1.0,delayrec);
               }
               else
               {
                   /*simtime_t delay=simTime()- dpdu->getTime();
                   EV << "Delay for packet id: " << counter +1 << " is " << delay;
                   delayStats.collect(delay);
                   delayVector.record(delay);*/
                   copydpdu=dpdu->dup();
                  send(dpdu->decapsulate(),gate("toAout"));
                  counter++;
                  numSent++;
                  if (ev.isGUI())
                              updateDisplay();
                  cancelEvent(delayrec);
                  if(uniform(0,1)<0.40)
                        scheduleAt(simTime()+2.0,delayrec);
                    else
                        scheduleAt(simTime()+1.0,delayrec);

               }

           }
       }
}
void Dllayer::updateDisplay()
{
    char buf[40];
    sprintf(buf, "rcvd: %d sent: %d", numRec, numSent);
    getDisplayString().setTagArg("t",0,buf);
}
void Dllayer::finish()
{
        if(did==2)
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
