									DetectLayer

DetectLayer serves primarily as the data managment center of the system, with emphasis on interfacing with the various output systems in use.
The two core elements to DetectLayer are the master data table and Outdevice arrays. The data table contains the current state of the input region,
as provided by the input device, after it has been remapped to an internal scale and stored in a specific location based on it's value, and the position of the
reader when  it was taken. When the updates to the table for a particular cycle are complete, DetectLayer is used to pass the data table to the OutDevices then 
dictating the order and timing of their physical response. 



FORMAT:
	         The master data table of DetectLayer serves as a central staging ground for all information passing through the system; input values are 
remapped to a value on the range  [0. 180], with 0 representing the minimum input value ( by default maximum distance, or no result) and 180 representing the 
maximum input value (by default the minimum distance, or closest possible result). This value, in conjunction with the read angle of the InDevice, is used to select
a location on the master data table to store the newly parsed value in. Each Columnn of the table is associated with a specific postion of the the total range of the input
area, with each one being managed by a specific OutDevice. The size of these intervals are identical, with the specific values (and therefore proximity) they represent increasing
as the column index does. After an input value has been remaped, it will be evaluated to determine which of the subsections it falls within, then stored within the column paired with 
value set. Row position in the column is based on the horizontal angle the InDevice was at when this specific read occured. Each position along the row is paired with a specific portion of the
horizontal range (defined as the maximum angle the Indevice will move to), these intervals are even in size, and increase in specific value with the row index that represents them. These intervals 
are each associated with a motor by the OutDevice (with zero being the leftmost in the set by default); if a value that is not the system default is already stored at the location in question, the two
data points will be correlated to increase output accuracy (typically a simple average). It is acceptable for the number of reads taken to exceed the number of motors presented, but the reverse 
will result in wasted hardware. At any time the table may be given to the OutDevices, which will proccess and store a local copy then await permission to generate physical responces. The table 
should be reset between read cycles in most cases;  the specific default value does not need to be zero, but it will be discarded and replaced by the first relavant read rather than averaged with it. 

WORKFLOW:
	         In a typical cycle, the DetectLayer should receive at least as many reads as it has output regions. These reads should be provided sequentially, and should be processed by the helper
methods provided by DetectLayer before they are given to its storage function. After the input phase is complete, the table needs to be passed to the OutDevices and reset. Once the data has been passed
DetectLayer can allow a physical response from the Outdevices at any time, though it is recommended that this occur as early as possible in the output phase. 


ADDENDUM:
	        - Taking more readings than there are physical output regions will increase the accuracy of the result, but only in cases where the difference is granular enough that the change in 
		position will not point the InDevice at a completely new target. The likelyhood that a given read will move the device off target decreases as the system appraoches the target, but
		a minimum of a two to one relationship is reccomended when leveraging this feature. Higher ratios will result in more consistant averages as range increases, and higher detail as it drops. 
			