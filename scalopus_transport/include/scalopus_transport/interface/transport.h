/*
  Copyright (c) 2018, Ivor Wanders
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

  Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

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

#ifndef SCALOPUS_INTERFACE_TRANSPORT_H
#define SCALOPUS_INTERFACE_TRANSPORT_H

#include <scalopus_transport/interface/endpoint.h>
#include <future>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

namespace scalopus
{
class Transport
{
public:
  using Ptr = std::shared_ptr<Transport>;
  using PendingResponse = std::shared_ptr<std::future<Data>>;

  virtual ~Transport();

  /**
   * @brief Add an endpoitn to this transport, this allows endpoints to receive broadcast messages.
   */
  virtual void addEndpoint(const std::shared_ptr<Endpoint>& endpoint);

  /**
   * @brief Send a request to a remote endpoint.
   * @return A pointer to the future that may be fulfilled by this request.
   * Callee may let the future go out of scope, in that case the request will be dropped.
   * @note It is not wise to block without timeout, in case the request goes unfulfilled by the endpoint this would
   *       block indefinitely.
   */
  virtual PendingResponse request(const std::string& remote_endpoint_name, const Data& outgoing) = 0;

  /**
   * @brief Returnt the number of oustanding requests.
   */
  virtual std::size_t pendingRequests() const = 0;

  /**
   * @brief Broadcast is a non-blocking call, this is queued for broadcast, the worker than sends it at it's discretion
   */
  virtual void broadcast(const std::string& remote_endpoint_name, const Data& outgoing);

  /**
   * @brief Is this transport serving or connecting to a client?
   */
  virtual bool isConnected() const;

  /**
   * @brief The list of endpoints that are stored by this transport.
   */
  std::vector<std::string> endpoints() const;

  /**
   * @brief Retrieve an endpoint by a name.
   * @return Pointer to the endpoint that has the provided name, or nullptr if not found.
   */
  Endpoint::Ptr getEndpoint(const std::string& name) const;

protected:
  /**
   * @brief Returns whether or not there are any broadcasts in the queue to be sent out.
   * @return True if there are pending broadcasts in the queue and calling popBroadcast() would be valid.
   */
  bool haveBroadcast() const;

  /**
   * @brief Return an entry from the broadcast queue.
   * @note This function may only be called if haveBroadcast() is true.
   */
  std::pair<std::string, Data> popBroadcast();

  std::vector<std::pair<std::string, Data>> broadcast_messages_;  //!< List of broadcast messages to send out.
  mutable std::mutex broadcast_message_mutex_;

  std::map<std::string, Endpoint::Ptr> endpoints_;  //!< Endpoints known by this transport, key is their name.
  mutable std::mutex endpoint_mutex_;
};

}  // namespace scalopus
#endif  // SCALOPUS_INTERFACE_TRANSPORT_SERVER_H