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

#include "mironDDS_listener.hpp"
#include "change_velocity.hpp"
#include "abort_current_skill.hpp"

using namespace zmqserver;

MironDDSListener::MironDDSListener(std::shared_ptr<QueryClient> queryClient,
																	 std::shared_ptr<VariantClient> variantClient)
{
#ifdef SUBSCRIBE_TO_ROQME_CONTEXTS
	intReaderPtr = std::unique_ptr<RoqmeIntReader>(new RoqmeIntReader(new IntContextListener));
	uintReaderPtr = std::unique_ptr<RoqmeUIntReader>(new RoqmeUIntReader(new UIntContextListener));
	doubleReaderPtr = std::unique_ptr<RoqmeDoubleReader>(new RoqmeDoubleReader(new DoubleContextListener));
	boolReaderPtr = std::unique_ptr<RoqmeBoolReader>(new RoqmeBoolReader(new BoolContextListener));
	enumReaderPtr = std::unique_ptr<RoqmeEnumReader>(new RoqmeEnumReader(new EnumContextListener));
	eventReaderPtr = std::unique_ptr<RoqmeEventReader>(new RoqmeEventReader(new EventContextListener));
#endif
	estimateReaderPtr = std::unique_ptr<RoqmeEstimateReader>(
			new RoqmeEstimateReader(new EstimateListener(queryClient, variantClient)));
}

/* 
 * Roqme estimate listener implementation
 */
EstimateListener::EstimateListener(std::shared_ptr<QueryClient> queryClient,
																	 std::shared_ptr<VariantClient> variantClient) : 
		query_client_(queryClient),
		variant_client_(variantClient),
		safety_threshold_(0.1),
		tmp_safety_(-1),
		flag_power_autonomy_(false),
		flag_mission_completion_(false) {}

void EstimateListener::changeVelocity(double value)
{
	Velocity vel(0, value);
	ChangeVelocity vel_msg(query_client_->getID(), vel);
	query_client_->setMsg(std::move(vel_msg.dump()));
	query_client_->send();
}

void EstimateListener::abortCurrentSkill(double value)
{
	ApproachDist app_dist(100000);
	AbortCurrentSkill abort_msg(query_client_->getID(), app_dist);
	query_client_->setMsg(std::move(abort_msg.dump()));
	query_client_->send();
}

void EstimateListener::dataAvailable(const RoqmeDDSTopics::RoqmeEstimate &data,
																		 const dds::sub::SampleInfo &sampleInfo)
{

	double value = data.value();
	const std::string name = data.name();

	if (name == "safety" && value != -1)
	{
		if (tmp_safety_ < value * (1 - safety_threshold_) ||
				tmp_safety_ > value * (1 + safety_threshold_))
		{
			tmp_safety_ = value;
			double sq_value = sqrt(value);
			changeVelocity(1000 * sq_value / (sq_value + sqrt(1 - value)));
			return;
		}
	}

	if (name == "power_autonomy" && value != -1 && flag_power_autonomy_ == 0) 
	{
		if (value <= 0.3)
		{
			variant_client_->sendVariant("dock");
			abortCurrentSkill(100000); 
			flag_power_autonomy_ = 1;	 
		}
		return;
	}

	if (name == "power_autonomy" && value != -1 && flag_power_autonomy_ == 1) 
	{																																				 
		if (value > 0.3)
		{													 
			flag_power_autonomy_ = 0; 
		}													 
		return;										 
	}

	if (name == "mission_completion" && value != -1) 
	{																																				 
		if (value > 0.6 && !flag_mission_completion_)
		{									
			flag_mission_completion_ = true;				 
			variant_client_->sendVariant("deliver");
			abortCurrentSkill(100000); 
		}													 
		return;										 
	}														 

	return;
}

#ifdef SUBSCRIBE_TO_ROQME_CONTEXTS

/* 
 * Roqme context listeners implementation
 */

void IntContextListener::dataAvailable(
		const RoqmeDDSTopics::RoqmeIntContext &data,
		const dds::sub::SampleInfo &sampleInfo)
{
	/* cout << "INT sample available" << endl;
	cout << "\t name: " << data.name() << endl;
	for(auto elem:data.value())
	{
		cout << "\t " << elem << endl;
	}*/
}

void UIntContextListener::dataAvailable(
		const RoqmeDDSTopics::RoqmeUIntContext &data,
		const dds::sub::SampleInfo &sampleInfo)
{
	/*cout << "UINT sample available:" << endl;
	cout << "\t name: " << data.name() << endl;
	for(auto elem:data.value())
	{
		cout << "\t " << elem << endl;
	}*/
}

void BoolContextListener::dataAvailable(
		const RoqmeDDSTopics::RoqmeBoolContext &data,
		const dds::sub::SampleInfo &sampleInfo)
{
	/*cout << "BOOL sample available:" << endl;
	cout << "\t name: " << data.name() << endl;
	for(auto elem:data.value())
	{
		cout << "\t " << elem << endl;
	}*/
}

void EnumContextListener::dataAvailable(
		const RoqmeDDSTopics::RoqmeEnumContext &data,
		const dds::sub::SampleInfo &sampleInfo)
{
	cout << "ENUM sample available:" << endl;
	cout << "\t name: " << data.name() << endl;
	for (auto elem : data.value())
	{
		cout << "\t " << elem << endl;
	}
}

void EventContextListener::dataAvailable(
		const RoqmeDDSTopics::RoqmeEventContext &data,
		const dds::sub::SampleInfo &sampleInfo)
{
	/*cout << "EVENT sample available:" << endl;
	cout << "\t name: " << data.name() << endl;
	for(auto elem:data.value())
	{
		cout << "\t " << elem << endl;
	}*/
}

void DoubleContextListener::dataAvailable(
		const RoqmeDDSTopics::RoqmeDoubleContext &data,
		const dds::sub::SampleInfo &sampleInfo)
{
	/*cout << "DOUBLE sample available:" << endl;
	cout << "\t name: " << data.name() << endl;
	for(auto elem:data.value())
	{
		cout << "\t value: " << elem << endl;
	}*/
}

#endif
