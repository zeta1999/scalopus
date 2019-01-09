/*
  Copyright (c) 2019, Ivor Wanders
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
#include <iostream>
#include <scalopus_transport/transport_mock.h>
#include "scalopus_general/endpoint_process_info.h"
#include "scalopus_general/thread_naming.h"

#include <sys/types.h>
#include <unistd.h>

template <typename A, typename B>
void test(const A& a, const B& b)
{
  if (a != b)
  {
    ::exit(1);
  }
}
int main(int /* argc */, char** /* argv */)
{
  // Set the thread name.
  TRACE_THREAD_NAME("my_thread");

  // Create the endpoint.
  auto server_info = std::make_shared<scalopus::EndpointProcessInfo>();

  // Assign a process name.
  server_info->setProcessName("Fooo");

  auto factory = std::make_shared<scalopus::TransportMockFactory>();
  auto server = factory->serve();
  server->addEndpoint(server_info);
  auto client = factory->connect(server);

  // Add the endpoint to the client.
  auto client_info = std::make_shared<scalopus::EndpointProcessInfo>();
  client_info->setTransport(client);

  // Retrieve the servers' properties.
  auto process_info = client_info->processInfo();

  // Check if the values are correct.
  test(process_info.name, std::string("Fooo"));
  test(process_info.pid, static_cast<unsigned long>(::getpid()));
  test(process_info.threads[static_cast<unsigned long>(pthread_self())], "my_thread");
  return 0;
}