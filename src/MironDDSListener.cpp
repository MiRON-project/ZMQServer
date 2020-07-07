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
using namespace zmqserver;

MironDDSListener::MironDDSListener(std::shared_ptr<QueryClient> queryClient){
#ifdef SUBSCRIBE_TO_ROQME_CONTEXTS
	intReaderPtr = std::unique_ptr<RoqmeIntReader> (new RoqmeIntReader(new IntContextListener));
	uintReaderPtr = std::unique_ptr<RoqmeUIntReader> (new RoqmeUIntReader(new UIntContextListener));
	doubleReaderPtr = std::unique_ptr<RoqmeDoubleReader> (new RoqmeDoubleReader(new DoubleContextListener));
	boolReaderPtr = std::unique_ptr<RoqmeBoolReader> (new RoqmeBoolReader(new BoolContextListener));
	enumReaderPtr = std::unique_ptr<RoqmeEnumReader> (new RoqmeEnumReader(new EnumContextListener));
	eventReaderPtr = std::unique_ptr<RoqmeEventReader> (new RoqmeEventReader(new EventContextListener));
#endif
	estimateReaderPtr = std::unique_ptr<RoqmeEstimateReader> (new RoqmeEstimateReader(new EstimateListener(queryClient)));
}

/* 
 * Roqme estimate listener implementation
 */
EstimateListener::EstimateListener(std::shared_ptr<QueryClient> queryClient) :
	query_client_(queryClient) {}

void EstimateListener::changeVelocity(double value){
	Velocity vel(0, value);
	ChangeVelocity vel_msg(query_client_->getID(), vel);
        query_client_->setMsg(std::move(vel_msg.dump()));
       	query_client_->send();
}

void EstimateListener::dataAvailable(const RoqmeDDSTopics::RoqmeEstimate& data, 
	const dds::sub::SampleInfo& sampleInfo){

	double previousValue = -1;
	const double value = data.value();
	const std::string name = data.name();

	if(!name.compare("safety") && value != previousValue)
	{
		//TODO: update velocity according to estimate values
		if(value <= 0.1) {
			changeVelocity(0); //stop the robot?
		}		
		else if(value <= 0.5) {
			changeVelocity(500);
		}
		else if(value <= 0.7) {
			changeVelocity(700);
		}
		else {
			changeVelocity(1000);
		}
		
		previousValue = value;
	}
}

#ifdef SUBSCRIBE_TO_ROQME_CONTEXTS

/* 
 * Roqme context listeners implementation
 */

void IntContextListener::dataAvailable(
	const RoqmeDDSTopics::RoqmeIntContext& data, 
	const dds::sub::SampleInfo& sampleInfo){
	/* cout << "INT sample available" << endl;
	cout << "\t name: " << data.name() << endl;
	for(auto elem:data.value())
	{
		cout << "\t " << elem << endl;
	}*/
}

void UIntContextListener::dataAvailable(
		const RoqmeDDSTopics::RoqmeUIntContext& data, 
		const dds::sub::SampleInfo& sampleInfo){
	/*cout << "UINT sample available:" << endl;
	cout << "\t name: " << data.name() << endl;
	for(auto elem:data.value())
	{
		cout << "\t " << elem << endl;
	}*/
}

void BoolContextListener::dataAvailable(
		const RoqmeDDSTopics::RoqmeBoolContext& data, 
		const dds::sub::SampleInfo& sampleInfo){
	/*cout << "BOOL sample available:" << endl;
	cout << "\t name: " << data.name() << endl;
	for(auto elem:data.value())
	{
		cout << "\t " << elem << endl;
	}*/
}

void EnumContextListener::dataAvailable(
		const RoqmeDDSTopics::RoqmeEnumContext& data, 
		const dds::sub::SampleInfo& sampleInfo){
	cout << "ENUM sample available:" << endl;
	cout << "\t name: " << data.name() << endl;
	for(auto elem:data.value())
	{
		cout << "\t " << elem << endl;
	}
}

void EventContextListener::dataAvailable(
			const RoqmeDDSTopics::RoqmeEventContext& data, 
			const dds::sub::SampleInfo& sampleInfo){
	/*cout << "EVENT sample available:" << endl;
	cout << "\t name: " << data.name() << endl;
	for(auto elem:data.value())
	{
		cout << "\t " << elem << endl;
	}*/
}

void DoubleContextListener::dataAvailable(
		const RoqmeDDSTopics::RoqmeDoubleContext& data, 
		const dds::sub::SampleInfo& sampleInfo) {
	/*cout << "DOUBLE sample available:" << endl;
	cout << "\t name: " << data.name() << endl;
	for(auto elem:data.value())
	{
		cout << "\t value: " << elem << endl;
	}*/
}

#endif

