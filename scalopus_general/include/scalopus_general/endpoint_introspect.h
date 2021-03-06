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
#ifndef SCALOPUS_GENERAL_ENDPOINT_INTROSPECT_H
#define SCALOPUS_GENERAL_ENDPOINT_INTROSPECT_H

#include <scalopus_interface/endpoint.h>
#include <scalopus_interface/transport.h>

namespace scalopus
{
class EndpointIntrospect : public Endpoint
{
public:
  using Ptr = std::shared_ptr<EndpointIntrospect>;
  static const char* name;

  /**
   * @brief Provide a list of endpoint names supported by the remote endpoint.
   */
  std::vector<std::string> supported();

  /**
   * @brief Function to create a new instance of this class and assign the transport to it.
   */
  static Ptr factory(const Transport::Ptr& transport);

  // Methods from the superclass
  std::string getName() const;
  bool handle(Transport& transport, const Data& incoming, Data& outgoing);
};

}  // namespace scalopus

#endif  // SCALOPUS_GENERAL_ENDPOINT_INTROSPECT_H
