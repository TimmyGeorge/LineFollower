#ifndef SENSOR_POSITION_CALCULATOR_HPP
#define SENSOR_POSITION_CALCULATOR_HPP

struct Sensor_Position_Calculator{
  public:
    virtual double calcPositionalValue(double sensor_num, unsigned char num_sensors) const = 0;
    virtual double getMiddlePosition(unsigned char num_sensors) const = 0;
};

struct Sensor_Position_Calculator_With_Negative_Positions : public Sensor_Position_Calculator{
  public:
    double calcPositionalValue(double sensor_num, unsigned char num_sensors) const override;
    double getMiddlePosition(unsigned char num_sensors) const override;
};

struct Sensor_Position_Calculator_Without_Negative_Positions : public Sensor_Position_Calculator{
  public:
    double calcPositionalValue(double sensor_num, unsigned char num_sensors) const override;
    double getMiddlePosition(unsigned char num_sensors) const override;
};

extern Sensor_Position_Calculator_With_Negative_Positions sensor_position_calculator_with_negative_positions;
extern Sensor_Position_Calculator_Without_Negative_Positions sensor_position_calculator_without_negative_positions;

#endif
