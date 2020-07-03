/* Copyright (C) 2020 Adrian Romero -  All Rights Reserved
 * Copyright (C) 2020 Renan Freitas, Blue Ocean Robotics -  All Rights Reserved
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
*   to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
*   and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
*   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <RoqmeReaderImpl.h>
#include "query_client.hpp"

using namespace Roqme;

/*
 *  Roqme context listeners
 */
#ifdef SUBSCRIBE_TO_ROQME_CONTEXTS

class IntContextListener : public RoqmeDDSListener<RoqmeDDSTopics::RoqmeIntContext>
{
	public:
		void dataAvailable(const RoqmeDDSTopics::RoqmeIntContext &data, 
			const dds::sub::SampleInfo &sampleInfo);
};

class UIntContextListener : public RoqmeDDSListener<RoqmeDDSTopics::RoqmeUIntContext>
{
	public:
		void dataAvailable(const RoqmeDDSTopics::RoqmeUIntContext &data, 
			const dds::sub::SampleInfo &sampleInfo);
};

class BoolContextListener : public RoqmeDDSListener<RoqmeDDSTopics::RoqmeBoolContext>
{
	public:
		void dataAvailable(const RoqmeDDSTopics::RoqmeBoolContext &data, 
			const dds::sub::SampleInfo &sampleInfo);
};

class EnumContextListener : public RoqmeDDSListener<RoqmeDDSTopics::RoqmeEnumContext>
{
	public:
		void dataAvailable(const RoqmeDDSTopics::RoqmeEnumContext &data, 
			const dds::sub::SampleInfo &sampleInfo);
};

class EventContextListener : public RoqmeDDSListener<RoqmeDDSTopics::RoqmeEventContext>
{
	public:
		void dataAvailable(const RoqmeDDSTopics::RoqmeEventContext &data, 
			const dds::sub::SampleInfo &sampleInfo);
};

class DoubleContextListener : public RoqmeDDSListener<RoqmeDDSTopics::RoqmeDoubleContext>
{
	public:
		void dataAvailable(const RoqmeDDSTopics::RoqmeDoubleContext &data, 
			const dds::sub::SampleInfo &sampleInfo);
};
#endif

/*
 * Roqme reasoner estimation listener
 */

class EstimateListener : public RoqmeDDSListener<RoqmeDDSTopics::RoqmeEstimate>
{
	public:
		EstimateListener(QueryClient *queryClient);
		~EstimateListener() { delete queryClient; }
		void dataAvailable(const RoqmeDDSTopics::RoqmeEstimate &data, 
			const dds::sub::SampleInfo &sampleInfo);
	private:
		QueryClient *queryClient;

	private:
		void changeVelocity(double value);
};

/*
 * This class contains all the listeners denfined above
 */

class MironDDSListener
{
	public:
		MironDDSListener(QueryClient *queryClient);
		~MironDDSListener() = default;

private:
#ifdef SUBSCRIBE_TO_ROQME_CONTEXTS
	std::unique_ptr<RoqmeIntReader> intReaderPtr;
	std::unique_ptr<RoqmeUIntReader> uintReaderPtr;
	std::unique_ptr<RoqmeDoubleReader> doubleReaderPtr;
	std::unique_ptr<RoqmeBoolReader> boolReaderPtr;
	std::unique_ptr<RoqmeEnumReader> enumReaderPtr;
	std::unique_ptr<RoqmeEventReader> eventReaderPtr;
#endif
	std::unique_ptr<RoqmeEstimateReader> estimateReaderPtr;
};