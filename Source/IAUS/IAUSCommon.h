// Copyright 2017-2021 Project Borealis

#pragma once

#include "CoreMinimal.h"
#include "Logging/LogMacros.h"

#if UE_BUILD_SHIPPING
DECLARE_LOG_CATEGORY_EXTERN(LogIAUS, Warning, Warning);
#else
DECLARE_LOG_CATEGORY_EXTERN(LogIAUS, Log, All);
#endif

DECLARE_STATS_GROUP(TEXT("IAUS"), STATGROUP_IAUS, STATCAT_Advanced);
