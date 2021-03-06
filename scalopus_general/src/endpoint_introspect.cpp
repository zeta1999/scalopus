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
#include "scalopus_general/endpoint_introspect.h"
#include <scalopus_interface/transport.h>
#include <nlohmann/json.hpp>

namespace scalopus
{
using json = nlohmann::json;

const char* EndpointIntrospect::name = "introspect";

std::string EndpointIntrospect::getName() const
{
  return name;
}

bool EndpointIntrospect::handle(Transport& server, const Data& /* request */, Data& response)
{
  const auto endpoints = server.endpoints();
  std::vector<std::string> names{ endpoints.size() };
  std::transform(endpoints.begin(), endpoints.end(), names.begin(),
                 [](const auto& name_ptr) { return name_ptr.first; });

  json jdata = json::object();
  jdata["endpoints"] = names;
  response = json::to_bson(jdata);
  return true;
}

std::vector<std::string> EndpointIntrospect::supported()
{
  // send message...
  if (transport_ == nullptr)
  {
    throw communication_error("No transport provided to endpoint, cannot communicate.");
  }

  // Obtain the response data
  auto future_ptr = transport_->request(name, {});
  if (future_ptr->wait_for(std::chrono::milliseconds(200)) == std::future_status::ready)
  {
    json jdata = json::from_bson(future_ptr->get());  // This line may throw
    return jdata["endpoints"].get<std::vector<std::string>>();
  }

  return {};
}

EndpointIntrospect::Ptr EndpointIntrospect::factory(const Transport::Ptr& transport)
{
  auto endpoint = std::make_shared<scalopus::EndpointIntrospect>();
  endpoint->setTransport(transport);
  return endpoint;
}

}  // namespace scalopus
