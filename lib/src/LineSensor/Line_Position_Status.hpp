#ifndef LINE_POSITION_STATUS
#define LINE_POSITION_STATUS

struct Line_Position_Status{
  enum Status : unsigned char{
    off_line = 0,
    on_line = 1,
    unknown = 2,
    error = 255
  };
  double estimate;
  Status status;
};

#endif