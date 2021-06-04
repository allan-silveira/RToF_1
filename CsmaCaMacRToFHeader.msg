//
// Copyright (C) 2016 OpenSim Ltd.
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

import inet.common.INETDefs;
import inet.common.packet.chunk.Chunk;
import inet.linklayer.common.FcsMode;
import inet.linklayer.common.MacAddress;

namespace inet;

enum CsmaCaMacRToFHeaderType {
    CSMA_DATA = 1;
    CSMA_ACK = 2;
};

class CsmaCaMacRToFHeader extends FieldsChunk
{
    CsmaCaMacRToFHeaderType type;    // header type (1 byte)
    uint8_t headerLengthField;    // header length in bytes, max value is 255 (1 byte)
    MacAddress transmitterAddress;    // (6 byte)
    MacAddress receiverAddress;    // (6 byte)
}

class CsmaCaMacRToFAckHeader extends CsmaCaMacRToFHeader
{
    chunkLength = B(14);
    type = CSMA_ACK;
}

class CsmaCaMacRToFDataHeader extends CsmaCaMacRToFHeader
{
    chunkLength = B(17);
    type = CSMA_DATA;
    int networkProtocol = -1;    // ethernet protocol ID (2 bytes)
    int priority = -1;    // 802.1d UP (User Priority) (1 byte)
}

class CsmaCaMacRToFTrailer extends FieldsChunk
{
    chunkLength = B(4);
    uint32_t fcs = 0;
    FcsMode fcsMode = FCS_MODE_UNDEFINED;
}

class CsmaCaMacRToFBackoffHeader extends BytesChunk
{
    simtime_t backoffTime; // (8 bytes)
}

