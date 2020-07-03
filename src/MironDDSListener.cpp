#include "mironDDS_listener.hpp"
#include "change_velocity.hpp"
/* Copyright (C) 2020 Adrian Romero -  All Rights Reserved
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

using namespace zmqserver;

MironDDSListener::MironDDSListener(QueryClient* queryClient){
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
EstimateListener::EstimateListener(QueryClient* queryClient){
	this->queryClient = queryClient;
}

void EstimateListener::changeVelocity(double value){
	Velocity vel(value, 0);
	ChangeVelocity vel_msg(queryClient->getID(), vel);
        queryClient->setMsg(std::move(vel_msg.dump()));
       	queryClient->send();
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
			changeVelocity(0.5);
		}
		else if(value <= 0.7) {
			changeVelocity(0.7);
		}
		else {
			changeVelocity(1);
		}
		
		previousValue = value;
	}
}

#ifdef SUBSCRIBE_TO_ROQME_CONTEXTS

/* 
 * Roqme context listeners implementation
 */

void IntContextListener::dataAvailable(const RoqmeDDSTopics::RoqmeIntContext& data, const dds::sub::SampleInfo& sampleInfo){
	/* cout << "INT sample available" << endl;
	cout << "\t name: " << data.name() << endl;
	for(auto elem:data.value())
	{
		cout << "\t " << elem << endl;
	}*/
}

void UIntContextListener::dataAvailable(const RoqmeDDSTopics::RoqmeUIntContext& data, const dds::sub::SampleInfo& sampleInfo){
	/*cout << "UINT sample available:" << endl;
	cout << "\t name: " << data.name() << endl;
	for(auto elem:data.value())
	{
		cout << "\t " << elem << endl;
	}*/
}

void BoolContextListener::dataAvailable(const RoqmeDDSTopics::RoqmeBoolContext& data, const dds::sub::SampleInfo& sampleInfo){
	/*cout << "BOOL sample available:" << endl;
	cout << "\t name: " << data.name() << endl;
	for(auto elem:data.value())
	{
		cout << "\t " << elem << endl;
	}*/
}

void EnumContextListener::dataAvailable(const RoqmeDDSTopics::RoqmeEnumContext& data, const dds::sub::SampleInfo& sampleInfo){
	cout << "ENUM sample available:" << endl;
	cout << "\t name: " << data.name() << endl;
	for(auto elem:data.value())
	{
		cout << "\t " << elem << endl;
	}
}

void EventContextListener::dataAvailable(const RoqmeDDSTopics::RoqmeEventContext& data, const dds::sub::SampleInfo& sampleInfo){
	/*cout << "EVENT sample available:" << endl;
	cout << "\t name: " << data.name() << endl;
	for(auto elem:data.value())
	{
		cout << "\t " << elem << endl;
	}*/
}

void DoubleContextListener::dataAvailable(const RoqmeDDSTopics::RoqmeDoubleContext& data, const dds::sub::SampleInfo& sampleInfo){
	/*cout << "DOUBLE sample available:" << endl;
	cout << "\t name: " << data.name() << endl;
	for(auto elem:data.value())
	{
		cout << "\t value: " << elem << endl;
	}*/
}

#endif

