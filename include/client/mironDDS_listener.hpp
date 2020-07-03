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