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
#include <scalopus_tracing/internal/trace_configuration_raii.h>
#include <scalopus_tracing/trace_configurator.h>

namespace scalopus
{
TraceConfigurationRAII::TraceConfigurationRAII(const bool is_process, const bool new_state) : is_process_(is_process)
{
  TraceConfigurator::Ptr configurator = TraceConfigurator::getInstance();
  if (is_process_)
  {
    previous_state_ = configurator->setProcessState(new_state);
  }
  else
  {
    previous_state_ = configurator->setThreadState(new_state);
  }
}

TraceConfigurationRAII::~TraceConfigurationRAII()
{
  TraceConfigurator::Ptr configurator = TraceConfigurator::getInstance();
  if (is_process_)
  {
    configurator->setProcessState(previous_state_);
  }
  else
  {
    configurator->setThreadState(previous_state_);
  }
}

}  // namespace scalopus
