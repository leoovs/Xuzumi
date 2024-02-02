#include "Xuzumi/Instrumentation/PerformanceRecordStream.hpp"
#include <iomanip>

namespace Xuzumi
{
	TraceEventPerformanceRecordStream::TraceEventPerformanceRecordStream(
		std::string_view fileName
	)
		: mTraceEvents(fileName.data())
	{
		WriteHeader();
		mTraceEvents << std::fixed << std::showpoint;
	}

	TraceEventPerformanceRecordStream::~TraceEventPerformanceRecordStream()
	{
		WriteFooter();
	}

	void TraceEventPerformanceRecordStream::Out(const PerformanceRecord& record)
	{
		std::scoped_lock traceEventsLock(mTraceEventsMutex);

		constexpr double cMicroOverNano = 1e-3;

		mTraceEvents << ",{"
			<< "\"cat\": \"function\","
			<< "\"dur\": " << record.Duration.count() * cMicroOverNano << ','
			<< "\"name\": \"" << record.BlockName << "\","
			<< "\"ph\": \"X\","
			<< "\"pid\": 0,"
			<< "\"tid\": " << record.ThreadID << ','
			<< "\"ts\": " << record.StartTimestamp.count() * cMicroOverNano
			<< '}';
	}

	void TraceEventPerformanceRecordStream::WriteHeader()
	{
		mTraceEvents << "{\"otherData\": {},\"traceEvents\":[{}";
		mTraceEvents.flush();
	}

	void TraceEventPerformanceRecordStream::WriteFooter()
	{
		mTraceEvents << "]}";
		mTraceEvents.flush();
	}
}
