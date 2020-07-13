/*
    Copyright (C) 2020  University of Extremadura, University of Málaga, Blue Ocean Robotics.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

		Author: Adrian Romero, argarces@uma.es
		Maintainer: Renan Freitas, renan028@gmail.com
*/

#pragma once

#include <RoqmeReaderImpl.h>
#include <query_client.hpp>
#include <variant_client.hpp>

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
		EstimateListener(std::shared_ptr<QueryClient> queryClient,
			std::shared_ptr<VariantClient> variantClient);
		~EstimateListener() = default;
		void dataAvailable(const RoqmeDDSTopics::RoqmeEstimate &data, 
			const dds::sub::SampleInfo &sampleInfo);
	private:
		std::shared_ptr<QueryClient> query_client_;
		std::shared_ptr<VariantClient> variant_client_;

	private:
		void changeVelocity(double value);
};

/*
 * This class contains all the listeners denfined above
 */

class MironDDSListener
{
	public:
		MironDDSListener(std::shared_ptr<QueryClient> queryClient,
			std::shared_ptr<VariantClient> variantClient);
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