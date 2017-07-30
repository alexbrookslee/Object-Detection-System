									InDevice 

InDevice is meant to provide the framework for managing the function and position of a set of rangefinders. Each rangefinder will require at least one motor, but may utalize more to
increase the number of axes it can move along. Exceeding two degrees of freedom per motor is not advised, though it is supported. Each InDevice tracks the location of its rangefinders, 
and manages their motion. From an external viewpoint, it is only possible to prompt an InDevice to take a reading, and to ask it for the results.


FORMAT:
	       Each InDevice uses a pair of arrays to relate motors to the range finders they control. Ports are paired by index between the final motor array and the range finder port array.
The InDevice moves in "steps" based on the ratio of the size of the angle it is rotating through and the number of reads it is set to take along that range. The position of a range finder is tracked 
and used to to determine when it must change directions to stay within the region. Each read is stored locally until another is taken, or it is accessed via prevDistance. The entire collection and 
repositioning process is managed through RunRead, while access to the results is controlled by prevDistance and prevAngle. 


WORKFLOW:
	        During regular operation RunRead will be called multiple times, with prevAngle and prevDistance returning the results between each measurement. It is typical to allow InDevice to make 
a complete sweep back and forth across its range before allowing the device to move on to it's output phase. 

ADDENDUM:
	        - When using more than one axis, take the sum of each region's size as the maximum angular value. Set behaviors around this range and use it to parse specific position outside of this class,.
  	        - The motor - range finder pairing is only valid between the two initial port arrays when using one axis of rotation. When using multiple degrees of freedom, the index pairing must be based on 
		the motor controller array. 