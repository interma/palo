// Modifications copyright (C) 2017, Baidu.com, Inc.
// Copyright 2017 The Apache Software Foundation

// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#ifndef BDG_PALO_BE_SRC_COMMON_UTIL_CPU_INFO_H
#define BDG_PALO_BE_SRC_COMMON_UTIL_CPU_INFO_H

#include <string>
#include <boost/cstdint.hpp>

#include "common/logging.h"

namespace palo {

// CpuInfo is an interface to query for cpu information at runtime.  The caller can
// ask for the sizes of the caches and what hardware features are supported.
// On Linux, this information is pulled from a couple of sys files (/proc/cpuinfo and
// /sys/devices)
class CpuInfo {
public:
    static const int64_t SSE3    = (1 << 1);
    static const int64_t SSE4_1  = (1 << 2);
    static const int64_t SSE4_2  = (1 << 3);
    static const int64_t POPCNT  = (1 << 4);

    // Cache enums for L1 (data), L2 and L3
    enum CacheLevel {
        L1_CACHE = 0,
        L2_CACHE = 1,
        L3_CACHE = 2,
    };

    // Initialize CpuInfo.
    static void init();

    // Returns all the flags for this cpu
    static int64_t hardware_flags() {
        DCHECK(_s_initialized);
        return _s_hardware_flags;
    }

    // Returns whether of not the cpu supports this flag
    inline static bool is_supported(long flag) {
        DCHECK(_s_initialized);
        return (_s_hardware_flags & flag) != 0;
    }

    // Toggle a hardware feature on and off.  It is not valid to turn on a feature
    // that the underlying hardware cannot support. This is useful for testing.
    static void enable_feature(long flag, bool enable);

    // Returns the size of the cache in KB at this cache level
    static long cache_size(CacheLevel level) {
        DCHECK(_s_initialized);
        return _s_cache_sizes[level];
    }

    // Returns the number of cpu cycles per millisecond
    static int64_t cycles_per_ms() {
        DCHECK(_s_initialized);
        return _s_cycles_per_ms;
    }

    // Returns the number of cores (including hyper-threaded) on this machine.
    static int num_cores() {
        DCHECK(_s_initialized);
        return _s_num_cores;
    }

    // Returns the model name of the cpu (e.g. Intel i7-2600)
    static std::string model_name() {
        DCHECK(_s_initialized);
        return _s_model_name;
    }

    static std::string debug_string();

private:
    static bool _s_initialized;
    static int64_t _s_hardware_flags;
    static int64_t _s_original_hardware_flags;
    static long _s_cache_sizes[L3_CACHE + 1];
    static int64_t _s_cycles_per_ms;
    static int _s_num_cores;
    static std::string _s_model_name;
};

}
#endif
