//
// Generated file, do not edit! Created by nedtool 5.6 from CsmaCaMacRToFHeader.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include "CsmaCaMacRToFHeader_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace {
template <class T> inline
typename std::enable_if<std::is_polymorphic<T>::value && std::is_base_of<omnetpp::cObject,T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)(static_cast<const omnetpp::cObject *>(t));
}

template <class T> inline
typename std::enable_if<std::is_polymorphic<T>::value && !std::is_base_of<omnetpp::cObject,T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)dynamic_cast<const void *>(t);
}

template <class T> inline
typename std::enable_if<!std::is_polymorphic<T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)static_cast<const void *>(t);
}

}

namespace inet {

// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule to generate operator<< for shared_ptr<T>
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const std::shared_ptr<T>& t) { return out << t.get(); }

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
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

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("inet::CsmaCaMacRToFHeaderType");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("inet::CsmaCaMacRToFHeaderType"));
    e->insert(CSMA_DATA, "CSMA_DATA");
    e->insert(CSMA_ACK, "CSMA_ACK");
)

Register_Class(CsmaCaMacRToFHeader)

CsmaCaMacRToFHeader::CsmaCaMacRToFHeader() : ::inet::FieldsChunk()
{
}

CsmaCaMacRToFHeader::CsmaCaMacRToFHeader(const CsmaCaMacRToFHeader& other) : ::inet::FieldsChunk(other)
{
    copy(other);
}

CsmaCaMacRToFHeader::~CsmaCaMacRToFHeader()
{
}

CsmaCaMacRToFHeader& CsmaCaMacRToFHeader::operator=(const CsmaCaMacRToFHeader& other)
{
    if (this == &other) return *this;
    ::inet::FieldsChunk::operator=(other);
    copy(other);
    return *this;
}

void CsmaCaMacRToFHeader::copy(const CsmaCaMacRToFHeader& other)
{
    this->type = other.type;
    this->headerLengthField = other.headerLengthField;
    this->transmitterAddress = other.transmitterAddress;
    this->receiverAddress = other.receiverAddress;
}

void CsmaCaMacRToFHeader::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::FieldsChunk::parsimPack(b);
    doParsimPacking(b,this->type);
    doParsimPacking(b,this->headerLengthField);
    doParsimPacking(b,this->transmitterAddress);
    doParsimPacking(b,this->receiverAddress);
}

void CsmaCaMacRToFHeader::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::FieldsChunk::parsimUnpack(b);
    doParsimUnpacking(b,this->type);
    doParsimUnpacking(b,this->headerLengthField);
    doParsimUnpacking(b,this->transmitterAddress);
    doParsimUnpacking(b,this->receiverAddress);
}

inet::CsmaCaMacRToFHeaderType CsmaCaMacRToFHeader::getType() const
{
    return this->type;
}

void CsmaCaMacRToFHeader::setType(inet::CsmaCaMacRToFHeaderType type)
{
    handleChange();
    this->type = type;
}

uint8_t CsmaCaMacRToFHeader::getHeaderLengthField() const
{
    return this->headerLengthField;
}

void CsmaCaMacRToFHeader::setHeaderLengthField(uint8_t headerLengthField)
{
    handleChange();
    this->headerLengthField = headerLengthField;
}

const MacAddress& CsmaCaMacRToFHeader::getTransmitterAddress() const
{
    return this->transmitterAddress;
}

void CsmaCaMacRToFHeader::setTransmitterAddress(const MacAddress& transmitterAddress)
{
    handleChange();
    this->transmitterAddress = transmitterAddress;
}

const MacAddress& CsmaCaMacRToFHeader::getReceiverAddress() const
{
    return this->receiverAddress;
}

void CsmaCaMacRToFHeader::setReceiverAddress(const MacAddress& receiverAddress)
{
    handleChange();
    this->receiverAddress = receiverAddress;
}

class CsmaCaMacRToFHeaderDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
    enum FieldConstants {
        FIELD_type,
        FIELD_headerLengthField,
        FIELD_transmitterAddress,
        FIELD_receiverAddress,
    };
  public:
    CsmaCaMacRToFHeaderDescriptor();
    virtual ~CsmaCaMacRToFHeaderDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(CsmaCaMacRToFHeaderDescriptor)

CsmaCaMacRToFHeaderDescriptor::CsmaCaMacRToFHeaderDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::CsmaCaMacRToFHeader)), "inet::FieldsChunk")
{
    propertynames = nullptr;
}

CsmaCaMacRToFHeaderDescriptor::~CsmaCaMacRToFHeaderDescriptor()
{
    delete[] propertynames;
}

bool CsmaCaMacRToFHeaderDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<CsmaCaMacRToFHeader *>(obj)!=nullptr;
}

const char **CsmaCaMacRToFHeaderDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *CsmaCaMacRToFHeaderDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int CsmaCaMacRToFHeaderDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount() : 4;
}

unsigned int CsmaCaMacRToFHeaderDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_type
        FD_ISEDITABLE,    // FIELD_headerLengthField
        0,    // FIELD_transmitterAddress
        0,    // FIELD_receiverAddress
    };
    return (field >= 0 && field < 4) ? fieldTypeFlags[field] : 0;
}

const char *CsmaCaMacRToFHeaderDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "type",
        "headerLengthField",
        "transmitterAddress",
        "receiverAddress",
    };
    return (field >= 0 && field < 4) ? fieldNames[field] : nullptr;
}

int CsmaCaMacRToFHeaderDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0] == 't' && strcmp(fieldName, "type") == 0) return base+0;
    if (fieldName[0] == 'h' && strcmp(fieldName, "headerLengthField") == 0) return base+1;
    if (fieldName[0] == 't' && strcmp(fieldName, "transmitterAddress") == 0) return base+2;
    if (fieldName[0] == 'r' && strcmp(fieldName, "receiverAddress") == 0) return base+3;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *CsmaCaMacRToFHeaderDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "inet::CsmaCaMacRToFHeaderType",    // FIELD_type
        "uint8_t",    // FIELD_headerLengthField
        "inet::MacAddress",    // FIELD_transmitterAddress
        "inet::MacAddress",    // FIELD_receiverAddress
    };
    return (field >= 0 && field < 4) ? fieldTypeStrings[field] : nullptr;
}

const char **CsmaCaMacRToFHeaderDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case FIELD_type: {
            static const char *names[] = { "enum",  nullptr };
            return names;
        }
        default: return nullptr;
    }
}

const char *CsmaCaMacRToFHeaderDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case FIELD_type:
            if (!strcmp(propertyname, "enum")) return "inet::CsmaCaMacRToFHeaderType";
            return nullptr;
        default: return nullptr;
    }
}

int CsmaCaMacRToFHeaderDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFHeader *pp = (CsmaCaMacRToFHeader *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *CsmaCaMacRToFHeaderDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFHeader *pp = (CsmaCaMacRToFHeader *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string CsmaCaMacRToFHeaderDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFHeader *pp = (CsmaCaMacRToFHeader *)object; (void)pp;
    switch (field) {
        case FIELD_type: return enum2string(pp->getType(), "inet::CsmaCaMacRToFHeaderType");
        case FIELD_headerLengthField: return ulong2string(pp->getHeaderLengthField());
        case FIELD_transmitterAddress: return pp->getTransmitterAddress().str();
        case FIELD_receiverAddress: return pp->getReceiverAddress().str();
        default: return "";
    }
}

bool CsmaCaMacRToFHeaderDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFHeader *pp = (CsmaCaMacRToFHeader *)object; (void)pp;
    switch (field) {
        case FIELD_type: pp->setType((inet::CsmaCaMacRToFHeaderType)string2enum(value, "inet::CsmaCaMacRToFHeaderType")); return true;
        case FIELD_headerLengthField: pp->setHeaderLengthField(string2ulong(value)); return true;
        default: return false;
    }
}

const char *CsmaCaMacRToFHeaderDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *CsmaCaMacRToFHeaderDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFHeader *pp = (CsmaCaMacRToFHeader *)object; (void)pp;
    switch (field) {
        case FIELD_transmitterAddress: return toVoidPtr(&pp->getTransmitterAddress()); break;
        case FIELD_receiverAddress: return toVoidPtr(&pp->getReceiverAddress()); break;
        default: return nullptr;
    }
}

Register_Class(CsmaCaMacRToFAckHeader)

CsmaCaMacRToFAckHeader::CsmaCaMacRToFAckHeader() : ::inet::CsmaCaMacRToFHeader()
{
    this->setChunkLength(B(14));
    this->setType(CSMA_ACK);
}

CsmaCaMacRToFAckHeader::CsmaCaMacRToFAckHeader(const CsmaCaMacRToFAckHeader& other) : ::inet::CsmaCaMacRToFHeader(other)
{
    copy(other);
}

CsmaCaMacRToFAckHeader::~CsmaCaMacRToFAckHeader()
{
}

CsmaCaMacRToFAckHeader& CsmaCaMacRToFAckHeader::operator=(const CsmaCaMacRToFAckHeader& other)
{
    if (this == &other) return *this;
    ::inet::CsmaCaMacRToFHeader::operator=(other);
    copy(other);
    return *this;
}

void CsmaCaMacRToFAckHeader::copy(const CsmaCaMacRToFAckHeader& other)
{
}

void CsmaCaMacRToFAckHeader::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::CsmaCaMacRToFHeader::parsimPack(b);
}

void CsmaCaMacRToFAckHeader::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::CsmaCaMacRToFHeader::parsimUnpack(b);
}

class CsmaCaMacRToFAckHeaderDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
    enum FieldConstants {
    };
  public:
    CsmaCaMacRToFAckHeaderDescriptor();
    virtual ~CsmaCaMacRToFAckHeaderDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(CsmaCaMacRToFAckHeaderDescriptor)

CsmaCaMacRToFAckHeaderDescriptor::CsmaCaMacRToFAckHeaderDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::CsmaCaMacRToFAckHeader)), "inet::CsmaCaMacRToFHeader")
{
    propertynames = nullptr;
}

CsmaCaMacRToFAckHeaderDescriptor::~CsmaCaMacRToFAckHeaderDescriptor()
{
    delete[] propertynames;
}

bool CsmaCaMacRToFAckHeaderDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<CsmaCaMacRToFAckHeader *>(obj)!=nullptr;
}

const char **CsmaCaMacRToFAckHeaderDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *CsmaCaMacRToFAckHeaderDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int CsmaCaMacRToFAckHeaderDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount() : 0;
}

unsigned int CsmaCaMacRToFAckHeaderDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    return 0;
}

const char *CsmaCaMacRToFAckHeaderDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    return nullptr;
}

int CsmaCaMacRToFAckHeaderDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *CsmaCaMacRToFAckHeaderDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    return nullptr;
}

const char **CsmaCaMacRToFAckHeaderDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *CsmaCaMacRToFAckHeaderDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int CsmaCaMacRToFAckHeaderDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFAckHeader *pp = (CsmaCaMacRToFAckHeader *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *CsmaCaMacRToFAckHeaderDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFAckHeader *pp = (CsmaCaMacRToFAckHeader *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string CsmaCaMacRToFAckHeaderDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFAckHeader *pp = (CsmaCaMacRToFAckHeader *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool CsmaCaMacRToFAckHeaderDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFAckHeader *pp = (CsmaCaMacRToFAckHeader *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *CsmaCaMacRToFAckHeaderDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    return nullptr;
}

void *CsmaCaMacRToFAckHeaderDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFAckHeader *pp = (CsmaCaMacRToFAckHeader *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(CsmaCaMacRToFDataHeader)

CsmaCaMacRToFDataHeader::CsmaCaMacRToFDataHeader() : ::inet::CsmaCaMacRToFHeader()
{
    this->setChunkLength(B(17));
    this->setType(CSMA_DATA);

}

CsmaCaMacRToFDataHeader::CsmaCaMacRToFDataHeader(const CsmaCaMacRToFDataHeader& other) : ::inet::CsmaCaMacRToFHeader(other)
{
    copy(other);
}

CsmaCaMacRToFDataHeader::~CsmaCaMacRToFDataHeader()
{
}

CsmaCaMacRToFDataHeader& CsmaCaMacRToFDataHeader::operator=(const CsmaCaMacRToFDataHeader& other)
{
    if (this == &other) return *this;
    ::inet::CsmaCaMacRToFHeader::operator=(other);
    copy(other);
    return *this;
}

void CsmaCaMacRToFDataHeader::copy(const CsmaCaMacRToFDataHeader& other)
{
    this->networkProtocol = other.networkProtocol;
    this->priority = other.priority;
}

void CsmaCaMacRToFDataHeader::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::CsmaCaMacRToFHeader::parsimPack(b);
    doParsimPacking(b,this->networkProtocol);
    doParsimPacking(b,this->priority);
}

void CsmaCaMacRToFDataHeader::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::CsmaCaMacRToFHeader::parsimUnpack(b);
    doParsimUnpacking(b,this->networkProtocol);
    doParsimUnpacking(b,this->priority);
}

int CsmaCaMacRToFDataHeader::getNetworkProtocol() const
{
    return this->networkProtocol;
}

void CsmaCaMacRToFDataHeader::setNetworkProtocol(int networkProtocol)
{
    handleChange();
    this->networkProtocol = networkProtocol;
}

int CsmaCaMacRToFDataHeader::getPriority() const
{
    return this->priority;
}

void CsmaCaMacRToFDataHeader::setPriority(int priority)
{
    handleChange();
    this->priority = priority;
}

class CsmaCaMacRToFDataHeaderDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
    enum FieldConstants {
        FIELD_networkProtocol,
        FIELD_priority,
    };
  public:
    CsmaCaMacRToFDataHeaderDescriptor();
    virtual ~CsmaCaMacRToFDataHeaderDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(CsmaCaMacRToFDataHeaderDescriptor)

CsmaCaMacRToFDataHeaderDescriptor::CsmaCaMacRToFDataHeaderDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::CsmaCaMacRToFDataHeader)), "inet::CsmaCaMacRToFHeader")
{
    propertynames = nullptr;
}

CsmaCaMacRToFDataHeaderDescriptor::~CsmaCaMacRToFDataHeaderDescriptor()
{
    delete[] propertynames;
}

bool CsmaCaMacRToFDataHeaderDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<CsmaCaMacRToFDataHeader *>(obj)!=nullptr;
}

const char **CsmaCaMacRToFDataHeaderDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *CsmaCaMacRToFDataHeaderDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int CsmaCaMacRToFDataHeaderDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int CsmaCaMacRToFDataHeaderDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_networkProtocol
        FD_ISEDITABLE,    // FIELD_priority
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *CsmaCaMacRToFDataHeaderDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "networkProtocol",
        "priority",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int CsmaCaMacRToFDataHeaderDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0] == 'n' && strcmp(fieldName, "networkProtocol") == 0) return base+0;
    if (fieldName[0] == 'p' && strcmp(fieldName, "priority") == 0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *CsmaCaMacRToFDataHeaderDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_networkProtocol
        "int",    // FIELD_priority
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **CsmaCaMacRToFDataHeaderDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *CsmaCaMacRToFDataHeaderDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int CsmaCaMacRToFDataHeaderDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFDataHeader *pp = (CsmaCaMacRToFDataHeader *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *CsmaCaMacRToFDataHeaderDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFDataHeader *pp = (CsmaCaMacRToFDataHeader *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string CsmaCaMacRToFDataHeaderDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFDataHeader *pp = (CsmaCaMacRToFDataHeader *)object; (void)pp;
    switch (field) {
        case FIELD_networkProtocol: return long2string(pp->getNetworkProtocol());
        case FIELD_priority: return long2string(pp->getPriority());
        default: return "";
    }
}

bool CsmaCaMacRToFDataHeaderDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFDataHeader *pp = (CsmaCaMacRToFDataHeader *)object; (void)pp;
    switch (field) {
        case FIELD_networkProtocol: pp->setNetworkProtocol(string2long(value)); return true;
        case FIELD_priority: pp->setPriority(string2long(value)); return true;
        default: return false;
    }
}

const char *CsmaCaMacRToFDataHeaderDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *CsmaCaMacRToFDataHeaderDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFDataHeader *pp = (CsmaCaMacRToFDataHeader *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(CsmaCaMacRToFTrailer)

CsmaCaMacRToFTrailer::CsmaCaMacRToFTrailer() : ::inet::FieldsChunk()
{
    this->setChunkLength(B(4));

}

CsmaCaMacRToFTrailer::CsmaCaMacRToFTrailer(const CsmaCaMacRToFTrailer& other) : ::inet::FieldsChunk(other)
{
    copy(other);
}

CsmaCaMacRToFTrailer::~CsmaCaMacRToFTrailer()
{
}

CsmaCaMacRToFTrailer& CsmaCaMacRToFTrailer::operator=(const CsmaCaMacRToFTrailer& other)
{
    if (this == &other) return *this;
    ::inet::FieldsChunk::operator=(other);
    copy(other);
    return *this;
}

void CsmaCaMacRToFTrailer::copy(const CsmaCaMacRToFTrailer& other)
{
    this->fcs = other.fcs;
    this->fcsMode = other.fcsMode;
}

void CsmaCaMacRToFTrailer::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::FieldsChunk::parsimPack(b);
    doParsimPacking(b,this->fcs);
    doParsimPacking(b,this->fcsMode);
}

void CsmaCaMacRToFTrailer::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::FieldsChunk::parsimUnpack(b);
    doParsimUnpacking(b,this->fcs);
    doParsimUnpacking(b,this->fcsMode);
}

uint32_t CsmaCaMacRToFTrailer::getFcs() const
{
    return this->fcs;
}

void CsmaCaMacRToFTrailer::setFcs(uint32_t fcs)
{
    handleChange();
    this->fcs = fcs;
}

inet::FcsMode CsmaCaMacRToFTrailer::getFcsMode() const
{
    return this->fcsMode;
}

void CsmaCaMacRToFTrailer::setFcsMode(inet::FcsMode fcsMode)
{
    handleChange();
    this->fcsMode = fcsMode;
}

class CsmaCaMacRToFTrailerDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
    enum FieldConstants {
        FIELD_fcs,
        FIELD_fcsMode,
    };
  public:
    CsmaCaMacRToFTrailerDescriptor();
    virtual ~CsmaCaMacRToFTrailerDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(CsmaCaMacRToFTrailerDescriptor)

CsmaCaMacRToFTrailerDescriptor::CsmaCaMacRToFTrailerDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::CsmaCaMacRToFTrailer)), "inet::FieldsChunk")
{
    propertynames = nullptr;
}

CsmaCaMacRToFTrailerDescriptor::~CsmaCaMacRToFTrailerDescriptor()
{
    delete[] propertynames;
}

bool CsmaCaMacRToFTrailerDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<CsmaCaMacRToFTrailer *>(obj)!=nullptr;
}

const char **CsmaCaMacRToFTrailerDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *CsmaCaMacRToFTrailerDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int CsmaCaMacRToFTrailerDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int CsmaCaMacRToFTrailerDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_fcs
        FD_ISEDITABLE,    // FIELD_fcsMode
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *CsmaCaMacRToFTrailerDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "fcs",
        "fcsMode",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int CsmaCaMacRToFTrailerDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0] == 'f' && strcmp(fieldName, "fcs") == 0) return base+0;
    if (fieldName[0] == 'f' && strcmp(fieldName, "fcsMode") == 0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *CsmaCaMacRToFTrailerDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint32_t",    // FIELD_fcs
        "inet::FcsMode",    // FIELD_fcsMode
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **CsmaCaMacRToFTrailerDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case FIELD_fcsMode: {
            static const char *names[] = { "enum",  nullptr };
            return names;
        }
        default: return nullptr;
    }
}

const char *CsmaCaMacRToFTrailerDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case FIELD_fcsMode:
            if (!strcmp(propertyname, "enum")) return "inet::FcsMode";
            return nullptr;
        default: return nullptr;
    }
}

int CsmaCaMacRToFTrailerDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFTrailer *pp = (CsmaCaMacRToFTrailer *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *CsmaCaMacRToFTrailerDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFTrailer *pp = (CsmaCaMacRToFTrailer *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string CsmaCaMacRToFTrailerDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFTrailer *pp = (CsmaCaMacRToFTrailer *)object; (void)pp;
    switch (field) {
        case FIELD_fcs: return ulong2string(pp->getFcs());
        case FIELD_fcsMode: return enum2string(pp->getFcsMode(), "inet::FcsMode");
        default: return "";
    }
}

bool CsmaCaMacRToFTrailerDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFTrailer *pp = (CsmaCaMacRToFTrailer *)object; (void)pp;
    switch (field) {
        case FIELD_fcs: pp->setFcs(string2ulong(value)); return true;
        case FIELD_fcsMode: pp->setFcsMode((inet::FcsMode)string2enum(value, "inet::FcsMode")); return true;
        default: return false;
    }
}

const char *CsmaCaMacRToFTrailerDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *CsmaCaMacRToFTrailerDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFTrailer *pp = (CsmaCaMacRToFTrailer *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(CsmaCaMacRToFBackoffHeader)

CsmaCaMacRToFBackoffHeader::CsmaCaMacRToFBackoffHeader() : ::inet::BytesChunk()
{
}

CsmaCaMacRToFBackoffHeader::CsmaCaMacRToFBackoffHeader(const CsmaCaMacRToFBackoffHeader& other) : ::inet::BytesChunk(other)
{
    copy(other);
}

CsmaCaMacRToFBackoffHeader::~CsmaCaMacRToFBackoffHeader()
{
}

CsmaCaMacRToFBackoffHeader& CsmaCaMacRToFBackoffHeader::operator=(const CsmaCaMacRToFBackoffHeader& other)
{
    if (this == &other) return *this;
    ::inet::BytesChunk::operator=(other);
    copy(other);
    return *this;
}

void CsmaCaMacRToFBackoffHeader::copy(const CsmaCaMacRToFBackoffHeader& other)
{
    this->backoffTime = other.backoffTime;
}

void CsmaCaMacRToFBackoffHeader::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::BytesChunk::parsimPack(b);
    doParsimPacking(b,this->backoffTime);
}

void CsmaCaMacRToFBackoffHeader::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::BytesChunk::parsimUnpack(b);
    doParsimUnpacking(b,this->backoffTime);
}

omnetpp::simtime_t CsmaCaMacRToFBackoffHeader::getBackoffTime() const
{
    return this->backoffTime;
}

void CsmaCaMacRToFBackoffHeader::setBackoffTime(omnetpp::simtime_t backoffTime)
{
    handleChange();
    this->backoffTime = backoffTime;
}

class CsmaCaMacRToFBackoffHeaderDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
    enum FieldConstants {
        FIELD_backoffTime,
    };
  public:
    CsmaCaMacRToFBackoffHeaderDescriptor();
    virtual ~CsmaCaMacRToFBackoffHeaderDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(CsmaCaMacRToFBackoffHeaderDescriptor)

CsmaCaMacRToFBackoffHeaderDescriptor::CsmaCaMacRToFBackoffHeaderDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::CsmaCaMacRToFBackoffHeader)), "inet::BytesChunk")
{
    propertynames = nullptr;
}

CsmaCaMacRToFBackoffHeaderDescriptor::~CsmaCaMacRToFBackoffHeaderDescriptor()
{
    delete[] propertynames;
}

bool CsmaCaMacRToFBackoffHeaderDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<CsmaCaMacRToFBackoffHeader *>(obj)!=nullptr;
}

const char **CsmaCaMacRToFBackoffHeaderDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *CsmaCaMacRToFBackoffHeaderDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int CsmaCaMacRToFBackoffHeaderDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount() : 1;
}

unsigned int CsmaCaMacRToFBackoffHeaderDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        0,    // FIELD_backoffTime
    };
    return (field >= 0 && field < 1) ? fieldTypeFlags[field] : 0;
}

const char *CsmaCaMacRToFBackoffHeaderDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "backoffTime",
    };
    return (field >= 0 && field < 1) ? fieldNames[field] : nullptr;
}

int CsmaCaMacRToFBackoffHeaderDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0] == 'b' && strcmp(fieldName, "backoffTime") == 0) return base+0;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *CsmaCaMacRToFBackoffHeaderDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "omnetpp::simtime_t",    // FIELD_backoffTime
    };
    return (field >= 0 && field < 1) ? fieldTypeStrings[field] : nullptr;
}

const char **CsmaCaMacRToFBackoffHeaderDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *CsmaCaMacRToFBackoffHeaderDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int CsmaCaMacRToFBackoffHeaderDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFBackoffHeader *pp = (CsmaCaMacRToFBackoffHeader *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *CsmaCaMacRToFBackoffHeaderDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFBackoffHeader *pp = (CsmaCaMacRToFBackoffHeader *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string CsmaCaMacRToFBackoffHeaderDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFBackoffHeader *pp = (CsmaCaMacRToFBackoffHeader *)object; (void)pp;
    switch (field) {
        case FIELD_backoffTime: return simtime2string(pp->getBackoffTime());
        default: return "";
    }
}

bool CsmaCaMacRToFBackoffHeaderDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFBackoffHeader *pp = (CsmaCaMacRToFBackoffHeader *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *CsmaCaMacRToFBackoffHeaderDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *CsmaCaMacRToFBackoffHeaderDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    CsmaCaMacRToFBackoffHeader *pp = (CsmaCaMacRToFBackoffHeader *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

} // namespace inet

