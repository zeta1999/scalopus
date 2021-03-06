/*
  Copyright (c) 2018-2019, Ivor Wanders
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  * Neither the name of the author nor the names of contributors may be used to
    endorse or promote products derived from this software without specific
    prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef SCALOPUS_INTERFACE_TRANSPORT_FACTORY_H
#define SCALOPUS_INTERFACE_TRANSPORT_FACTORY_H

#include <memory>
#include <vector>
#include "scalopus_interface/destination.h"
#include "scalopus_interface/transport.h"

namespace scalopus
{
/**
 * @brief The transport factory provides a standardized way to start transport servers, discover and connect to them.
 */
class TransportFactory
{
public:
  using Ptr = std::shared_ptr<TransportFactory>;
  using LoggingFunction = std::function<void(const std::string& output)>;

  /**
   * @brief Provide a list of discovered servers.
   */
  virtual std::vector<Destination::Ptr> discover() = 0;

  /**
   * @brief Start a server.
   */
  virtual Transport::Ptr serve() = 0;

  /**
   * @brief Connect to a specified destination, connectable destinations are provided by the discover method.
   */
  virtual Transport::Ptr connect(const Destination::Ptr& destination) = 0;

  /**
   * @brief Set the logger function to be used for all clients and servers that are created from this factory.
   * @param logger A function to provide logging strings to.
   */
  virtual void setLogger(LoggingFunction logger);

  virtual ~TransportFactory() = default;

protected:
  LoggingFunction logger_{ [](const std::string&) {} };
};

}  // namespace scalopus
#endif  // SCALOPUS_INTERFACE_TRANSPORT_FACTORY_H