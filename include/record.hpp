#include <string>
# include "definitions.hpp"
using namespace Record;

typedef struct {
  //header
  uint timestamp;
  uint block_no;
  Record* nextRecord;
  //record body
  std::string date; //the data type ios meant to be changed from string to something that holds 3 bytes
  uint teamID;
  short uint pts;
  float fg_pct;
  float ft_pct;
  float fg3_pct;
  short uint ast;
  short uint reb;
  bool win;

} Record;


