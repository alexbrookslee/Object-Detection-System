									OutDevice

OutDevice represents a single group of associated motor used to represent positional information gathered by the system as pressure. Any number of these bands can be used in a given system, and 
support as many motors as are required. All OutDevices in a given system should have an identical number of motors, which should not exceed the number of reads taken by the system across one 
rotation of the InDevice. The length of the input region is split evenly between the OutDevices in the system, with each portion being associated with a particular positon in the master data table 
and motor port list based on index values. Each column (in both tables) is associated with a sequetially  "closer" section of the input region. Each OutDevice uses the index of it's portion of the data table 
and the size of the interval between units to define its range, then scales the response produced by its subordinate motors to match the ratio of the given value to the largest value (and minimum distance)
it can represent. 





FORMAT:
	      Each OutDevice contains a number of motors, which it manipulates based on the index and values of a specific column in in a larger table. Port numbers for the output motors during 
initialization, creating an identically indexed array of motor controllers. Each motor in the array is responsible for a segment of the horizontal range of the input device, in a manner similar to OutDevices
along the vertical axis. The number of motors representing this axis should not exceed the number of reads taken along it, but the reverse is not true. Multiple reads within the same segment will be correlated
to increase the accuaracy of the information, then displayed on the same motor (By default a simple average). Positional data for the entire device is updated simultaneously, and is a distinct step from actually 
writting that information to the motors.  OutDevice updates its data by taking a pointer to the entire master data table, then mapping each value in its specific column to an equivalent on the local scale. The 
new values are stored within the OutDevice, in an array associated by index with the relevant motor. These values will only be written to the actual motors when the OutDevice's apply method is called.


WORKFLOW:
	        In the typical use case, OutDevices will be generated in sets associated with specific columns of a master data table. Each will be aware of the interval size, and the location within the
master table relevant to its function. At the end of each operational cycle of the system, after the master data table has been updated, each OutDevice in the set will be given a pointer to the table, 
allowing it to copy and remap the data to a local equivalent. The actual application of this data to the motors usually occurs directly after this, across all devices in the set. It is possible, however, to apply 
the update to each part of the set in whatever order, and at whatever interval, is required. 

ADDENDUM:
	        - Vertical refers to the axis related to distance, and is split evenly between different OutDevices
	        - Horizontal refers to the axis related to the angle of the InDevice when taking a reading along the vertical axis, and is split evenly between
		different motors within each OutDevice. 
	       
	        - The Type of motor does not need to be the same, but it is inadvisable to mix types.
	        - Using more motors than input steps will result in wasted hardware, the reverse will allow for more detailed ranging information.
	        - By default, the "degree" representation of a distance value shares an inverse relationship to the distance of the object in question. 
		Deviation from this standard may be required for some devices, but it is recommended that this be accomplished by altering 
		how input data is evaluated.  
	        - When generating sets of OutDevices, it is recommended that each one's index match that of it's section of the master data table.
	        - The remaping tools are primarily intended to allow for motor types that do not accept the system level format, and to increase 
		output clarity, but also allows for mixing different motor types in the same system. 