#include <string>
using namespace std;

typedef unsigned int uint;

typedef struct {
  //header
  uint timestamp;
  uint block_no;
  Record* nextRecord;
  //record body
  std::string date; //the data type ios meant to be changed from string to something that holds 3 bytes
  uint teamID;
  short pts;
  float fg_pct;
  float ft_pct;
  float fg3_pct;
  short ast;
  short reb;
  bool win;

} Record;


