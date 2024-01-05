#pragma once

#include "Xuzumi/Precompiled.hpp"

#include "Xuzumi/Core/Delegate.hpp"
#include "Xuzumi/Core/FuncName.hpp"
#include "Xuzumi/Core/StringUtils.hpp"
#include "Xuzumi/Core/CompilerInfo.hpp"
#include "Xuzumi/Core/Templates/IsCompatible.hpp"
#include "Xuzumi/Core/Templates/SizeOf.hpp"
#include "Xuzumi/Core/Templates/AlignOf.hpp"

#include "Xuzumi/String/UtfEncoding.hpp"
#include "Xuzumi/String/UnicodeFacts.hpp"
#include "Xuzumi/String/Utf8CharView.hpp"
#include "Xuzumi/String/Utf16CharView.hpp"
#include "Xuzumi/String/UtfTextReader.hpp"

#include "Xuzumi/Debug/Logging.hpp"
#include "Xuzumi/Debug/Assertion.hpp"
#include "Xuzumi/Debug/DebugSession.hpp"
#include "Xuzumi/Debug/FileLogWriter.hpp"
#include "Xuzumi/Debug/ConsoleLogWriter.hpp"
#include "Xuzumi/Debug/TimedLogFormatter.hpp"

#include "Xuzumi/IDFramework/IDBase.hpp"
#include "Xuzumi/IDFramework/HandleGenerator.hpp"

#include "Xuzumi/Input/InputDevice.hpp"
#include "Xuzumi/Input/KeyboardKey.hpp"
#include "Xuzumi/Input/InputEvents.hpp"

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
