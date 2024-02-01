#pragma once

#include "Xuzumi/Precompiled.hpp"

#include "Xuzumi/Core/Delegate.hpp"
#include "Xuzumi/Core/FuncName.hpp"
#include "Xuzumi/Core/StringUtils.hpp"
#include "Xuzumi/Core/CompilerInfo.hpp"
#include "Xuzumi/Core/Templates/IsCompatible.hpp"
#include "Xuzumi/Core/Templates/SizeOf.hpp"
#include "Xuzumi/Core/Templates/AlignOf.hpp"

#include "Xuzumi/String/UnicodeFacts.hpp"
#include "Xuzumi/String/Utf8CharView.hpp"
#include "Xuzumi/String/Utf16CharView.hpp"
#include "Xuzumi/String/UtfTextReader.hpp"
#include "Xuzumi/String/StringEncoder.hpp"

#include "Xuzumi/IDFramework/IDBase.hpp"
#include "Xuzumi/IDFramework/HandleGenerator.hpp"

#include "Xuzumi/Input/InputDevice.hpp"
#include "Xuzumi/Input/KeyboardKey.hpp"
#include "Xuzumi/Input/InputEvents.hpp"

#include "Xuzumi/Instrumentation/AssertMacros.hpp"
#include "Xuzumi/Instrumentation/AssertRecord.hpp"
#include "Xuzumi/Instrumentation/AssertRecordBuilder.hpp"
#include "Xuzumi/Instrumentation/AssertService.hpp"
#include "Xuzumi/Instrumentation/InstrumentationSession.hpp"
#include "Xuzumi/Instrumentation/InstrumentationProfile.hpp"
#include "Xuzumi/Instrumentation/LogEmitter.hpp"
#include "Xuzumi/Instrumentation/LogFormatter.hpp"
#include "Xuzumi/Instrumentation/LogRecord.hpp"
#include "Xuzumi/Instrumentation/LogRecordBuilder.hpp"
#include "Xuzumi/Instrumentation/Logger.hpp"
#include "Xuzumi/Instrumentation/LoggerMacros.hpp"
#include "Xuzumi/Instrumentation/LoggerSink.hpp"
#include "Xuzumi/Instrumentation/LoggerSinkBuilder.hpp"

#include "Xuzumi/Memory/WeakPtr.hpp"
#include "Xuzumi/Memory/SharedPtr.hpp"
#include "Xuzumi/Memory/ObserverPtr.hpp"
#include "Xuzumi/Memory/PoolAllocator.hpp"
#include "Xuzumi/Memory/FactoryExpirationGuard.hpp"

#include "Xuzumi/Messaging/Event.hpp"
#include "Xuzumi/Messaging/EventBus.hpp"
#include "Xuzumi/Messaging/EventPublisher.hpp"
#include "Xuzumi/Messaging/EventSubscriber.hpp"

#include "Xuzumi/Platform/PlatformEvents.hpp"
#include "Xuzumi/Platform/PlatformService.hpp"

#include "Xuzumi/TypeMeta/TypeID.hpp"
#include "Xuzumi/TypeMeta/TypeName.hpp"
#include "Xuzumi/TypeMeta/TypeInfo.hpp"
