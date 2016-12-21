//
// Generated file, do not edit! Created by nedtool 4.6 from NW_PDU.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "NW_PDU_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

Register_Class(NW_PDU);

NW_PDU::NW_PDU(const char *name, int kind) : ::cPacket(name,kind)
{
    this->time_var = 0;
    this->srcAdd_var = 0;
    this->destAdd_var = 0;
    this->type_var = 0;
}

NW_PDU::NW_PDU(const NW_PDU& other) : ::cPacket(other)
{
    copy(other);
}

NW_PDU::~NW_PDU()
{
}

NW_PDU& NW_PDU::operator=(const NW_PDU& other)
{
    if (this==&other) return *this;
    ::cPacket::operator=(other);
    copy(other);
    return *this;
}

void NW_PDU::copy(const NW_PDU& other)
{
    this->time_var = other.time_var;
    this->srcAdd_var = other.srcAdd_var;
    this->destAdd_var = other.destAdd_var;
    this->type_var = other.type_var;
}

void NW_PDU::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    doPacking(b,this->time_var);
    doPacking(b,this->srcAdd_var);
    doPacking(b,this->destAdd_var);
    doPacking(b,this->type_var);
}

void NW_PDU::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    doUnpacking(b,this->time_var);
    doUnpacking(b,this->srcAdd_var);
    doUnpacking(b,this->destAdd_var);
    doUnpacking(b,this->type_var);
}

simtime_t NW_PDU::getTime() const
{
    return time_var;
}

void NW_PDU::setTime(simtime_t time)
{
    this->time_var = time;
}

int NW_PDU::getSrcAdd() const
{
    return srcAdd_var;
}

void NW_PDU::setSrcAdd(int srcAdd)
{
    this->srcAdd_var = srcAdd;
}

int NW_PDU::getDestAdd() const
{
    return destAdd_var;
}

void NW_PDU::setDestAdd(int destAdd)
{
    this->destAdd_var = destAdd;
}

int NW_PDU::getType() const
{
    return type_var;
}

void NW_PDU::setType(int type)
{
    this->type_var = type;
}

class NW_PDUDescriptor : public cClassDescriptor
{
  public:
    NW_PDUDescriptor();
    virtual ~NW_PDUDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(NW_PDUDescriptor);

NW_PDUDescriptor::NW_PDUDescriptor() : cClassDescriptor("NW_PDU", "cPacket")
{
}

NW_PDUDescriptor::~NW_PDUDescriptor()
{
}

bool NW_PDUDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<NW_PDU *>(obj)!=NULL;
}

const char *NW_PDUDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int NW_PDUDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int NW_PDUDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *NW_PDUDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "time",
        "srcAdd",
        "destAdd",
        "type",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int NW_PDUDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "time")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcAdd")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "destAdd")==0) return base+2;
    if (fieldName[0]=='t' && strcmp(fieldName, "type")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *NW_PDUDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "simtime_t",
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *NW_PDUDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int NW_PDUDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    NW_PDU *pp = (NW_PDU *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string NW_PDUDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    NW_PDU *pp = (NW_PDU *)object; (void)pp;
    switch (field) {
        case 0: return double2string(pp->getTime());
        case 1: return long2string(pp->getSrcAdd());
        case 2: return long2string(pp->getDestAdd());
        case 3: return long2string(pp->getType());
        default: return "";
    }
}

bool NW_PDUDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    NW_PDU *pp = (NW_PDU *)object; (void)pp;
    switch (field) {
        case 0: pp->setTime(string2double(value)); return true;
        case 1: pp->setSrcAdd(string2long(value)); return true;
        case 2: pp->setDestAdd(string2long(value)); return true;
        case 3: pp->setType(string2long(value)); return true;
        default: return false;
    }
}

const char *NW_PDUDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *NW_PDUDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    NW_PDU *pp = (NW_PDU *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


