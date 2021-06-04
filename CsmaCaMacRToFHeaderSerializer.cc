//
// Copyright (C) OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//
#define BUILDING_DLL
#define OPP_DLLIMPORT

#include "inet/common/packet/serializer/ChunkSerializerRegistry.h"
#include "CsmaCaMacRToFHeaderSerializer.h"

using namespace inet;

Register_Serializer(CsmaCaMacRToFHeader, CsmaCaMacRToFHeaderSerializer);
Register_Serializer(CsmaCaMacRToFDataHeader, CsmaCaMacRToFHeaderSerializer);
Register_Serializer(CsmaCaMacRToFAckHeader, CsmaCaMacRToFHeaderSerializer);
Register_Serializer(CsmaCaMacRToFTrailer, CsmaCaMacRToFTrailerSerializer);
Register_Serializer(CsmaCaMacRToFBackoffHeader, CsmaCaMacRToFBackoffHeaderSerializer);

void CsmaCaMacRToFHeaderSerializer::serialize(MemoryOutputStream& stream, const Ptr<const Chunk>& chunk) const
{
//    std::cout << "Inside serialize CsmaCaMacRToFHeaderSerializer"  <<endl;
    auto startPos = stream.getLength();
    if (auto macHeader = dynamicPtrCast<const CsmaCaMacRToFDataHeader>(chunk)) {
        stream.writeUint8(macHeader->getType());
        auto length = macHeader->getHeaderLengthField();
        stream.writeUint8(length);
        stream.writeMacAddress(macHeader->getReceiverAddress());
        stream.writeMacAddress(macHeader->getTransmitterAddress());
        stream.writeUint16Be(macHeader->getNetworkProtocol());
        stream.writeByte(macHeader->getPriority());
        if (macHeader->getChunkLength() > stream.getLength() - startPos)
            stream.writeByteRepeatedly('?', B(macHeader->getChunkLength() - (stream.getLength() - startPos)).get());
    }
    else if (auto macHeader = dynamicPtrCast<const CsmaCaMacRToFAckHeader>(chunk)) {
        stream.writeUint8(0x01);
        auto length = macHeader->getHeaderLengthField();
        stream.writeUint8(length);
        stream.writeMacAddress(macHeader->getReceiverAddress());
        stream.writeMacAddress(macHeader->getTransmitterAddress());
        if (macHeader->getChunkLength() > stream.getLength() - startPos)
            stream.writeByteRepeatedly('?', B(macHeader->getChunkLength() - (stream.getLength() - startPos)).get());
    }
    else
        throw cRuntimeError("CsmaCaMacRToFSerializer: cannot serialize chunk");
}

const Ptr<Chunk> CsmaCaMacRToFHeaderSerializer::deserialize(MemoryInputStream& stream) const
{
//    std::cout << "Inside deserialize CsmaCaMacRToFHeaderSerializer"  <<endl;
    auto startPos = stream.getPosition();
    CsmaCaMacRToFHeaderType type = static_cast<CsmaCaMacRToFHeaderType>(stream.readUint8());
    uint8_t length = stream.readUint8();
    switch(type) {
        case CSMA_DATA: {
            auto macHeader = makeShared<CsmaCaMacRToFDataHeader>();
            macHeader->setType(type);
            macHeader->setHeaderLengthField(length);
            macHeader->setReceiverAddress(stream.readMacAddress());
            macHeader->setTransmitterAddress(stream.readMacAddress());
            macHeader->setNetworkProtocol(stream.readUint16Be());
            macHeader->setPriority(stream.readByte());
            if (B(length) > stream.getPosition() - startPos)
                stream.readByteRepeatedly('?', length - B(stream.getPosition() - startPos).get());
            return macHeader;
        }
        case CSMA_ACK: {
            auto macHeader = makeShared<CsmaCaMacRToFAckHeader>();
            macHeader->setType(type);
            macHeader->setHeaderLengthField(length);
            macHeader->setReceiverAddress(stream.readMacAddress());
            macHeader->setTransmitterAddress(stream.readMacAddress());
            if (B(length) > stream.getPosition() - startPos)
                stream.readByteRepeatedly('?', length - B(stream.getPosition() - startPos).get());
            return macHeader;
        }
        default:
            throw cRuntimeError("CsmaCaMacRToFSerializer: cannot deserialize chunk");
    }
    return nullptr;
}

void CsmaCaMacRToFTrailerSerializer::serialize(MemoryOutputStream& stream, const Ptr<const Chunk>& chunk) const
{
    const auto& macTrailer = dynamicPtrCast<const CsmaCaMacRToFTrailer>(chunk);
    auto fcsMode = macTrailer->getFcsMode();
    if (fcsMode != FCS_COMPUTED)
        throw cRuntimeError("Cannot serialize CsmaCaMacRToFTrailer without properly computed FCS, try changing the value of the fcsMode parameter (e.g. in the CsmaCaMacRToF module)");
    stream.writeUint32Be(macTrailer->getFcs());
}

const Ptr<Chunk> CsmaCaMacRToFTrailerSerializer::deserialize(MemoryInputStream& stream) const
{
    auto macTrailer = makeShared<CsmaCaMacRToFTrailer>();
    auto fcs = stream.readUint32Be();
    macTrailer->setFcs(fcs);
    macTrailer->setFcsMode(FCS_COMPUTED);
    return macTrailer;
}

void CsmaCaMacRToFBackoffHeaderSerializer::serialize(MemoryOutputStream& stream, const Ptr<const Chunk>& chunk) const
{
    auto startPos = stream.getLength();
//    std::cout << "Inside serialize CsmaCaMacRToFBackoffHeader"  <<endl;
    if (auto macHeader = dynamicPtrCast<const CsmaCaMacRToFBackoffHeader>(chunk)) {
        stream.writeUint64Be(uint64(macHeader->getBackoffTime().raw()));
//        if (macHeader->getChunkLength() > stream.getLength() - startPos)
//            stream.writeByteRepeatedly('?', B(macHeader->getChunkLength() - (stream.getLength() - startPos)).get());
    }
    else
        throw cRuntimeError("CsmaCaMacRToFSerializer: cannot serialize chunk");
}

const Ptr<Chunk> CsmaCaMacRToFBackoffHeaderSerializer::deserialize(MemoryInputStream& stream) const
{
//    std::cout << "Inside deserialize backOffTime " <<endl;
    auto startPos = stream.getPosition();
    auto macHeader = makeShared<CsmaCaMacRToFBackoffHeader>();
    macHeader->setBackoffTime(macHeader->getBackoffTime().setRaw(stream.readUint64Be()));

//    if (B(length) > stream.getPosition() - startPos)
//        stream.readByteRepeatedly('?', length - B(stream.getPosition() - startPos).get());
    return macHeader;
}

// namespace inet

