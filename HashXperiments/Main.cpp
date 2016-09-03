#include "HashMap.h"
#include <cstdlib>
#include <vector>
#include <map>
#include <unordered_map>
#include "PreciseTimer.h"

int main(void)
{
  struct IntHolder
  {
    IntHolder() { }
    IntHolder(int val) : _val(val) { }
    int _val;
    bool Compare(const IntHolder& another) const
    {
      return _val == another._val;
    }

    int GetHashCode() const
    {
      return _val;
    }
  };

  std::vector<int> src;

  int size = 1000 * 1000 * 10;

  PreciseTimer timer;

  {
    printf("Generating %i numbers ..\n", size);
    timer.Reset();
    for (int i = 0; i < size; i++)
    {
      src.push_back(i * rand());
    }
    double ms = timer.MeasureNow();
    printf("Generating numbers .. done in %lf millis\n\n", ms);
  }

  {
    printf("==== Testing my own hashmap ===\n");
    printf("Own hash insert begin..\n");
    timer.Reset();
    HashMap<IntHolder> map;
    for (int i = 0; i < size; i++)
    {
      map.Add(IntHolder(src[i]));
    }
    double ms = timer.MeasureNow();
    printf("Own hash insert end  in %lf millis\n", ms);

    printf("Own hash retrieve begin..\n");
    timer.Reset();
    int found = 0;
    for (int i = 0; i < size; i++)
    {
      if (map.NotNull(map.Get(IntHolder(src[i]))))
        found++;
    }
    ms = timer.MeasureNow();
    printf("Own hash retrieve found %i values\n", found);
    printf("Own hash retrieve end in %lf millis\n", ms);

    printf("Own hash delete begin..\n");
    timer.Reset();
    for (int i = 0; i < size; i++)
    {
      map.Remove(IntHolder(src[i]));
    }
    ms = timer.MeasureNow();
    printf("Own hash delete end in %lf millis\n", ms);

    printf("Final size: %zu\n\n", map.GetSize());
  }

  {
    printf("==== Testing std::unordered_map ===\n");
    printf("stdumap insert begin..\n");
    timer.Reset();
    std::unordered_map<int, int> map;
    for (int i = 0; i < size; i++)
    {
      map.insert(std::pair<int, int>(src[i], src[i]));
    }
    double ms = timer.MeasureNow();
    printf("stdumap insert end  in %lf millis\n", ms);

    printf("stdumap retrieve begin..\n");
    int found = 0;
    timer.Reset();
    for (int i = 0; i < size; i++)
    {
      if (map.find(src[i]) != map.end())
        found++;
    }
    ms = timer.MeasureNow();
    printf("stdumap retrieve found %i values\n", found);
    printf("stdumap retrieve end in %lf millis\n", ms);

    printf("stdumap delete begin..\n");
    timer.Reset();
    for (int i = 0; i < size; i++)
    {
      map.erase(src[i]);
    }
    ms = timer.MeasureNow();
    printf("stdumap delete end in %lf millis\n", ms);

    printf("Final size: %zu\n\n", map.size());
  }

  {
    printf("==== Testing std::map ===\n");
    printf("stdmap insert begin..\n");
    timer.Reset();
    std::map<int, int> map;
    for (int i = 0; i < size; i++)
    {
      map.insert(std::pair<int, int>(src[i], src[i]));
    }
    double ms = timer.MeasureNow();
    printf("stdmap insert end in %lf millis\n", ms);

    printf("stdmap retrieve begin..\n");
    int found = 0;
    timer.Reset();
    for (int i = 0; i < size; i++)
    {
      if (map.find(src[i]) != map.end())
        found++;
    }
    ms = timer.MeasureNow();
    printf("stdmap retrieve found %i values\n", found);
    printf("stdmap retrieve end in %lf millis\n", ms);

    printf("stdmap delete begin..\n");
    timer.Reset();
    for (int i = 0; i < size; i++)
    {
      map.erase(src[i]);
    }
    ms = timer.MeasureNow();
    printf("stdmap delete end in %lf millis\n", ms);

    printf("Final size: %zu\n\n", map.size());
  }

  getc(stdin);

  return 0;
}