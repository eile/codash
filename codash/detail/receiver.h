
/* Copyright (c) 2012, EFPL/Blue Brain Project
 *                     Daniel Nachbaur <daniel.nachbaur@epfl.ch>
 *
 * This file is part of CODASH <https://github.com/BlueBrain/codash>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *  
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef CODASH_DETAIL_RECEIVER_H
#define CODASH_DETAIL_RECEIVER_H

#include "communicator.h"
#include "types.h"

#include <boost/function/function0.hpp>

#include <lunchbox/mtQueue.h>


namespace codash
{
namespace detail
{

class Receiver : public Communicator
{
public:
    Receiver( int argc, char** argv );

    Receiver( co::LocalNodePtr localNode );

    ~Receiver();

    bool connect( co::ConnectionDescriptionPtr conn );

    bool connect( const co::NodeID& nodeID );

    bool disconnect();

    bool isConnected() const;

    const dash::Nodes& getNodes() const;

    bool sync();

protected:
    virtual void serialize( co::DataOStream& os, const uint64_t dirtyBits );
    virtual void deserialize( co::DataIStream& is, const uint64_t dirtyBits );

    virtual void notifyNewHeadVersion( const uint128_t& version );

private:
    void connect_();
    void handleInit_( const uint128_t& groupID, const uint128_t& typeID,
                      const uint128_t& objectID, co::DataIStream& istream );

    void processMappings_();

    typedef boost::function< void() > WorkFunc;
    typedef std::vector< uint128_t > IDVector;

    co::NodePtr proxyNode_;
    std::deque< WorkFunc > mapQueue_;
    IDVector nodes_;
    lunchbox::MTQueue< uint128_t > queuedVersions_;
    uint128_t objectMapVersion_;
    lunchbox::Monitor<bool> initialized_;
};

}
}

#endif
