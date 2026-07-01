/*
Testing
Battery_Monitor(float _minimum_vin, float _maximum_vin, Simple_Potential_Divider _potential_divider, Esp32_Voltage_Reader* _p_voltage_reader);
Test Data, Expected Output, Actual Output
Simple_Potential_Divider divider(1000, 2000)  Esp32_Voltage_Reader(5) Esp32_Battery_Monitor bm(2.7, 4.2, &pd, &vr),
  {minv=2.7, maxv=4.2, pd={1000,2000}, vr={mav=3.30, ap=5, arr=12, aar=12}},
  {minv=2.70, maxv=4.20, pd={1000, 2000}, vr={mav=3.30, ap=5, arr=12, aar=12}}, true
Simple_Potential_Divider(1500000, 4500000) Esp32_Voltage_Reader(1, 7) Esp32_Battery_Monitor monitor(0.5, 2.3, &divider, &reader)
  {minv=0.50, maxv=2.30, pd={1500000,4500000}, vr={mav=3.30, ap=1, arr=12, aar=7}},
  {minv=0.50, maxv=2.30, pd={1500000, 4500000}, vr={mav=3.30, ap=1, arr=12, aar=7}}, true
  

Testing
float convVinToPercentage(float _vin);
Test Data, Expected Output, Actual Output
Normal, minv=3.5 maxv=6.3 _vin=4.5, 35.71, 35.71, true
Normal, minv=2 maxv=4.3 _vin=12, 100, 100.00, true
Normal, minv=12.34 mav=13.12 _vin=1, 0, 0.00, true


Testing readVoutVoltage();
Test Data, Expected Output, Actual Output
adc_res = 12 CURRENT_VOLTAGE_CONNECTED as VOUT, voltage ~ 2.82, voltage = 2.89, true (close enough)


Testing getBatteryVoltage();
Test Data, Expected Output, Actual Output
BV = 7.69V, ~7.69V, 7.88V, true (close enough) 


Testing getBatteryPercentage();
BV = 7.69V maxv=8.4 minv=5.4, ~76.33, 82.69%, true (ecf from getBatteryVoltage)





  








*/