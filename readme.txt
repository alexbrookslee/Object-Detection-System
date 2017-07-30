									The Object Detection Toolkit

	The tools provided with this document are meant to facillitate the operation of a device capable of collecting positional information about a region fixed around the detection aparatus, and mapping this data on to a 2D plane. This plane 
is intended to be used to set the output parameters of a group of motors that constitute the device's user feedback.  The essential purpose of this system is to allow for the user to feel the relative positions of objects in a specified region
(in relation to themselves) as a sensation of pressure at various points along the output region which map to subsections of the input zone. 

	The three basic components of this system are InDevices, OutDevices, and DetectLayers. In the standard layout a single InDevive is used to govern the detection systems, supplying data to the associated DetectLayer. 
The DetectLayer in turn parses raw data into an internal format that can be converted into meangingful instructions for the set of OutDevices it contains.

FORMAT:
	The input region is definded by the maximum range of the input hardware, and the arc(s) along which those device(s) move. The raw distance is mapped onto a 180  degree arc, 
these values decrease as range increases by default. Each reading is stored in the master data table maintained by the DetectLayer at a position determined by distance of the target, and the angle the reading was taken at. 
By default, the data table has one column for each OutDevice, and one row for each motor present within them. The 180 degree representation of the input region is split into a number equally sized regions matching the number of 
OutDevices in the system. All readings that fall into a particular range are stored in the portion of the data table associated with that band; the larger index of a particular column is, the greater the gross values of the region are. 
Each OutDevice is capable of adjusting it's output based on the bounds of its range; by default taking a ratio of the given value and the maximum value of the range and directly setting the output motors
 to the equivalent percentage of their pressure response.

WORKFLOW:
	       It is reccomended that the InDevice be allowed to operate continously until some condition  is met, feeding its results to the DetectLayer as it generates them. The DetectLayer will parse this information and store it in the
appropriate location. Overlapping values should not overwrite each other, but rather be used to generate a more accurate result (by default they are averaged). Once the end condition of the input phase is reached, the DetectLayer should
be used to trigger a write cycle in all of it's subordinate OutDevices and a reset of the master data table (not including the motors). User control should be managed using an Interrupt Request Service to ensure the signal is not missed.
	

ADDENDUM:
	         - Symmetry is not required, but commonly assumed across the Toolkit, deviation from this format should be approached with care