#pragma once

#include "Xuzumi/Instrumentation/PerformanceRecord.hpp"

namespace Xuzumi
{
	class PerformanceRecordStream
	{
	public:
		virtual ~PerformanceRecordStream() = default;

		virtual void Out(const PerformanceRecord& record) = 0;
	};

	class TraceEventPerformanceRecordStream : public PerformanceRecordStream
	{
	public:
		TraceEventPerformanceRecordStream(std::string_view fileName);
		~TraceEventPerformanceRecordStream() override;

		void Out(const PerformanceRecord& record) override;

	private:
		void WriteHeader();
		void WriteFooter();

		std::mutex mTraceEventsMutex;
		std::ofstream mTraceEvents;
	};
}

