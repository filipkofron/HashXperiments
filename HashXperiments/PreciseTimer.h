#pragma once

#include <windows.h>

class PreciseTimer
{
private:
  double _pCFreq;
  __int64 _counterStart;
public:
  void Reset()
  {
    LARGE_INTEGER li;
    QueryPerformanceFrequency(&li);
    _pCFreq = double(li.QuadPart) / 1000.0;
    QueryPerformanceCounter(&li);
    _counterStart = li.QuadPart;
  }

  double MeasureNow()
  {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - _counterStart) / _pCFreq;
  }
};
