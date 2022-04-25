/*
 * Copyright (C) 2012 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

#include <sys/cdefs.h>

#include <mutex>
#include <set>
#include <string>

#include <platform/bionic/macros.h>

struct MapEntry {
  MapEntry(uintptr_t start, uintptr_t end, uintptr_t offset, const char* name, size_t name_len, int flags)
      : start(start), end(end), offset(offset), name(name, name_len), flags(flags) {}

  explicit MapEntry(uintptr_t pc) : start(pc), end(pc) {}

  uintptr_t start;
  uintptr_t end;
  uintptr_t offset;
  uintptr_t load_bias;
  uintptr_t elf_start_offset = 0;
  std::string name;
  int flags;
  bool init = false;
  bool valid = false;
};

// Ordering comparator that returns equivalence for overlapping entries
struct compare_entries {
  bool operator()(const MapEntry* a, const MapEntry* b) const { return a->end <= b->start; }
};

class MapData {
 public:
  MapData() = default;
  ~MapData();

  const MapEntry* find(uintptr_t pc, uintptr_t* rel_pc = nullptr);

 private:
  bool ReadMaps();

  std::mutex m_;
  std::set<MapEntry*, compare_entries> entries_;

  BIONIC_DISALLOW_COPY_AND_ASSIGN(MapData);
};
