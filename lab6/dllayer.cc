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
#include "NW_PDU_m.h"
#include "P_PDU_m.h"
#include "Ack_m.h"
#include <cqueue.h>
Define_Module(Dllayer);

void Dllayer::initialize()
{
    timeout = 5.0;
    start=par("start");
    end=par("end");
    start=0;
    end=0;
    counter=0;
    copydpdu=NULL;
    did=par("did");
    numSent=0;
    numRec=0;
    WATCH(numRec);
    WATCH(numSent);
    delaysen=new cMessage("delay_sender");
    delayrec=new cMessage("delay_reciever");
    timeoutEvent = new cMessage("timeoutEvent");
    /*if(did==1)
    {

        timeoutEvent = new cMessage("timeoutEvent");
        //scheduleAt(simTime()+timeout, timeoutEvent);
    }*/
}

void Dllayer::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
     if(msg==delaysen)
    {
//        copysen=buffer[start]->dup();
//        send(buffer[start],gate("toPout"));
         copysen=buffer.front()->dup();
         send(buffer.front(),gate("toPout"));
        numSent++;
        if (ev.isGUI())
                    updateDisplay();
      scheduleAt(simTime()+timeout, timeoutEvent);
      start++;
    }else if(msg==delayrec)
    {
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
        EV << "Timeout!! " << endl;
        //send(buffer[start-1]->dup(),gate("toPout"));
        send(copysen,gate("toPout"));
        scheduleAt(simTime()+timeout, timeoutEvent);
        numSent++;
        if (ev.isGUI())
                    updateDisplay();
    }else if(msg->getArrivalGate()==gate("fromnw"))
       {
            numRec++;
            if (ev.isGUI())
                        updateDisplay();
           counter++;
           char msgname[20];
           NW_PDU *apdu=check_and_cast<NW_PDU*>(msg);
           sprintf(msgname,"-%d",(counter+1)%2);
           DL_PDU* dpdu=new DL_PDU(msgname);
           dpdu->encapsulate(apdu);
           dpdu->setType('D');
           //buffer[end]=dpdu;
           buffer.push(dpdu);
           end++;
           dpdu->setId((counter+1)%2);
           if(buffer.size()==1)
             {
                 //cancelEvent(timeoutEvent);
//                 copysen=buffer[start]->dup();
//                 send(buffer[start]->dup(),gate("toPout"));
               copysen=buffer.front()->dup();
               send(buffer.front()->dup(),gate("toPout"));
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
               if(copydpdu!=NULL && dpdu->getId()==copydpdu->getId())
               {
                   delete(dpdu->decapsulate());
               }
               else
               {
                   copydpdu=dpdu->dup();
                   cancelEvent(timeoutEvent);
                 delete(dpdu->decapsulate());
                 buffer.pop();
                  if(start<counter)
                  {
                      if(uniform(0,1)<0.30)
                          scheduleAt(simTime()+2.0,delaysen);
                      else
                          scheduleAt(simTime()+1.0,delaysen);
                  }
               }

           }else if(dpdu->getType()=='D')
           {
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
                   copydpdu=dpdu->dup();
                  send(dpdu->decapsulate(),gate("tonw"));
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
