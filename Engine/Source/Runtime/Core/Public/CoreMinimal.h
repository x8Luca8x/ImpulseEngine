#pragma once

#include "Definitions.h"

#include "Windows/WindowsDebug.h"
#include "Math/Math.h"

#include "Containers/Map.h"
#include "Containers/Name.h"
#include "Containers/Variant.h"
#include "Containers/ArrayView.h"
#include "Containers/ImpulseString.h"

#include "Misc/StringConverter.h"

#include "Serialization/Archive.h"

#include "Misc/Paths.h"
#include "Misc/Parse.h"
#include "Misc/ScopeLock.h"
#include "Misc/CommandLine.h"

#include "Platform/PlatformMisc.h"
#include "Platform/PlatformConsole.h"
#include "Platform/PlatformCirticalSection.h"

#include "Templates/SharedPtr.h"

#include "Delegate/Delegate.h"
#include "Delegate/MulticastDelegate.h"