#include <string>
#include <iomanip>

#include "format.h"

using std::string;
using std::to_string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int hrs, min, sec;
  std::stringstream h, m, s;
  string time;

  min = seconds / 60;
  sec = seconds % 60;
  hrs = min / 60;
  min = min % 60;

  if (hrs < 10) {
    h << std::setfill('0') << std::setw(2) << hrs;
    time += h.str();
  }
  else {
    time += to_string(hrs);
  }

  if (min < 10) {
    m << std::setfill('0') << std::setw(2) << min;
    time += ":" + m.str();
  }
  else {
    time += ":" + to_string(min);
  }

  if (sec < 10) {
    s << std::setfill('0') << std::setw(2) << sec;
    time += ":" + s.str();
  }
  else {
    time += ":" + to_string(sec);
  }

  return time;
}