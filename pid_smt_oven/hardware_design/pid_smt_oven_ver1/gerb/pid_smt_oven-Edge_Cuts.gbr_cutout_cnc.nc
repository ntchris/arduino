(G-CODE GENERATED BY FLATCAM v8.994 - www.flatcam.org - Version Date: 2020/11/7)

(Name: pid_smt_oven-Edge_Cuts.gbr_cutout_cnc)
(Type: G-code from Geometry)
(Units: MM)

(Created on Sunday, 24 October 2021 at 20:42)

(This preprocessor is the default preprocessor used by FlatCAM.)
(It is made to work with MACH3 compatible motion controllers.)

(TOOL DIAMETER: 0.9 mm)
(Feedrate_XY: 120.0 mm/min)
(Feedrate_Z: 30.0 mm/min)
(Feedrate rapids 1500.0 mm/min)

(Z_Cut: -1.8 mm)
(DepthPerCut: 0.6 mm <=>3 passes)
(Z_Move: 0.7 mm)
(Z Start: None mm)
(Z End: 25.0 mm)
(X,Y End: None mm)
(Steps per circle: 64)
(Preprocessor Geometry: default)

(X range:   -0.2000 ...   69.7000  mm)
(Y range:  -45.1000 ...    0.3000  mm)

(Spindle Speed: 12000.0 RPM)
G21
G90
G94

$1=255		
G01 F120.00

M5
G00 Z2.0000
G00 X0.0000 Y0.0000
T1
M6    
(MSG, Change to Tool Dia = 0.9000)
M0
G00 Z2.0000

M03 S12000.0
G01 F120.00
G00 X34.1600 Y-45.1000
G01 F30.00
G01 Z-0.6000
G01 F120.00
G01 X0.2500 Y-45.1000
G01 X0.2059 Y-45.0979
G01 X0.1622 Y-45.0914
G01 X0.1193 Y-45.0806
G01 X0.0778 Y-45.0658
G01 X0.0378 Y-45.0469
G01 X-0.0000 Y-45.0242
G01 X-0.0355 Y-44.9979
G01 X-0.0682 Y-44.9682
G01 X-0.0979 Y-44.9355
G01 X-0.1242 Y-44.9000
G01 X-0.1469 Y-44.8622
G01 X-0.1658 Y-44.8222
G01 X-0.1806 Y-44.7807
G01 X-0.1914 Y-44.7378
G01 X-0.1979 Y-44.6941
G01 X-0.2000 Y-44.6500
G01 X-0.2000 Y-22.9900
G00 X-0.2000 Y-22.9900
G01 F30.00
G01 Z-1.2000
G01 F120.00
G01 X-0.2000 Y-44.6500
G01 X-0.1979 Y-44.6941
G01 X-0.1914 Y-44.7378
G01 X-0.1806 Y-44.7807
G01 X-0.1658 Y-44.8222
G01 X-0.1469 Y-44.8622
G01 X-0.1242 Y-44.9000
G01 X-0.0979 Y-44.9355
G01 X-0.0682 Y-44.9682
G01 X-0.0355 Y-44.9979
G01 X-0.0000 Y-45.0242
G01 X0.0378 Y-45.0469
G01 X0.0778 Y-45.0658
G01 X0.1193 Y-45.0806
G01 X0.1622 Y-45.0914
G01 X0.2059 Y-45.0979
G01 X0.2500 Y-45.1000
G01 X34.1600 Y-45.1000
G00 X34.1600 Y-45.1000
G01 F30.00
G01 Z-1.8000
G01 F120.00
G01 X0.2500 Y-45.1000
G01 X0.2059 Y-45.0979
G01 X0.1622 Y-45.0914
G01 X0.1193 Y-45.0806
G01 X0.0778 Y-45.0658
G01 X0.0378 Y-45.0469
G01 X-0.0000 Y-45.0242
G01 X-0.0355 Y-44.9979
G01 X-0.0682 Y-44.9682
G01 X-0.0979 Y-44.9355
G01 X-0.1242 Y-44.9000
G01 X-0.1469 Y-44.8622
G01 X-0.1658 Y-44.8222
G01 X-0.1806 Y-44.7807
G01 X-0.1914 Y-44.7378
G01 X-0.1979 Y-44.6941
G01 X-0.2000 Y-44.6500
G01 X-0.2000 Y-22.9900
G00 Z0.7000
G00 X34.1600 Y-45.1000
G01 F30.00
G01 Z-0.6000
G01 F120.00
G01 X0.2500 Y-45.1000
G01 X0.2059 Y-45.0979
G01 X0.1622 Y-45.0914
G01 X0.1193 Y-45.0806
G01 X0.0778 Y-45.0658
G01 X0.0378 Y-45.0469
G01 X-0.0000 Y-45.0242
G01 X-0.0355 Y-44.9979
G01 X-0.0682 Y-44.9682
G01 X-0.0979 Y-44.9355
G01 X-0.1242 Y-44.9000
G01 X-0.1469 Y-44.8622
G01 X-0.1658 Y-44.8222
G01 X-0.1806 Y-44.7807
G01 X-0.1914 Y-44.7378
G01 X-0.1979 Y-44.6941
G01 X-0.2000 Y-44.6500
G01 X-0.2000 Y-22.9900
G00 X-0.2000 Y-22.9900
G01 F30.00
G01 Z-1.2000
G01 F120.00
G01 X-0.2000 Y-44.6500
G01 X-0.1979 Y-44.6941
G01 X-0.1914 Y-44.7378
G01 X-0.1806 Y-44.7807
G01 X-0.1658 Y-44.8222
G01 X-0.1469 Y-44.8622
G01 X-0.1242 Y-44.9000
G01 X-0.0979 Y-44.9355
G01 X-0.0682 Y-44.9682
G01 X-0.0355 Y-44.9979
G01 X-0.0000 Y-45.0242
G01 X0.0378 Y-45.0469
G01 X0.0778 Y-45.0658
G01 X0.1193 Y-45.0806
G01 X0.1622 Y-45.0914
G01 X0.2059 Y-45.0979
G01 X0.2500 Y-45.1000
G01 X34.1600 Y-45.1000
G00 X34.1600 Y-45.1000
G01 F30.00
G01 Z-1.8000
G01 F120.00
G01 X0.2500 Y-45.1000
G01 X0.2059 Y-45.0979
G01 X0.1622 Y-45.0914
G01 X0.1193 Y-45.0806
G01 X0.0778 Y-45.0658
G01 X0.0378 Y-45.0469
G01 X-0.0000 Y-45.0242
G01 X-0.0355 Y-44.9979
G01 X-0.0682 Y-44.9682
G01 X-0.0979 Y-44.9355
G01 X-0.1242 Y-44.9000
G01 X-0.1469 Y-44.8622
G01 X-0.1658 Y-44.8222
G01 X-0.1806 Y-44.7807
G01 X-0.1914 Y-44.7378
G01 X-0.1979 Y-44.6941
G01 X-0.2000 Y-44.6500
G01 X-0.2000 Y-22.9900
G00 Z0.7000
G00 X-0.2000 Y-21.8100
G01 F30.00
G01 Z-0.6000
G01 F120.00
G01 X-0.2000 Y-0.1500
G01 X-0.1979 Y-0.1059
G01 X-0.1914 Y-0.0622
G01 X-0.1806 Y-0.0193
G01 X-0.1658 Y0.0222
G01 X-0.1469 Y0.0622
G01 X-0.1242 Y0.1000
G01 X-0.0979 Y0.1355
G01 X-0.0682 Y0.1682
G01 X-0.0355 Y0.1979
G01 X-0.0000 Y0.2242
G01 X0.0378 Y0.2469
G01 X0.0778 Y0.2658
G01 X0.1193 Y0.2806
G01 X0.1622 Y0.2914
G01 X0.2059 Y0.2979
G01 X0.2500 Y0.3000
G01 X34.1600 Y0.3000
G00 X34.1600 Y0.3000
G01 F30.00
G01 Z-1.2000
G01 F120.00
G01 X0.2500 Y0.3000
G01 X0.2059 Y0.2979
G01 X0.1622 Y0.2914
G01 X0.1193 Y0.2806
G01 X0.0778 Y0.2658
G01 X0.0378 Y0.2469
G01 X-0.0000 Y0.2242
G01 X-0.0355 Y0.1979
G01 X-0.0682 Y0.1682
G01 X-0.0979 Y0.1355
G01 X-0.1242 Y0.1000
G01 X-0.1469 Y0.0622
G01 X-0.1658 Y0.0222
G01 X-0.1806 Y-0.0193
G01 X-0.1914 Y-0.0622
G01 X-0.1979 Y-0.1059
G01 X-0.2000 Y-0.1500
G01 X-0.2000 Y-21.8100
G00 X-0.2000 Y-21.8100
G01 F30.00
G01 Z-1.8000
G01 F120.00
G01 X-0.2000 Y-0.1500
G01 X-0.1979 Y-0.1059
G01 X-0.1914 Y-0.0622
G01 X-0.1806 Y-0.0193
G01 X-0.1658 Y0.0222
G01 X-0.1469 Y0.0622
G01 X-0.1242 Y0.1000
G01 X-0.0979 Y0.1355
G01 X-0.0682 Y0.1682
G01 X-0.0355 Y0.1979
G01 X-0.0000 Y0.2242
G01 X0.0378 Y0.2469
G01 X0.0778 Y0.2658
G01 X0.1193 Y0.2806
G01 X0.1622 Y0.2914
G01 X0.2059 Y0.2979
G01 X0.2500 Y0.3000
G01 X34.1600 Y0.3000
G00 Z0.7000
G00 X35.3400 Y0.3000
G01 F30.00
G01 Z-0.6000
G01 F120.00
G01 X69.2500 Y0.3000
G01 X69.2941 Y0.2979
G01 X69.3378 Y0.2914
G01 X69.3807 Y0.2806
G01 X69.4222 Y0.2658
G01 X69.4622 Y0.2469
G01 X69.5000 Y0.2242
G01 X69.5355 Y0.1979
G01 X69.5682 Y0.1682
G01 X69.5979 Y0.1355
G01 X69.6242 Y0.1000
G01 X69.6469 Y0.0622
G01 X69.6658 Y0.0222
G01 X69.6806 Y-0.0193
G01 X69.6914 Y-0.0622
G01 X69.6979 Y-0.1059
G01 X69.7000 Y-0.1500
G01 X69.7000 Y-21.8100
G00 X69.7000 Y-21.8100
G01 F30.00
G01 Z-1.2000
G01 F120.00
G01 X69.7000 Y-0.1500
G01 X69.6979 Y-0.1059
G01 X69.6914 Y-0.0622
G01 X69.6806 Y-0.0193
G01 X69.6658 Y0.0222
G01 X69.6469 Y0.0622
G01 X69.6242 Y0.1000
G01 X69.5979 Y0.1355
G01 X69.5682 Y0.1682
G01 X69.5355 Y0.1979
G01 X69.5000 Y0.2242
G01 X69.4622 Y0.2469
G01 X69.4222 Y0.2658
G01 X69.3807 Y0.2806
G01 X69.3378 Y0.2914
G01 X69.2941 Y0.2979
G01 X69.2500 Y0.3000
G01 X35.3400 Y0.3000
G00 X35.3400 Y0.3000
G01 F30.00
G01 Z-1.8000
G01 F120.00
G01 X69.2500 Y0.3000
G01 X69.2941 Y0.2979
G01 X69.3378 Y0.2914
G01 X69.3807 Y0.2806
G01 X69.4222 Y0.2658
G01 X69.4622 Y0.2469
G01 X69.5000 Y0.2242
G01 X69.5355 Y0.1979
G01 X69.5682 Y0.1682
G01 X69.5979 Y0.1355
G01 X69.6242 Y0.1000
G01 X69.6469 Y0.0622
G01 X69.6658 Y0.0222
G01 X69.6806 Y-0.0193
G01 X69.6914 Y-0.0622
G01 X69.6979 Y-0.1059
G01 X69.7000 Y-0.1500
G01 X69.7000 Y-21.8100
G00 Z0.7000
G00 X69.7000 Y-22.9900
G01 F30.00
G01 Z-0.6000
G01 F120.00
G01 X69.7000 Y-44.6500
G01 X69.6979 Y-44.6941
G01 X69.6914 Y-44.7378
G01 X69.6806 Y-44.7807
G01 X69.6658 Y-44.8222
G01 X69.6469 Y-44.8622
G01 X69.6242 Y-44.9000
G01 X69.5979 Y-44.9355
G01 X69.5682 Y-44.9682
G01 X69.5355 Y-44.9979
G01 X69.5000 Y-45.0242
G01 X69.4622 Y-45.0469
G01 X69.4222 Y-45.0658
G01 X69.3807 Y-45.0806
G01 X69.3378 Y-45.0914
G01 X69.2941 Y-45.0979
G01 X69.2500 Y-45.1000
G01 X35.3400 Y-45.1000
G00 X35.3400 Y-45.1000
G01 F30.00
G01 Z-1.2000
G01 F120.00
G01 X69.2500 Y-45.1000
G01 X69.2941 Y-45.0979
G01 X69.3378 Y-45.0914
G01 X69.3807 Y-45.0806
G01 X69.4222 Y-45.0658
G01 X69.4622 Y-45.0469
G01 X69.5000 Y-45.0242
G01 X69.5355 Y-44.9979
G01 X69.5682 Y-44.9682
G01 X69.5979 Y-44.9355
G01 X69.6242 Y-44.9000
G01 X69.6469 Y-44.8622
G01 X69.6658 Y-44.8222
G01 X69.6806 Y-44.7807
G01 X69.6914 Y-44.7378
G01 X69.6979 Y-44.6941
G01 X69.7000 Y-44.6500
G01 X69.7000 Y-22.9900
G00 X69.7000 Y-22.9900
G01 F30.00
G01 Z-1.8000
G01 F120.00
G01 X69.7000 Y-44.6500
G01 X69.6979 Y-44.6941
G01 X69.6914 Y-44.7378
G01 X69.6806 Y-44.7807
G01 X69.6658 Y-44.8222
G01 X69.6469 Y-44.8622
G01 X69.6242 Y-44.9000
G01 X69.5979 Y-44.9355
G01 X69.5682 Y-44.9682
G01 X69.5355 Y-44.9979
G01 X69.5000 Y-45.0242
G01 X69.4622 Y-45.0469
G01 X69.4222 Y-45.0658
G01 X69.3807 Y-45.0806
G01 X69.3378 Y-45.0914
G01 X69.2941 Y-45.0979
G01 X69.2500 Y-45.1000
G01 X35.3400 Y-45.1000
G00 Z0.7000
M05
G00 Z0.7000
G00 Z25.00


$1=254